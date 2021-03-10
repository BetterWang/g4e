#include <vector>

#include "ReferenceDetectorConstruction.hh"
#include "CommonCalorimeterSD.hh"
#include "CommonVertexSD.hh"
#include "subdetectors/ce_EMCAL/ce_EMCAL_SD.hh"

#include "G4SDManager.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"
#include "G4RegionStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VisAttributes.hh" 
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"

// export geometry through VGM
#include "GeometryExport.hh"
#include "VolumeChangeSteppingAction.hh"
#include "ReferenceDetectorMessenger.hh"
#include "DetectorConfig.hh"

ReferenceDetectorConstruction::ReferenceDetectorConstruction(g4e::InitializationContext *initContext, DetectorConfig &config) :
    fInitContext(initContext),
    fConfig(config),
    ce_EMCAL(fConfig.ce_EMCAL, initContext)
{
    fDetectorMessenger = new ReferenceDetectorMessenger(this);
    fMat = new g4e::Materials();

    // Transition Radiation Physics
    initContext->PhysicsList->RegisterPhysics(ci_TRD.PhysicsConstructor);

    // Add stepping action that is executed on volume change
    initContext->ActionInitialization->AddUserActionGenerator([initContext](){
        auto action = new g4e::VolumeChangeSteppingAction(initContext->RootManager);
        return static_cast<G4UserSteppingAction*>(action);
    });
}


ReferenceDetectorConstruction::~ReferenceDetectorConstruction()
{
    delete fDetectorMessenger;
    delete fMat;
}


G4VPhysicalVolume *ReferenceDetectorConstruction::Construct()
{
    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    using namespace fmt;

    //===================================================================================
    //==                    create a world                                            ==
    //===================================================================================

    // World_Material    = Air;
    World_Material = fMat->GetMaterial("G4_Galactic");
    World_Solid = new G4Box("World_Solid", fConfig.World.SizeR, fConfig.World.SizeR, fConfig.World.SizeZ / 2.);
    World_Logic = new G4LogicalVolume(World_Solid, World_Material, "World_Logic");
    fWorldPhysical = new G4PVPlacement(nullptr, G4ThreeVector(), "World_Phys", World_Logic, nullptr, false, 0);

    fmt::print("Initializing WORLD. x:{:<10}m y:{:<10}m z:{:<10}m", fConfig.World.SizeR * 2 / m, fConfig.World.SizeR * 2 / m, fConfig.World.SizeZ / m);


    //==========================================================================
    //                          B E A M   E L E M E N T S
    //==========================================================================
    // -- use JLEIC  lattice

    // Checking the beamline
    auto beamLineName = g4e::ToLowerCopy(fConfig.BeamlineName);
    auto beamLine = fConfig.BeamlineName == "ip6" ? BeamLines::IP6 : BeamLines::IP8;

    // Different Shifts for 0 IP
    if(BeamLines::IP8 == beamLine  ) {
        fConfig.World.ShiftVTX=0*cm;
    } else if(BeamLines::IP6 == beamLine ) {
        fConfig.World.ShiftVTX=0.*cm;
    } else {  fConfig.World.ShiftVTX=40.*cm;}

    // Load beam lines
    if(USE_FFQs )
    {
        if(!fInitContext->Arguments->IsSetHomePath) {
            G4Exception("ReferenceDetectorConstruction::Construct",
                        "InvalidSetup", FatalException,
                        "AcceleratorMagnets file opening err :: please setup env. G4E_HOME");
        }

        auto eFileName = fmt::format("{}/resources/{}/mdi/e_ir_10.txt", fInitContext->Arguments->HomePath, fConfig.BeamlineName);
        auto ionFileName = fmt::format("{}/resources/{}/mdi/ion_ir_275.txt", fInitContext->Arguments->HomePath, fConfig.BeamlineName);

        fmt::print("Init AcceleratorMagnets... I\n");
        fmt::print(" |- Ion E      {}   A={}  Z={} \n", fConfig.IonBeamEnergy, fConfig.IonBeamA, fConfig.IonBeamZ);
        fmt::print(" |- Electron E {}\n", fConfig.ElectronBeamEnergy);
        fmt::print(" |- File names:\n");
        fmt::print("    |- Electron {}\n", eFileName);
        fmt::print("    |- Ion      {}\n", ionFileName);

        // Create electron and ion beam lines
        fElectronLineMagnets = new AcceleratorMagnets(eFileName, fWorldPhysical, World_Material, beamLine, 0, fConfig.ElectronBeamEnergy);
        fIonLineMagnets = new AcceleratorMagnets(ionFileName, fWorldPhysical, World_Material, beamLine, 1, fConfig.IonBeamEnergy, fConfig.IonBeamA, fConfig.IonBeamZ);
    }

    //=========================================================================
    //                    Beampipe
    //=========================================================================
    if(USE_BEAMPIPE ) {
        //fConfig.ir_Beampipe.Zpos = fConfig.World.ShiftVTX;
        //auto bpCadDirectory = fmt::format("{}/resources/ip6/beampipe/", fInitContext->Arguments->HomePath);
        //ir_Beampipe.Construct(fConfig.ir_Beampipe, fWorldPhysical, bpCadDirectory);
        //ir_Beampipe.ConstructForwardCone(fConfig.ir_Beampipe, World_Material, World_Phys);
    }

    //=========================================================================
    //                    Create Central Detector Major Parts
    //=========================================================================

    if (USE_BARREL) {
        // CREATE SOLENOID
        fConfig.cb_Solenoid.ShiftZ = fConfig.World.ShiftVTX;
        cb_Solenoid.Construct(fConfig.cb_Solenoid, World_Material, fWorldPhysical);
    }


    if (USE_E_ENDCAP) {
        // Create electron endcap
        fConfig.ce_Endcap.ROut = fConfig.cb_Solenoid.ROut -1 * cm;
        fConfig.ce_Endcap.PosZ = -fConfig.ce_Endcap.SizeZ / 2 - fConfig.cb_Solenoid.SizeZ / 2 + fConfig.World.ShiftVTX -2*cm;
        Create_ce_Endcap(fConfig.ce_Endcap);
    }

    if (USE_CB_HCAL) {
        // Create  HCAL ( Iron  BARREL)
        fConfig.cb_HCAL.RIn = fConfig.cb_Solenoid.ROut;
        fConfig.cb_HCAL.ROut = fConfig.cb_Solenoid.ROut + fConfig.cb_HCAL.Thickness;
        fConfig.cb_HCAL.SizeZ = fConfig.cb_Solenoid.SizeZ + fConfig.ce_Endcap.SizeZ;
        //  fConfig.cb_HCAL.SizeZ = fConfig.cb_Solenoid.SizeZ ;

        fConfig.cb_HCAL.ShiftZ = -fConfig.ce_Endcap.SizeZ / 2. + fConfig.World.ShiftVTX;

        //  fConfig.cb_HCAL.ShiftZ = fConfig.World.ShiftVTX;
        cb_HCAL.Construct(fConfig.cb_HCAL, World_Material, fWorldPhysical);

        // Create a layered structure for the Iron
        if (USE_CB_HCAL_D) {
            cb_HCAL.ConstructLayers();
            // cb_HCAL.ConstructRings();
        }
    }


    if (USE_CI_ENDCAP) {
        //Create Hadron endcap
        if (USE_CB_HCAL) {
            fConfig.ci_Endcap.ROut = fConfig.cb_HCAL.ROut;
        } else {
            fConfig.ci_Endcap.ROut = fConfig.cb_Solenoid.ROut + 100 * cm;
        }

        fConfig.ci_Endcap.PosZ = fConfig.cb_Solenoid.SizeZ / 2. + fConfig.World.ShiftVTX + fConfig.ci_Endcap.ShiftZ + fConfig.ci_Endcap.SizeZ / 2.;
        Create_ci_Endcap(fConfig.ci_Endcap);

        // Create HCAL in hadron endcap
        if (USE_CI_HCAL) {
            // G4double ci_HCAL_GVol_RIn[2]={60*cm, 70*cm } ;
            //  G4double ci_HCAL_GVol_RIn[2]={0*cm, 0*cm }
            // G4double ci_HCAL_GVol_ROut[2]={cb_HCAL_GVol_ROut,cb_HCAL_GVol_ROut };

            if (USE_CB_HCAL) { fConfig.ci_HCAL.ROut = fConfig.cb_HCAL.ROut; }
            else { fConfig.ci_HCAL.ROut = 300 * cm; }

            fConfig.ci_HCAL.PosZ = fConfig.ci_Endcap.PosZ + fConfig.ci_Endcap.SizeZ / 2 + fConfig.ci_HCAL.ShiftZ + fConfig.ci_HCAL.SizeZ / 2;
            ci_HCAL.Construct(fConfig.ci_HCAL, World_Material, fWorldPhysical);

            // HCAL IRON
            if (USE_CI_HCAL_D) {
                if(beamLine == BeamLines::IP6) { fConfig.ci_HCAL.det_RIn = 30 * cm;}
                if(beamLine == BeamLines::IP8) { fConfig.ci_HCAL.det_RIn = 80 * cm; }
                ci_HCAL.ConstructDetectors(fConfig.ci_HCAL);
            }
        }
    }

//***********************************************************************************
//***********************************************************************************
//**                                DETECTOR VOLUMES                               **
//***********************************************************************************
//***********************************************************************************

    // ******************************************************************************
    //                       CENTRAL BARREL (CB)
    // ******************************************************************************

    if (USE_BARREL && USE_BARREL_DETECTORS)  {
        //------------------------------------------------
        //           cb_VTX (Central Barrel Vertex)
        //------------------------------------------------

        if (USE_CB_VTX) {

            fConfig.cb_VTX.ShiftZ = -fConfig.World.ShiftVTX;
            cb_VTX.Construct(fConfig.cb_VTX, World_Material, cb_Solenoid.Phys);

            if (USE_CB_VTX_LADDERS) {
                //----------vtx barrel ladder geometry--------------
                cb_VTX.ConstructLaddersCentral();
            }
            if (USE_CB_VTX_ENDCAPS) {
                cb_VTX.ConstructLaddersEndcaps();
            }
        }     // end VTX detector

        //------------------------------------------------
        //      Silicone detector along the beamline
        //------------------------------------------------

        if (USE_CB_SiDISCS) {

            fConfig.cb_SiDISCS.Zpos = 100.*cm;
            cb_SiDISCS.Construct(fConfig.cb_SiDISCS, World_Material, cb_Solenoid.Phys);
        }


        //------------------------------------------------
        //           CTD (Central Tracking Detector)
        //------------------------------------------------
        if (USE_CB_CTD) {

            fConfig.cb_CTD.SizeZ = fConfig.cb_Solenoid.SizeZ - fConfig.cb_CTD.SizeZCut;
            cb_CTD.Construct(fConfig.cb_CTD, World_Material, cb_Solenoid.Phys);

            if (USE_CB_CTD_Si) {
                printf("Det construction cb_CTD_detSi::1 Number of layers=%d \n",fConfig.cb_CTD.SiLayerCount);
                cb_CTD.ConstructLadders();
                printf("Det construction cb_CTD_detSi::2 Number of layers =%d \n ",fConfig.cb_CTD.SiLayerCount);

            }
            else if (USE_CB_CTD_Straw) { cb_CTD.ConstructStraws(); }

        } // end CTD detector


        //------------------------------------------------
        //                      cb_DIRC
        //------------------------------------------------
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
            }

        } // end DIRC detector


        //------------------------------------------------
        //                    cb_EMCAL
        //------------------------------------------------
        if (USE_CB_EMCAL) {

            fConfig.cb_EMCAL.ROut = fConfig.cb_Solenoid.ROut - fConfig.cb_EMCAL.ROutshift;
            fConfig.cb_EMCAL.RIn = fConfig.cb_EMCAL.ROut - fConfig.cb_EMCAL.Thickness;
            // cb_EMCAL_GVol_SizeZ=SizeZ -30*cm;

            cb_EMCAL.Construct(fConfig.cb_EMCAL, fConfig.cb_Solenoid, World_Material, cb_Solenoid.Phys);
            cb_EMCAL.ConstructBars();
        }
    }  // end Barrel



    // ***********************************************************************************
    //                       CE_ENDCAP
    // ***********************************************************************************
    if (USE_E_ENDCAP) {


        //------------------------------------------------
        //               Hadron endcap GEM
        //------------------------------------------------

        if (USE_CE_GEM) {
            fConfig.ce_GEM.PosZ = -fConfig.cb_Solenoid.SizeZ / 2 + fConfig.ce_GEM.SizeZ / 2;

            ce_GEM.Construct(fConfig.ce_GEM, World_Material, cb_Solenoid.Phys);
            ce_GEM.ConstructDetectors();
        }  // end USE_CI_GEM

        //------------------------------------------------
        //                    mRICH
        //------------------------------------------------
        if (USE_CE_MRICH) {
            fConfig.ce_MRICH.PosZ = fConfig.ce_Endcap.SizeZ / 2 - fConfig.ce_MRICH.SizeZ / 2 - 2 * cm;

            ce_MRICH.Construct(fConfig.ce_MRICH, World_Material, ce_ENDCAP_GVol_Phys);
            ce_MRICH.ConstructModules();
        }

        //------------------------------------------------
        //                   CE_EMCAL
        //------------------------------------------------
        if (USE_CE_EMCAL) {
            fConfig.ce_EMCAL.PosZ = -fConfig.ce_Endcap.SizeZ / 2 + fConfig.ce_EMCAL.Thickness / 2.;
            fConfig.ce_EMCAL.ROut = fConfig.ce_Endcap.ROut -3*cm;

            ce_EMCAL.Construct(World_Material, ce_ENDCAP_GVol_Phys);
            ce_EMCAL.OuterVolumeVisAttr->SetColor(G4Color(0.3, 0.5, 0.9, 0.1));

            ce_EMCAL.ConstructCrystalsSquare(); // --- inner detector with Crystals
            ce_EMCAL.ConstructGlassSquare();    // --- outer part with Glass
        }

    } // USE_E_ENDCAP


    // ***********************************************************************************
    //                       CI_ENDCAP
    // ***********************************************************************************
    if (USE_CI_ENDCAP) {

        //------------------------------------------------
        //            Hadron endcap GEM
        //------------------------------------------------
        if (USE_CI_GEM) {
            fConfig.ci_GEM.PosZ = fConfig.cb_Solenoid.SizeZ / 2 - fConfig.ci_GEM.SizeZ / 2 -fConfig.cb_CTD.SizeZCut /4.;   // --- need to find out why this 5 cm are needed


            // --- different crossing angle direction for IP8 and IP6
            if(beamLine == BeamLines::IP8)  {
                //  for JLEIC         fConfig.ci_GEM.PosX = -5 * cm;
                fConfig.ci_GEM.PosX = 6 * cm; // answer - for crossing angle
            } else {
                fConfig.ci_GEM.PosX = 5 * cm;// answer - for crossing angle
            }

            ci_GEM.Construct(fConfig.ci_GEM, World_Material, cb_Solenoid.Phys);
            ci_GEM.ConstructDetectors();
        }  // end USE_CI_GEM

        //------------------------------------------------
        //              dRICH     Hadron endcap
        //------------------------------------------------
        if (USE_CI_DRICH) {
            fConfig.ci_DRICH.RIn = fConfig.ci_Endcap.RIn;
            fConfig.ci_DRICH.PosZ = -fConfig.ci_Endcap.SizeZ / 2. + fConfig.ci_DRICH.ThicknessZ / 2.;
            // printf(" DRICH Poz =%f  DRICH.ThicknessZ =%f \n", fConfig.ci_DRICH.PosZ, fConfig.ci_DRICH.ThicknessZ);
            // double ci_DRICH_GVol_PosZ= 0*cm;
            ci_DRICH.Construct(fConfig.ci_DRICH, World_Material, ci_ENDCAP_GVol_Phys);
            ci_DRICH.ConstructDetectors();
        } // end USE_CI_DRICH

        //------------------------------------------------
        //                TRD     Hadron endcap
        //------------------------------------------------
        if (USE_CI_TRD) {
            fConfig.ci_TRD.RIn = fConfig.ci_Endcap.RIn;
            fConfig.ci_TRD.PosZ = fConfig.ci_Endcap.SizeZ / 2. - fConfig.ci_EMCAL.ThicknessZ - fConfig.ci_TRD.ThicknessZ / 2.;

            //    double ci_DRICH_GVol_PosZ= 0*cm;
            ci_TRD.Construct(fConfig.ci_TRD, World_Material, ci_ENDCAP_GVol_Phys);
            ci_TRD.ConstructDetectors();

        } // end USE_CI_TRD

        //------------------------------------------------
        //             CI_EMCAL Hadron endcap
        //------------------------------------------------
        if (USE_CI_EMCAL) {
            fConfig.ci_EMCAL.PosZ = fConfig.ci_Endcap.SizeZ / 2 -fConfig.ci_EMCAL.ThicknessZ / 2;
            if(beamLine == BeamLines::IP6)  {
                fConfig.ci_EMCAL.USE_ERHIC = true;
                fConfig.ci_EMCAL.det_Rin1 = 30*cm;
                fConfig.ci_EMCAL.det_Rin2 = 30*cm;
            } else {
                fConfig.ci_EMCAL.USE_ERHIC=true;
                fConfig.ci_EMCAL.det_Rin1 = 30*cm;
                fConfig.ci_EMCAL.det_Rin2 = 30*cm;
            }

            ci_EMCAL.Construct(fConfig.ci_EMCAL, World_Material, ci_ENDCAP_GVol_Phys);
            ci_EMCAL.ConstructDetectors();    // --- outer part with Glass
        } // end USE_CI_EMCAL
    }

//****************************************************************************************
//
//                                   Forward Detectors
//
//****************************************************************************************

    //====================================================================================
    //==                         B0(D1a) DIPOLE  Tracker and EMCAL                      ==
    //====================================================================================

    //------------------------------------------------
    //    Tracker at B0  ( D1a )
    //------------------------------------------------
    if (USE_FI_B0_TRK) {
        for (auto magnet: fIonLineMagnets->fMagnets) {
            if ((BeamLines::IP6 == beamLine && magnet->name == "iB0PF") || (BeamLines::IP8 == beamLine && magnet->name == "ionBXSP01")) {
                if(beamLine == BeamLines::IP6) {
                    fConfig.fi_B0_TRK.PhiStart = -130*deg;
                    fConfig.fi_B0_TRK.PhiTot = 275*deg;
                } else {
                    fConfig.fi_B0_TRK.PhiStart = -130*deg;
                    fConfig.fi_B0_TRK.PhiTot = 275*deg;
                };

                fConfig.fi_B0_TRK.ROut = magnet->Rin2 * cm;
                fConfig.fi_B0_TRK.Zpos = (magnet->LengthZ / 2.) * cm - fConfig.fi_B0_TRK.SizeZ / 2.;
                fi_B0_TRK.ConstructA(fConfig.fi_B0_TRK, World_Material, magnet->fPhysics_BigDi_m);
                fi_B0_TRK.ConstructDetectorsA();
            }
        }
    }


    //------------------------------------------------
    //           B0 near beampipe calorimeter
    //------------------------------------------------
    if (USE_CI_HCAL) {

        if (USE_FI_B0_EMCAL) {
            // Ecal module  AFTER !!!   Dipole1
            fConfig.fi_B0_EMCAL.Zpos = -fConfig.ci_HCAL.SizeZ / 2 + fConfig.fi_B0_EMCAL.SizeZ / 2;

            fConfig.fi_B0_EMCAL.rot_matx.rotateY(fConfig.fi_B0_EMCAL.Angle * rad);
            fi_B0_EMCAL.Construct(fConfig.fi_B0_EMCAL, World_Material, ci_HCAL.Phys);
        }
    }

//====================================================================================
//==                    Far-Forward Area    D2, D3  ZDC. Roman Pots                 ==
//====================================================================================

    //------------------------------------------------
    //             OFF MOMENTUM TRK
    //------------------------------------------------

    if (USE_FFI_OFFM_TRK) {
        if(beamLine == BeamLines::IP8) {

            // fConfig.ffi_OFFM_TRK.ROut = 35*cm;
            fConfig.ffi_OFFM_TRK.SizeX=100 *cm;
            fConfig.ffi_OFFM_TRK.SizeY=100 *cm;

            fConfig.ffi_OFFM_TRK.SizeZ = 10. * cm;
            fConfig.ffi_OFFM_TRK.Zpos = 30.5 * m;
            // fConfig.ffi_OFFM_TRK.Zpos = 27.5 * m;

            fConfig.ffi_OFFM_TRK.Xpos = 160 * cm;
            fConfig.ffi_OFFM_TRK.Nlayers=2;
        }
        if(beamLine == BeamLines::IP6) {
            // fConfig.ffi_OFFM_TRK.RIn = 10 * cm;
            // fConfig.ffi_OFFM_TRK.ROut = 35*cm;
            fConfig.ffi_OFFM_TRK.SizeZ = 10. * cm;
            fConfig.ffi_OFFM_TRK.Zpos = 22.5 * m;
            // fConfig.ffi_OFFM_TRK.Zpos = 27.5 * m;
            fConfig.ffi_OFFM_TRK.Xpos = 75 * cm;
            fConfig.ffi_OFFM_TRK.Nlayers=2;

            // virtual plane 2
            fConfig.ffi_OFFM_TRK2.SizeZ = 10. * cm;
            // fConfig.ffi_OFFM_TRK.Zpos = 22.5 * m;
            fConfig.ffi_OFFM_TRK2.Zpos = 27.5 * m;
            fConfig.ffi_OFFM_TRK2.Xpos = 75 * cm;
            fConfig.ffi_OFFM_TRK2.Nlayers=1;
        }

        ffi_OFFM_TRK.Construct(fConfig.ffi_OFFM_TRK, World_Material, fWorldPhysical);
        ffi_OFFM_TRK.ConstructDetectors();

        ffi_OFFM_TRK2.Construct(fConfig.ffi_OFFM_TRK2, World_Material, fWorldPhysical);
        ffi_OFFM_TRK2.ConstructDetectors();
    }


    //------------------------------------------------
    //             NEG TRK for Lambda decays
    //------------------------------------------------
    if (USE_FFI_NEG_TRK) {
        if(beamLine == BeamLines::IP6) {
            // for angle 0
            // fConfig.ffi_NEG_TRK.SizeZ = 10. * cm;
            // fConfig.ffi_NEG_TRK.Zpos = 25 * m;  // 22.1 *m
            // fConfig.ffi_NEG_TRK.Xpos = 45 * cm; //25 *cm
            fConfig.ffi_NEG_TRK.SizeZ = 10. * cm;
            //   fConfig.ffi_NEG_TRK.Zpos = 22.0 * m;
            //   fConfig.ffi_NEG_TRK.Xpos = 45 * cm;
            fConfig.ffi_NEG_TRK.Zpos = 7.0 * m;
            fConfig.ffi_NEG_TRK.Xpos = 25 * cm;

            fConfig.ffi_NEG_TRK.rot_matx.rotateY(fConfig.ffi_NEG_TRK.Angle);
            fConfig.ffi_NEG_TRK.Nlayers=1;
        }

        // -- for Second IR
        if(beamLine == BeamLines::IP8) {
            // for angle 0
            // fConfig.ffi_NEG_TRK.SizeZ = 10. * cm;
            // fConfig.ffi_NEG_TRK.Zpos = 25 * m;  // 22.1 *m
            // fConfig.ffi_NEG_TRK.Xpos = 45 * cm; //25 *cm
            fConfig.ffi_NEG_TRK.SizeZ = 10. * cm;
            //   fConfig.ffi_NEG_TRK.Zpos = 22.0 * m;
            //   fConfig.ffi_NEG_TRK.Xpos = 45 * cm;
            fConfig.ffi_NEG_TRK.Zpos = 7.0 * m;
            fConfig.ffi_NEG_TRK.Xpos = 25 * cm;

            fConfig.ffi_NEG_TRK.rot_matx.rotateY(fConfig.ffi_NEG_TRK.Angle);
            fConfig.ffi_NEG_TRK.Nlayers=1;
        }
        ffi_NEG_TRK.Construct(fConfig.ffi_NEG_TRK, World_Material, fWorldPhysical);
        ffi_NEG_TRK.ConstructDetectors();
    }

    //------------------------------------------------
    //             ZDC
    //------------------------------------------------
    if (USE_FFI_ZDC) {

        if(beamLine == BeamLines::IP6) {
            fConfig.ffi_ZDC.Angle=-0.0125;
            fConfig.ffi_ZDC.rot_matx.rotateY(-fConfig.ffi_ZDC.Angle * rad);

            fConfig.ffi_ZDC.Zpos = 3800 * cm;
            //fConfig.ffi_ZDC.Xpos = 98.5 * cm;
            fConfig.ffi_ZDC.Xpos = 90 * cm;
        }
        if(beamLine == BeamLines::IP8){
            fConfig.ffi_ZDC.Angle=-0.025;
            fConfig.ffi_ZDC.rot_matx.rotateY(-fConfig.ffi_ZDC.Angle * rad);
            //         fConfig.ffi_ZDC.Zpos = 3800 * cm;
            //         fConfig.ffi_ZDC.Xpos = 98.5 * cm;
            fConfig.ffi_ZDC.Zpos = 4200 * cm;
            fConfig.ffi_ZDC.Xpos = 220 * cm;
        }
        ffi_ZDC.Construct(fConfig.ffi_ZDC, World_Material, fWorldPhysical);
        // if(USE_FFI_ZDC_CRYSTAL) { ffi_ZDC.ConstructTowels(1); }
        // else if(USE_FFI_ZDC_GLASS) { ffi_ZDC.ConstructTowels(0); }
        // else if(USE_FFI_ZDC_ALICE) {  ffi_ZDC.ConstructALICE(); }
    } // end ffi_ZDC

    if (USE_FFI_RPOT_D2 ) {  //---- First Roman Pot
        if (beamLine == BeamLines::IP6) {
            fConfig.ffi_RPOT_D2.Angle = 0.025;
            fConfig.ffi_RPOT_D2.ROut = 20 * cm;
            fConfig.ffi_RPOT_D2.rot_matx.rotateY(fConfig.ffi_RPOT_D2.Angle * rad);
            fConfig.ffi_RPOT_D2.PosX = 82 * cm;
            fConfig.ffi_RPOT_D2.PosZ = 2620 * cm;
        }
        if (beamLine == BeamLines::IP8) {
            fConfig.ffi_RPOT_D2.Angle = 0.025;
            fConfig.ffi_RPOT_D2.ROut = 20 * cm;
            fConfig.ffi_RPOT_D2.rot_matx.rotateY(fConfig.ffi_RPOT_D2.Angle * rad);
            fConfig.ffi_RPOT_D2.PosX = 190 * cm;
            fConfig.ffi_RPOT_D2.PosZ = 4620 * cm;
        }

        ffi_RPOT_D2.Construct(fConfig.ffi_RPOT_D2, World_Material, fWorldPhysical);
        ffi_RPOT_D2.ConstructDetectors();
    }

    //------------------------------------------------
    //            Roman Pots
    //------------------------------------------------

    if (USE_FFI_RPOT_D3 ) {
        if (beamLine == BeamLines::IP6) {
            fConfig.ffi_RPOT_D3.Angle = 0.025;
            fConfig.ffi_RPOT_D3.rot_matx.rotateY(fConfig.ffi_RPOT_D3.Angle * rad);
            fConfig.ffi_RPOT_D3.PosZ = 2820 * cm;
            fConfig.ffi_RPOT_D3.PosX = 91 * cm;;
        } // end ffi_RPOT_D3

        if(beamLine == BeamLines::IP8) {
            fConfig.ffi_RPOT_D3.Angle = 0.025;
            fConfig.ffi_RPOT_D3.rot_matx.rotateY(fConfig.ffi_RPOT_D3.Angle * rad);
            // fConfig.ffi_RPOT_D3.PosZ = 3820 * cm;
            fConfig.ffi_RPOT_D3.PosZ = 4820 * cm;
            fConfig.ffi_RPOT_D3.PosX = 190 * cm;
        } // end ffi_RPOT_D3

        ffi_RPOT_D3.Construct(fConfig.ffi_RPOT_D3, World_Material, fWorldPhysical);
        ffi_RPOT_D3.ConstructDetectors();
    }


    //************************************************************************************
    //==                         Rear  Detectors                                        ==
    //************************************************************************************

    //------------------------------------------------
    //              Compton Polarimeter
    //------------------------------------------------
    if (USE_FFE_CPOL) {
        ffe_CPOL.Construct(fConfig.ffe_CPOL, World_Material, fWorldPhysical);
    }

    //------------------------------------------------
    //                    Lumi
    //------------------------------------------------
    if (USE_FFE_LUMI) {
        fConfig.ffe_LUMI.PosX=+0.7*m;
        fConfig.ffe_LUMI.PosY=0;
        fConfig.ffe_LUMI.PosZ=-25*m;
        ffe_LUMI.Construct(fConfig.ffe_LUMI, World_Material, fWorldPhysical);
    }

    //------------------------------------------------
    //                    Low-Q2
    //------------------------------------------------
    if (USE_FFE_LOWQ2) {
        fConfig.ffe_LOWQ2.PosX=+0.3*m;
        fConfig.ffe_LOWQ2.PosY=0;
        // just after dipole
        fConfig.ffe_LOWQ2.PosZ=-28*m;

        ffe_LOWQ2.Construct(fConfig.ffe_LOWQ2, World_Material, fWorldPhysical);
        ffe_LOWQ2.ConstructDetectors();
    }


    //===================================================================================
    //                     END detector construction....
    //===================================================================================

    // Exporting geometry
    spdlog::info(" - exporting geometry");
    g4e::GeometryExport::Export(fInitContext->Arguments->OutputBaseName, fWorldPhysical);
    PrintGeometryParameters();
    return fWorldPhysical;
}


void ReferenceDetectorConstruction::Create_ci_Endcap(DetectorConfig::ci_Endcap_Config cfg)
{
    /// This function creates ION-ENDCAP (but doesn't fill its contents)

    // Make endcup radius the same as Barrel Hadron Calorimeter
    ci_ENDCAP_GVol_Solid = new G4Tubs("ci_ENDCAP_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
    ci_ENDCAP_GVol_Logic = new G4LogicalVolume(ci_ENDCAP_GVol_Solid, World_Material, "ci_ENDCAP_GVol_Logic");
    ci_ENDCAP_GVol_Phys = new G4PVPlacement(nullptr, G4ThreeVector(cfg.PosX, 0, cfg.PosZ), "ci_ENDCAP_GVol_Phys", ci_ENDCAP_GVol_Logic, fWorldPhysical, false, 0);

    // Visual attributes
    ci_ENDCAP_GVol_VisAttr = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    ci_ENDCAP_GVol_VisAttr->SetLineWidth(1);
    ci_ENDCAP_GVol_VisAttr->SetForceSolid(false);
    ci_ENDCAP_GVol_Logic->SetVisAttributes(ci_ENDCAP_GVol_VisAttr);
}


void ReferenceDetectorConstruction::Create_ce_Endcap(DetectorConfig::ce_Endcap_Config cfg)
{
    /// This function creates ELECTRON-ENDCAP (but doesn't fill its contents)

    ce_ENDCAP_GVol_Solid = new G4Tubs("ce_ENDCAP_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
    ce_ENDCAP_GVol_Logic = new G4LogicalVolume(ce_ENDCAP_GVol_Solid, World_Material, "ce_ENDCAP_GVol_Logic");
    ce_ENDCAP_GVol_Phys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, cfg.PosZ), "ce_ENDCAP_GVol_Phys", ce_ENDCAP_GVol_Logic, fWorldPhysical, false, 0);

    // Visual attributes
    ce_ENDCAP_VisAttr = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    ce_ENDCAP_VisAttr->SetLineWidth(1);
    ce_ENDCAP_VisAttr->SetForceSolid(true);
    ce_ENDCAP_GVol_Logic->SetVisAttributes(ce_ENDCAP_VisAttr);
}


void ReferenceDetectorConstruction::ConstructSDandField()
{
    //=========================================================================
    //                    Sensitive detectors
    //=========================================================================

    G4SDManager *sdManager = G4SDManager::GetSDMpointer();

    // common calorimeter SD
    if (!fCalorimeterSD.Get()) {
        fCalorimeterSD.Put(new CommonCalorimeterSD("CommonCalorimeterSD", fInitContext->RootManager));
    }
    sdManager->AddNewDetector(fCalorimeterSD.Get());

    // Common vertex SD
    if (!fVertexSD.Get()) {
        fVertexSD.Put(new CommonVertexSD("CommonVertexSD", fInitContext->RootManager, this));
    }
    sdManager->AddNewDetector(fVertexSD.Get());


    // Central electron calorimeter SD
    if (!fCe_EMCAL_SD.Get()) {
        auto ce_emcal_sd = new ce_EMCAL_SD("ce_EMCAL_SD", fInitContext->RootManager);
        fCe_EMCAL_SD.Put(ce_emcal_sd);
    }
    sdManager->AddNewDetector(fCe_EMCAL_SD.Get());


    if (USE_BARREL && USE_BARREL_DETECTORS)  {

        // cb_VTX (Central Barrel Vertex)
        if (USE_CB_VTX && USE_CB_VTX_LADDERS) {
            for (size_t lay = 0; lay < cb_VTX.Lays.size(); lay++) {
                if (cb_VTX.cb_VTX_ladder_Logic) {
                    cb_VTX.cb_VTX_ladder_Logic[lay]->SetSensitiveDetector(fVertexSD.Get());
                }
            }

        }     // end VTX detector

        //------------------------------------------------
        //           CTD (Central Tracking Detector)
        //------------------------------------------------
        if (USE_CB_CTD && USE_CB_CTD_Si) {
                for (int lay = 0; lay < fConfig.cb_CTD.SiLayerCount; lay++) {
                    if(cb_CTD.SiLogics[lay]) cb_CTD.SiLogics[lay]->SetSensitiveDetector(fVertexSD.Get());
                }
        } // end CTD detector


        //------------------------------------------------
        //                      cb_DIRC
        //------------------------------------------------
        if (USE_CB_DIRC) {
            if (USE_CB_DIRC_bars) {
                cb_DIRC.ConstructBars();
                cb_DIRC.cb_DIRC_bars_Logic->SetSensitiveDetector(fCalorimeterSD.Get());
            }
        } // end DIRC detector


        //------------------------------------------------
        //                    cb_EMCAL
        //------------------------------------------------
        if (USE_CB_EMCAL) {
            cb_EMCAL.Logic->SetSensitiveDetector(fCalorimeterSD.Get());
        }
    }  // end Barrel



    // CE_ENDCAP
    // ****************
    if (USE_E_ENDCAP) {

        // Hadron endcap GEM
        if (USE_CE_GEM) {
            for (int lay = 0; lay < fConfig.ce_GEM.Nlayers; lay++) {
                SetSensitiveDetector(ce_GEM.Layers[lay].LogicName, fCalorimeterSD.Get());
            }
        }

        //  CE_EMCAL
        if (USE_CE_EMCAL) {
            ce_EMCAL.ce_EMCAL_detPWO_Logic->SetSensitiveDetector(fCe_EMCAL_SD.Get());
            ce_EMCAL.ce_EMCAL_detGLASS_Logic->SetSensitiveDetector(fCe_EMCAL_SD.Get());
        }
    } // USE_E_ENDCAP


    if (USE_CI_ENDCAP) {

        // Hadron endcap GEM
        if (USE_CI_GEM) {
            for (int lay = 0; lay < fConfig.ci_GEM.Nlayers; lay++) {
                if (ci_GEM.lay_Logic[lay]) ci_GEM.lay_Logic[lay]->SetSensitiveDetector(fCalorimeterSD.Get());
            }
        }

        // CI_EMCAL Hadron endcap
        //-----------------------
        if (USE_CI_EMCAL) {
            ci_EMCAL.ci_EMCAL_det_Logic->SetSensitiveDetector(fCalorimeterSD.Get());
        }
    }

    //    Tracker at B0  ( D1a )
    if (USE_FI_B0_TRK) {
        for (int lay = 0; lay < fConfig.fi_B0_TRK.Nlayers; lay++) {
            if (fi_B0_TRK.fi_B0_TRK_lay_Logic[lay]) fi_B0_TRK.fi_B0_TRK_lay_Logic[lay]->SetSensitiveDetector(fCalorimeterSD.Get());
        }
    }

    // OFF MOMENTUM TRK
    if (USE_FFI_OFFM_TRK) {
        for (int lay = 0; lay < fConfig.ffi_OFFM_TRK.Nlayers; lay++) {
            if (ffi_OFFM_TRK.lay_Logic) ffi_OFFM_TRK.lay_Logic->SetSensitiveDetector(fCalorimeterSD.Get());
        }
        for (int lay = 0; lay < fConfig.ffi_OFFM_TRK2.Nlayers; lay++) {
            if (ffi_OFFM_TRK2.lay_Logic) ffi_OFFM_TRK2.lay_Logic->SetSensitiveDetector(fCalorimeterSD.Get());
        }
    }

    // NEG TRK for Lambda decays
    if (USE_FFI_NEG_TRK) {
        for (int lay = 0; lay < fConfig.ffi_NEG_TRK.Nlayers; lay++) {
            if (ffi_NEG_TRK.lay_Logic) ffi_NEG_TRK.lay_Logic->SetSensitiveDetector(fCalorimeterSD.Get());
        }
    }

    if (USE_FFI_RPOT_D2 ) {  //---- First Roman Pot
        for (int lay = 0; lay < fConfig.ffi_RPOT_D2.Nlayers; lay++) {
            if (ffi_RPOT_D2.lay_Logic[lay]) ffi_RPOT_D2.lay_Logic[lay]->SetSensitiveDetector(fVertexSD.Get());
        }
    }

    if (USE_FFI_RPOT_D3 ) {
        for (int lay = 0; lay < fConfig.ffi_RPOT_D3.Nlayers; lay++) {
            if (ffi_RPOT_D3.lay_Logic[lay]) ffi_RPOT_D3.lay_Logic[lay]->SetSensitiveDetector(fVertexSD.Get());
        }
    }

    // Low-Q2 tagger
    if (USE_FFE_LOWQ2) {
        for (int lay = 0; lay < fConfig.ffe_LOWQ2.Nlayers; lay++) {
            if (ffe_LOWQ2.lay_Logic) SetSensitiveDetector(ffe_LOWQ2.Logic->GetName(), fCalorimeterSD.Get() );
        }
        if (ffe_LOWQ2.BPC_Logic) SetSensitiveDetector(ffe_LOWQ2.BPC_Logic->GetName(), fCalorimeterSD.Get());
    }

    // ZDC
    if(USE_FFI_ZDC) {
        //if (ffi_ZDC.ffi_ZDC_HCAL_Logic) ffi_ZDC.ffi_ZDC_HCAL_Logic->SetSensitiveDetector(fCalorimeterSD.Get());
    }


    //=========================================================================
    //                    VOLUME ENTER ACTIONS
    //=========================================================================

    if(USE_FFI_ZDC) { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_ZDC.Phys);}
    //  fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_RPOT_D2.lay_Phys[0]);
    if(USE_FFI_RPOT_D2)   { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_RPOT_D2.Phys);}
    if(USE_FFI_RPOT_D3)   { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_RPOT_D3.Phys);}
    if(USE_FFI_OFFM_TRK)  { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_OFFM_TRK.Phys);}
    if(USE_FFI_OFFM_TRK2) { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_OFFM_TRK2.Phys);}
    if(USE_FFI_NEG_TRK)   { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_NEG_TRK.Phys);}
    if(USE_FI_B0_TRK)     { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->fi_B0_TRK.Phys);}
    if(USE_CI_GEM)        { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ci_GEM.Phys);}
    if(USE_CI_TRD)        { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ci_TRD.Phys);}
    if(USE_CB_CTD)        { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->cb_CTD.Phys);}
    if(USE_CE_GEM)        { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ce_GEM.PhysicalVolume);}
    if(USE_CE_EMCAL)      { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ce_EMCAL.Phys);}
    if(USE_CI_HCAL)       { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ci_HCAL.Phys);}

    // fIonLineMagnets->CreateMagneticFiles();
    // fElectronLineMagnets->CreateMagneticFiles();
}


void ReferenceDetectorConstruction::UpdateGeometry()
{
    G4RunManager::GetRunManager()->DefineWorldVolume(fWorldPhysical);
}

