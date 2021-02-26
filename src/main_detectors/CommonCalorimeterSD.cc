#include <stdio.h>
#include "G4UserRunAction.hh"
#include "CommonCalorimeterSD.hh"
#include "CommonCalorimeterHit.hh"


#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

//--- my 
#include "Randomize.hh"


CommonCalorimeterSD::CommonCalorimeterSD(G4String name, g4e::RootOutputManager* rootManager) :
    G4VSensitiveDetector(name),
    mRootEventsOut(rootManager)
{
    if(mVerbose) {
        printf("--> CommonCalorimeterSD::Constructor(%s) \n", name.c_str());
    }

    collectionName.insert("CalCollection");
}


CommonCalorimeterSD::~CommonCalorimeterSD()
{
}


void CommonCalorimeterSD::Initialize(G4HCofThisEvent *)
{
    if (mVerbose > 2) {
        printf("JLeicCalorimeterSD()::Initialize\n");
    }

    CalCollection = new CommonCalorimeterHitsCollection(SensitiveDetectorName, collectionName[0]);

    mHitsCount = 0;
    spdlog::debug("JLeicCalorimeterSD()::Initialize exit\n");
}


G4bool CommonCalorimeterSD::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    if (mVerbose > 2) printf("--> JLeicCalorimeterSD::ProcessHits() Enter\n");

    //  const G4TouchableHandle touchablepre[128];
    G4double edep = aStep->GetTotalEnergyDeposit();

    G4double stepl = 0.;
    if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.) //-- gamma ??
        stepl = aStep->GetStepLength();


    const G4TouchableHandle touchablepre = aStep->GetPreStepPoint()->GetTouchableHandle();
    const G4TouchableHandle touchablepost = aStep->GetPostStepPoint()->GetTouchableHandle();
    // depth 1 --> x
    // depth 0 --> y
    G4int copyIDy_pre = touchablepre->GetCopyNumber();
    G4int copyIDx_pre = touchablepre->GetCopyNumber(1);
    
    //JMF got crash on "run beam on"   here. Needs to be fixed ... commenting this for a moment
    G4double xstep = (aStep->GetTrack()->GetStep()->GetPostStepPoint()->GetPosition()).x();
    G4double ystep = (aStep->GetTrack()->GetStep()->GetPostStepPoint()->GetPosition()).y();
    G4double zstep = (aStep->GetTrack()->GetStep()->GetPostStepPoint()->GetPosition()).z();
    //--- convert to the local volume --
    G4ThreeVector worldPosition = aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector localPosition = touchablepre->GetHistory()->GetTopTransform().TransformPoint(worldPosition);
    G4double xinp = localPosition.x();
    G4double yinp = localPosition.y();
    G4double zinp = localPosition.z();

    G4ThreeVector worldPosition2 = aStep->GetPostStepPoint()->GetPosition();
    G4ThreeVector localPosition2 = touchablepre->GetHistory()->GetTopTransform().TransformPoint(worldPosition2);
    G4double xend = localPosition2.x();
    G4double yend = localPosition2.y();
    G4double zend = localPosition2.z();

    G4double xloc = (xinp + xend) / 2;
    G4double yloc = (yinp + yend) / 2;
    G4double zloc = (zinp + zend) / 2;

    if (mVerbose > 2) printf("--> JLeicCalorimeterSD::ProcessHits() xloc=%f yloc=%f zloc=%f  copyIDy_pre=%d \n", xloc, yloc, zloc, copyIDy_pre);

    if (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "gamma") {
        xloc = xend;
        yloc = yend;
        zloc = zend;
    }

      G4TouchableHistory *theTouchable = (G4TouchableHistory *) (aStep->GetPreStepPoint()->GetTouchable());

    if (mVerbose > 3)
        printf("--> JLeicCalorimeterSD::ProcessHits() Vol: 0=%s \n", theTouchable->GetVolume()->GetName().c_str());

    // process    track
    G4Track *aTrack = aStep->GetTrack();

    // particle
    //G4ParticleDefinition * aParticle = aTrack->GetDefinition();
    //G4DynamicParticle*     dParticle = aTrack->GetDynamicParticle();
    G4ThreeVector momentum = aTrack->GetMomentum();
    G4ThreeVector momentumDir = aTrack->GetMomentumDirection();

    G4ThreeVector position = aTrack->GetPosition();
    G4ThreeVector vertex = aTrack->GetVertexPosition();
    G4ThreeVector vertexMom = aTrack->GetVertexMomentumDirection();

    if ((edep == 0.) && (stepl == 0.)) return false;

    if(mVerbose > 3)  printf("--> JLeicCalorimeterSD::ProcessHits() de=%f len=%f  IDxy=(%d,%d) step(x,y,z)=(%f, %f %f) in=(%f, %f %f) um  out=(%f, %f %f) um \n  loc=(%f, %f %f) um part=%s\n"
    	 ,edep,stepl/um,copyIDx_pre,copyIDy_pre,xstep,ystep,zstep,xinp/um,yinp/um,zinp/um,xend/um,yend/um,zend/um,xloc/um,yloc/um,zloc/um, aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str());

    //--- save hits ------
    if (mVerbose > 6) {
        printf("New CAL Hit:: IdVect=%d XYZloc (%f,%f,%f) dEdx=%f \n", aStep->GetTrack()->GetTrackID(), xloc, yloc, zloc, edep / keV);
    }


    std::string volumeName = theTouchable->GetVolume()->GetName().c_str();

    mRootEventsOut->SaveStep(aStep, g4e::WriteStepPointChoices::PreStepPoint, copyIDx_pre, copyIDy_pre);

   if (mVerbose > 2) printf("--> JLeicCalorimeterSD::ProcessHits() Exit\n");

    return true;
}




void CommonCalorimeterSD::EndOfEvent(G4HCofThisEvent *HCE)
{

    static G4int hitCollectionID = -1;
    if (hitCollectionID < 0) { hitCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
    HCE->AddHitsCollection(hitCollectionID, CalCollection);

    // Total hits/steps per event. Set it back to 0
    mHitsCount = 0;
}


void CommonCalorimeterSD::clear()
{
    printf("--> JLeicCalorimeterSD::clear() \n");
}


void CommonCalorimeterSD::PrintAll()
{
    printf("--> JLeicCalorimeterSD::PrintAll() \n");

}
