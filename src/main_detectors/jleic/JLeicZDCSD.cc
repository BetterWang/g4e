//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//
#include <stdio.h>
//#include "G4UserRunAction.hh"
#include "JLeicZDCSD.hh"
//#include "JLeicZDCHit.hh"


#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

//--- my
#include "Randomize.hh"


JLeicZDCSD::JLeicZDCSD(G4String name, g4e::RootOutputManager* rootManager, JLeicDetectorConstruction *det) :
    G4VSensitiveDetector(name),
    Detector(det),
    mRootEventsOut(rootManager)
{
    if(mVerbose) {
        printf("JLeicZDCSD()::constructor  enter\n");
    }

    collectionName.insert("ZDCCollection");
    printf("--> JLeicZDCSD::Constructor(%s) \n", name.c_str());

}


JLeicZDCSD::~JLeicZDCSD()
{
    printf("JLeicZDCSD():: Done ...  \n");
}


void JLeicZDCSD::Initialize(G4HCofThisEvent *)
{
    if (mVerbose > 2) {
        printf("JLeicZDCSD()::Initialize\n");
    }

    ZDCCollection = new JLeicZDCHitsCollection(SensitiveDetectorName, collectionName[0]);

    mHitsCount = 0;
    spdlog::debug("JLeicZDCSD()::Initialize exit\n");
}


G4bool JLeicZDCSD::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    std::cout << " QW!! --> JLeicZDCSD::ProcessHits" << std::endl;
    if (mVerbose > 2) printf("--> JLeicZDCSD::ProcessHits() Enter\n");

    //  const G4TouchableHandle touchablepre[128];
    G4double edep = aStep->GetTotalEnergyDeposit();

    G4double stepl = 0.;
    //if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0.) //-- gamma ??
    stepl = aStep->GetStepLength();

    const G4TouchableHandle touchablepre = aStep->GetPreStepPoint()->GetTouchableHandle();
    //const G4TouchableHandle touchablepost = aStep->GetPostStepPoint()->GetTouchableHandle();
    // depth 1 --> x
    // depth 0 --> y
    int copyID_pre = touchablepre->GetCopyNumber();

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

    if (mVerbose > 2) printf("--> JLeicZDCSD::ProcessHits() xloc=%f yloc=%f zloc=%f  copyIDy_pre=%d \n", xloc, yloc, zloc, copyIDy_pre);

    if (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "gamma") {
        xloc = xend;
        yloc = yend;
        zloc = zend;
    }

      G4TouchableHistory *theTouchable = (G4TouchableHistory *) (aStep->GetPreStepPoint()->GetTouchable());

    if (mVerbose > 3)
        printf("--> JLeicZDCSD::ProcessHits() Vol: 0=%s \n", theTouchable->GetVolume()->GetName().c_str());

    // process    track
    G4Track *aTrack = aStep->GetTrack();

    // particle
    //G4ParticleDefinition * aParticle = aTrack->GetDefinition();
    //G4DynamicParticle*     dParticle = aTrack->GetDynamicParticle();
    G4ThreeVector momentum = aTrack->GetMomentum();
    G4ThreeVector momentumDir = aTrack->GetMomentumDirection();
    G4int parentId = aTrack->GetParentID();
    G4ThreeVector position = aTrack->GetPosition();
    G4ThreeVector vertex = aTrack->GetVertexPosition();
    G4ThreeVector vertexMom = aTrack->GetVertexMomentumDirection();
    G4int PDG = aTrack->GetDefinition()->GetPDGEncoding();


    if ((edep == 0.) && (stepl == 0.)) return false;

    if(mVerbose > 3)  printf("--> JLeicZDCSD::ProcessHits() de=%f len=%f  IDxy=(%d,%d) step(x,y,z)=(%f, %f %f) in=(%f, %f %f) um  out=(%f, %f %f) um \n  loc=(%f, %f %f) um part=%s\n"
    	 ,edep,stepl/um,copyIDx_pre,copyIDy_pre,xstep,ystep,zstep,xinp/um,yinp/um,zinp/um,xend/um,yend/um,zend/um,xloc/um,yloc/um,zloc/um, aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str());


    //--- save hits ------

    if (mVerbose > 6) printf("New CAL Hit:: IdVect=%d XYZloc (%f,%f,%f) dEdx=%f \n", aStep->GetTrack()->GetTrackID(), xloc, yloc, zloc, edep / keV);

    int curTrackID = aStep->GetTrack()->GetTrackID();
    auto track = aStep->GetTrack();

    std::string volumeName = theTouchable->GetVolume()->GetName().c_str();

    mRootEventsOut->SaveStep(aStep, g4e::WriteStepPointChoices::PreStepPoint, copyIDx_pre, copyIDy_pre);

   if (mVerbose > 2) printf("--> JLeicZDCSD::ProcessHits() Exit\n");

    return true;
}


void JLeicZDCSD::EndOfEvent(G4HCofThisEvent *HCE)
{

    static G4int HCID = -1;
    if (HCID < 0) { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
    HCE->AddHitsCollection(HCID, ZDCCollection);
    //printf("--> JLeicZDCSD::EndOfEvent() \n");
    // Total hits/steps per event. Set it back to 0
    mHitsCount = 0;
}


void JLeicZDCSD::clear()
{
    printf("--> JLeicZDCSD::clear() \n");
}


void JLeicZDCSD::PrintAll()
{
    printf("--> JLeicZDCSD::PrintAll() \n");

}