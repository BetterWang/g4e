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
#include "G4UserRunAction.hh"
#include "JLeicCalorimeterSD.hh"
#include "JLeicCalorimeterHit.hh"


#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

//--- my 
#include "Randomize.hh"


JLeicCalorimeterSD::JLeicCalorimeterSD(G4String name, g4e::RootOutputManager* rootManager, JLeicDetectorConstruction *det) :
    G4VSensitiveDetector(name),
    Detector(det),
    mRootEventsOut(rootManager)
{
    if(mVerbose) {
        printf("JLeicCalorimeterSD()::constructor  enter\n");
    }

    collectionName.insert("CalCollection");
    printf("--> JLeicCalorimeterSD::Constructor(%s) \n", name.c_str());

}

void JLeicCalorimeterSD::DigitizeHits(G4Step *step, G4TouchableHistory *)
{
    //  const G4TouchableHandle touchablepre[128];
    G4double edep = step->GetTotalEnergyDeposit();


    const G4TouchableHandle touchablepre = step->GetPreStepPoint()->GetTouchableHandle();
    // depth 1 --> x
    // depth 0 --> y
    G4int copyIdX = touchablepre->GetCopyNumber(1);
    G4int copyIdY = touchablepre->GetCopyNumber();
    G4int copyIdZ = 0;

    //JMF got crash on "run beam on"   here. Needs to be fixed ... commenting this for a moment
    // if (use_depfet > 0) copyIDz_pre = touchablepre->GetCopyNumber(2);
    G4double xstep = (step->GetTrack()->GetStep()->GetPostStepPoint()->GetPosition()).x();
    G4double ystep = (step->GetTrack()->GetStep()->GetPostStepPoint()->GetPosition()).y();
    G4double zstep = (step->GetTrack()->GetStep()->GetPostStepPoint()->GetPosition()).z();

    //--- convert to the local volume --
    G4ThreeVector worldPosition = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector localPosition = touchablepre->GetHistory()->GetTopTransform().TransformPoint(worldPosition);
    G4double xinp = localPosition.x();
    G4double yinp = localPosition.y();
    G4double zinp = localPosition.z();

    G4ThreeVector worldPosition2 = step->GetPostStepPoint()->GetPosition();
    G4ThreeVector localPosition2 = touchablepre->GetHistory()->GetTopTransform().TransformPoint(worldPosition2);
    G4double xend = localPosition2.x();
    G4double yend = localPosition2.y();
    G4double zend = localPosition2.z();

    G4double xloc = (xinp + xend) / 2;
    G4double yloc = (yinp + yend) / 2;
    G4double zloc = (zinp + zend) / 2;

    if (mVerbose > 2) printf("xloc=%f yloc=%f zloc=%f  \n", xloc, yloc, zloc);


    if (step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "gamma") {
        xloc = xend;
        yloc = yend;
        zloc = zend;
    }

    //---------digitizing------------------------------

    G4double gain = 1400. * 1.055;
    G4double sigma_gain = 8. / 100.;
    gain = G4RandGauss::shoot(gain, sigma_gain / gain);
    short int ADC = edep / keV * gain / 59.5;

    if (char_sh == 0 && copyIdY < NumRow && copyIdX < NumCol) FRAME[copyIdY * NumCol + copyIdX] += ADC;

    G4TouchableHistory *theTouchable
            = (G4TouchableHistory *) (step->GetPreStepPoint()->GetTouchable());

    if (jDebug > 3)
        printf("--> JLeicCalorimeterSD::ProcessHits() Vol: 0=%s \n", theTouchable->GetVolume()->GetName().c_str());

    //-------------------------------------------------------------------------------------
    if (char_sh > 0) {
        int iz = (225 - zloc / um) / 10;
        if (iz > N_bin) iz = N_bin;
        if (iz < 0) iz = 0;
        int ix = (xloc / um) / 20. * 10;
        if (ix > 5) ix = 5;
        if (ix < -5) ix = -5;
        int iy = (yloc / um) / 20. * 10;
        if (iy > 5) iy = 5;
        if (iy < -5) iy = -5;

        G4double sum = 0, total = 0;
        int ic = 0, ky = 0, kx = 0, i0 = 51, j0 = 51;
        for (int i = (i0 - 25); i <= (i0 + 15); i += 10) {
            kx = kx + 1;
            ky = 0;
            for (int j = (j0 - 25); j <= (j0 + 15); j += 10) {
                ky = ky + 1;
                sum = 0;
                for (int k = 0; k <= 9; k++) {
                    for (int n = 0; n <= 9; n++) {
                        sum = sum + hist_charge[iz]->GetBinContent(i + k + ix, j + n + iy) * 1e-6; //HIJ(30,i+k,j+n);
                        //printf(" chan=%d %d\n",i-i0+k,j-j0+n);
                    }
                }
                if (save_frames_root) hmatrix2->Fill(copyIdX - 3 + kx, copyIdY - 3 + ky, edep * sum / keV);
                int ycell = copyIdY - 3 + ky;
                int xcell = copyIdX - 3 + kx;
                ADC = edep * sum / keV * gain / 59.5;
                if (0 <= ycell && ycell < NumRow && 0 < xcell && xcell < NumCol) FRAME[ycell * NumCol + xcell] += ADC;
                total = total + sum;
                ic = ic + 1;
                //printf(" iz=%d (%d,%d) ic=%d x=%d y=%d sum=%f kx=%d ky=%d\n", iz,ix,iy,ic,i-i0,j-j0,sum, kx, ky);
            }
        }
        //printf("total=%f \n", total);
    }
    //-------------------------------------------------------------------------------------

    // process
    //const G4VProcess * aProcessPointer = aStep->GetPostStepPoint()->GetProcessDefinedStep();
    // track
    G4Track *aTrack = step->GetTrack();

    // particle
    //G4ParticleDefinition * aParticle = aTrack->GetDefinition();
    //G4DynamicParticle*     dParticle = aTrack->GetDynamicParticle();
    G4ThreeVector momentum = aTrack->GetMomentum();
    G4ThreeVector momentumDir = aTrack->GetMomentumDirection();
    G4int ParrentID = aTrack->GetParentID();
    G4ThreeVector position = aTrack->GetPosition();
    G4ThreeVector vertex = aTrack->GetVertexPosition();
    G4ThreeVector vertexMom = aTrack->GetVertexMomentumDirection();
    G4int PDG = aTrack->GetDefinition()->GetPDGEncoding();

    //vertex (,where this track was created) information
    /*
      const G4ThreeVector& GetVertexPosition() const;
      void  SetVertexPosition(const G4ThreeVector& aValue);
      const G4ThreeVector& GetVertexMomentumDirection() const;
      G4int GetCurrentStepNumber() const;
      void  IncrementCurrentStepNumber();
    */


    if ((edep == 0.) && (stepLength == 0.)) return false;

    //  printf("--> JLeicCalorimeterSD::ProcessHits() de=%f ADC=%d  len=%f  IDxy=(%d,%d) step(x,y,z)=(%f, %f %f) in=(%f, %f %f) um  out=(%f, %f %f) um \n  loc=(%f, %f %f) um part=%s\n"
    //	 ,edep/keV,ADC,stepl/um,copyIDx_pre,copyIDy_pre,xstep,ystep,zstep,xinp/um,yinp/um,zinp/um,xend/um,yend/um,zend/um,xloc/um,yloc/um,zloc/um, aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str());



    G4VPhysicalVolume *physVol = theTouchable->GetVolume();
    //theTouchable->MoveUpHistory();
    G4int JLeicNumber = 0;

    if (jDebug > 2)
        printf("--> JLeicCalorimeterSD::ProcessHits() Vol=(%s) %p Abs=%p \n", physVol->GetName().c_str(),
               (void *) physVol, (void *) Detector->GetAbsorber());

    if (HitID[JLeicNumber] == -1) {
        JLeicCalorimeterHit *calHit = new JLeicCalorimeterHit();
        if (physVol == Detector->GetAbsorber()) calHit->AddAbs(edep, stepLength);
        HitID[JLeicNumber] = CalCollection->insert(calHit) - 1;
        if (verboseLevel > 0)
            G4cout << " New Calorimeter Hit on JLeic: " << JLeicNumber << G4endl;
        //printf("--> JLeicCalorimeterSD::ProcessHits()  New Calorimeter Hit on JLeic: %d de=%f\n",JLeicNumber,edep/keV);
    } else {
        if (physVol == Detector->GetAbsorber())
            (*CalCollection)[HitID[JLeicNumber]]->AddAbs(edep, stepLength);
        if (verboseLevel > 0)
            G4cout << " Energy added to JLeic: " << JLeicNumber << G4endl;
        //printf("--> JLeicCalorimeterSD::ProcessHits()  Energy added to JLeic: %d  de=%f\n",JLeicNumber,edep/keV);
    }

    if (jDebug > 2) printf("--> JLeicCalorimeterSD::ProcessHits() Exit\n");
}


JLeicCalorimeterSD::~JLeicCalorimeterSD()
{

    /*
      if(mHitsFile)
      {
          mHitsFile->cd();
          mRootEventsOut.Write();
          mHitsFile->Close();
      }
    */

    printf("JLeicCalorimeterSD():: Done ...  \n");
}


void JLeicCalorimeterSD::Initialize(G4HCofThisEvent *)
{
    if (mVerbose > 2) {
        printf("JLeicCalorimeterSD()::Initialize\n");
    }

    CalCollection = new JLeicCalorHitsCollection(SensitiveDetectorName, collectionName[0]);

    mHitsCount = 0;
    spdlog::debug("JLeicCalorimeterSD()::Initialize exit\n");
}


G4bool JLeicCalorimeterSD::ProcessHits(G4Step *aStep, G4TouchableHistory *)
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
    G4int copyIDz_pre = 0;

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
    G4int parentId = aTrack->GetParentID();
    G4ThreeVector position = aTrack->GetPosition();
    G4ThreeVector vertex = aTrack->GetVertexPosition();
    G4ThreeVector vertexMom = aTrack->GetVertexMomentumDirection();
    G4int PDG = aTrack->GetDefinition()->GetPDGEncoding();


    if ((edep == 0.) && (stepl == 0.)) return false;

    if(mVerbose > 3)  printf("--> JLeicCalorimeterSD::ProcessHits() de=%f len=%f  IDxy=(%d,%d) step(x,y,z)=(%f, %f %f) in=(%f, %f %f) um  out=(%f, %f %f) um \n  loc=(%f, %f %f) um part=%s\n"
    	 ,edep,stepl/um,copyIDx_pre,copyIDy_pre,xstep,ystep,zstep,xinp/um,yinp/um,zinp/um,xend/um,yend/um,zend/um,xloc/um,yloc/um,zloc/um, aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str());


    //--- save hits ------

    if (mVerbose > 6) printf("New CAL Hit:: IdVect=%d XYZloc (%f,%f,%f) dEdx=%f \n", aStep->GetTrack()->GetTrackID(), xloc, yloc, zloc, edep / keV);

    int curTrackID = aStep->GetTrack()->GetTrackID();
    auto track = aStep->GetTrack();

    std::string volumeName = theTouchable->GetVolume()->GetName().c_str();

    mRootEventsOut->SaveStep(aStep, g4e::WriteStepPointChoices::PreStepPoint, copyIDx_pre, copyIDy_pre);

   if (mVerbose > 2) printf("--> JLeicCalorimeterSD::ProcessHits() Exit\n");

    return true;
}




void JLeicCalorimeterSD::EndOfEvent(G4HCofThisEvent *HCE)
{

    static G4int HCID = -1;
    if (HCID < 0) { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
    HCE->AddHitsCollection(HCID, CalCollection);
    //printf("--> JLeicCalorimeterSD::EndOfEvent() \n");
    // Total hits/steps per event. Set it back to 0
    mHitsCount = 0;
}


void JLeicCalorimeterSD::clear()
{
    printf("--> JLeicCalorimeterSD::clear() \n");
}


void JLeicCalorimeterSD::PrintAll()
{
    printf("--> JLeicCalorimeterSD::PrintAll() \n");

}
