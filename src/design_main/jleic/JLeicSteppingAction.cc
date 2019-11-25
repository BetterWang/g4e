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

#include "design_main/jleic/JLeicDetectorConstruction.hh"
#include "JLeicSteppingAction.hh"
#include "JLeicEventAction.hh"
#include "JLeicRunAction.hh"
#include "JleicHistogramming.hh"

#include <G4ios.hh>
#include <G4SystemOfUnits.hh>
#include <G4SteppingManager.hh>
#include <G4TrackVector.hh>

#include <iomanip>

#include "TLorentzVector.h"

int FIRST = 0,
FIRSTFOUT = 0;
int InsideD1 = 0, InsideQ4 = 0;
int myevno = 0;
static double pt = 0., pz = 0., xL = 0.;
FILE *rc5;
TLorentzVector ka;


JLeicSteppingAction::JLeicSteppingAction(JLeicDetectorConstruction *detectorConstruction, JLeicEventAction *eventAction, JLeicRunAction *runAction, JLeicHistogramming *histo) :
    detector(detectorConstruction),
    eventaction(eventAction),
    runaction(runAction),
    fHistos(histo),
    IDold(-1),
    evnoold(-1)
{
    //steppingMessenger = new JLeicSteppingMessenger(this);
}


JLeicSteppingAction::~JLeicSteppingAction() = default;


void JLeicSteppingAction::UserSteppingAction(const G4Step *aStep) {

    G4double Theta, Thetaback, Ttrans, Tback, Tsec, Egamma, DEgamma, xend, yend, zend, rend, xp, yp, zp, rp;
    G4int evno = eventaction->GetEventno();
    IDnow = evno + 10000 * (aStep->GetTrack()->GetTrackID()) + 100000000 * (aStep->GetTrack()->GetParentID());                //-- 100 k events only ???

    auto preStepVolumeName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    auto postStepVolumeName = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
    const double x = aStep->GetTrack()->GetPosition().x();
    const double y = aStep->GetTrack()->GetPosition().y();
    const double z = aStep->GetTrack()->GetPosition().z();

    if (evno != myevno) {

        InsideD1 = 0, InsideQ4 = 0;
        myevno = evno;
    }

#if 0
    Nauka_11
    //===========================================================================
    //   DEBUG 
    //===========================================================================

    
    if( aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
  std::cout << "===========<<<<<<<<< PostStepPoint on Boundary" << G4endl;
    }
    if( aStep->GetPreStepPoint()->GetStepStatus() == fGeomBoundary) {
  std::cout << "==========>>>>>>>>>> PreStepPoint on Boundary" << G4endl;
    }
    std::cout << "PreStepPoint Step Status: "
          << aStep->GetPreStepPoint()->GetStepStatus()
          << G4endl;

    if (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {    //-- @ Geom Boundary --

  {  //   --- pre-step ---
      //--- convert to the local volume --
      const G4TouchableHandle touchablepre = aStep->GetPreStepPoint()->GetTouchableHandle();
      G4ThreeVector worldPosition = aStep->GetPreStepPoint()->GetPosition();
      G4ThreeVector localPosition = touchablepre->GetHistory()->GetTopTransform().TransformPoint(worldPosition);
      G4double xinp = localPosition.x();
      G4double yinp = localPosition.y();
      G4double zinp = localPosition.z();
      printf("SteppingAction:: Pre:: Volume=%s world pos  x=%f(%f) y=%f(%f) z=%f(%f) \n",aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str(), worldPosition.x(),xinp, worldPosition.y(),yinp, worldPosition.z(),zinp);
      printf("SteppingAction:: Exit Volume=%s  x=%f(%f) y=%f(%f) z=%f(%f)   mom_dir (%f,%f, %f ) particle=%s \n",aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str(),
         aStep->GetTrack()->GetPosition().x(),xinp, y,yinp, z,zinp,
         aStep->GetTrack()->GetMomentumDirection().x(),aStep->GetTrack()->GetMomentumDirection().y(),aStep->GetTrack()->GetMomentumDirection().z()
         ,aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str());
    }

    if (aStep->GetPostStepPoint()->GetPhysicalVolume()) {  // -- if post step volume exists ---

      //--- convert to the local volume --
      const G4TouchableHandle touchablepre = aStep->GetPostStepPoint()->GetTouchableHandle();
      G4ThreeVector worldPosition = aStep->GetPostStepPoint()->GetPosition();
      //G4ThreeVector worldPosition = aStep->GetTrack()->GetPosition();
      G4ThreeVector localPosition = touchablepre->GetHistory()->GetTopTransform().TransformPoint(worldPosition);
      G4double xinp = localPosition.x();
      G4double yinp = localPosition.y();
      G4double zinp = localPosition.z();

      printf("SteppingAction:: Post:: Volume=%s world pos  x=%f(%f) y=%f(%f) z=%f(%f) \n",postStepVolumeName.c_str(), worldPosition.x(),xinp, worldPosition.y(),yinp, worldPosition.z(),zinp);

      printf("SteppingAction:: Enter Volume=%s  x=%f(%f) y=%f(%f) z=%f(%f)   mom_dir (%f,%f, %f ) particle=%s \n",postStepVolumeName.c_str(),
         aStep->GetTrack()->GetPosition().x(),xinp, y,yinp, z,zinp,
         aStep->GetTrack()->GetMomentumDirection().x(),aStep->GetTrack()->GetMomentumDirection().y(),aStep->GetTrack()->GetMomentumDirection().z()
         ,aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str());
    }
      }
#endif

    //===========================================================================

    // print positions at Roman_pot location
    if (preStepVolumeName == "ffi_RPOT_D3_GVol_Phys") {

        fmt::print("SteppingAction:: Volume={}  x={} y={} z={}   mom_dir ({}, {}, {}) particle={} \n",
                   preStepVolumeName,
                   x, y, z ,
                   aStep->GetTrack()->GetMomentumDirection().y(), aStep->GetTrack()->GetMomentumDirection().z(),
                   aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str());

    }
    char myfposout[256];
    if (FIRSTFOUT == 0) {
        sprintf(myfposout, "mypositions.txt");
        rc5 = fopen(myfposout, "w");
        if (rc5 == NULL) return;
        FIRSTFOUT = 1;
    }

    //====== Entry position of the volume=========================================================================
    if (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary && aStep->GetPostStepPoint()->GetPhysicalVolume()) {
        if ((postStepVolumeName == "Physics_DIPOLE_m_iBDS1a") ||
            (postStepVolumeName == "Physics_DIPOLE_m_iBDS1b") ||
            (postStepVolumeName == "Physics_QUADS_hd_m_iQDS1a") ||
            (postStepVolumeName == "Physics_QUADS_hd_m_iQDS1b") ||
            (postStepVolumeName == "Physics_QUADS_hd_m_iQDS1S") ||
            (postStepVolumeName == "Physics_QUADS_hd_m_iQDS2S") ||
            (postStepVolumeName == "Physics_QUADS_hd_m_iQDS2") ||
            (postStepVolumeName == "Physics_QUADS_hd_m_iQDS3S") ||
            (postStepVolumeName == "Physics_QUADS_hd_m_iQDS3") ||
            (postStepVolumeName == "Physics_DIPOLE_m_iBDS2") ||
            (postStepVolumeName == "Physics_DIPOLE_m_iBDS3") ||
            (postStepVolumeName == "Physics_ASOLENOID_hd_m_iASUS") ||
            (postStepVolumeName == "Physics_Chicane_m_eBDS1") ||
            (postStepVolumeName == "Physics_DIPOLE")) {

            /** >oO Dbug printing
            fprintf(rc5, "SteppingAction::iBDS1 Entry: Volume=%s  x=%f y=%f z=%f   mom_dir (%f,%f, %f ) theta=%f particle=%s  Exit_volume=%s\n",
                    postStepVolumeName.c_str(), x, y,
                    z, aStep->GetTrack()->GetMomentumDirection().x(), aStep->GetTrack()->GetMomentumDirection().y(),
                    aStep->GetTrack()->GetMomentumDirection().z(), atan(aStep->GetTrack()->GetMomentumDirection().x() / aStep->GetTrack()->GetMomentumDirection().z()),
                    aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str(), aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str());
                    */


            if (strcmp(postStepVolumeName.c_str(), "Physics_DIPOLE_m_iBDS1a") == 0) {
                fHistos->FillHist2d(4, x, y, 1.);

                if (InsideD1 == 0) {
                    InsideD1 = 1;
                    ka.SetXYZM(aStep->GetTrack()->GetMomentumDirection().x() * aStep->GetTrack()->GetMomentum().mag(),
                               aStep->GetTrack()->GetMomentumDirection().y() * aStep->GetTrack()->GetMomentum().mag(),
                               aStep->GetTrack()->GetMomentumDirection().z() * aStep->GetTrack()->GetMomentum().mag(), aStep->GetTrack()->GetParticleDefinition()->GetPDGMass());
                    ka.RotateY(0.05);
                    pt = aStep->GetTrack()->GetMomentum().perp();
                    pz = aStep->GetTrack()->GetMomentumDirection().z() * aStep->GetTrack()->GetMomentum().mag();

                    double xL1 = ka.Pz() / ka.P();
                    xL = pz / aStep->GetTrack()->GetMomentum().mag();

                    printf("RPOT:: %f %f  %f   (direction = %f %f %f ) ka :: %f %f %f xL=%f\n ", pt, pz, aStep->GetTrack()->GetMomentum().mag(),
                           aStep->GetTrack()->GetMomentumDirection().x(), aStep->GetTrack()->GetMomentumDirection().y(), aStep->GetTrack()->GetMomentumDirection().z(), ka.Perp(),
                           ka.Pz(), ka.P(), xL);
                    pt = ka.Perp();
                    pz = ka.Pz();
                    //  xL=pz/ka.P();
                }
            }
            if (strcmp(postStepVolumeName.c_str(), "Physics_DIPOLE_m_iBDS1b") == 0) {
                fHistos->FillHist2d(5, x, y, 1.);
            }

            if (strcmp(postStepVolumeName.c_str(), "Physics_QUADS_hd_m_iQDS1a") == 0) {
                fHistos->FillHist2d(8, x, y, 1.);
            }

            if (strcmp(postStepVolumeName.c_str(), "Physics_QUADS_hd_m_iQDS1S") == 0) {
                fHistos->FillHist2d(9, x, y, 1.);
            }
            if (strcmp(postStepVolumeName.c_str(), "Physics_QUADS_hd_m_iQDS1b") == 0) {
                fHistos->FillHist2d(10, x, y, 1.);
            }
            if (strcmp(postStepVolumeName.c_str(), "Physics_QUADS_hd_m_iQDS2S") == 0) {
                fHistos->FillHist2d(11, x, y, 1.);
            }
            if (strcmp(postStepVolumeName.c_str(), "Physics_QUADS_hd_m_iQDS2") == 0) {
                fHistos->FillHist2d(12, x, y, 1.);
            }
            if (strcmp(postStepVolumeName.c_str(), "Physics_QUADS_hd_m_iQDS3S") == 0) {
                fHistos->FillHist2d(13, x, y, 1.);
            }


            if (strcmp(postStepVolumeName.c_str(), "Physics_DIPOLE_m_iBDS2") == 0) {
                fHistos->FillHist2d(6, x, y, 1.);
            }

            if (strcmp(postStepVolumeName.c_str(), "Physics_DIPOLE_m_iBDS3") == 0) {

                fHistos->FillHist2d(7, x, y, 1.);

            }

            if (strcmp(postStepVolumeName.c_str(), "Physics_QUADS_hd_m_iQDS4") == 0) {
                fHistos->FillHist2d(14, x, y, 1.);

            }
            if (strcmp(postStepVolumeName.c_str(), "Physics_Chicane_m_eBDS1") == 0) {
                fHistos->FillHist2d(15, x, y, 1.);

            }


        }
    }
//====== Exit position of the volume=========================================================================
    if (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary && aStep->GetPreStepPoint()->GetPhysicalVolume()) {
        if ((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_DIPOLE_m_iBDS1a") ||
            (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_DIPOLE_m_iBDS1b") ||
            (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_QUADS_hd_m_iQDS1a") ||
            (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_QUADS_hd_m_iQDS1b") ||
            (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_QUADS_hd_m_iQDS1S") ||
            (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_QUADS_hd_m_iQDS2S") ||
            (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_QUADS_hd_m_iQDS2") ||
            (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_QUADS_hd_m_iQDS3S") ||
            (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_QUADS_hd_m_iQDS3") ||
            (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_DIPOLE_m_iBDS2") ||
            (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_DIPOLE_m_iBDS3") ||
            (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_ASOLENOID_hd_m_iASUS")


                ) {
//  if(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Physics_DIPOLE_m_iBDS1b") {

            fprintf(rc5, "SteppingAction::iBDS1 Exit Volume=%s  x=%f y=%f z=%f   mom_dir (%f,%f, %f ) theta =%f particle=%s Entry_Volume =%s\n",
                    aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str(), x, y,
                    z, aStep->GetTrack()->GetMomentumDirection().x(), aStep->GetTrack()->GetMomentumDirection().y(),
                    aStep->GetTrack()->GetMomentumDirection().z(), atan(aStep->GetTrack()->GetMomentumDirection().x() / aStep->GetTrack()->GetMomentumDirection().z()),
                    aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str(), postStepVolumeName.c_str());

            //   fHistos->FillHist2d(1, x,y,1.);



            if (preStepVolumeName == "Physics_DIPOLE_m_iBDS1a") {
                fHistos->FillHist2d(24, x, y, 1.);
            }
            if (preStepVolumeName == "Physics_DIPOLE_m_iBDS1b") {
                fHistos->FillHist2d(25, x, y, 1.);
            }

            if (preStepVolumeName == "Physics_QUADS_hd_m_iQDS1a") {
                fHistos->FillHist2d(28, x, y, 1.);
            }

            if (preStepVolumeName == "Physics_QUADS_hd_m_iQDS1S") {
                fHistos->FillHist2d(29, x, y, 1.);
            }
            if (preStepVolumeName == "Physics_QUADS_hd_m_iQDS1b") {
                fHistos->FillHist2d(30, x, y, 1.);
            }
            if (preStepVolumeName == "Physics_QUADS_hd_m_iQDS2S") {
                fHistos->FillHist2d(31, x, y, 1.);
            }
            if (preStepVolumeName == "Physics_QUADS_hd_m_iQDS2") {
                fHistos->FillHist2d(32, x, y, 1.);
            }
            if (preStepVolumeName == "Physics_QUADS_hd_m_iQDS3S") {
                fHistos->FillHist2d(33, x, y, 1.);

            }


            if (preStepVolumeName == "Physics_DIPOLE_m_iBDS2") {
                fHistos->FillHist2d(26, x, y, 1.);
            }

            if (preStepVolumeName == "Physics_DIPOLE_m_iBDS3") {

                fHistos->FillHist2d(27, x, y, 1.);


                //---------------- acceptance plot ----------------------------

                if (InsideD1 == 1 && InsideQ4 == 0) {
                    InsideQ4 = 1;
                    printf("SteppingAction:: RPOT:: pt =%f pz=%f xL=%f\n", pt, pz, xL);
                    fHistos->FillHist2d(20, abs(xL), abs(pt / 1000.), 1.);
                }

            }

            if (preStepVolumeName == "Physics_QUADS_hd_m_iQDS4") {
                fHistos->FillHist2d(34, x, y, 1.);
            }


        }
    }

#ifdef USE_TUNE
    //--------------------------for TUNE-------------------------------------------------------------------------
    if(
       //      strcmp(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str(),"World")==0 &&
        strcmp(postStepVolumeName.c_str(),"Physics_QUADS_hd_m_iQDS1a")==0

     ){


   char myftune[256];
   float my_or;
   // printf("read Di file\n");
   if(FIRST==0 ) {  sprintf(myftune,"tune.txt");
     rc=fopen(myftune,"w");  if (rc == NULL) return; FIRST=1;
   }
   //
   // (x+0.3529979921)^2+(px+200*tan(0.05299800449))^2+y^2+py^2 where x, px, y, py are at the entrance into iQDS1a
   //
  //   my_or= (x+0.3529979921)*(x+0.3529979921)
   //       +( aStep->GetTrack()->GetMomentumDirection().x()+200*tan(0.05299800449))*( aStep->GetTrack()->GetMomentumDirection().x()+200*tan(0.05299800449))
   //       +y*y
   //       +aStep->GetTrack()->GetMomentumDirection().y()*aStep->GetTrack()->GetMomentumDirection().y();
     my_or= (x/1000.+0.3529979921)*(x/1000.+0.3529979921)
          +(200*aStep->GetTrack()->GetMomentumDirection().x()+200*tan(0.05299800449))*(200*aStep->GetTrack()->GetMomentumDirection().x()+200*tan(0.05299800449))
       +(y/1000.)*(y/1000.)
       +(200*aStep->GetTrack()->GetMomentumDirection().y())*(200*aStep->GetTrack()->GetMomentumDirection().y());

     runaction->FillHist2d(8, x,y,1.);

     fprintf(rc,"SteppingAction:: iQDS1a %f %f %f %f %f %f ", x, y, z,  aStep->GetTrack()->GetMomentumDirection().x(),aStep->GetTrack()->GetMomentumDirection().y(),aStep->GetTrack()->GetMomentumDirection().z());
        printf("SteppingAction:: %f \n", my_or );
        fprintf(rc,"%f\n", my_or );
        //  fclose(rc);

    }


  //----------------------------- dipoles -------------------------------
    if( strcmp(postStepVolumeName.c_str(),"Physics_DIPOLE_m_iBDS3")==0 ){

        fprintf(rc,"SteppingAction:: iBDS3 %f %f %f %f %f %f \n", x, y, z,  aStep->GetTrack()->GetMomentumDirection().x(),aStep->GetTrack()->GetMomentumDirection().y(),aStep->GetTrack()->GetMomentumDirection().z());
         runaction->FillHist2d(7, x,y,1.);
    }

#endif

    //--------------------------end for TUNE-------------------------------------------------------------------------

    if (IDnow != IDold) {
        IDold = IDnow;
        //printf(" primary=%s\n",PrimaryGeneratorAction::GetPrimaryName().c_str());
        //-----------------
        //-- e+ e- vertex
        if ((((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName()) == "e-") &&
             ((aStep->GetTrack()->GetTrackID() != 1) || (aStep->GetTrack()->GetParentID() != 0))) ||
            (((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName()) == "e+") &&
             ((aStep->GetTrack()->GetTrackID() != 1) || (aStep->GetTrack()->GetParentID() != 0))))
            fHistos->Fillvertexz(aStep->GetTrack()->GetVertexPosition().z()); //-- Z ?
        //-----------


        if (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "pxdPixel" || aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Absorber") {
            if (((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName()) == "e-") &&
                ((aStep->GetTrack()->GetTrackID() != 1) || (aStep->GetTrack()->GetParentID() != 0))) {
                eventaction->AddCharged();
                eventaction->AddE();
                Tsec = aStep->GetTrack()->GetKineticEnergy();  // !!!!!!!!!!!!
                //Tsec += aStep->GetTotalEnergyDeposit() ;        // !!!!!!!!!!!!
                fHistos->FillTsec(Tsec / keV);
            } else if (((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName()) == "e+") &&
                       ((aStep->GetTrack()->GetTrackID() != 1) || (aStep->GetTrack()->GetParentID() != 0))) {
                eventaction->AddCharged();
                eventaction->AddP();
                Tsec = aStep->GetTrack()->GetKineticEnergy();  // !!!!!!!!!!!!
                //Tsec += aStep->GetTotalEnergyDeposit() ;        // !!!!!!!!!!!!
                fHistos->FillTsec(Tsec / keV);
            } else  //-- gamma START in absorber
            if (((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName()) == "gamma") &&
                ((aStep->GetTrack()->GetTrackID() != 1) || (aStep->GetTrack()->GetParentID() != 0))) {
                eventaction->AddNeutral();
                //eventaction->AddGammaDE(aStep->GetTotalEnergyDeposit()) ;
                Egamma = aStep->GetTrack()->GetKineticEnergy();
                //printf("SEC gammas FROM absorber E=%f dE=%f ID=%d/%d ",Egamma,aStep->GetTotalEnergyDeposit(),IDnow,IDold);
                //printf("  >  created by %s\n",aStep->GetTrack()->GetCreatorProcess()->GetProcessName().c_str());
            }

        } //-- end pxdPixel
    }  //-- (IDnow != IDold)


    //----------------------------------------------------------------------------
    //-- STEP in pxdPixel
    if (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "pxdPixel" ||
        aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "svdPixel" ||
        aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Absorber")
    {

        printf("step in absorber de=%f  step=%f Particle=%s energy=%f [mev]\n", aStep->GetTotalEnergyDeposit() / keV, aStep->GetStepLength() / um,
               aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName().c_str(), aStep->GetTrack()->GetKineticEnergy() / MeV);

        zend = z;

        fHistos->FillHist(7, zend - detector->fConfig.ci_TRD.fAbsorberZ + detector->fConfig.ci_TRD.fAbsorberThickness / 2., aStep->GetTotalEnergyDeposit() / keV);
        if (((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName()) == "gamma")) {
            fHistos->FillHist(27, aStep->GetTotalEnergyDeposit() / keV, 1.);
        } else {
            if (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "pxdPixel") {
                fHistos->FillHist(25, aStep->GetTotalEnergyDeposit() / keV, 1.);
                fHistos->FillHist2d(2, aStep->GetTrack()->GetKineticEnergy() / MeV, aStep->GetTotalEnergyDeposit() / keV, 1.);
            } else if (aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "svdPixel") {
                fHistos->FillHist(26, aStep->GetTotalEnergyDeposit() / keV, 1.);
                fHistos->FillHist2d(3, aStep->GetTrack()->GetKineticEnergy() / MeV, aStep->GetTotalEnergyDeposit() / keV, 1.);
            }
        }
        //if ( 201 < zend && zend < 203 )  fHistos->FillHist(11,aStep->GetTotalEnergyDeposit()/keV);
        //if ( 225 < zend && zend < 227 )  fHistos->FillHist(12,aStep->GetTotalEnergyDeposit()/keV);
        //if ( 235 < zend && zend < 237 )  fHistos->FillHist(13,aStep->GetTotalEnergyDeposit()/keV);

        if (((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName()) == "e-") ||
            ((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName()) == "e+")) {

            eventaction->CountStepsCharged();

            fHistos->FillHist(3, aStep->GetStepLength() / um);
            if ((aStep->GetTrack()->GetTrackID() == 1) && (aStep->GetTrack()->GetParentID() == 0)) {
                fHistos->FillHist(5, zend - detector->fConfig.ci_TRD.fAbsorberZ + detector->fConfig.ci_TRD.fAbsorberThickness / 2., aStep->GetTotalEnergyDeposit() / keV);
            } else {
                fHistos->FillHist(4, zend - detector->fConfig.ci_TRD.fAbsorberZ + detector->fConfig.ci_TRD.fAbsorberThickness / 2., aStep->GetTotalEnergyDeposit() / keV);
            }
            fHistos->FillHist(6, zend - detector->fConfig.ci_TRD.fAbsorberZ + detector->fConfig.ci_TRD.fAbsorberThickness / 2., aStep->GetTotalEnergyDeposit() / keV);
        }
        if ((aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName()) == "gamma") {
            eventaction->CountStepsNeutral();
            DEgamma = aStep->GetTotalEnergyDeposit();
            //eventaction->AddGammaDE(aStep->GetTotalEnergyDeposit()) ;
            if (DEgamma > 0.00000001)
                fHistos->FillGamDE(DEgamma);


            G4TouchableHandle theTouchable = aStep->GetPreStepPoint()->GetTouchableHandle();
            G4int copyNo = theTouchable->GetCopyNumber();

            G4int histDepth = theTouchable->GetHistoryDepth();
            G4int motherCopyNo = theTouchable->GetCopyNumber(1);
            G4int motherRepNo = theTouchable->GetReplicaNumber(1);

            // depth 1 --> x
            // depth 0 --> y
            G4int copyIDy_pre = theTouchable->GetCopyNumber();
            G4int copyIDx_pre = theTouchable->GetCopyNumber(1);
            //if (DEgamma>0.00000001)
            printf("UserSteppingAction():: gamma: E=%f dep=%f histDepth=%d moCopy=%d, moRep=%d IDxy=(%d,%d)\n", aStep->GetTrack()->GetKineticEnergy() / keV,
                   aStep->GetTotalEnergyDeposit() / keV, histDepth, motherCopyNo, motherRepNo, copyIDx_pre, copyIDy_pre);

            if (DEgamma > 0.00000001) {
                fHistos->FillGammaEStep(DEgamma);
                xend = x;
                yend = y;
                zend = z;
                fHistos->FillHist(1, zend - detector->fConfig.ci_TRD.fAbsorberZ + detector->fConfig.ci_TRD.fAbsorberThickness / 2.);
                fHistos->FillHist(2, zend - detector->fConfig.ci_TRD.fAbsorberZ + detector->fConfig.ci_TRD.fAbsorberThickness / 2., DEgamma / keV);
            }
            //G4VProcess* creatorProcess = aStep->GetTrack()->GetCreatorProcess();
            //if(creatorProcess->GetProcessName() == “GammaXTRadiator”) G4cout << “Particle was created by the LowEnergy” << “Ionization process“ << G4endl;
            //printf(" Gamma greated by %s\n",aStep->GetTrack()->GetCreatorProcess()->GetProcessName().c_str());
            //printf(" ALL gammas STEP in absorber de=%f ID=%d/%d ParID=%d TrkID=%d\n",aStep->GetTotalEnergyDeposit(),IDnow,IDold
            //     ,aStep->GetTrack()->GetParentID(),aStep->GetTrack()->GetTrackID());
        }
    } //-- if absorber
    else {

        if (aStep->GetTrack()->GetDefinition()->GetPDGCharge() != 0)
            fHistos->FillHist(3, aStep->GetStepLength() / um);
    }


    //----------------------------------------------------------------------------
    //-- Primary EXIT absorber forward ? GetMomentumDirection().x()>0. !!! x !!!  ERROR !! .z !!!
    bool is_beam_particle = aStep->GetTrack()->GetParentID() == 0 && aStep->GetTrack()->GetTrackID() == 1;
    bool was_in_pixel = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "pxdPixel";
    bool was_in_absorber = aStep->GetPreStepPoint()->GetPhysicalVolume() == detector->GetAbsorberPhysicalVolume();
    bool is_next_world = aStep->GetTrack()->GetNextVolume() == detector->GetWorldPhysicalVolume();
    bool is_transportation = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Transportation";

    bool is_primary_exit_absorber = is_beam_particle && (was_in_pixel || was_in_absorber) && is_next_world && is_transportation;

    if (is_primary_exit_absorber && (aStep->GetTrack()->GetMomentumDirection().z() > 0.)) {
        eventaction->SetTr();
        Theta = std::acos(aStep->GetTrack()->GetMomentumDirection().x());
        fHistos->FillTh(Theta);
        Ttrans = aStep->GetTrack()->GetKineticEnergy();
        fHistos->FillTt(Ttrans);
        yend = y;
        zend = z;
        rend = std::sqrt(yend * yend + zend * zend);
        fHistos->FillR(rend);
    }
    //----------------------------------------------------------------------------
    //--  Primary EXIT absorber backward ? GetMomentumDirection().z()<0. ---
    if (is_primary_exit_absorber && (aStep->GetTrack()->GetMomentumDirection().z() < 0.)) {
        eventaction->SetRef();
        Thetaback = std::acos(aStep->GetTrack()->GetMomentumDirection().x());
        Thetaback -= 0.5 * CLHEP::pi;
        fHistos->FillThBack(Thetaback);
        Tback = aStep->GetTrack()->GetKineticEnergy();
        fHistos->FillTb(Tback);
    }

    //----------------------------------------------------------------------------
    //--  gamma EXIT absorber forward
    if (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary && aStep->GetPostStepPoint()->GetPhysicalVolume()) {
        if (((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "pxdPixel" || aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "Absorber") &&
             (postStepVolumeName == "World") &&
             //(aStep->GetTrack()->GetNextVolume()->GetName()=="World") &&
             (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Transportation") && (aStep->GetTrack()->GetMomentumDirection().z() > 0.) &&
             (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "gamma"))) {
            Egamma = aStep->GetTrack()->GetKineticEnergy();
            fHistos->FillGammaOutSpectrum(Egamma);
            //printf("O::test PreStep = %s ",aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str());
            //printf("O::test PostStep = %s \n",postStepVolumeName.c_str());
            //printf("E::gammas EXIT absorber de=%f E=%f step=%f\n",aStep->GetTotalEnergyDeposit(),Egamma/keV,aStep->GetStepLength());
        }
    }

    //--  gamma ENTER absorber ???

    //printf("----------- test ----------------- fGeomBoundary-%d(%d) \n",aStep->GetPostStepPoint()->GetStepStatus(),fGeomBoundary);

    if (aStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary && aStep->GetPostStepPoint()->GetPhysicalVolume()) {
        if ((aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName() == "World") &&
            ((postStepVolumeName == "Absorber") || (postStepVolumeName == "pxdPixel")) &&
            (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "Transportation") && (aStep->GetTrack()->GetMomentumDirection().z() > 0.) &&
            (aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName() == "gamma")) {
            Egamma = aStep->GetTrack()->GetKineticEnergy();
            fHistos->FillGammaInSpectrum(Egamma);
            //printf("I::test PreStep = %s ",aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName().c_str());
            //printf("I::test PostStep = %s \n",postStepVolumeName.c_str());
            //printf("I::gammas ENTER absorber de=%f E=%f ID=%d\n",aStep->GetTotalEnergyDeposit()/keV,Egamma/keV,IDnow);

            xp = aStep->GetTrack()->GetMomentumDirection().x();
            yp = aStep->GetTrack()->GetMomentumDirection().y();
            zp = aStep->GetTrack()->GetMomentumDirection().z();
            rp = std::sqrt(yp * yp + xp * xp);
            Theta = std::acos(zp);
            fHistos->FillGamAngle(Theta);
            Ttrans = aStep->GetTrack()->GetKineticEnergy();
            fHistos->FillTt(Ttrans);
            xend = x;
            yend = y;
            zend = z;
            rend = std::sqrt(yend * yend + zend * zend);
        }
    }
    //------------------
}