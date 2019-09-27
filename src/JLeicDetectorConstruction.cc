//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *G4ThreeVector
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
//
//  JLeicDetectorConstruction.cc, v1  -- JF--- 2019-02-19------
//
// 

#include <spdlog/spdlog.h>
#include "vector"
#include "JLeicDetectorConstruction.hh"
//#include "JLeicDetectorMessenger.hh"
#include "JLeicCalorimeterSD.hh"
#include "JLeicVertexSD.hh"
//#include "JLeicMaterials.hh"

//#include "G4Material.hh"
//#include "G4Box.hh"
//#include "G4LogicalVolume.hh"
//#include "G4UniformMagField.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"

//#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4ProductionCuts.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4UnitsTable.hh"
#include "G4ios.hh"

#include "G4RotationMatrix.hh"

#include "G4PVDivision.hh"
#include "G4SystemOfUnits.hh"
#include "JLeicSolenoid3D.hh"

#define USE_TGEOM 1
//--------BEAM elements------
#define USE_FFQs
//#define USE_FFQs_iu
//#define USE_FFQs_ed
//#define USE_FFQs_eu

//===========================================
//--------BARREL------
#define USE_BARREL
#define USE_BARREL_det
//#define USE_BEAMPIPE 1 // beampipe 
//------- subdetector-volumes  barrel ----- 

#define USE_CB_VTX
//#define  USE_VTX0 1   // for simple vtx geom
#define USE_CB_VTX_LADDERS
#define  USE_CB_VTX_ENDCAPS    // for vxt endcaps ladders
//#define  USE_VTX_DISKS    // for vxt disks along beampipe
//#define USE_VTX_E 1   // for vxt endcaps 


#define USE_CB_CTD
#define USE_CB_CTD_Si  1 // silicon version of CTD
//#define USE_CB_CTD_Straw 1 // straw version of CTD

#define USE_CB_DIRC
#define USE_CB_DIRC_bars  1 // bars for DIRC

#define USE_CB_EMCAL
#define USE_CB_HCAL
#define USE_CB_HCAL_D // hcal detector
#define USE_GEM   // volumes
#define USE_GEMb  // detectors


//==============================================
//--------H-encap------
#define USE_CI_ENDCAP
//------- subdetector-volumes H-encap -----
#define USE_CI_GEM
#define USE_CI_DRICH
#define USE_CI_TRD
#define USE_CI_TRD_D  // -detector and radiator
#define USE_CI_EMCAL
#define USE_CI_HCAL
#define USE_CI_HCAL_D
//--------- Forward D1
#define USE_FI_EMCAL
#define USE_FI_TRKD1
// ==============================================
//--------E-encap------
#define USE_E_ENDCAP
//------- subdetector-volumes E-encap ----- 
#define  USE_CE_GEM
#define  USE_CE_EMCAL
#define  USE_CE_MRICH

//==============================================
#define  USE_FFE_CPOL
//--------FARFORWARD HADRON------
//#define USE_DIPOLE1_SI
//
//#define USE_FI_DIPOLE1_B
//#define USE_FI_DIPOLE2

#define USE_FFI_TRKD2
#define USE_FFI_ZDC
#define USE_FFI_RPOT
//#define USE_FARFORWARD_GEM

//#define USE_FARFORWARD_VP

//--------barrel------
//#define  USE_VTX0 1   // for simple vtx geom
//#define USE_VTX_E 1   // for vxt endcaps 
//-------------CTD----------
//#define  USE_CB_EMCAL 1
//--------Endcap ------
//#define USE_ENDCAP 1
//#define  USE_EMCALe
//#define  USE_VTXB 1


#ifdef USE_TGEOM
// VGM demo
#include "Geant4GM/volumes/Factory.h"
#include "RootGM/volumes/Factory.h"
#include "TGeoManager.h"
#include "XmlVGM/GDMLExporter.h"
// end VGM demo
#endif

/////////////////////////////////////////////////////////////////////////////
//
//

JLeicDetectorConstruction::JLeicDetectorConstruction()
        :
           fWorldChanged(false), fAbsorberMaterial(0), fGapMat(0), fSetUp("jleic2019"),
          World_Material(nullptr), World_Solid(0), World_Logic(0), World_Phys(0),
          fSolidRadSlice(0), fLogicRadSlice(0), fPhysicRadSlice(0),
           fPipe(false),
// fSolidAbsorber(0),    fLogicAbsorber(0),   fPhysicsAbsorber(0),
          fCalorimeterSD(0), fVertexSD(0),   fMat(0)
          {
    fDetectorMessenger = new JLeicDetectorMessenger(this);
    fMat = new JLeicMaterials();
}

//////////////////////////////////////////////////////////////////////////
//
//

JLeicDetectorConstruction::~JLeicDetectorConstruction() {
    delete fDetectorMessenger;
    delete fMat;
}

//////////////////////////////////////////////////////////////////////////
//
//

G4VPhysicalVolume *JLeicDetectorConstruction::Construct() {
    return ConstructDetectorXTR();
}


/////////////////////////////////////////////////////////////////////////
//
//

G4VPhysicalVolume *JLeicDetectorConstruction::ConstructDetectorXTR() {
    // Cleanup old geometry

    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    G4cout << "USE Experimental setup : " << fSetUp << G4endl;

    if (fSetUp == "jleic2019") {
        return SetUpJLEIC2019();
    } else {
        G4cout << "Experimental setup is unsupported.  " << G4endl;
        return SetUpJLEIC09();

        //  return 0;
    }
}


void JLeicDetectorConstruction::Create_ci_Endcap(JLeicDetectorConfig::ci_Endcap_Config cfg)
{
    //===================================================================================
    //==                           ION-ENDCAP                                          ==
    //===================================================================================
    // Make endcup radius the same as Barrel Hadron Calorimeter
    ci_ENDCAP_GVol_Solid = new G4Tubs("ci_ENDCAP_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);

    ci_ENDCAP_GVol_Logic = new G4LogicalVolume(ci_ENDCAP_GVol_Solid, World_Material, "ci_ENDCAP_GVol_Logic");

    ci_ENDCAP_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(cfg.PosX, 0, cfg.PosZ), "ci_ENDCAP_GVol_Phys",
                                            ci_ENDCAP_GVol_Logic,
                                            World_Phys, false, 0);

    attr_ci_ENDCAP_GVol = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    attr_ci_ENDCAP_GVol->SetLineWidth(1);
    attr_ci_ENDCAP_GVol->SetForceSolid(true);
    ci_ENDCAP_GVol_Logic->SetVisAttributes(attr_ci_ENDCAP_GVol);
}

void JLeicDetectorConstruction::Create_ce_Endcap(JLeicDetectorConfig::ce_Endcap_Config cfg)
{
    //===================================================================================
    //==                           ELECTRON-ENDCAP                                     ==
    //===================================================================================
    ce_ENDCAP_GVol_Solid = new G4Tubs("ce_ENDCAP_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
    ce_ENDCAP_GVol_Logic = new G4LogicalVolume(ce_ENDCAP_GVol_Solid, World_Material, "ce_ENDCAP_GVol_Logic");

    ce_ENDCAP_GVol_Phys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, cfg.PosZ), "ce_ENDCAP_GVol_Phys", ce_ENDCAP_GVol_Logic,
                                            World_Phys, false, 0);

    attr_ce_ENDCAP_GVol = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    attr_ce_ENDCAP_GVol->SetLineWidth(1);
    attr_ce_ENDCAP_GVol->SetForceSolid(true);
    ce_ENDCAP_GVol_Logic->SetVisAttributes(attr_ce_ENDCAP_GVol);
}

//==========================================================================================================
//                              JLEIC 2018
//==========================================================================================================

/////////////////////////////////////////////////////////////////////////////////
//


G4VPhysicalVolume *JLeicDetectorConstruction::SetUpJLEIC2019() {

    char abname[128];
    int i, j;


    fAbsorberMaterial = fMat->GetMaterial("Si");


    G4VisAttributes *vtpc1;

    //===================================================================================
    //==                    create a world                                            ==
    //===================================================================================


    spdlog::flush_on(spdlog::level::info);
    spdlog::info("fConfig.World.SizeR={}", fConfig.World.SizeR);
    spdlog::info("fConfig.World.SizeZ={}", fConfig.World.SizeZ);


    // World_Material    = Air;
    World_Material = fMat->GetMaterial("G4_Galactic");
    World_Solid = new G4Box("World_Solid", fConfig.World.SizeR, fConfig.World.SizeR, fConfig.World.SizeZ / 2.);
    World_Logic = new G4LogicalVolume(World_Solid, World_Material, "World_Logic");
    World_Phys = new G4PVPlacement(nullptr, G4ThreeVector(), "World_Phys", World_Logic, nullptr, false, 0);

    spdlog::info("World_Solid->GetCubicVolume() = {}", World_Solid->GetCubicVolume());
    G4cout << "World_Solid->GetCubicVolume() = " << World_Solid->GetCubicVolume() << std::endl;
    G4cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  " << fConfig.World.SizeR << std::endl;
    G4cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  " << fConfig.World.SizeZ << std::endl;

    printf("World_Solid->GetCubicVolume() %f \n", World_Solid->GetCubicVolume());
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!! SizeR %f \n", fConfig.World.SizeR);
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!! SizeZ %f \n", fConfig.World.SizeZ);

    //=========================================================================
    //                    Sensitive
    //=========================================================================

    G4SDManager *SDman = G4SDManager::GetSDMpointer();

    if (!fCalorimeterSD) {

        fCalorimeterSD = new JLeicCalorimeterSD("CalorSD", this);
        SDman->AddNewDetector(fCalorimeterSD);
    }
    if (!fVertexSD) {

        fVertexSD = new JLeicVertexSD("VertexSD", this);
        SDman->AddNewDetector(fVertexSD);
    }

    printf("VertexSD done\n");


#ifdef  USE_BARREL
    //----------------------CREATE SOLENOID ------------------------
    fConfig.cb_Solenoid.ShiftZ = fConfig.World.ShiftVTX;
    cb_Solenoid.Construct(fConfig.cb_Solenoid, World_Material, World_Phys);

#endif

#ifdef USE_E_ENDCAP


    fConfig.ce_Endcap.ROut = fConfig.cb_Solenoid.ROut - 1 * cm;
    fConfig.ce_Endcap.PosZ = -fConfig.ce_Endcap.SizeZ / 2 - fConfig.cb_Solenoid.SizeZ / 2 + fConfig.World.ShiftVTX;

    Create_ce_Endcap(fConfig.ce_Endcap);

#endif

    //===================================================================================
    //==                           HCAL  BARREL                            ==
    //===================================================================================
#ifdef USE_CB_HCAL
    fConfig.cb_HCAL.RIn = fConfig.cb_Solenoid.ROut;
    fConfig.cb_HCAL.ROut = fConfig.cb_Solenoid.ROut + fConfig.cb_HCAL.Thickness;
    fConfig.cb_HCAL.SizeZ = fConfig.cb_Solenoid.SizeZ + fConfig.ce_Endcap.SizeZ;
    //  fConfig.cb_HCAL.SizeZ = fConfig.cb_Solenoid.SizeZ ;

    fConfig.cb_HCAL.ShiftZ = -fConfig.ce_Endcap.SizeZ / 2. + fConfig.World.ShiftVTX;

    //  fConfig.cb_HCAL.ShiftZ = fConfig.World.ShiftVTX;
    cb_HCAL.Construct(fConfig.cb_HCAL, World_Material, World_Phys);

#ifdef USE_CB_HCAL_D
    cb_HCAL.ConstructLayers();

#endif // end HCALbdet

#endif // end HCALb
    //===================================================================================


#ifdef USE_CI_ENDCAP
    //===================================================================================
    //==                           HADRON-ENDCAP                                       ==
    //===================================================================================

#ifdef USE_CB_HCAL

    fConfig.ci_Endcap.ROut = fConfig.cb_HCAL.ROut;
#else
    fConfig.ci_Endcap.ROut = fConfig.cb_Solenoid.ROut +  100 * cm;
#endif


    fConfig.ci_Endcap.PosZ = fConfig.cb_Solenoid.SizeZ / 2. + fConfig.World.ShiftVTX + fConfig.ci_Endcap.ShiftZ +
                             fConfig.ci_Endcap.SizeZ / 2.;
    Create_ci_Endcap(fConfig.ci_Endcap);


#endif




//===================================================================================

    //===================================================================================
    //==                          HADRON-ENDCAP    HCAL   DETECTOR VOLUME              ==
    //===================================================================================
#ifdef USE_CI_HCAL
    // G4double ci_HCAL_GVol_RIn[2]={60*cm, 70*cm } ;
    //  G4double ci_HCAL_GVol_RIn[2]={0*cm, 0*cm }
    // G4double ci_HCAL_GVol_ROut[2]={cb_HCAL_GVol_ROut,cb_HCAL_GVol_ROut };

#ifdef USE_CB_HCAL
    fConfig.ci_HCAL.ROut = fConfig.cb_HCAL.ROut;
#else
    fConfig.ci_HCAL.ROut=300*cm;
#endif
    fConfig.ci_HCAL.PosZ =
            fConfig.ci_Endcap.PosZ + fConfig.ci_Endcap.SizeZ / 2 + fConfig.ci_HCAL.ShiftZ + fConfig.ci_HCAL.SizeZ / 2;
    ci_HCAL.Construct(fConfig.ci_HCAL, World_Material, World_Phys);
    //---------------------------- HCAL IRON--------------------------------------

#ifdef USE_CI_HCAL_D
    ci_HCAL.ConstructDetectors();
#endif // end HCAL
#endif // end HCAL


//===================================================================================
//                          START placement of BEAM ELEMENTS                       ==
//===================================================================================
#ifdef USE_FFQs


    Read_Di_File();
    Read_dE_File();

#endif

    //***********************************************************************************
    //***********************************************************************************
    //**                                DETECTOR VOLUMES                               **
    //***********************************************************************************
    //***********************************************************************************


#ifdef  USE_BARREL_det
    //===================================================================================
    //==                          VERTEX DETECTOR VOLUME                               ==
    //===================================================================================

#ifdef  USE_CB_VTX

    fConfig.cb_VTX.ShiftZ = -fConfig.World.ShiftVTX;
    cb_VTX.Construct(fConfig.cb_VTX, World_Material, cb_Solenoid.Phys);

#ifdef  USE_CB_VTX_LADDERS
    //----------vtx barrel ladder geometry--------------
    cb_VTX.ConstructLaddersCentral();
    for (int lay = 0; lay < cb_VTX.Lays.size(); lay++) {
      if (cb_VTX.cb_VTX_ladder_Logic) { cb_VTX.cb_VTX_ladder_Logic[lay]->SetSensitiveDetector(fVertexSD); }
    }
#endif
#ifdef  USE_CB_VTX_ENDCAPS
    cb_VTX.ConstructLaddersEndcaps();
  //         if (fLogicVTXEndH[lay]) { fLogicVTXEndH[lay]->SetSensitiveDetector(fCalorimeterSD); }

#endif

#endif  // end VTX

    //===================================================================================
    //==                         CTD DETECTOR                                  ==
    //===================================================================================
#ifdef  USE_CB_CTD

    fConfig.cb_CTD.SizeZ = fConfig.cb_Solenoid.SizeZ - fConfig.cb_CTD.SizeZCut;

    cb_CTD.Construct(fConfig.cb_CTD, World_Material, cb_Solenoid.Phys);

#ifdef USE_CB_CTD_Si
    cb_CTD.ConstructLadders();
#endif
#ifdef  USE_CB_CTD_Straw
    cb_CTD.ConstructStraws();
#endif
#endif  // end CTD


    //===================================================================================
    //==                         RICH DETECTOR                                  ==
    //===================================================================================
#ifdef  USE_CB_DIRC

    fConfig.cb_DIRC.RIn = fConfig.cb_CTD.ROut + 1 * cm;
    // for new magnet
    //   cb_DIRC_GVol_ROut = 95 * cm;
    // for CLEO and BABAR DIRC
    fConfig.cb_DIRC.ROut = fConfig.cb_DIRC.RIn + 10 * cm;
    //   cb_DIRC_GVol_SizeZ = SizeZ;
    fConfig.cb_DIRC.SizeZ = fConfig.cb_CTD.SizeZ;

    cb_DIRC.Construct(fConfig.cb_DIRC, World_Material, cb_Solenoid.Phys);

#ifdef  USE_CB_DIRC_bars
    cb_DIRC.ConstructBars();
#endif


#endif  // end DIRC


    //===================================================================================
    //==                         EMCAL DETECTOR VOLUME                                   ==
    //===================================================================================

#ifdef  USE_CB_EMCAL

    fConfig.cb_EMCAL.ROut = fConfig.cb_Solenoid.ROut - fConfig.cb_EMCAL.ROutshift;
    fConfig.cb_EMCAL.RIn = fConfig.cb_EMCAL.ROut - fConfig.cb_EMCAL.Thickness;
    // cb_EMCAL_GVol_SizeZ=SizeZ -30*cm;

    cb_EMCAL.Construct(fConfig.cb_EMCAL, fConfig.cb_Solenoid, World_Material, cb_Solenoid.Phys);
    cb_EMCAL.ConstructBars();
#endif  // end cb_EMCAL

#endif   // end Barrel



// ***********************************************************************************
//                       CE_ENDCAP
// ***********************************************************************************
#ifdef USE_E_ENDCAP


//===================================================================================
// ==                      GEM     Hadron endcap                                ==
//==================================================================================

#ifdef USE_CE_GEM
    fConfig.ce_GEM.PosZ = -fConfig.cb_Solenoid.SizeZ / 2 +
                          fConfig.ce_GEM.SizeZ / 2;

    ce_GEM.Construct(fConfig.ce_GEM, World_Material, cb_Solenoid.Phys);
    ce_GEM.ConstructDetectors();
 //   for (int lay = 0; lay < fConfig.ce_GEM.Nlayers; lay++) {
 //       if (ce_GEM.lay_Logic[lay]) ce_GEM.lay_Logic[lay]->SetSensitiveDetector(fCalorimeterSD);
 //   }

//    for (int lay = 0; lay < fConfig.ce_GEM.Nlayers; lay++) {
//        if (ce_GEM.lay_Logic[lay]) ce_GEM.lay_Logic[lay]->SetSensitiveDetector(fCalorimeterSD);
 //   }
#endif  // end USE_CI_GEM

//===================================================================================
//                         mRICH
//===================================================================================

#ifdef USE_CE_MRICH
    fConfig.ce_MRICH.PosZ = fConfig.ce_Endcap.SizeZ / 2 - fConfig.ce_MRICH.SizeZ / 2 - 2 * cm;

    ce_MRICH.Construct(fConfig.ce_MRICH, World_Material, ce_ENDCAP_GVol_Phys);

    ce_MRICH.ConstructModules();

#endif
//===================================================================================
//                         CE_EMCAL
//===================================================================================
#ifdef USE_CE_EMCAL
    fConfig.ce_EMCAL.PosZ = -fConfig.ce_Endcap.SizeZ / 2 + fConfig.ce_EMCAL.Thickness / 2.;
    fConfig.ce_EMCAL.ROut = fConfig.ce_Endcap.ROut;
    ce_EMCAL.Construct(fConfig.ce_EMCAL, World_Material, ce_ENDCAP_GVol_Phys);
    ce_EMCAL.ConstructCrystals(); // --- inner detector with Crystals
    ce_EMCAL.ConstructGlass();    // --- outer part with Glass

#endif

#endif //end USE_E_ENDCAP


// ***********************************************************************************
//                       CI_ENDCAP
// ***********************************************************************************
#   ifdef USE_CI_ENDCAP


//===================================================================================
// ==                      GEM     Hadron endcap                                ==
//==================================================================================

#ifdef USE_CI_GEM
    fConfig.ci_GEM.PosZ = fConfig.cb_Solenoid.SizeZ / 2 -
                          fConfig.ci_GEM.SizeZ / 2;   // --- need to find out why this 5 cm are needed
    fConfig.ci_GEM.PosX = -5*cm;
    ci_GEM.Construct(fConfig.ci_GEM, World_Material, cb_Solenoid.Phys);
    ci_GEM.ConstructDetectors();
    for (int lay = 0; lay < fConfig.ci_GEM.Nlayers; lay++) {
     if (ci_GEM.lay_Logic[lay]) ci_GEM.lay_Logic[lay]->SetSensitiveDetector(fCalorimeterSD);
    }
#endif  // end USE_CI_GEM

#ifdef USE_CI_DRICH
//===================================================================================
// ==                       dRICH     Hadron endcap                                ==
//==================================================================================
    fConfig.ci_DRICH.RIn = fConfig.ci_Endcap.RIn;

    fConfig.ci_DRICH.PosZ = -fConfig.ci_Endcap.SizeZ / 2. + fConfig.ci_DRICH.ThicknessZ / 2.;
    //    double ci_DRICH_GVol_PosZ= 0*cm;
    ci_DRICH.Construct(fConfig.ci_DRICH, World_Material, ci_ENDCAP_GVol_Phys);
    ci_DRICH.ConstructDetectors();

//===================================================================================
#endif // end USE_CI_DRICH



#ifdef USE_CI_TRD
//===================================================================================
// ==                       TRD     Hadron endcap                                ==
//==================================================================================
 //   ci_TRD_GVol_PosZ = -fConfig.ci_Endcap.SizeZ / 2 + fConfig.ci_DRICH.ThicknessZ + ci_TRD_GVol_ThicknessZ/2.;

    fConfig.ci_TRD.RIn = fConfig.ci_Endcap.RIn;

    fConfig.ci_TRD.PosZ = -fConfig.ci_Endcap.SizeZ / 2. + fConfig.ci_DRICH.ThicknessZ + fConfig.ci_TRD.ThicknessZ / 2.;
    //    double ci_DRICH_GVol_PosZ= 0*cm;
    ci_TRD.Construct(fConfig.ci_TRD, World_Material, ci_ENDCAP_GVol_Phys);

    ci_TRD.ConstructDetectors();
    printf("FoilNumbers=%d (%d) \n",fConfig.ci_TRD.fFoilNumber,ci_TRD.ConstructionConfig.fFoilNumber); // --- ????????? ---
    //ci_TRD.ConstructionConfig.fFoilNumber
//===================================================================================
#endif // end USE_CI_TRD

#ifdef USE_CI_EMCAL

    fConfig.ci_EMCAL.PosZ = -fConfig.ci_Endcap.SizeZ / 2 + fConfig.ci_DRICH.ThicknessZ + fConfig.ci_TRD.ThicknessZ + fConfig.ci_EMCAL.ThicknessZ / 2;
    ci_EMCAL.Construct(fConfig.ci_EMCAL, World_Material, ci_ENDCAP_GVol_Phys);
    ci_EMCAL.ConstructDetectors();    // --- outer part with Glass

#endif // end USE_CI_EMCAL
#endif  // ============end USE_CI_ENDCAP  ===================================


    //====================================================================================
    //==                          DIPOLE-1 Tracker and EMCAL                            ==
    //====================================================================================

#ifdef USE_FI_TRKD1
    //-------------------------------------------------------------------------------
    //                      Place Si_disks inside D1a
    //-------------------------------------------------------------------------------
    int mydipole_id=-1;

    for (int id = 0; id < 20; id++) {
        if (strcmp(fSolid_BigDi_ffqsNAME[id], "iBDS1a") == 0) {
            printf("found D21=%s  Z=%f dZ=%f Rout=%f \n", fSolid_BigDi_ffqsNAME[id], fSolid_BigDi_ffqsZ[id],
                   fSolid_BigDi_ffqsSizeZDi[id],
                   fSolid_BigDi_ffqsRinDi[id]);
            mydipole_id = id;
        };
    };

    if (mydipole_id==-1) { printf("ERROR mydipole_id=-1\n"); sleep(3); exit(1); }

    fConfig.fi_TRKD1.ROut = fSolid_BigDi_ffqsRinDi[mydipole_id] * cm;
    fConfig.fi_TRKD1.Zpos = (fSolid_BigDi_ffqsSizeZDi[mydipole_id]/2.) * cm  -fConfig.fi_TRKD1.SizeZ/2.;
    fi_TRKD1.ConstructA(fConfig.fi_TRKD1, World_Material, fPhysics_BigDi_m[mydipole_id]);
    fi_TRKD1.ConstructDetectorsA();

   // fi_TRKD1.ConstructDetectorsB();
    //  if (f1_D1A_Lay_Logic) f1_D1A_Lay_Logic->SetSensitiveDetector(fCalorimeterSD);



#endif
//------------------------------------------------
#ifdef USE_CI_HCAL

#ifdef USE_FI_EMCAL
    // Ecal module  AFTER !!!   Dipole1

    fConfig.fi_EMCAL.Zpos = - fConfig.ci_HCAL.SizeZ/ 2 + fConfig.fi_EMCAL.SizeZ / 2;

    fConfig.fi_EMCAL.rot_matx.rotateY(fConfig.fi_EMCAL.Angle * rad);
    fi_EMCAL.Construct(fConfig.fi_EMCAL, World_Material, ci_HCAL.Phys);


#endif
#endif

    //====================================================================================
    //==                    Far-Forward Area    D2, D3  ZDC. Roman Pots                 ==
    //====================================================================================
#ifdef USE_FFI_TRKD2
    for (int id = 0; id < 20; id++) {
        if (strcmp(fSolid_BigDi_ffqsNAME[id], "iBDS2") == 0) {
            printf("fi_D2_GVol :: found D2=%s  Z=%f dZ=%f Rout=%f \n", fSolid_BigDi_ffqsNAME[id], fSolid_BigDi_ffqsZ[id],
                   fSolid_BigDi_ffqsSizeZDi[id],
                   fSolid_BigDi_ffqsRinDi[id]);
            mydipole_id = id;
        };
    };
    fConfig.ffi_TRKD2.RIn = 0 * cm;
    fConfig.ffi_TRKD2.ROut = fSolid_BigDi_ffqsRinDi[mydipole_id] * cm;
    fConfig.ffi_TRKD2.SizeZ = fSolid_BigDi_ffqsSizeZDi[mydipole_id] * m;

    ffi_TRKD2.Construct(fConfig.ffi_TRKD2, World_Material, fPhysics_BigDi_m[mydipole_id]);
    ffi_TRKD2.ConstructDetectors();
 //   for (int lay = 0; lay < fConfig.ffi_TRKD2.Nlayers; lay++) {
        if (ffi_TRKD2.lay_Logic) ffi_TRKD2.lay_Logic->SetSensitiveDetector(fCalorimeterSD);
 //   }

#endif
    //------------------------------------------------
#ifdef USE_FFI_ZDC
    fConfig.ffi_ZDC.rot_matx.rotateY(fConfig.ffi_ZDC.Angle * rad);
    fConfig.ffi_ZDC.Zpos = 4000*cm;
    fConfig.ffi_ZDC.Xpos = -190*cm;

    ffi_ZDC.Construct(fConfig.ffi_ZDC, World_Material, World_Phys);
 //   ffi_ZDC.ConstructTowels();
     if (ffi_ZDC.Logic) ffi_ZDC.Logic->SetSensitiveDetector(fCalorimeterSD);

#endif // end ffi_ZDC

    //------------------------------------------------
#ifdef USE_FFI_RPOT
    fConfig.ffi_RPOT.rot_matx.rotateY(fConfig.ffi_RPOT.Angle * rad);
    fConfig.ffi_RPOT.PosZ = 3100*cm;
    fConfig.ffi_RPOT.PosX = -170*cm;

    ffi_RPOT.Construct(fConfig.ffi_RPOT, World_Material, World_Phys);
    if (ffi_RPOT.Logic) ffi_RPOT.Logic->SetSensitiveDetector(fCalorimeterSD);

#endif // end ffi_RPOT

    //===================================================================================
    //==                        Compton Polarimeter                                  ==
    //===================================================================================
#ifdef  USE_FFE_CPOL

     ffe_CPOL.Construct(fConfig.ffe_CPOL, World_Material, World_Phys);
#endif // end ffe_CPOL





#ifdef  USE_BEAMPIPE
                                                                                                                            //===================================================================================
 //==                          BEAMPIPE                                             ==
 //===================================================================================
 /*----------Beampipe barrel--------------*/
  fBeamSizeRinBe=3.114*cm;
  fBeamSizeRoutBe=3.115*cm;
  // fBeamSizeZ_Be=cb_VTX_GVol_SizeZ;
  fBeamSizeZ_Be=1600*cm;

  /*--------------------------------------*/;
  /*-------Volume for Inner Berilium------*/;
  fBeampipeVacuum  = fMat->GetMaterial("G4_Galactic");
  fSolidBeamBe = new G4Tubs("BeampipeBe", 0,fBeamSizeRoutBe,fBeamSizeZ_Be/2.,0.,360*deg);
  fLogicBeamBe = new G4LogicalVolume(fSolidBeamBe, fBeampipeVacuum ,  "BeampipeBe");

  G4RotationMatrix  bpr_h;
  bpr_h.rotateY((-0.05*180/3.1415)*deg);

  fPhysicsBeamBe = new G4PVPlacement(G4Transform3D(bpr_h, G4ThreeVector(0,0,-World_ShiftVTX)), "BeampipeBe1-b",fLogicBeamBe,
                                  Phys, false,  0 );

   vb1= new G4VisAttributes(G4Color(0.4, 0.4, 0.5,1.));
   vb1->SetForceSolid(true); fLogicBeamBe->SetVisAttributes(vb1);
  //-------Wall------

   /*
   //  fBeamPipeMaterialBe = fMat->GetMaterial("Be");
   fBeamPipeMaterialBe = fMat->GetMaterial("G4_Galactic");
  fSolidBeamBe1 = new G4Tubs("BeampipeBe_wall",  fBeamSizeRinBe,fBeamSizeRoutBe,fBeamSizeZ_Be/2.,0.,360*deg);
  fLogicBeamBe1 = new G4LogicalVolume(fSolidBeamBe1,  fBeamPipeMaterialBe,  "BeampipeBe_wall");

  fPhysicsBeamBe1 = new G4PVPlacement(0, G4ThreeVector(), "BeampipeBe_wall",fLogicBeamBe1,
                                   fPhysicsBeamBe, false,  0 );


 //-------Vacuum------

   fBeampipeVacuum  = fMat->GetMaterial("G4_Galactic");
   fSolidBeamBe2 = new G4Tubs("BeampipeBe_vac", 0.0,fBeamSizeRinBe,fBeamSizeZ_Be/2.,0.,360*deg);
  fLogicBeamBe2 = new G4LogicalVolume(fSolidBeamBe2,  fBeampipeVacuum,  "BeampipeBe_vac");

  fPhysicsBeamBe2 = new G4PVPlacement(0, G4ThreeVector(), "BeampipeBe_vac",fLogicBeamBe2,
                                  fPhysicsBeamBe, false,  0 );


  //-------End -Volume for Inner Berilium-------
  //--------------------------------------------

  //--------------------------------------
  //-------Beampipe Hadrons---------------
  //--------------------------------------
 fBeamSizeRoutH=3.3*cm; fBeamSizeZ_H=3200.*cm;
  fSolidBeamH = new G4Tubs("BeampipeH", 0,fBeamSizeRoutH,fBeamSizeZ_H/2.,0.,360*deg);
  fLogicBeamH = new G4LogicalVolume(fSolidBeamH, fBeamPipeMaterialBe,  "BeampipeH");
  G4RotationMatrix  bpr_h;
  //   bpr_h.rotateY(-2.5*deg);
  bpr_h.rotateY((-0.05*180/3.1415)*deg);
  // bpr_h.rotateY(-2.5*deg);
  fPhysicsBeamH = new G4PVPlacement(G4Transform3D(bpr_h, G4ThreeVector(0,0,-World_ShiftVTX)), "BeampipeH",fLogicBeamH,
                                  Phys, false,  0 );

   vb1= new G4VisAttributes(G4Color(0.5, 0., 0.3,1.));
   vb1->SetForceSolid(true);  fLogicBeamH->SetVisAttributes(vb1);


   //--------------------------------------
  //--------------------cone-------------------------------
  //--------------------------------------

 fBeamPipeConeMaterial = fMat->GetMaterial("G4_Galactic");
 G4RotationMatrix bpr1,bpr2,bpr;
 double xPos=-2*cm;


  fBeamSizeZCone=100*cm;
  double fBeamSizeZCone2=155*cm;
  fBeamSizeRin1Cone=3.115*cm;
  fBeamSizeRin2Cone=fBeamSizeRin1Cone +0.025*(fBeamSizeZCone);

  G4double R_cone[4]={ 0.*cm, 3.5*cm , 15.*cm, 0.*cm};
  G4double Z_cone[4]={ 0.*cm,  0.*cm,  (SizeZ/2),  (SizeZ/2)};
  //World_ShiftVTX
  // Z1=  fBeamSizeZ_Be

  //  printf("zPos=%f,fBeamSizeRin2Cone=%f fBeamSizeRout2Cone=%f fBeamSizeZCone+zPos=%f\n",zPos,
  //       fBeamSizeRin2Cone,fBeamSizeRout2Cone,fBeamSizeZCone+zPos);

  fSolidBeamCone = new G4Polycone("BeampipeCone", 0., 360., 4, R_cone , Z_cone );

   fLogicBeamCone = new G4LogicalVolume(fSolidBeamCone,  fBeamPipeConeMaterial,  "BeampipeCone");

   //  fPhysicsBeamCone = new G4PVPlacement(0, G4ThreeVector(0,0,-30*cm), "BeampipeCone",fLogicBeamCone,
   //                           Phys, false,    0 );



   bpr1.rotateY(-2.5*deg);
  fPhysicsBeamCone = new G4PVPlacement(G4Transform3D(bpr1, G4ThreeVector(0,0,0*cm)), "BeampipeCone",fLogicBeamCone,
                              Phys, false,    0 );

  vb1= new G4VisAttributes(G4Color(1.0, 1.0, 0.1,1));

  */;

  /*


   double zPos2=(fBeamSizeZCone2/2)+ 25*cm;

  fSolidBeamCone2 = new G4Cons("VTX1pipeCone2",  fBeamSizeRin1Cone, fBeamSizeRout1Cone,
                            fBeamSizeRin2Cone,fBeamSizeRout2Cone,fBeamSizeZCone2/2.,0.,360*deg);

  fLogicBeamCone2 = new G4LogicalVolume(fSolidBeamCone2,  fBeamPipeConeMaterial,  "Beam2Cone");


  brm.rotateY((180.-0.026*180/3.1415)*deg);
  xPos= 2*cm;
   fPhysicsBeamCone2 = new G4PVPlacement(G4Transform3D(brm, G4ThreeVector(xPos,0,-zPos2)), "Beam2Cone",fLogicBeamCone2,
                                 Phys, false,  0 );


   vb1->SetForceSolid(true); fLogicBeamCone2->SetVisAttributes(vb1);fLogicBeamCone->SetVisAttributes(vb1);


  */

#endif






#ifdef USE_FI_DIPOLE1_B

    //-------------------------------------------------------------------------------
    //                      Place Si_disks inside D1b
    //-------------------------------------------------------------------------------

    for (int id = 0; id < 20; id++) {
        if (strcmp(fSolid_BigDi_ffqsNAME[id], "iBDS1b") == 0) {
            printf("found D1b=%s  Z=%f dZ=%f Rout=%f \n", fSolid_BigDi_ffqsNAME[id], fSolid_BigDi_ffqsZ[id],
                   fSolid_BigDi_ffqsSizeZDi[id],
                   fSolid_BigDi_ffqsRinDi[id]);
            mydipole_id = id;
        };
    };
    fi_D1B_GVol_RIn = 0 * cm;
    fi_D1B_GVol_ROut = fSolid_BigDi_ffqsRinDi[mydipole_id] * cm;
    fi_D1B_GVol_SizeZ = fSolid_BigDi_ffqsSizeZDi[mydipole_id] * m;


    fi_D1B_GVol_Solid = new G4Tubs("fi_D1B_GVol_Solid", fi_D1B_GVol_RIn, fi_D1B_GVol_ROut,
                                    fi_D1B_GVol_SizeZ / 2., 0., 360 * deg);

    fi_D1B_GVol_Logic = new G4LogicalVolume(fi_D1B_GVol_Solid, World_Material, "fi_D1B_GVol_Logic");

    // ci_GEM_GVol_PosZ= SizeZ/2-abs(World_ShiftVTX)+ci_GEM_GVol_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed
    fi_D1B_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "fi_D1B_GVol_Phys", fi_D1B_GVol_Logic,
                                             fPhysics_BigDi_m[mydipole_id], false, 0);

    attr_fi_D1B_GVol = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    attr_fi_D1B_GVol->SetLineWidth(1);
    attr_fi_D1B_GVol->SetForceSolid(true);
    fi_D1B_GVol_Logic->SetVisAttributes(attr_fi_D1B_GVol);

    // ---------------------------------------------------------------------------
    //                     Si all
    // ---------------------------------------------------------------------------
    fi_D1B_lay_RIn = 5 * cm;
    fi_D1B_lay_ROut = fi_D1B_GVol_ROut - 1 * cm;
    fi_D1B_lay_SizeZ = 1 * cm;

    //  fi_D1B_lay_Material = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----
    fi_D1B_lay_Material =fMat->GetMaterial("G4_Galactic");
    fi_D1B_lay_Solid = new G4Tubs("fi_D1B_lay_Solid", fi_D1B_lay_RIn, fi_D1B_lay_ROut,
                                        fi_D1B_lay_SizeZ / 2., 170., 330 * deg);
    fi_D1B_lay_Logic = new G4LogicalVolume(fi_D1B_lay_Solid, fi_D1B_lay_Material, "fi_D1B_lay_Logic");

    if (fi_D1B_lay_Logic) fi_D1B_lay_Logic->SetSensitiveDetector(fCalorimeterSD);


    ffsi_counter = 0;
    for (int fflay = 0; fflay < 5; fflay++) {
        double Z = -fi_D1B_GVol_SizeZ / 2 + (fflay + 1) * fi_D1B_lay_SizeZ / 2 + (fflay + 1) * 5 * cm;
        fi_D1B_lay_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, Z),
                                                     "fi_D1B_lay_Phys", fi_D1B_lay_Logic,
                                                     fi_D1B_GVol_Phys, false, ffsi_counter);
        ffsi_counter++;
        attr_fi_D1B_lay= new G4VisAttributes(G4Color(0.8, 0.4 + 0.1 * fflay, 0.3, 1.));
        attr_fi_D1B_lay->SetLineWidth(1);
        attr_fi_D1B_lay->SetForceSolid(true);
        fi_D1B_lay_Logic->SetVisAttributes(attr_fi_D1B_lay);
    }

#endif


    //*********************************************************************
    //====================================================================================
    //==                          DETECTOR VOLUME  FAR-FORWARD                           ==
    //====================================================================================
    //*********************************************************************


    //====================================================================================
    //==                         FI  VOLUMES  inside D2                      ==
    //====================================================================================
#ifdef USE_FI_DIPOLE2

    //-------------------------------------------------------------------------------
    //                      Place Si_disks inside D2
    //-------------------------------------------------------------------------------

    for (int id = 0; id < 20; id++) {
        if (strcmp(fSolid_BigDi_ffqsNAME[id], "iBDS2") == 0) {
            printf("fi_D2_GVol :: found D2=%s  Z=%f dZ=%f Rout=%f \n", fSolid_BigDi_ffqsNAME[id], fSolid_BigDi_ffqsZ[id],
                   fSolid_BigDi_ffqsSizeZDi[id],
                   fSolid_BigDi_ffqsRinDi[id]);
            mydipole_id = id;
        };
    };
    ffi_D2_GVol_RIn = 0 * cm;
    ffi_D2_GVol_ROut = fSolid_BigDi_ffqsRinDi[mydipole_id] * cm;
    ffi_D2_GVol_SizeZ = fSolid_BigDi_ffqsSizeZDi[mydipole_id] * m;


    ffi_D2_GVol_Solid = new G4Tubs("ffi_D2_GVol_Solid", ffi_D2_GVol_RIn, ffi_D2_GVol_ROut,
                                   ffi_D2_GVol_SizeZ / 2., 0., 360 * deg);

    ffi_D2_GVol_Logic = new G4LogicalVolume(ffi_D2_GVol_Solid, World_Material, "ffi_D2_GVol_Logic");

    // ci_GEM_GVol_PosZ= SizeZ/2-abs(World_ShiftVTX)+ci_GEM_GVol_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed
    ffi_D2_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "ffi_D2_GVol_Phys", ffi_D2_GVol_Logic,
                                            fPhysics_BigDi_m[mydipole_id], false, 0);

    attr_ffi_D2_GVol = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    attr_ffi_D2_GVol->SetLineWidth(1);
    attr_ffi_D2_GVol->SetForceSolid(true);
    ffi_D2_GVol_Logic->SetVisAttributes(attr_ffi_D2_GVol);

    // ---------------------------------------------------------------------------
    //                     Tracking  all  D2
     // ---------------------------------------------------------------------------
    ffi_D2_TRK_Lay_RIn = 0 * cm;
    ffi_D2_TRK_Lay_ROut = ffi_D2_GVol_ROut - 1 * cm;
    ffi_D2_TRK_Lay_SizeZ = 1 * cm;

    //  ffi_D2_TRK_Lay_Material = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----
  ffi_D2_TRK_Lay_Material  =fMat->GetMaterial("G4_Galactic");
    ffi_D2_TRK_Lay_Solid = new G4Tubs("ffi_D2_TRK_Lay_Solid", ffi_D2_TRK_Lay_RIn, ffi_D2_TRK_Lay_ROut,
                                       ffi_D2_TRK_Lay_SizeZ / 2., 0., 360 * deg);
    ffi_D2_TRK_Lay_Logic = new G4LogicalVolume(ffi_D2_TRK_Lay_Solid, ffi_D2_TRK_Lay_Material, "ffi_D2_TRK_Lay_Logic");

    if (ffi_D2_TRK_Lay_Logic) ffi_D2_TRK_Lay_Logic->SetSensitiveDetector(fCalorimeterSD);


    ffsi_counter = 0;
    //------------------ in front ------
    for (int fflay = 0; fflay < 5; fflay++) {
        double Z = -ffi_D2_GVol_SizeZ / 2 + (fflay + 1) * ffi_D2_TRK_Lay_SizeZ / 2 + (fflay + 1) * 5 * cm;
        ffi_D2_TRK_Lay_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, Z),
                                                    "ffi_D2_TRK_Lay_Phys", ffi_D2_TRK_Lay_Logic, ffi_D2_GVol_Phys,
                                                    false, ffsi_counter);
        ffsi_counter++;
        attr_ffi_D2_TRK_Lay = new G4VisAttributes(G4Color(0.8, 0.4 + 0.1 * fflay, 0.3, 1.));
        attr_ffi_D2_TRK_Lay->SetLineWidth(1);
        attr_ffi_D2_TRK_Lay->SetForceSolid(true);
        ffi_D2_TRK_Lay_Logic->SetVisAttributes(attr_ffi_D2_TRK_Lay);
    }

    //------------------ in middle  ------
    for (int fflay = 0; fflay < 5; fflay++) {
        double Z = (fflay + 1) * ffi_D2_TRK_Lay_SizeZ / 2 + (fflay + 1) * 5 * cm;
        ffi_D2_TRK_Lay_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, Z),
                                                    "ffi_D2_TRK_Lay_Phys", ffi_D2_TRK_Lay_Logic, ffi_D2_GVol_Phys,
                                                    false, ffsi_counter);
        ffsi_counter++;
        attr_ffi_D2_TRK_Lay= new G4VisAttributes(G4Color(0.8, 0.4 + 0.1 * fflay, 0.3, 1.));
        attr_ffi_D2_TRK_Lay->SetLineWidth(1);
        attr_ffi_D2_TRK_Lay->SetForceSolid(true);
        ffi_D2_TRK_Lay_Logic->SetVisAttributes(attr_ffi_D2_TRK_Lay);
    }
#endif
    //====================================================================================
    //==                          GEM DETECTOR VOLUME  after  D2                      ==
    //====================================================================================

#ifdef USE_FARFORWARD_GEM

    ffi_D2AFTER_GVol_RIn = 0 * cm;
    ffi_D2AFTER_GVol_ROut = 90 * cm;
    ffi_D2AFTER_GVol_SizeZ = 30 * cm;
    //double fGEM_FARFORWD_Zshift=0*cm;
    // ffi_D2AFTER_GVol_PosZ= 4000*cm;

    for (int id = 0; id < 20; id++) {
        if (strcmp(fSolid_BigDi_ffqsNAME[id], "iBDS2") ==
            0) { //printf("found D2 =%s  Z=%f dZ=%f \n",fSolid_BigDi_ffqsNAME[id],fSolid_BigDi_ffqsZ[id], fSolid_BigDi_ffqsSizeZDi[id]);
            ffi_D2AFTER_GVol_PosZ =
                    fSolid_BigDi_ffqsZ[id] * m + fSolid_BigDi_ffqsSizeZDi[id] / 2 * m + ffi_D2AFTER_GVol_SizeZ / 2 +
                    10 * cm;
            ffi_D2AFTER_GVol_PosX = fSolid_BigDi_ffqsX[id] * m - 10 * cm;
            //  printf("found D2 =%f \n", ffi_D2AFTER_GVol_PosZ);
        };
    };


    ffi_D2AFTER_GVol_Solid = new G4Tubs("ffi_D2AFTER_GVol_Solid", ffi_D2AFTER_GVol_RIn, ffi_D2AFTER_GVol_ROut,
                                    ffi_D2AFTER_GVol_SizeZ / 2., 0., 360 * deg);

    ffi_D2AFTER_GVol_Logic = new G4LogicalVolume(ffi_D2AFTER_GVol_Solid, World_Material, "ffi_D2AFTER_GVol_Logic");

    // ci_GEM_GVol_PosZ= SizeZ/2-abs(World_ShiftVTX)+ci_GEM_GVol_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed
    ffi_D2AFTER_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(ffi_D2AFTER_GVol_PosX, 0, ffi_D2AFTER_GVol_PosZ),
                                             "ffi_D2AFTER_GVol_Phys", ffi_D2AFTER_GVol_Logic,
                                             World_Phys, false, 0);
    attr_ffi_D2AFTER_GVol = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    attr_ffi_D2AFTER_GVol->SetLineWidth(1);
    attr_ffi_D2AFTER_GVol->SetForceSolid(true);
    ffi_D2AFTER_GVol_Logic->SetVisAttributes(attr_ffi_D2AFTER_GVol);
    // ---------------------------------------------------------------------------
    //                     GEM detector layers
    // ---------------------------------------------------------------------------
    ffi_D2AFTER_TRK_Lay_RIn = 0 * cm;
    ffi_D2AFTER_TRK_Lay_ROut = 45 * cm;
    ffi_D2AFTER_TRK_Lay_SizeZ = 1 * cm;
    //    ffi_D2AFTER_TRK_Lay_Material = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----
 ffi_D2AFTER_TRK_Lay_Material =fMat->GetMaterial("G4_Galactic");
    ffi_D2AFTER_TRK_Lay_Solid = new G4Tubs("ffi_D2AFTER_TRK_Lay_Solid", ffi_D2AFTER_TRK_Lay_RIn, ffi_D2AFTER_TRK_Lay_ROut,
                                        ffi_D2AFTER_TRK_Lay_SizeZ / 2., 0., 360 * deg);
    ffi_D2AFTER_TRK_Lay_Logic = new G4LogicalVolume(ffi_D2AFTER_TRK_Lay_Solid, ffi_D2AFTER_TRK_Lay_Material, "ffi_D2AFTER_TRK_Lay_Logic");

    if (ffi_D2AFTER_TRK_Lay_Logic) ffi_D2AFTER_TRK_Lay_Logic->SetSensitiveDetector(fCalorimeterSD);

    attr_ffi_D2AFTER_TRK_Lay = new G4VisAttributes(G4Color(0.8, 0.4, 0.3, 0.8));
    attr_ffi_D2AFTER_TRK_Lay->SetLineWidth(1);
    attr_ffi_D2AFTER_TRK_Lay->SetForceSolid(true);
    ffi_D2AFTER_TRK_Lay_Logic->SetVisAttributes(attr_ffi_D2AFTER_TRK_Lay);

    //---------------------------- after D2-----------------------
    int ff_counter = 0;
    for (int fflay = 0; fflay < 5; fflay++) {
        double Z = -ffi_D2AFTER_GVol_SizeZ / 2 + (fflay + 1) * ffi_D2AFTER_TRK_Lay_SizeZ / 2 + (fflay + 1) * 5 * cm;
        ffi_D2AFTER_TRK_Lay_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, Z),
                                                     "ffi_D2AFTER_TRK_Lay_Phys", ffi_D2AFTER_TRK_Lay_Logic,
                                                     ffi_D2AFTER_GVol_Phys, false, ff_counter);
        ff_counter++;
    }

#endif

#ifdef USE_FARFORWARD_VP

//====================================================================================
    //==                          VIRTUAL PLANES                                      ==
    //====================================================================================

    fFARFORWARD_VP_Rout = 90 * cm;
    fFARFORWARD_VP_SizeZ = 1 * cm;
    fFARFORWARD_VP_X = 0.;
    fFARFORWARD_VP_Z = 0.;
    fFARFORWARD_VP_angle = 0.;
//   float fSolid_ffqsSizeZ[100],  fSolid_ffqsRin[100],  fSolid_ffqsRout[100];
//  float  fSolid_ffqsX[100], fSolid_ffqsY[100],fSolid_ffqsZ[100];

    fSolid_FARFORWARD_VP = new G4Tubs("VP_FARFORWD_solid", 0., fFARFORWARD_VP_Rout, fFARFORWARD_VP_SizeZ / 2., 0.,
                                      360 * deg);
    fLogic_FARFORWARD_VP = new G4LogicalVolume(fSolid_FARFORWARD_VP, ffqsMaterial_G, "VP_FARFORWD_logic");

    int myid;
    for (int id = 0; id < 20; id++) {
        if (strcmp(fSolid_ffqsNAME[id], "iQDS1a") != 0) { continue; }
        else { myid = id; }
    }

    printf("found iBDS1a =%s  Z=%f dZ=%f \n", fSolid_ffqsNAME[myid], fSolid_ffqsZ[myid], fSolid_ffqsSizeZ[myid]);
    fFARFORWARD_VP_Z = fSolid_ffqsZ[myid] * m - fSolid_ffqsSizeZ[myid] / 2 * m - fFARFORWARD_VP_SizeZ / 2 - 2 * cm;
    fFARFORWARD_VP_X = fSolid_ffqsX[myid] * m + 20 * cm;

    fPhysics_FARFORWARD_VP = new G4PVPlacement(
            G4Transform3D(brm_hd[myid], G4ThreeVector(fFARFORWARD_VP_X, 0, fFARFORWARD_VP_Z)), "VP_FARFORWD_physics_1",
            fLogic_FARFORWARD_VP,
            World_Phys, false, 0);

    fFARFORWARD_VP_Z = fSolid_ffqsZ[myid] * m + fSolid_ffqsSizeZ[myid] / 2 * m + fFARFORWARD_VP_SizeZ / 2 + 5 * cm;
    fFARFORWARD_VP_X = fSolid_ffqsX[myid] * m + 20 * cm;
    fPhysics_FARFORWARD_VP = new G4PVPlacement(
            G4Transform3D(brm_hd[myid], G4ThreeVector(fFARFORWARD_VP_X, 0, fFARFORWARD_VP_Z)), "VP_FARFORWD_physics_2",
            fLogic_FARFORWARD_VP,
            World_Phys, false, 0);


    vvpf1 = new G4VisAttributes(G4Color(0.9, 0.3, 0., 1.));
    vvpf1->SetLineWidth(1);
    vvpf1->SetForceSolid(true);
    fLogic_FARFORWARD_VP->SetVisAttributes(vtpc1);

    //  if ( fLogic_FARFORWARD_VP)   fLogic_FARFORWARD_VP->SetSensitiveDetector(fCalorimeterSD);

#endif




//=========================================================================
    printf("=================>  before printing parameters !!!\n");
    PrintGeometryParameters();
//=========================================================================
#ifdef USE_TGEOM
    printf("=================>  EXPORT ROOT GEOMETRY !!!\n");
    // ---------------------------------------------------------------------------
    // VGM demo
    // Export geometry in Root and save it in a file
    //
    //
    // Import Geant4 geometry to VGM
    Geant4GM::Factory g4Factory;
    g4Factory.SetDebug(0);
    g4Factory.Import(World_Phys);
    //
    // Export VGM geometry to Root
    RootGM::Factory rtFactory;
    rtFactory.SetDebug(0);
    g4Factory.Export(&rtFactory);
    //
    // Import Geant4 geometry to VGM
    Geant4GM::Factory factory;
    factory.Import(World_Phys);    
    // Export VGM geometry in GDML
    XmlVGM::GDMLExporter gdmlExporter(&factory);
    gdmlExporter.GenerateXMLGeometry();
    //
    gGeoManager->CloseGeometry();
    gGeoManager->Export("geometryJLEIC.root");
    //
    // end VGM demo
    //---------------------------------------------------------------------------
#endif

    printf("FoilNumbers3=%d\n",ci_TRD.ConstructionConfig.fFoilNumber);
    printf("exit Detector Construction\n");


    return World_Phys;
}



//==========================================================================================================
//                                      JLEIC   END
//==========================================================================================================



G4VPhysicalVolume *JLeicDetectorConstruction::SetUpJLEIC09() {
    return nullptr;
}



////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::PrintGeometryParameters() {
    G4cout << "\n The  WORLD   is made of "
           << fConfig.World.SizeZ / mm << "mm of " << World_Material->GetName();
    G4cout << ", the transverse size (R) of the world is " << fConfig.World.SizeR / mm << " mm. " << G4endl;
    G4cout << "WorldMaterial = " << World_Material->GetName() << G4endl;
    //  G4cout<<"fVTX_END_Z = "<<fVTX_END_Z/mm<<" mm"<<G4endl;
    G4cout << G4endl;
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetAbsorberMaterial(G4String materialChoice) {
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

void JLeicDetectorConstruction::SetRadiatorMaterial(G4String materialChoice) {
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

void JLeicDetectorConstruction::SetWorldMaterial(G4String materialChoice) {
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

void JLeicDetectorConstruction::SetAbsorberThickness(G4double val) {
    // change Absorber thickness and recompute the calorimeter parameters
    fConfig.ci_TRD.fAbsorberThickness = val;
    //  ComputeCalorParameters();
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetRadiatorThickness(G4double val) {
    // change XTR radiator thickness and recompute the calorimeter parameters
    fConfig.ci_TRD.fRadThickness = val;
    // ComputeCalorParameters();
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetGasGapThickness(G4double val) {
    // change XTR gas gap thickness and recompute the calorimeter parameters
    fConfig.ci_TRD.fGasGap = val;
    // ComputeCalorParameters();
}

/////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetAbsorberRadius(G4double val) {
    // change the transverse size and recompute the calorimeter parameters
    fConfig.ci_TRD.fAbsorberRadius = val;
    // ComputeCalorParameters();
}

////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetWorldSizeZ(G4double val) {
    fWorldChanged = true;
    fConfig.World.SizeZ = val;
    // ComputeCalorParameters();
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetWorldSizeR(G4double val) {
    fWorldChanged = true;
    fConfig.World.SizeR = val;
    // ComputeCalorParameters();
}

//////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetAbsorberZpos(G4double val) {
    fConfig.ci_TRD.fAbsorberZ = val;
    // ComputeCalorParameters();
}

//////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetMagField(G4double) {
    //apply a global uniform magnetic field along X axis

    /* *********************************************************

  G4FieldManager* fieldMgr
   = G4TransportationManager::GetTransportationManager()->GetFieldManager();

  if(magField) delete magField;		//delete the existing magn field

  if(fieldValue!=0.)			// create a new one if non nul
  {
    magField = new G4UniformMagField(G4ThreeVector(fieldValue,0.,0.));
    fieldMgr->SetDetectorField(magField);
    fieldMgr->CreateChordFinder(magField);
  }
  else
  {
    magField = 0;
    fieldMgr->SetDetectorField(magField);
  }

  *************************************************************** */

}


///////////////////////////////////////////////////////////////////////////////
//
//
G4FieldManager *JLeicDetectorConstruction::SetQMagField(float field, float skew, float theta, G4ThreeVector fieldorigin)
//
//
////////////////////////////////////////////////////////////////////////////

{
    G4RotationMatrix *qrm_f;
    printf("SetQMagField:: got parameters %f %f \n", field, skew);


    //G4double fGradient = -223.*tesla/m;
    G4double fGradient = sqrt(field * field + skew * skew) * tesla / m;
    G4double angle;
    if (field != 0) { angle = 0.5 * atan2(skew, field); }
    else {
        if (skew != 0) { angle = 0.5 * (3.1415 / 2 - atan2(field, skew)); }
        else { angle = 0; }
    }

    qrm_f = new G4RotationMatrix();

    qrm_f->rotateZ(angle * rad);
    qrm_f->rotateY(theta * rad);

    //G4ThreeVector fieldValue = G4ThreeVector( fGradient_x,fGradient_y,fGradient_z);
    // old  G4QuadrupoleMagField* pipeField = new G4QuadrupoleMagField(fGradient);

    // G4ThreeVector fieldorigin = G4ThreeVector(0,0,0);

    G4QuadrupoleMagField *pipeField = new G4QuadrupoleMagField(fGradient, fieldorigin, qrm_f);

    G4Mag_UsualEqRhs *myEquation = new G4Mag_UsualEqRhs(pipeField);
    G4MagIntegratorStepper *myStepper = new G4ClassicalRK4(myEquation);
    G4ChordFinder *myChordFinder = new G4ChordFinder(pipeField, 0.1 * mm, myStepper);

    G4FieldManager *fieldMgr = new G4FieldManager(pipeField, myChordFinder, false);

    //logicSpace_Pipe->SetFieldManager(fieldMgr,true);

/*
G4Box *solidB = new G4Bo"solidB", 1*m, 1*m, 1*m);
G4LogicalVolume* logicB = new G4LogicalVolume(solidB, Mat("G4_Galactic"), "logicB",fieldMgrq);
new G4PVPlacement(0, G4ThreeVector(5*m,5*m,-7*m), logicB,
                                           "physiB", world->getLogic(), false, 0);
*/
    //delete qrm_f;
    return fieldMgr;
}


G4FieldManager *JLeicDetectorConstruction::SetDipoleMagField(G4double fx, G4double fy, G4double fz, float theta) {
    // G4Box  *sMagField = new G4Bo "dipole_magfield", 10/2*cm, 10/2*cm, 10/2*cm);
    //  G4LogicalVolume  *lMagField = new G4LogicalVolume(sMagField, World_Material, "logical_magnet");

    double ffx, ffy, ffz;
    ffx = fx * cos(theta * rad) + fz * sin(theta * rad);
    ffy = fy;
    ffz = -fx * sin(theta * rad) + fz * cos(theta * rad);

    G4ThreeVector fieldValue = G4ThreeVector(ffx * tesla, ffy * tesla, ffz * tesla);
    G4UniformMagField *magField = new G4UniformMagField(fieldValue);
    G4FieldManager *fieldMgr = new G4FieldManager(magField);
    fieldMgr->SetDetectorField(magField);
    fieldMgr->CreateChordFinder(magField);
    //accuracy mag.
    G4double minEps = 1.0e-5; // Minimum & value for smallest steps
    G4double maxEps = 1.0e-4; // Maximum & value for largest steps
    fieldMgr->SetMinimumEpsilonStep(minEps);
    fieldMgr->SetMaximumEpsilonStep(maxEps);
    fieldMgr->SetDeltaOneStep(0.5 * um); // 0.5 micrometer
    return fieldMgr;
//  new G4PVPlacement(0, G4ThreeVector(0,0,ZPosVect*cm), lMagField, "Magnet",World_Logic, false, 0);
}
///////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::UpdateGeometry() {
    G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetectorXTR());
}

//
//
////////////////////////////////////////////////////////////////////////////



void JLeicDetectorConstruction::Read_Di_File() {
    //Downstream elements
//Element name	Type	   Length [m] Good_field half-aperture [cm]
//                                           Inner Half-A [cm]
//                                                     Outer Radius [cm]
//                                                            Dipole field [T]
//                                                                   Quadrupole field [T/m]
//                                                                                            Sextupole [T/m^2]
//                                                                                                     Solenoid [T]
//                                                                                                              X center [m] Y center [m]
//                                                                                                                        	Z center [m]
//                                                                                                                                      Theta center [rad]	Phi [rad]

    //iDSDS	SOLENOID	2.4	2	160	210	0	0	0	0	0	-3.0	-0.060	0	1.199	-0.050	0
    //iBDS1a	RBEND   	0.85	4	35.2	45.2	1.30	2.47	0	0	0	0	-0.271	0	5.418	-0.051	0
    //iBDS1b	RBEND   	0.85	4	40.5	50.5	-1.09	2.24	0	0	0	0	-0.327	0	6.467	-0.054	0
    //iQDS1a	QUADRUPOLE	2.25	4	9.2	23.1	0	0	-37.24	0.63	0	0	-0.419	0	8.114	-0.056	0
    //iQDS1S	QUADRUPOLE	0.5	4	9.9	24.8	0	0	0	-0.50	0	0	-0.507	0	9.687	-0.056	0
    //iQDS1b	QUADRUPOLE	2.25	4	12.3	31.0	0	0	-37.24	0	0	0	-0.595	0	11.259	-0.056	0
    //iQDS2S	QUADRUPOLE	0.5	4	13.0	32.7	0	0	0	0.00078	0	0	-0.684	0	12.832	-0.056	0
    //iQDS2	QUADRUPOLE	4.5	4	17.7	44.4	0	0	25.96	0	0	0	-0.835	0	15.528	-0.056	0
    //iQDS3S	QUADRUPOLE	0.5	4	18.4	46.2	0	0	0	-1.24	0	0	-0.986	0	18.223	-0.056	0
    //iASDS	SOLENOID	1.2	4	19.8	49.7	0	0	0	0	0	6.0	-1.045	0	19.272	-0.056	0
    //iBDS2	RBEND   	8.00	4	40	90	0	-4.67	0	0	0	0	-1.296	0	25.766	-0.028	0
    //iBDS3	RBEND   	6.50	4	4	30	0	5.75	0	0	0	0	-1.499	0	44.560	-0.028	0
    //iQDS4	QUADRUPOLE	0.8	3	4	30	0	0	144.1	0	0	0	-1.875	0	52.890	-0.056	0
    FILE *rc;
    char buffer[512];
/*
    char ffqnameDi[20][128];
    char ffqnameDi[20][128];
    double  ffqsRinDi[20];
    double  ffqsRoutDi[20];
    double ffqsSizeZDi[20];
    double ffqsZ1Di[20];
    double ffqsX1Di[20];
    double colorDi[20];
    double qFIELDx[20];
    double qFIELDy[20];
*/
    char ffqtype[256];
    char fname[256];
    char mychar[40];
    char ffqnameDi[256];
    float ffqsRinDiG;
    float ffqsRinDi;
    float ffqsRoutDi;
    float ffqsSizeZDi;
    float qFIELDx;
    float qFIELDy;
    float qFIELQn;
    float qFIELQs;
    float qFIELSek;
    float qFIELSol;
    float ffqsZ;
    float ffqsY;
    float ffqsX;
    float ffqsTheta;
    float ffqsPhi;
    int iqmax_i;
    printf("read Di file\n");
    // sprintf(fname,"ion_ir_06feb19.txt");
     sprintf(fname,"ion_ir_23sep19.txt");

    // --- this is for tune!!
   // sprintf(fname, "i_ir_forward.txt");
 //   sprintf(fname, " ");
    rc = fopen(fname, "r");
    if (rc == NULL) return;


    // int iq=0,ik=0,is=0;
    printf("read Di file %s\n", fname);
    while (fgets(buffer, 512, (FILE *) rc)) {

        printf("*********************************************************************************\n");
        printf("%s\n", buffer);
        sscanf(buffer, "%s", mychar);
        printf("mychar=%s\n ", mychar);
        if (mychar[0] != 'i' || mychar[0] == '\n') {
            printf("SKIP LINE %s\n", buffer);
            continue;
        }

        sscanf(buffer, "%s %s %f %f %f %f %f %f  %f %f %f %f %f  %f %f %f %f", ffqnameDi, ffqtype, &ffqsSizeZDi,
               &ffqsRinDiG, &ffqsRinDi, &ffqsRoutDi, &qFIELDx, &qFIELDy,
               &qFIELQn, &qFIELQs, &qFIELSek, &qFIELSol, &ffqsX, &ffqsY, &ffqsZ, &ffqsTheta, &ffqsPhi);

        printf(" NUM: |%s| %s Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ",
               ffqnameDi, ffqtype, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi,
               qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

        // ----------- create volumes for QUADRUPOLE----------
	if (strcmp(ffqtype, "QUADRUPOLE") == 0) {
            printf(" found QUAD %s iq=%d \n", ffqtype, iq);
            CreateQuad(iq, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn,
                       qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);
            iq++; iqmax_i=iq;
        }
	
        // ----------- create volumes for kickers and rbend----------
        if ((strcmp(ffqtype, "KICKER") == 0) || (strcmp(ffqtype, "RBEND") == 0)) {
            printf(" found KICKER %s \n", ffqtype);
            CreateDipole(ik, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn,
                         qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);
            ik++;
        }
        // ----------- create volumes for solenoid  ----------
        if ((strcmp(ffqtype, "SOLENOID") == 0) &&
            ((strcmp(ffqnameDi, "iASDS") == 0) || (strcmp(ffqnameDi, "iASUS") == 0))) {
            printf(" found SOLENOID %s \n", ffqtype);

            CreateASolenoid(is, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn,
                            qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);
            is++;
        }
    }


    fclose(rc);
    return;
}


void JLeicDetectorConstruction::Read_dE_File() {
    //Downstream elements
//Element name	Type	   Length [m] Good_field half-aperture [cm]
//                                           Inner Half-A [cm]
//                                                     Outer Radius [cm]
//                                                            Dipole field [T]
//                                                                   Quadrupole field [T/m]
//                                                                                            Sextupole [T/m^2]
//                                                                                                     Solenoid [T]
//                                                                                                              X center [m] Y center [m]
//                                                                                                                        	Z center [m]
//                                                                                                                                      Theta center [rad]	Phi [rad]

    FILE *rc;
    char buffer[512];
/*
    char ffqnameDi[20][128];
    char ffqnameDi[20][128];
    double  ffqsRinDi[20];
    double  ffqsRoutDi[20];
    double ffqsSizeZDi[20];
    double ffqsZ1Di[20];
    double ffqsX1Di[20];
    double colorDi[20];
    double qFIELDx[20];
    double qFIELDy[20];
*/
    char ffqtype[256];
    char fname[256];
    char mychar[40];
    char ffqnameDi[256];
    float ffqsRinDiG;
    float ffqsRinDi;
    float ffqsRoutDi;
    float ffqsSizeZDi;
    float qFIELDx;
    float qFIELDy;
    float qFIELQn;
    float qFIELQs;
    float qFIELSek;
    float qFIELSol;
    float ffqsZ;
    float ffqsY;
    float ffqsX;
    float ffqsTheta;
    float ffqsPhi;

    printf("read dE file\n");
    // sprintf(fname,"jleic_v16v2_det_reg_elem_bg_sim_18Oct18_electron.txt");
    //sprintf(fname,"ele_06feb10.txt");
    sprintf(fname, "e_ir.txt");
    rc = fopen(fname, "r");
    if (rc == NULL) return;
    //int iq=0,ik=0,is=0;
    printf("read Di file %s\n", fname);
    while (fgets(buffer, 512, (FILE *) rc)) {

        printf("Read_dE*********************************************************************************\n");
        printf("Read_dE %s\n", buffer);
        sscanf(buffer, "%s", mychar);
        printf("Read_dE:: mychar=%s\n ", mychar);
        if (mychar[0] != 'e' || mychar[0] == '\n') {
            printf("SKIP LINE %s\n", buffer);
            continue;
        }

        sscanf(buffer, "%s %s %f %f %f %f %f %f  %f %f %f %f %f  %f %f %f %f", ffqnameDi, ffqtype, &ffqsSizeZDi,
               &ffqsRinDiG, &ffqsRinDi, &ffqsRoutDi, &qFIELDx, &qFIELDy,
               &qFIELQn, &qFIELQs, &qFIELSek, &qFIELSol, &ffqsX, &ffqsY, &ffqsZ, &ffqsTheta, &ffqsPhi);

        printf("Read_dE dE NUM: |%s| %s Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ",
               ffqnameDi, ffqtype, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi,
               qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

        // ----------- create volumes for QUADRUPOLE----------
        if (strcmp(ffqtype, "QUADRUPOLE") == 0) {
            printf("Read_dE found QUAD %s iq=%d\n", ffqtype, iq);
            CreateQuad(iq, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn,
                       qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);
            iq++;
        }
        // ----------- create volumes for kickers and rbend----------
        if ((strcmp(ffqtype, "KICKER") == 0) || (strcmp(ffqtype, "RBEND") == 0)) {
            printf("Read_dE found KICKER %s ik=%d \n", ffqtype, ik);
            CreateDipoleChicane(ik, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy,
                                qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);
            ik++;
        }
        // ----------- create volumes for solenoid  ----------
        if ((strcmp(ffqtype, "SOLENOID") == 0) &&
            ((strcmp(ffqnameDi, "eASDS") == 0) || (strcmp(ffqnameDi, "eASUS") == 0))) {
            printf("Read_dE found SOLENOID %s \n", ffqtype);

            CreateASolenoid(is, ffqnameDi, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi, qFIELDx, qFIELDy, qFIELQn,
                            qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);
            is++;
        }
    }

    fclose(rc);
    return;
}




//==============================================================================================================
//                          Quadrupole
//==============================================================================================================

void JLeicDetectorConstruction::CreateQuad(int j, char *ffqsNAME, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi,
                                           float ffqsRoutDi,
                                           float qFIELDx, float qFIELDy, float qFIELQn, float qFIELQs, float qFIELSek,
                                           float qFIELSol, float ffqsX, float ffqsY, float ffqsZ, float ffqsTheta,
                                           float ffqsPhi) {
    char abname[256];

    printf("CreateQuad:: NUM: %d |%s|  Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ",
           j, ffqsNAME, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi,
           qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

    //return;

    vb1 = new G4VisAttributes(G4Color(0.8, 0.3, 0.1, 0.9));
    vb1->SetForceSolid(true);

    printf("CreateQuad:: theta =%f %f %f \n", ffqsTheta, ffqsTheta * rad, (ffqsTheta * 180 / 3.1415) * deg);
    brm_hd[j].rotateY(ffqsTheta * rad);
    // brm_hd.rotateY((0*180/3.1415)*deg);

    ffqsMaterial = fMat->GetMaterial("IronAll");
    ffqsMaterial_G = fMat->GetMaterial("G4_Galactic");

    //fZ1=ffqsZ1Di[j];
    // printf(" FFQs name %s, Z=%f, L=%f, R=%f \n",ffqnameDi[j], ffqsZ1Di[j],  ffqsSizeZDi[j], ffqsRoutDi[j]);

    sprintf(fSolid_ffqsNAME[j], "%s", ffqsNAME);
    fSolid_ffqsSizeZ[j] = ffqsSizeZDi;
    fSolid_ffqsRin[j] = ffqsRinDi;
    fSolid_ffqsRout[j] = ffqsRoutDi;
    fSolid_ffqsX[j] = ffqsX;
    fSolid_ffqsY[j] = ffqsY;
    fSolid_ffqsZ[j] = ffqsZ;


    //--------------------Volumes ---------
    sprintf(abname, "Solid_QUADS_hd_v_%s", ffqsNAME);
    fSolid_QUADS_hd_v[j] = new G4Tubs(abname, 0., (ffqsRoutDi + 0.01) * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
    sprintf(abname, "Logic_QUADS_hd_v_%s", ffqsNAME);
    fLogic_QUADS_hd_v[j] = new G4LogicalVolume(fSolid_QUADS_hd_v[j], World_Material, abname);
    sprintf(abname, "Physics_QUADS_hd_v_%s", ffqsNAME);
    fPhysics_QUADS_hd_v[j] = new G4PVPlacement(G4Transform3D(brm_hd[j], G4ThreeVector(ffqsX * m, ffqsY * m, ffqsZ * m)),
                                               abname,
                                               fLogic_QUADS_hd_v[j], World_Phys, false, 0);
    //printf("create %s ");

    //--------------------Iron---------
    sprintf(abname, "Solid_QUADS_hd_ir_%s", ffqsNAME);
    fSolid_QUADS_hd_ir[j] = new G4Tubs(abname, ffqsRinDi * cm, (ffqsRoutDi + 0.005) * cm, (ffqsSizeZDi / 2.) * m, 0.,
                                       360 * deg);
    sprintf(abname, "Logic_QUADS_hd_ir_%s", ffqsNAME);
    fLogic_QUADS_hd_ir[j] = new G4LogicalVolume(fSolid_QUADS_hd_ir[j], ffqsMaterial, abname);
    sprintf(abname, "Physics_QUADS_hd_ir_%s", ffqsNAME);
    fPhysics_QUADS_hd_ir[j] = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_QUADS_hd_ir[j],
                                                fPhysics_QUADS_hd_v[j], false, 0);
    fLogic_QUADS_hd_ir[j]->SetVisAttributes(vb1);

    //---------------- set magnetic field ---------------
    sprintf(abname, "Solid_QUADS_hd_m_%s", ffqsNAME);
    fSolid_QUADS_hd_m[j] = new G4Tubs(abname, 0. * cm, ffqsRinDi * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
    sprintf(abname, "Logic_QUADS_hd_m_%s", ffqsNAME);
    fLogic_QUADS_hd_m[j] = new G4LogicalVolume(fSolid_QUADS_hd_m[j], ffqsMaterial_G, abname);
    sprintf(abname, "Physics_QUADS_hd_m_%s", ffqsNAME);
    fPhysics_QUADS_hd_m[j] = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_QUADS_hd_m[j], fPhysics_QUADS_hd_v[j],
                                               false, 0);


    //    G4FieldManager* fieldMgr = SetQMagField(qFIELDx[j],qFIELDy[j]);   // gradient tesla/m;
    // fLogic_QUADSm[j]->SetFieldManager(fieldMgr,true);

    printf("CreateQuad:: FIELD =%f %f --  %f %f -- %f %f \n", qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol);


    fieldMgr_QUADS_hd[j] = SetQMagField(qFIELQn, qFIELQs, ffqsTheta,
                                        G4ThreeVector(ffqsX * m, ffqsY * m, ffqsZ * m));   // gradient tesla/m;

    fLogic_QUADS_hd_m[j]->SetFieldManager(fieldMgr_QUADS_hd[j], true);

}


//==============================================================================================================
//                          DIPOLE
//==============================================================================================================

void
JLeicDetectorConstruction::CreateDipole(int j, char *ffqsNAME, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi,
                                        float ffqsRoutDi,
                                        float qFIELDx, float qFIELDy, float qFIELQn, float qFIELQs, float qFIELSek,
                                        float qFIELSol, float ffqsX, float ffqsY, float ffqsZ, float ffqsTheta,
                                        float ffqsPhi) {
    char abname[256];
    //G4RotationMatrix brm_hk;

    printf("CreateKicker:: NUM: %d |%s|  Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ",
           j, ffqsNAME, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi,
           qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

    /*----------FFQs ions--------------*/

    ffqsMaterial = fMat->GetMaterial("IronAll");
    ffqsMaterial_G = fMat->GetMaterial("G4_Galactic");

    brm_hk[j].rotateY(ffqsTheta * rad);

    sprintf(fSolid_BigDi_ffqsNAME[j], "%s", ffqsNAME);
    fSolid_BigDi_ffqsSizeZDi[j] = ffqsSizeZDi;
    fSolid_BigDi_ffqsRinDi[j] = ffqsRinDi;
    fSolid_BigDi_ffqsRoutDi[j] = ffqsRoutDi;
    fSolid_BigDi_ffqsX[j] = ffqsX;
    fSolid_BigDi_ffqsY[j] = ffqsY;
    fSolid_BigDi_ffqsZ[j] = ffqsZ;

    //-------------------------- Magnetic volume------------------------
    sprintf(abname, "Solid_DIPOLE_v_%s", ffqsNAME);
    fSolid_BigDi_v[j] = new G4Tubs(abname, 0., (ffqsRoutDi + 0.01) * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);

    // fSolid_BigDi_v[j] = new G4Box(abname, (double)(ffqsRoutDi+0.01)*cm, (double)(ffqsRoutDi+0.01)*cm,(double)(ffqsSizeZDi/2.)*m);
    sprintf(abname, "Logic_DIPOLE_v_%s", ffqsNAME);
    fLogic_BigDi_v[j] = new G4LogicalVolume(fSolid_BigDi_v[j], World_Material, abname);
    sprintf(abname, "Physics_DIPOLE_v_%s", ffqsNAME);
    fPhysics_BigDi_v[j] = new G4PVPlacement(G4Transform3D(brm_hk[j], G4ThreeVector(ffqsX * m, ffqsY * m, ffqsZ * m)),
                                            abname, fLogic_BigDi_v[j], World_Phys, false, 0);
    printf(" Finish magnetic volume and start physics volume \n");

    //-------------------------- Magnet iron------------------------
    sprintf(abname, "Solid_DIPOLE_i_%s", ffqsNAME);
    fSolid_BigDi_ir[j] = new G4Tubs(abname, ffqsRinDi * cm, ffqsRoutDi * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
    // fSolid_BigDi_ir[j] = new G4Box(abname, (double) ffqsRoutDi*cm, (double)ffqsRoutDi*cm,((double)ffqsSizeZDi/2.)*m);
    sprintf(abname, "Logic_DIPOLE_i_%s", ffqsNAME);
    fLogic_BigDi_ir[j] = new G4LogicalVolume(fSolid_BigDi_ir[j], ffqsMaterial, abname);
    sprintf(abname, "Physics_DIPOLE_i_%s", ffqsNAME);
    fPhysics_BigDi_ir[j] = new G4PVPlacement(0, G4ThreeVector(),
                                             abname, fLogic_BigDi_ir[j], fPhysics_BigDi_v[j], false, 0);

    vb1a = new G4VisAttributes(G4Color(0.2, 0.8, 0.2, 1.));
    vb1a->SetForceSolid(true);
    fLogic_BigDi_ir[j]->SetVisAttributes(vb1a);

    //-------------------------- Magnet field------------------------
    sprintf(abname, "Solid_DIPOLE_m_%s", ffqsNAME);
    fSolid_BigDi_m[j] = new G4Tubs(abname, 0., ffqsRinDi * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
    //  fSolid_BigDi_m[j] = new G4Box(abname,(double)ffqsRinDi*cm,(double)ffqsRinDi*cm,(double)(ffqsSizeZDi/2.)*m);
    sprintf(abname, "Logic_DIPOLE_m_%s", ffqsNAME);
    fLogic_BigDi_m[j] = new G4LogicalVolume(fSolid_BigDi_m[j], ffqsMaterial_G, abname);
    sprintf(abname, "Physics_DIPOLE_m_%s", ffqsNAME);
    fPhysics_BigDi_m[j] = new G4PVPlacement(0, G4ThreeVector(),
                                            abname, fLogic_BigDi_m[j], fPhysics_BigDi_v[j], false, 0);

    vb1b = new G4VisAttributes(G4Color(1., 1., 0.8, 0.5));
    vb1b->SetForceSolid(true);
    fLogic_BigDi_m[j]->SetVisAttributes(vb1b);


    printf(" Start assign magnet  \n");
    fieldMgr_BigDi[j] = SetDipoleMagField(qFIELDx, qFIELDy, 0., ffqsTheta);   // gradient tesla/m;
    fLogic_BigDi_m[j]->SetFieldManager(fieldMgr_BigDi[j], true);


}

//==============================================================================================================
//                         Electron Chicane Dipoles
//==============================================================================================================


void JLeicDetectorConstruction::CreateDipoleChicane(int j, char *ffqsNAME, float ffqsSizeZDi, float ffqsRinDiG,
                                                    float ffqsRinDi, float ffqsRoutDi,
                                                    float qFIELDx, float qFIELDy, float qFIELQn, float qFIELQs,
                                                    float qFIELSek, float qFIELSol, float ffqsX, float ffqsY,
                                                    float ffqsZ, float ffqsTheta, float ffqsPhi) {
    char abname[256];
    //G4RotationMatrix brm_hk;

    printf("CreateKicker:: NUM: %d |%s|  Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ",
           j, ffqsNAME, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi,
           qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

    /*----------FFQs ions--------------*/

    ffqsMaterial = fMat->GetMaterial("IronAll");
    ffqsMaterial_G = fMat->GetMaterial("G4_Galactic");

    brm_hk[j].rotateY(ffqsTheta * rad);

    //-------------------------- Magnetic volume------------------------
    sprintf(abname, "Solid_v_%s", ffqsNAME);
    //   fSolid_Chicane_v[j] = new G4Tubs(abname, 0., (ffqsRoutDi+0.01)*cm,(ffqsSizeZDi/2.)*m,0.,360*deg);

    fSolid_Chicane_v[j] = new G4Box(abname, (double) (ffqsRoutDi + 0.01) * cm, (double) (ffqsRoutDi + 0.01) * cm,
                                    (double) (ffqsSizeZDi / 2.) * m);
    sprintf(abname, "Logic_Chicane_v_%s", ffqsNAME);
    fLogic_Chicane_v[j] = new G4LogicalVolume(fSolid_Chicane_v[j], World_Material, abname);
    sprintf(abname, "Physics_Chicane_v_%s", ffqsNAME);
    fPhysics_Chicane_v[j] = new G4PVPlacement(G4Transform3D(brm_hk[j], G4ThreeVector(ffqsX * m, ffqsY * m, ffqsZ * m)),
                                              abname, fLogic_Chicane_v[j], World_Phys, false, 0);
    printf(" Finish magnetic volume and start physics volume \n");

    //-------------------------- Magnet iron------------------------
    sprintf(abname, "Solid_i_%s", ffqsNAME);
    fSolid_Chicane_ir[j] = new G4Box(abname, (double) ffqsRoutDi * cm, (double) ffqsRoutDi * cm,
                                     ((double) ffqsSizeZDi / 2.) * m);
    sprintf(abname, "Logic_Chicane_i_%d", j);
    fLogic_Chicane_ir[j] = new G4LogicalVolume(fSolid_Chicane_ir[j], ffqsMaterial, abname);
    sprintf(abname, "Physics_Chicane_i_%d", j);
    fPhysics_Chicane_ir[j] = new G4PVPlacement(0, G4ThreeVector(),
                                               abname, fLogic_Chicane_ir[j], fPhysics_Chicane_v[j], false, 0);

    vb1a = new G4VisAttributes(G4Color(0.2, 0.8, 0.2, 1.));
    vb1a->SetForceSolid(true);
    fLogic_Chicane_ir[j]->SetVisAttributes(vb1a);

    //-------------------------- Magnet field------------------------
    sprintf(abname, "Solid_m_%s", ffqsNAME);
    //    fSolid_Chicane_m[j] = new G4Tubs(abname, 0., ffqsRinDi*cm,(ffqsSizeZDi/2.)*m,0.,360*deg);
    fSolid_Chicane_m[j] = new G4Box(abname, (double) ffqsRinDi * cm, (double) ffqsRinDi * cm,
                                    (double) (ffqsSizeZDi / 2.) * m);
    sprintf(abname, "Logic_Chicane_m_%d", j);
    fLogic_Chicane_m[j] = new G4LogicalVolume(fSolid_Chicane_m[j], ffqsMaterial_G, abname);
    sprintf(abname, "Physics_Chicane_m_%d", j);
    fPhysics_Chicane_m[j] = new G4PVPlacement(0, G4ThreeVector(),
                                              abname, fLogic_Chicane_m[j], fPhysics_Chicane_v[j], false, 0);

    vb1b = new G4VisAttributes(G4Color(1., 1., 0.8, 0.5));
    vb1b->SetForceSolid(true);
    fLogic_Chicane_m[j]->SetVisAttributes(vb1b);


    printf(" Start assign magnet  \n");
    fieldMgr_Chicane[j] = SetDipoleMagField(qFIELDx, qFIELDy, 0., ffqsTheta);   // gradient tesla/m;
    fLogic_Chicane_m[j]->SetFieldManager(fieldMgr_Chicane[j], true);


}

//==============================================================================================================
//                          ASOLENOID
//==============================================================================================================

void
JLeicDetectorConstruction::CreateASolenoid(int j, char *ffqsNAME, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi,
                                           float ffqsRoutDi,
                                           float qFIELDx, float qFIELDy, float qFIELQn, float qFIELQs, float qFIELSek,
                                           float qFIELSol, float ffqsX, float ffqsY, float ffqsZ, float ffqsTheta,
                                           float ffqsPhi) {

    char abname[256];
    // G4RotationMatrix brm_as;

    printf("CreateQuad:: NUM: %d |%s|  Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ",
           j, ffqsNAME, ffqsSizeZDi, ffqsRinDiG, ffqsRinDi, ffqsRoutDi,
           qFIELDx, qFIELDy, qFIELQn, qFIELQs, qFIELSek, qFIELSol, ffqsX, ffqsY, ffqsZ, ffqsTheta, ffqsPhi);

    //return;

    vb1 = new G4VisAttributes(G4Color(0.8, 0.3, 0.1, 0.9));
    vb1->SetForceSolid(true);

    printf("CreateQuad:: theta =%f %f %f \n", ffqsTheta, ffqsTheta * rad, (ffqsTheta * 180 / 3.1415) * deg);
    brm_as[j].rotateY(ffqsTheta * rad);

    ffqsMaterial = fMat->GetMaterial("IronAll");
    ffqsMaterial_G = fMat->GetMaterial("G4_Galactic");

    //fZ1=ffqsZ1Di[j];
    // printf(" FFQs name %s, Z=%f, L=%f, R=%f \n",ffqnameDi[j], ffqsZ1Di[j],  ffqsSizeZDi[j], ffqsRoutDi[j]);


    //--------------------Volumes ---------
    sprintf(abname, "Solid_ASOLENOID_hd_v_%s", ffqsNAME);
    fSolid_ASOLENOID_hd_v[j] = new G4Tubs(abname, 0., (ffqsRoutDi + 0.01) * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
    sprintf(abname, "Logic_ASOLENOID_hd_v_%s", ffqsNAME);
    fLogic_ASOLENOID_hd_v[j] = new G4LogicalVolume(fSolid_ASOLENOID_hd_v[j], World_Material, abname);
    sprintf(abname, "Physics_ASOLENOID_hd_v_%s", ffqsNAME);
    fPhysics_ASOLENOID_hd_v[j] = new G4PVPlacement(
            G4Transform3D(brm_as[j], G4ThreeVector(ffqsX * m, ffqsY * m, ffqsZ * m)), abname,
            fLogic_ASOLENOID_hd_v[j], World_Phys, false, 0);
    //printf("create %s ");
    //--------------------Iron---------
    sprintf(abname, "Solid_ASOLENOID_hd_ir_%s", ffqsNAME);
    fSolid_ASOLENOID_hd_ir[j] = new G4Tubs(abname, ffqsRinDi * cm, ffqsRoutDi * cm, (ffqsSizeZDi / 2.) * m, 0.,
                                           360 * deg);
    sprintf(abname, "Logic_ASOLENOID_hd_ir_%s", ffqsNAME);
    fLogic_ASOLENOID_hd_ir[j] = new G4LogicalVolume(fSolid_ASOLENOID_hd_ir[j], ffqsMaterial, abname);
    sprintf(abname, "Physics_ASOLENOID_hd_ir_%s", ffqsNAME);
    fPhysics_ASOLENOID_hd_ir[j] = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_ASOLENOID_hd_ir[j],
                                                    fPhysics_ASOLENOID_hd_v[j], false, 0);
    //  fLogic_ASOLENOID_hd_ir[j]->SetVisAttributes(vb1);

    //---------------- set magnetic field ---------------
    sprintf(abname, "Solid_ASOLENOID_hd_m_%s", ffqsNAME);
    fSolid_ASOLENOID_hd_m[j] = new G4Tubs(abname, 0. * cm, ffqsRoutDi * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
    sprintf(abname, "Logic_ASOLENOID_hd_m_%s", ffqsNAME);
    fLogic_ASOLENOID_hd_m[j] = new G4LogicalVolume(fSolid_ASOLENOID_hd_m[j], ffqsMaterial_G, abname);
    sprintf(abname, "Physics_ASOLENOID_hd_m_%s", ffqsNAME);
    fPhysics_ASOLENOID_hd_m[j] = new G4PVPlacement(0, G4ThreeVector(), abname, fLogic_ASOLENOID_hd_m[j],
                                                   fPhysics_ASOLENOID_hd_v[j], false, 0);


    vb1as = new G4VisAttributes(G4Color(1., 0.5, 0.7, 1.));
    vb1as->SetForceSolid(true);
    fLogic_ASOLENOID_hd_ir[j]->SetVisAttributes(vb1as);


    //    G4FieldManager* fieldMgr = SetQMagField(qFIELDx[j],qFIELDy[j]);   // gradient tesla/m;
    // fLogic_ASOLENOIDm[j]->SetFieldManager(fieldMgr,true);

    // G4double fieldStrength = 3.0*tesla;  // 0.01*tesla; // field strength in pipe


    //JF just keep it for now! Need to move it back to nominal!
    qFIELSol=0;

    G4double fieldStrength = qFIELSol * tesla;  // 0.01*tesla; // field strength in pipe
    G4double alphaB = 0. * degree;
    fMagField_ASOLENOID[j] = new G4UniformMagField(G4ThreeVector(fieldStrength * std::sin(alphaB),
                                                                 0., fieldStrength * std::cos(alphaB)));

    fieldMgrAS[j] = new G4FieldManager(fMagField_ASOLENOID[j]);
    fieldMgrAS[j]->SetDetectorField(fMagField_ASOLENOID[j]);
    fieldMgrAS[j]->CreateChordFinder(fMagField_ASOLENOID[j]);
    fLogic_ASOLENOID_hd_m[j]->SetFieldManager(fieldMgrAS[j], true);


}







