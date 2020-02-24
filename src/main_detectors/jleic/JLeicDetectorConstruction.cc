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
// * Neither the authors of this software system, nox1r their employing *
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


#include <vector>

#include "JLeicDetectorConstruction.hh"
#include "JLeicCalorimeterSD.hh"
#include "JLeicVertexSD.hh"

#include "G4SDManager.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"
#include "G4RegionStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4ProductionCuts.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

// export geometry through VGM
#include "GeometryExport.hh"


JLeicDetectorConstruction::JLeicDetectorConstruction(g4e::InitializationContext *initContext) :
    fWorldChanged(false),
    fInitContext(initContext)
{
    fDetectorMessenger = new JLeicDetectorMessenger(this);
    fMat = new g4e::Materials();
}


JLeicDetectorConstruction::~JLeicDetectorConstruction()
{
    delete fDetectorMessenger;
    delete fMat;
}


G4VPhysicalVolume *JLeicDetectorConstruction::Construct()
{
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    SetUpJLEIC2019();
    //checkVolumeOverlap();// --- fsv
    return World_Phys;
}


void JLeicDetectorConstruction::Create_ci_Endcap(JLeicDetectorConfig::ci_Endcap_Config cfg)
{
    /// This function creates ION-ENDCAP (but doesn't fill its contents)
  
    // Make endcup radius the same as Barrel Hadron Calorimeter
    ci_ENDCAP_GVol_Solid = new G4Tubs("ci_ENDCAP_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
    ci_ENDCAP_GVol_Logic = new G4LogicalVolume(ci_ENDCAP_GVol_Solid, World_Material, "ci_ENDCAP_GVol_Logic");
    ci_ENDCAP_GVol_Phys = new G4PVPlacement(nullptr, G4ThreeVector(cfg.PosX, 0, cfg.PosZ), "ci_ENDCAP_GVol_Phys", ci_ENDCAP_GVol_Logic, World_Phys, false, 0);

    // Visual attributes
    ci_ENDCAP_GVol_VisAttr = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    ci_ENDCAP_GVol_VisAttr->SetLineWidth(1);
    ci_ENDCAP_GVol_VisAttr->SetForceSolid(false);
    ci_ENDCAP_GVol_Logic->SetVisAttributes(ci_ENDCAP_GVol_VisAttr);
  
}


void JLeicDetectorConstruction::Create_ce_Endcap(JLeicDetectorConfig::ce_Endcap_Config cfg)
{
    /// This function creates ELECTRON-ENDCAP (but doesn't fill its contents)

    ce_ENDCAP_GVol_Solid = new G4Tubs("ce_ENDCAP_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
    ce_ENDCAP_GVol_Logic = new G4LogicalVolume(ce_ENDCAP_GVol_Solid, World_Material, "ce_ENDCAP_GVol_Logic");
    ce_ENDCAP_GVol_Phys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, cfg.PosZ), "ce_ENDCAP_GVol_Phys", ce_ENDCAP_GVol_Logic, World_Phys, false, 0);

    // Visual attributes
    ce_ENDCAP_VisAttr = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    ce_ENDCAP_VisAttr->SetLineWidth(1);
    ce_ENDCAP_VisAttr->SetForceSolid(true);
    ce_ENDCAP_GVol_Logic->SetVisAttributes(ce_ENDCAP_VisAttr);
}



void JLeicDetectorConstruction::SetUpJLEIC2019()
{
    using namespace fmt;


    //===================================================================================
    //==                    create a world                                            ==
    //===================================================================================

    //Different Shifts for 0 IP
    if(fConfig.BeamlineName == "erhic") {fConfig.World.ShiftVTX=0.;} else { fConfig.World.ShiftVTX=40*cm;};


   // World_Material    = Air;
   World_Material = fMat->GetMaterial("G4_Galactic");
    World_Solid = new G4Box("World_Solid", fConfig.World.SizeR, fConfig.World.SizeR, fConfig.World.SizeZ / 2.);
    World_Logic = new G4LogicalVolume(World_Solid, World_Material, "World_Logic");
    World_Phys = new G4PVPlacement(nullptr, G4ThreeVector(), "World_Phys", World_Logic, nullptr, false, 0);

    printf("==>> create a world : xyz= %f %f %f [m]\n", fConfig.World.SizeR * 2 / m, fConfig.World.SizeR * 2 / m, fConfig.World.SizeZ / m);
    G4cout << "World_Solid->GetCubicVolume() = " << World_Solid->GetCubicVolume() << std::endl;
    G4cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  " << fConfig.World.SizeR << std::endl;
    G4cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  " << fConfig.World.SizeZ << std::endl;

    printf("World_Solid->GetCubicVolume() %f \n", World_Solid->GetCubicVolume());
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!! SizeR %f \n", fConfig.World.SizeR);
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!! SizeZ %f \n", fConfig.World.SizeZ);


    //==========================================================================
    //                          B E A M   E L E M E N T S
    //==========================================================================
    // -- use JLEIC  lattice
    /*
    ir_Lattice.SetMotherParams(World_Phys, World_Material);
    ir_Lattice.SetIonBeamEnergy(fConfig.IonBeamEnergy);
    ir_Lattice.SetElectronBeamEnergy(fConfig.ElectronBeamEnergy);

    //   ir_Lattice.LoadIonBeamLattice();
    //   ir_Lattice.LoadElectronBeamLattice();
    ir_Lattice.Read_JLEIC_ion_beam_lattice();
    ir_Lattice.Read_JLEIC_electron_beam_lattice();
    */
    if(USE_FFQs )
    {
        int USE_LINE = 1;
        std::string fileName;
        printf("AcceleratorMagnets start... Ion energy %d   Electron energy %d World_Phys=%p \n ", fConfig.IonBeamEnergy, fConfig.ElectronBeamEnergy, World_Phys);
        const char *g4eHomeCStr = std::getenv("G4E_HOME");
        //---------------- Electron  line -----------------------------------
        printf("========================================\n");
        if (g4eHomeCStr) {
            if (fConfig.BeamlineName == "erhic") {
                USE_LINE =1;
                fileName = fmt::format("{}/resources/erhic/mdi/e_ir_{}.txt", g4eHomeCStr, fConfig.ElectronBeamEnergy);
            } else {
                USE_LINE = 0;
                fileName = fmt::format("{}/resources/jleic/mdi/e_ir_{}.txt", g4eHomeCStr, fConfig.ElectronBeamEnergy);
            }
        } else {
            printf("AcceleratorMagnets  file opening err :: please setup  G4E_HOME \n");
        }

        printf("AcceleratorMagnets:: try to open file %s \n", fileName.c_str());

        AcceleratorMagnets *electron_line_magnets = new AcceleratorMagnets(fileName, World_Phys, World_Material, USE_LINE);



      //-----------------Ion line -----------------------------------
      printf("========================================\n");
      if (g4eHomeCStr) {
          if (fConfig.BeamlineName == "erhic") {
              USE_LINE=1;
              fileName = fmt::format("{}/resources/erhic/mdi/ion_ir_{}.txt", g4eHomeCStr, fConfig.IonBeamEnergy);
          } else if (fConfig.BeamlineName == "jleic") {
              USE_LINE=0;
              fileName = fmt::format("{}/resources/jleic/mdi/ion_ir_{}.txt", g4eHomeCStr, fConfig.IonBeamEnergy);
          } else {
              G4Exception("JLeicDetectorConstruction::Construct",
                          "InvalidSetup", FatalException,
                          "BeamlineName should be 'erhic' or 'jleic'");
          }
      } else {
          G4Exception("JLeicDetectorConstruction::Construct",
                      "InvalidSetup", FatalException,
                      "AcceleratorMagnets  file opening err :: please setup env. G4E_HOME");
      }

      printf("AcceleratorMagnets:: try to open file %s \n", fileName.c_str());


      ion_line_magnets = new AcceleratorMagnets(fileName, World_Phys, World_Material, USE_LINE);

  }
    //=========================================================================
    //                    Beampipe
    //=========================================================================
   if(USE_BEAMPIPE ) {

       fConfig.ir_Beampipe.Zpos = fConfig.World.ShiftVTX;
       ir_Beampipe.ConstructCentral(fConfig.ir_Beampipe, World_Material, World_Phys);
       ir_Beampipe.ConstructForwardCone(fConfig.ir_Beampipe, World_Material, World_Phys);

   }
    //=========================================================================
    //                    Sensitive detectors
    //=========================================================================

    G4SDManager *SDman = G4SDManager::GetSDMpointer();

    if (!fCalorimeterSD) {
        fCalorimeterSD = new JLeicCalorimeterSD("CalorSD", fInitContext->RootManager->GetJLeicRootOutput(), this);
        SDman->AddNewDetector(fCalorimeterSD);
    }
    if (!fVertexSD) {
        fVertexSD = new JLeicVertexSD("VertexSD", fInitContext->RootManager->GetJLeicRootOutput(), this);
        SDman->AddNewDetector(fVertexSD);
        printf("VertexSD done\n");
    }

    //=========================================================================
    //                    Create Central Detector
    //=========================================================================

    if (USE_BARREL) {
        //----------------------CREATE SOLENOID ---------------------------------------------
        fConfig.cb_Solenoid.ShiftZ = fConfig.World.ShiftVTX;
        cb_Solenoid.Construct(fConfig.cb_Solenoid, World_Material, World_Phys);
    }


    if (USE_E_ENDCAP) {
        // ------------------ create electon endcap ---------------------------------------------
        fConfig.ce_Endcap.ROut = fConfig.cb_Solenoid.ROut -1 * cm;
        fConfig.ce_Endcap.PosZ = -fConfig.ce_Endcap.SizeZ / 2 - fConfig.cb_Solenoid.SizeZ / 2 + fConfig.World.ShiftVTX -2*cm;
        Create_ce_Endcap(fConfig.ce_Endcap);
    }

    if (USE_CB_HCAL) {
        //----------------create  HCAL ( Iron  BARREL) ---------------------------------------
        fConfig.cb_HCAL.RIn = fConfig.cb_Solenoid.ROut;
        fConfig.cb_HCAL.ROut = fConfig.cb_Solenoid.ROut + fConfig.cb_HCAL.Thickness;
        fConfig.cb_HCAL.SizeZ = fConfig.cb_Solenoid.SizeZ + fConfig.ce_Endcap.SizeZ;
        //  fConfig.cb_HCAL.SizeZ = fConfig.cb_Solenoid.SizeZ ;

        fConfig.cb_HCAL.ShiftZ = -fConfig.ce_Endcap.SizeZ / 2. + fConfig.World.ShiftVTX;

        //  fConfig.cb_HCAL.ShiftZ = fConfig.World.ShiftVTX;
        cb_HCAL.Construct(fConfig.cb_HCAL, World_Material, World_Phys);

        //----------create a layered structure for the Iron ----------------------------------------
        if (USE_CB_HCAL_D) {
            cb_HCAL.ConstructLayers();
            // cb_HCAL.ConstructRings();
        }
    }


    if (USE_CI_ENDCAP) {
        //----------------- create Hadron endcap (ci_Endcap) ----------------------------------------
        if (USE_CB_HCAL) {
            fConfig.ci_Endcap.ROut = fConfig.cb_HCAL.ROut;
        } else {
            fConfig.ci_Endcap.ROut = fConfig.cb_Solenoid.ROut + 100 * cm;
        }


        fConfig.ci_Endcap.PosZ = fConfig.cb_Solenoid.SizeZ / 2. + fConfig.World.ShiftVTX + fConfig.ci_Endcap.ShiftZ + fConfig.ci_Endcap.SizeZ / 2.;
        Create_ci_Endcap(fConfig.ci_Endcap);

        //------------------ create    HCAL in hadron endcap ---------------------------------------

        if (USE_CI_HCAL) {
            // G4double ci_HCAL_GVol_RIn[2]={60*cm, 70*cm } ;
            //  G4double ci_HCAL_GVol_RIn[2]={0*cm, 0*cm }
            // G4double ci_HCAL_GVol_ROut[2]={cb_HCAL_GVol_ROut,cb_HCAL_GVol_ROut };

            if (USE_CB_HCAL) { fConfig.ci_HCAL.ROut = fConfig.cb_HCAL.ROut; }
            else { fConfig.ci_HCAL.ROut = 300 * cm; }

            fConfig.ci_HCAL.PosZ = fConfig.ci_Endcap.PosZ + fConfig.ci_Endcap.SizeZ / 2 + fConfig.ci_HCAL.ShiftZ + fConfig.ci_HCAL.SizeZ / 2;
            ci_HCAL.Construct(fConfig.ci_HCAL, World_Material, World_Phys);

            //---------------------------- HCAL IRON--------------------------------------
            if (USE_CI_HCAL_D) {
                if(fConfig.BeamlineName == "jleic") {fConfig.ci_HCAL.det_RIn=80*cm;}
                if(fConfig.BeamlineName == "erhic") {fConfig.ci_HCAL.det_RIn=60*cm;}
                ci_HCAL.ConstructDetectors(fConfig.ci_HCAL);
            }
        }
    }


    //***********************************************************************************
    //***********************************************************************************
    //**                                DETECTOR VOLUMES                               **
    //***********************************************************************************
    //***********************************************************************************


    if (USE_BARREL && USE_BARREL_det)  {
        //===================================================================================
        //==                          VERTEX DETECTOR VOLUME                               ==
        //===================================================================================

        if (USE_CB_VTX) {

            fConfig.cb_VTX.ShiftZ = -fConfig.World.ShiftVTX;
            cb_VTX.Construct(fConfig.cb_VTX, World_Material, cb_Solenoid.Phys);

            if (USE_CB_VTX_LADDERS) {
                //----------vtx barrel ladder geometry--------------
                cb_VTX.ConstructLaddersCentral();
                for (size_t lay = 0; lay < cb_VTX.Lays.size(); lay++) {
                    if (cb_VTX.cb_VTX_ladder_Logic) {
                        cb_VTX.cb_VTX_ladder_Logic[lay]->SetSensitiveDetector(fVertexSD);
                    }
                }
            }
            if (USE_CB_VTX_ENDCAPS) {
                cb_VTX.ConstructLaddersEndcaps();
                //         if (fLogicVTXEndH[lay]) { fLogicVTXEndH[lay]->SetSensitiveDetector(fCalorimeterSD); }
            }

        };    // end VTX detector
        //===================================================================================
        //==                          Silicone detector along the beamline                  ==
        //===================================================================================

        if (USE_CB_SiDISCS) {

            fConfig.cb_SiDISCS.Zpos = 100.*cm;
            cb_SiDISCS.Construct(fConfig.cb_SiDISCS, World_Material, cb_Solenoid.Phys);
        }


        //===================================================================================
        //==                         CTD DETECTOR                                          ==
        //===================================================================================
        if (USE_CB_CTD) {

            fConfig.cb_CTD.SizeZ = fConfig.cb_Solenoid.SizeZ - fConfig.cb_CTD.SizeZCut;
            cb_CTD.Construct(fConfig.cb_CTD, World_Material, cb_Solenoid.Phys);

            if (USE_CB_CTD_Si) {
                printf("Det construction cb_CTD_detSi::1 Number of layers=%d \n",fConfig.cb_CTD.SiLayerCount);
                cb_CTD.ConstructLadders();
                printf("Det construction cb_CTD_detSi::2 Number of layers =%d \n ",fConfig.cb_CTD.SiLayerCount);

               for (int lay = 0; lay < fConfig.cb_CTD.SiLayerCount; lay++) {
                  if(cb_CTD.SiLogics[lay]) cb_CTD.SiLogics[lay]->SetSensitiveDetector(fVertexSD);
               }

            }
            else if (USE_CB_CTD_Straw) { cb_CTD.ConstructStraws(); }

        };// end CTD detector


        //===================================================================================
        //==                         RICH DETECTOR                                         ==
        //===================================================================================
        if (USE_CB_DIRC) {

            fConfig.cb_DIRC.RIn = fConfig.cb_CTD.ROut + 1 * cm;
            // for new magnet
            //   cb_DIRC_GVol_ROut = 95 * cm;
            // for CLEO and BABAR DIRC
            fConfig.cb_DIRC.ROut = fConfig.cb_DIRC.RIn + 10 * cm;
            //   cb_DIRC_GVol_SizeZ = SizeZ;
            fConfig.cb_DIRC.SizeZ = fConfig.cb_CTD.SizeZ;

            cb_DIRC.Construct(fConfig.cb_DIRC, World_Material, cb_Solenoid.Phys);

            if (USE_CB_DIRC_bars) {
                cb_DIRC.ConstructBars();
                cb_DIRC.cb_DIRC_bars_Logic->SetSensitiveDetector(fCalorimeterSD);
            }

        }; // end DIRC detector


        //===================================================================================
        //==                         EMCAL DETECTOR VOLUME                                 ==
        //===================================================================================
        if (USE_CB_EMCAL) {

            fConfig.cb_EMCAL.ROut = fConfig.cb_Solenoid.ROut - fConfig.cb_EMCAL.ROutshift;
            fConfig.cb_EMCAL.RIn = fConfig.cb_EMCAL.ROut - fConfig.cb_EMCAL.Thickness;
            // cb_EMCAL_GVol_SizeZ=SizeZ -30*cm;

            cb_EMCAL.Construct(fConfig.cb_EMCAL, fConfig.cb_Solenoid, World_Material, cb_Solenoid.Phys);
            cb_EMCAL.ConstructBars();
            cb_EMCAL.Logic->SetSensitiveDetector(fCalorimeterSD);
        }


    }  // end Barrel



    // ***********************************************************************************
    //                       CE_ENDCAP
    // ***********************************************************************************
    if (USE_E_ENDCAP) {


        //===================================================================================
        // ==                      GEM     Hadron endcap                                ==
        //==================================================================================

        if (USE_CE_GEM) {
            fConfig.ce_GEM.PosZ = -fConfig.cb_Solenoid.SizeZ / 2 + fConfig.ce_GEM.SizeZ / 2;

            ce_GEM.Construct(fConfig.ce_GEM, World_Material, cb_Solenoid.Phys);
            ce_GEM.ConstructDetectors();
               for (int lay = 0; lay < fConfig.ce_GEM.Nlayers; lay++) {
                  if (ce_GEM.ce_GEM_lay_Logic[lay]) ce_GEM.ce_GEM_lay_Logic[lay]->SetSensitiveDetector(fCalorimeterSD);
               }

        }  // end USE_CI_GEM

//===================================================================================
//                         mRICH
//===================================================================================

        if (USE_CE_MRICH) {
            fConfig.ce_MRICH.PosZ = fConfig.ce_Endcap.SizeZ / 2 - fConfig.ce_MRICH.SizeZ / 2 - 2 * cm;

            ce_MRICH.Construct(fConfig.ce_MRICH, World_Material, ce_ENDCAP_GVol_Phys);

            ce_MRICH.ConstructModules();

        }
//===================================================================================
//                         CE_EMCAL
//===================================================================================
        if (USE_CE_EMCAL) {
            fConfig.ce_EMCAL.PosZ = -fConfig.ce_Endcap.SizeZ / 2 + fConfig.ce_EMCAL.Thickness / 2.;
            fConfig.ce_EMCAL.ROut = fConfig.ce_Endcap.ROut -3*cm;


            ce_EMCAL.Construct(fConfig.ce_EMCAL, World_Material, ce_ENDCAP_GVol_Phys);
            ce_EMCAL.ConstructCrystals(); // --- inner detector with Crystals
            ce_EMCAL.ce_EMCAL_detPWO_Logic->SetSensitiveDetector(fCalorimeterSD);
            ce_EMCAL.ConstructGlass();    // --- outer part with Glass
            ce_EMCAL.ce_EMCAL_detGLASS_Logic->SetSensitiveDetector(fCalorimeterSD);

        }

    } //------------------end USE_E_ENDCAP -----------------------------------------------
//
//
// ***********************************************************************************
//                       CI_ENDCAP
// ***********************************************************************************
    if (USE_CI_ENDCAP) {

    //===================================================================================
    // ==                      GEM     Hadron endcap                                ==
    //==================================================================================

        if (USE_CI_GEM) {
            fConfig.ci_GEM.PosZ = fConfig.cb_Solenoid.SizeZ / 2 - fConfig.ci_GEM.SizeZ / 2;   // --- need to find out why this 5 cm are needed
          if(fConfig.BeamlineName == "jleic")  { fConfig.ci_GEM.PosX = -5 * cm;}  // --- different crossing angle direction for JLEIC
          else if ( fConfig.BeamlineName == "erhic" ){ fConfig.ci_GEM.PosX = 5 * cm;} // --- different crossing angle direction for eRHIC
            else {  fConfig.ci_GEM.PosX = 0 * cm;}

            ci_GEM.Construct(fConfig.ci_GEM, World_Material, cb_Solenoid.Phys);
            ci_GEM.ConstructDetectors();
            for (int lay = 0; lay < fConfig.ci_GEM.Nlayers; lay++) {
                if (ci_GEM.lay_Logic[lay]) ci_GEM.lay_Logic[lay]->SetSensitiveDetector(fCalorimeterSD);
            }
        }  // end USE_CI_GEM

        if (USE_CI_DRICH) {
            //===================================================================================
            // ==                       dRICH     Hadron endcap                                ==
            //==================================================================================
            fConfig.ci_DRICH.RIn = fConfig.ci_Endcap.RIn;

            fConfig.ci_DRICH.PosZ = -fConfig.ci_Endcap.SizeZ / 2. + fConfig.ci_DRICH.ThicknessZ / 2.;
            //    double ci_DRICH_GVol_PosZ= 0*cm;
            ci_DRICH.Construct(fConfig.ci_DRICH, World_Material, ci_ENDCAP_GVol_Phys);
            ci_DRICH.ConstructDetectors();

            //===================================================================================
        } // end USE_CI_DRICH


        if (USE_CI_TRD) {
            //===================================================================================
            // ==                       TRD     Hadron endcap                                ==
            //==================================================================================
            //   ci_TRD_GVol_PosZ = -fConfig.ci_Endcap.SizeZ / 2 + fConfig.ci_DRICH.ThicknessZ + ci_TRD_GVol_ThicknessZ/2.;

            fConfig.ci_TRD.RIn = fConfig.ci_Endcap.RIn;

            fConfig.ci_TRD.PosZ = -fConfig.ci_Endcap.SizeZ / 2. + fConfig.ci_DRICH.ThicknessZ + fConfig.ci_TRD.ThicknessZ / 2.;
            //    double ci_DRICH_GVol_PosZ= 0*cm;
            ci_TRD.Construct(fConfig.ci_TRD, World_Material, ci_ENDCAP_GVol_Phys);

            ci_TRD.ConstructDetectors();
            printf("FoilNumbers=%d (%d) \n", fConfig.ci_TRD.fFoilNumber, ci_TRD.ConstructionConfig.fFoilNumber); // --- ????????? ---
            //ci_TRD.ConstructionConfig.fFoilNumber
            //===================================================================================
        }// end USE_CI_TRD

        if (USE_CI_EMCAL) {
            //===================================================================================
            // ==                      CI_EMCAL    Hadron endcap                              ==
            //==================================================================================

            fConfig.ci_EMCAL.PosZ = -fConfig.ci_Endcap.SizeZ / 2 + fConfig.ci_DRICH.ThicknessZ + fConfig.ci_TRD.ThicknessZ + fConfig.ci_EMCAL.ThicknessZ / 2;
            if(fConfig.BeamlineName == "jleic")  {
                fConfig.ci_EMCAL.USE_JLEIC = true;
                fConfig.ci_EMCAL.det_Rin1 = 20*cm;
                fConfig.ci_EMCAL.det_Rin2 = 55*cm;
            }
            if (fConfig.BeamlineName == "erhic") {
                fConfig.ci_EMCAL.USE_ERHIC=true;
                fConfig.ci_EMCAL.det_Rin1=30*cm;
                fConfig.ci_EMCAL.det_Rin2=30*cm;
            }

            ci_EMCAL.Construct(fConfig.ci_EMCAL, World_Material, ci_ENDCAP_GVol_Phys);
            ci_EMCAL.ConstructDetectors();    // --- outer part with Glass
            ci_EMCAL.ci_EMCAL_det_Logic->SetSensitiveDetector(fCalorimeterSD);

        } // end USE_CI_EMCAL
    } // ============end USE_CI_ENDCAP  ===================================

//****************************************************************************************
//==                         Forward Detectors                                          ==
//****************************************************************************************

    //====================================================================================
    //==                          DIPOLE-1 Tracker and EMCAL                            ==
    //====================================================================================

    if (USE_FI_D1TRK) {
        //-------------------------------------------------------------------------------
        //                      Place Si_disks inside D1a ir B0
        //-------------------------------------------------------------------------------

        for (size_t i = 0; i < ion_line_magnets->allmagnets.size(); i++) {
            if ((fConfig.BeamlineName == "jleic" && ion_line_magnets->allmagnets.at(i)->name == "iBDS1a") || (fConfig.BeamlineName == "erhic" && ion_line_magnets->allmagnets.at(i)->name == "iB0PF")) {
                if(fConfig.BeamlineName == "erhic") {
                    fConfig.fi_D1TRK.PhiStart=-130.* deg;
                    fConfig.fi_D1TRK.PhiTot=275 * deg;
                } else if (fConfig.BeamlineName=="jleic") {
                    fConfig.fi_D1TRK.PhiStart=170.;
                    fConfig.fi_D1TRK.PhiTot=330 * deg;
                };
                fConfig.fi_D1TRK.ROut = ion_line_magnets->allmagnets.at(i)->Rin2 * cm;
                fConfig.fi_D1TRK.Zpos = (ion_line_magnets->allmagnets.at(i)->LengthZ / 2.) * cm - fConfig.fi_D1TRK.SizeZ / 2.;
                fi_D1TRK.ConstructA(fConfig.fi_D1TRK, World_Material, ion_line_magnets->allmagnets.at(i)->fPhysics_BigDi_m);
                fi_D1TRK.ConstructDetectorsA();
                for (int lay = 0; lay < fConfig.fi_D1TRK.Nlayers; lay++) {
                  if (fi_D1TRK.f1_D1_Lay_Logic) fi_D1TRK.f1_D1_Lay_Logic->SetSensitiveDetector(fCalorimeterSD);
                 }
            }
        }
     }

    //------------------------------------------------
    if (USE_CI_HCAL) {

        if (USE_FI_D1EMCAL) {
            // Ecal module  AFTER !!!   Dipole1

            fConfig.fi_D1EMCAL.Zpos = -fConfig.ci_HCAL.SizeZ / 2 + fConfig.fi_D1EMCAL.SizeZ / 2;

            fConfig.fi_D1EMCAL.rot_matx.rotateY(fConfig.fi_D1EMCAL.Angle * rad);
            fi_D1EMCAL.Construct(fConfig.fi_D1EMCAL, World_Material, ci_HCAL.Phys);


        }
    }

    //====================================================================================
    //==                    Far-Forward Area    D2, D3  ZDC. Roman Pots                 ==
    //====================================================================================
    if (USE_FFI_D2TRK) {

        for (int i = 0; i < ion_line_magnets->allmagnets.size(); i++) {
            if ((fConfig.BeamlineName == "jleic" && ion_line_magnets->allmagnets.at(i)->name == "iBDS2")) {

                fConfig.ffi_D2TRK.RIn = 0 * cm;
                fConfig.ffi_D2TRK.ROut = ion_line_magnets->allmagnets.at(i)->Rin2 * cm -0.1*cm;
                fConfig.ffi_D2TRK.SizeZ = ion_line_magnets->allmagnets.at(i)->LengthZ  * m - 2. * cm;

                ffi_D2TRK.Construct(fConfig.ffi_D2TRK, World_Material, ion_line_magnets->allmagnets.at(i)->fPhysics_BigDi_m);
                ffi_D2TRK.ConstructDetectors();
                for (int lay = 0; lay < fConfig.ffi_D2TRK.Nlayers; lay++) {
                  if (ffi_D2TRK.lay_Logic) ffi_D2TRK.lay_Logic->SetSensitiveDetector(fCalorimeterSD);
                }
            }
        }
    }

    //------------------------------------------------
    //             ZDC
    //------------------------------------------------
    if (USE_FFI_ZDC) {
        if (fConfig.BeamlineName == "jleic") {
            fConfig.ffi_ZDC.Angle=-0.0265;
        fConfig.ffi_ZDC.rot_matx.rotateY(fConfig.ffi_ZDC.Angle * rad);
        fConfig.ffi_ZDC.Zpos = 4000 * cm;
        fConfig.ffi_ZDC.Xpos = -190 * cm;
        }
    if(fConfig.BeamlineName == "erhic") {
          fConfig.ffi_ZDC.Angle=-0.0125;
          fConfig.ffi_ZDC.rot_matx.rotateY(-fConfig.ffi_ZDC.Angle * rad);
          fConfig.ffi_ZDC.Zpos = 3800 * cm;
          fConfig.ffi_ZDC.Xpos = 98.5 * cm;
    }

        ffi_ZDC.Construct(fConfig.ffi_ZDC, World_Material, World_Phys);
        ffi_ZDC.ConstructTowels();
        if (ffi_ZDC.Logic) ffi_ZDC.Logic->SetSensitiveDetector(fCalorimeterSD);

    } // end ffi_ZDC

    //------------------------------------------------
    //------------------------------------------------
    //            Roman Pots for eRHIC
    //------------------------------------------------
  if (fConfig.BeamlineName == "erhic") {

          if (USE_FFI_RPOT_D2 ) {  //---- First Roman Pot
               fConfig.ffi_RPOT_D2.Angle = 0.025;
                fConfig.ffi_RPOT_D2.ROut = 25 * cm;
              fConfig.ffi_RPOT_D2.rot_matx.rotateY(fConfig.ffi_RPOT_D2.Angle * rad);
              fConfig.ffi_RPOT_D2.PosZ = 2620 * cm;
              fConfig.ffi_RPOT_D2.PosX = 82 * cm;

              ffi_RPOT_D2.Construct(fConfig.ffi_RPOT_D2, World_Material, World_Phys);
              if (ffi_RPOT_D2.Logic) ffi_RPOT_D2.Logic->SetSensitiveDetector(fCalorimeterSD);

          }
           //------------------------------------------------
        if (USE_FFI_RPOT_D3 ) {
          fConfig.ffi_RPOT_D3.Angle = 0.025;
          fConfig.ffi_RPOT_D3.rot_matx.rotateY(fConfig.ffi_RPOT_D3.Angle * rad);
          fConfig.ffi_RPOT_D3.PosZ = 2820 * cm;
          fConfig.ffi_RPOT_D3.PosX = 91 * cm;

          ffi_RPOT_D3.Construct(fConfig.ffi_RPOT_D3, World_Material, World_Phys);
          if (ffi_RPOT_D3.Logic) ffi_RPOT_D3.Logic->SetSensitiveDetector(fCalorimeterSD);

      } // end ffi_RPOT_D3


  }
      //------------------------------------------------
    //            Roman Pots for JLEIC
    //------------------------------------------------

    //------------------------------------------------
  if(fConfig.BeamlineName == "jleic") {
      if (USE_FFI_RPOT_D2 ) {
            fConfig.ffi_RPOT_D2.Angle=-0.05;
             fConfig.ffi_RPOT_D2.ROut = 120 * cm;
          fConfig.ffi_RPOT_D2.rot_matx.rotateY(fConfig.ffi_RPOT_D2.Angle * rad);
          fConfig.ffi_RPOT_D2.PosZ = 3100 * cm;
          fConfig.ffi_RPOT_D2.PosX = -170 * cm;

          ffi_RPOT_D2.Construct(fConfig.ffi_RPOT_D2, World_Material, World_Phys);
          if (ffi_RPOT_D2.Logic) ffi_RPOT_D2.Logic->SetSensitiveDetector(fCalorimeterSD);

      }// end ffi_RPOT_D2
      //------------------------------------------------
      if (USE_FFI_RPOT_D3 ) {
          fConfig.ffi_RPOT_D3.Angle = -0.053;
          fConfig.ffi_RPOT_D3.rot_matx.rotateY(fConfig.ffi_RPOT_D3.Angle * rad);
          fConfig.ffi_RPOT_D3.PosZ = 5000 * cm;
          fConfig.ffi_RPOT_D3.PosX = -153 * cm;

          ffi_RPOT_D3.Construct(fConfig.ffi_RPOT_D3, World_Material, World_Phys);
          if (ffi_RPOT_D3.Logic) ffi_RPOT_D3.Logic->SetSensitiveDetector(fCalorimeterSD);

      } // end ffi_RPOT_D3
  }
    //************************************************************************************
    //==                         Rear  Detectors                                     ==
    //************************************************************************************

    //===================================================================================
    //==                        Compton Polarimeter                                  ==
    //===================================================================================
    if (USE_FFE_CPOL) {

        ffe_CPOL.Construct(fConfig.ffe_CPOL, World_Material, World_Phys);
    } // end ffe_CPOL

    //===================================================================================
    //==                        Lumi                                                  ==
    //===================================================================================

    if (USE_FFE_LUMI) {
         fConfig.ffe_LUMI.PosX=+0.7*m;
         fConfig.ffe_LUMI.PosY=0;
         fConfig.ffe_LUMI.PosZ=-25*m;

         ffe_LUMI.Construct(fConfig.ffe_LUMI, World_Material, World_Phys);
        //ffe_LUMI.ConstructDetectors(fConfig.ffe_LUMI);
    } // end ffe_LUMI

   //===================================================================================
    //==                        Low-Q2                                                  ==
    //===================================================================================

    if (USE_FFE_LOWQ2) {
         fConfig.ffe_LOWQ2.PosX=+0.3*m;
         fConfig.ffe_LOWQ2.PosY=0;
   //just after dipole      fConfig.ffe_LOWQ2.PosZ=-18*m;
      fConfig.ffe_LOWQ2.PosZ=-28*m;

         ffe_LOWQ2.Construct(fConfig.ffe_LOWQ2, World_Material, World_Phys);
          ffe_LOWQ2.ConstructDetectors();
          for (int lay = 0; lay < fConfig.ffe_LOWQ2.Nlayers; lay++) {
             if (ffe_LOWQ2.lay_Logic) ffe_LOWQ2.Logic->SetSensitiveDetector(fCalorimeterSD);
          }
             if (ffe_LOWQ2.BPC_Logic) ffe_LOWQ2.BPC_Logic->SetSensitiveDetector(fCalorimeterSD);
     } // end ffe_LOWQ2



   //===================================================================================
   //                     END detector construction.... Exporting geometry
   //===================================================================================

    spdlog::info(" - exporting geometry");
    g4e::GeometryExport::Export(fInitContext->Arguments->OutputBaseName, World_Phys);

    PrintGeometryParameters();

    printf("FoilNumbers3=%d\n", ci_TRD.ConstructionConfig.fFoilNumber);
    printf("exit Detector Construction\n");
}


////////////////////////////////////////////////////////////////////////////
//
//
void JLeicDetectorConstruction::PrintGeometryParameters()
{
    G4cout << "\n The  WORLD   is made of " << fConfig.World.SizeZ / mm << "mm of " << World_Material->GetName();
    G4cout << ", the transverse size (R) of the world is " << fConfig.World.SizeR / mm << " mm. " << G4endl;
    G4cout << "WorldMaterial = " << World_Material->GetName() << G4endl;
    //  G4cout<<"fVTX_END_Z = "<<fVTX_END_Z/mm<<" mm"<<G4endl;
    G4cout << G4endl;
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetAbsorberMaterial(G4String materialChoice)
{
    // get the pointer to the material table
    const G4MaterialTable *theMaterialTable = G4Material::GetMaterialTable();

    // search the material by its name
    G4Material *pttoMaterial;

    for (size_t J = 0; J < theMaterialTable->size(); J++) {
        pttoMaterial = (*theMaterialTable)[J];

        if (pttoMaterial->GetName() == materialChoice) {
            fAbsorberMaterial = pttoMaterial;
            fLogicAbsorber->SetMaterial(pttoMaterial);
            // PrintCalorParameters();
        }
    }
}
///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetRadiatorMaterial(G4String materialChoice)
{
    // get the pointer to the material table

    const G4MaterialTable *theMaterialTable = G4Material::GetMaterialTable();

    // search the material by its name

    G4Material *pttoMaterial;
    for (size_t J = 0; J < theMaterialTable->size(); J++) {
        pttoMaterial = (*theMaterialTable)[J];

        if (pttoMaterial->GetName() == materialChoice) {
            fConfig.ci_TRD.fRadiatorMat = pttoMaterial;
            fLogicRadSlice->SetMaterial(pttoMaterial);
            // PrintCalorParameters();
        }
    }
}

////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
    // get the pointer to the material table
    const G4MaterialTable *theMaterialTable = G4Material::GetMaterialTable();

    // search the material by its name
    G4Material *pttoMaterial;

    for (size_t J = 0; J < theMaterialTable->size(); J++) {
        pttoMaterial = (*theMaterialTable)[J];

        if (pttoMaterial->GetName() == materialChoice) {
            World_Material = pttoMaterial;
            World_Logic->SetMaterial(pttoMaterial);
            //  PrintCalorParameters();
        }
    }
}


///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetAbsorberThickness(G4double val)
{
    // change Absorber thickness and recompute the calorimeter parameters
    fConfig.ci_TRD.fAbsorberThickness = val;
    //  ComputeCalorParameters();
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetRadiatorThickness(G4double val)
{
    // change XTR radiator thickness and recompute the calorimeter parameters
    fConfig.ci_TRD.fRadThickness = val;
    // ComputeCalorParameters();
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetGasGapThickness(G4double val)
{
    // change XTR gas gap thickness and recompute the calorimeter parameters
    fConfig.ci_TRD.fGasGap = val;
    // ComputeCalorParameters();
}

/////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetAbsorberRadius(G4double val)
{
    // change the transverse size and recompute the calorimeter parameters
    fConfig.ci_TRD.fAbsorberRadius = val;
    // ComputeCalorParameters();
}

////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetWorldSizeZ(G4double val)
{
    fWorldChanged = true;
    fConfig.World.SizeZ = val;
    // ComputeCalorParameters();
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetWorldSizeR(G4double val)
{
    fWorldChanged = true;
    fConfig.World.SizeR = val;
    // ComputeCalorParameters();
}

//////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetAbsorberZpos(G4double val)
{
    fConfig.ci_TRD.fAbsorberZ = val;
    // ComputeCalorParameters();
}


void JLeicDetectorConstruction::UpdateGeometry()
{
    G4RunManager::GetRunManager()->DefineWorldVolume(World_Phys);
}


void JLeicDetectorConstruction::checkVolumeOverlap()
{
    // loop inside all the daughters volumes
    G4cout << " loop inside all the daughters volumes" << G4endl;
    //        bool bCheckOverlap;
    //        bCheckOverlap=false;

    int nSubWorlds, nSubWorlds2;
    for (int i = 0; i < (int) World_Phys->GetLogicalVolume()->GetNoDaughters(); i++) {
        World_Phys->GetLogicalVolume()->GetDaughter(i)->CheckOverlaps();
        nSubWorlds = (int) World_Phys->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetNoDaughters();
        for (int j = 0; j < nSubWorlds; j++) {
            World_Phys->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->CheckOverlaps();
            nSubWorlds2 = (int) World_Phys->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetNoDaughters();
            for (int k = 0; k < nSubWorlds2; k++) {
                World_Phys->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetDaughter(k)->CheckOverlaps();
            }
        }
    }
    G4cout << G4endl;
}

void JLeicDetectorConstruction::EnableHCalRings()
{
//    cb_HCAL.ConstructRings();
}
