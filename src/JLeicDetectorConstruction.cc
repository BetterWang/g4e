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

#include "vector"
#include "JLeicDetectorConstruction.hh"
//#include "JLeicDetectorMessenger.hh"
#include "JLeicCalorimeterSD.hh"
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

#define USE_TGEOM 1
//--------BEAM elements------
#define USE_FFQs
//#define USE_FFQs_iu
//#define USE_FFQs_ed
//#define USE_FFQs_eu


//--------BARREL------
#define USE_BARREL
#define USE_BARREL_det
//#define USE_BEAMPIPE 1 // beampipe 
//------- subdetector-volumes  barrel ----- 

#define USE_CB_VTX
//#define  USE_VTX0 1   // for simple vtx geom
#define USE_CB_VTX_LADDERS
//#define  USE_VTX_ENDCAP    // for vxt endcaps ladders
//#define  USE_VTX_DISKS    // for vxt disks along beampipe

//#define USE_VTX_E 1   // for vxt endcaps 
#define USE_CB_CTD
#define USE_CB_CTD_Si  1 // silicon version of CTD
//#define USE_cb_CTD_Straw 1 // straw version of CTD

#define USE_CB_EMCAL
#define USE_CB_HCAL
#define USE_CB_HCAL_D // hcal detector
#define USE_GEM   // volumes
#define USE_GEMb  // detectors



//--------H-encap------
#define USE_CI_ENDCAP
//------- subdetector-volumes H-encap ----- 
#define USE_CI_DRICH
#define USE_CI_EMCAL
#define USE_CI_HCAL
#define USE_CI_HCAL_D
//--------E-encap------
#define USE_E_ENDCAP
//#define USE_E_ENDCAP_HCAL
//#define USE_E_ENDCAP_HCAL_D
//------- subdetector-volumes E-encap ----- 
#define  USE_CE_EMCAL
#define  USE_CE_MRICH


//--------FARFORWARD HADRON------
#define USE_DIPOLE1_SI
#define USE_FI_DIPOLE1_A
#define USE_FI_DIPOLE1_B
#define USE_FI_DIPOLE2

#define USE_FARFORWARD_GEM

#define USE_FARFORWARD_VP

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
// end VGM demo
#endif

/////////////////////////////////////////////////////////////////////////////
//
//

JLeicDetectorConstruction::JLeicDetectorConstruction()
        : fWorldChanged(false), fAbsorberMaterial(0), fGapMat(0), fSetUp("simpleALICE"),
          World_Material(0), World_Solid(0), World_Logic(0), World_Phys(0),
          fSolidRadSlice(0), fLogicRadSlice(0), fPhysicRadSlice(0),
          fSolidRadiator(0), fLogicRadiator(0), fPhysicsRadiator(0),
          fRadiatorMat(0), fPipe(false), fPipeField(false),
// fSolidAbsorber(0),    fLogicAbsorber(0),   fPhysicsAbsorber(0),
          cb_CTD_GVol_Phys(0),
          fMagField(0), fCalorimeterSD(0), fRegGasDet(0), fRadRegion(0), fMat(0) {
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


//==========================================================================================================
//                              JLEIC 2018
//==========================================================================================================

/////////////////////////////////////////////////////////////////////////////////
//


G4VPhysicalVolume *JLeicDetectorConstruction::SetUpJLEIC2019() {

    char abname[128];
    int i,j;

    // Preparation of mixed radiator material

    G4Material *Mylar = fMat->GetMaterial("Mylar");
    G4Material *Air = fMat->GetMaterial("Air");
    G4Material *Al = fMat->GetMaterial("Al");
    G4Material *CH2 = fMat->GetMaterial("CH2");
    G4Material *He = fMat->GetMaterial("He");


    fAbsorberMaterial = fMat->GetMaterial("Si");

    G4cout << "G4Material* CH2   = " << CH2 << G4endl;
    G4cout << "G4Material* Mylar = " << Mylar << G4endl;
    G4double foilDensity = 0.91 * g / cm3;  // CH2 1.39*g/cm3; // Mylar //  0.534*g/cm3; //Li
    G4double gasDensity = 1.2928 * mg / cm3; // Air // 1.977*mg/cm3; // CO2 0.178*mg/cm3; // He
    G4double totDensity = foilDensity * foilGasRatio + gasDensity * (1.0 - foilGasRatio);
    G4cout << "Rad totDensity = " << totDensity / (g / cm3) << " g/cm3 " << G4endl;
    G4cout << "use for Rad totDensity = " << totDensity / (g / cm3) << " g/cm3 " << G4endl;


    G4double fractionFoil = foilDensity * foilGasRatio / totDensity;
    G4double fractionGas = gasDensity * (1.0 - foilGasRatio) / totDensity;
    G4Material *radiatorMat0 = new G4Material("radiatorMat0", totDensity, 2);
    radiatorMat0->AddMaterial(CH2, fractionFoil);
    radiatorMat0->AddMaterial(Air, fractionGas);
    G4double NewDensity = 0.083 * (g / cm3);
    G4Material *radiatorMat = new G4Material("radiatorMat", NewDensity, 1);
    radiatorMat->AddMaterial(radiatorMat0, 1.);
    G4cout << "new  Rad with totDensity = " << NewDensity / (g / cm3) << " g/cm3 " << G4endl;

    G4double XTR_density = radiatorMat->GetDensity();
    G4cout << "Read back Rad totDensity = " << XTR_density / (g / cm3) << " g/cm3 " << G4endl;
    // default materials of the detector and TR radiator
    fRadiatorMat = radiatorMat;
    fFoilMat = CH2; // Kapton; // Mylar ; // Li ; // CH2 ;
    fGasMat = Air; // CO2; // He; //

    G4VisAttributes *vtpc1;

    //===================================================================================
    //==                    create a world                                            ==
    //===================================================================================

    World_SizeZ = 40000. * cm;
    World_SizeR = 10000. * cm;

    // World_Material    = Air;
    World_Material = fMat->GetMaterial("G4_Galactic");
    World_Solid = new G4Box("World_Solid", World_SizeR, World_SizeR, World_SizeZ / 2.);

    World_Logic = new G4LogicalVolume(World_Solid, World_Material, "World_Logic");

    World_Phys = new G4PVPlacement(0, G4ThreeVector(), "World_Phys",
                                      World_Logic, 0, false, 0);


    //=========================================================================
    //                    Sensitive
    //=========================================================================
    if (fRegGasDet != 0) delete fRegGasDet;
    if (fRegGasDet == 0) fRegGasDet = new G4Region("XTRdEdxDetector");

    G4SDManager *SDman = G4SDManager::GetSDMpointer();

    if (!fCalorimeterSD) {

        fCalorimeterSD = new JLeicCalorimeterSD("CalorSD", this);
        SDman->AddNewDetector(fCalorimeterSD);
    }

    printf("CalorSD done\n");

    //=========================================================================
    //                   TR radiator envelope
    //=========================================================================
    printf("Radiator\n");
    fRadThickness = 0.020 * mm;    // 16 um // ZEUS NIMA 323 (1992) 135-139, D=20um, dens.= 0.1 g/cm3
    fGasGap = 0.600 * mm;    // for ZEUS  300-publication
    fRadThick = .3 * cm - fGasGap + fDetGap;
    fFoilNumber = fRadThick / (fRadThickness + fGasGap);
    fRadZ = -1000 * cm;
    foilGasRatio = fRadThickness / (fRadThickness + fGasGap);
    fAbsorberThickness = 0.050 * mm;
    fAbsorberRadius = 100. * mm;
    fAbsorberZ = 136. * cm;
    fDetGap = 0.01 * mm;
    fModuleNumber = 1;

    fSolidRadiator = new G4Box("Radiator", 1 * mm, 1 * mm, 0.5 * fRadThick);
    fLogicRadiator = new G4LogicalVolume(fSolidRadiator, fRadiatorMat,
                                         "Radiator");
    fPhysicsRadiator = new G4PVPlacement(0,
                                         G4ThreeVector(0, -60 * cm, fRadZ),
                                         "Radiator", fLogicRadiator,
                                         World_Phys, false, 0);

    if (fRadRegion != 0) delete fRadRegion;
    if (fRadRegion == 0) fRadRegion = new G4Region("XTRradiator");
    fRadRegion->AddRootLogicalVolume(fLogicRadiator);

    printf("Radiator done \n");

#ifdef  USE_BARREL
    //===================================================================================
    //==                          Solenoid  magnetic field                             ==
    //===================================================================================
    Solenoid_RIn = 0.0 * cm;
    Solenoid_ROut = 144. * cm;
    Solenoid_SizeZ = 355.8 * cm;
    World_ShiftVTX = 40. * cm;

    printf("Solenoid_SizeZ=%f", Solenoid_SizeZ);

    Solenoid_Solid = new G4Tubs("Solenoid_Solid", Solenoid_RIn, Solenoid_ROut, Solenoid_SizeZ / 2., 0., 360 * deg);

    Solenoid_Logic = new G4LogicalVolume(Solenoid_Solid, World_Material, "Solenoid_Logic");

    Solenoid_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, World_ShiftVTX), "Solenoid_Phys", Solenoid_Logic,
                                         World_Phys, false, 0);

    // G4VisAttributes* vsol= new G4VisAttributes(G4Color(0.1,0,1.,0.1));
    //   vsol->SetLineWidth(1); vsol->SetForceSolid(true);
    //   Solenoid_Logic->SetVisAttributes(attr_cb_VTX);

    // G4double Solenoid_Field_Strength = 3.0*tesla;  // 0.01*tesla; // field strength in pipe
    Solenoid_Field_Strength = -2.0 * tesla;  // 0.01*tesla; // field strength in pipe
    Solenoid_AlphaB = 0. * degree;
    //  fPipeField     =  true;   // field in helium pipe used?
    fPipeField = true;   // field in helium pipe used?
    if (fPipeField) {
        G4cout << "Set Magnetic field = " << Solenoid_Field_Strength << G4endl << G4endl;
        if (fMagField) delete fMagField; //delete the existing mag field

        fMagField = new G4UniformMagField(G4ThreeVector(Solenoid_Field_Strength * std::sin(Solenoid_AlphaB),
                                                        0., Solenoid_Field_Strength * std::cos(Solenoid_AlphaB)));

        G4FieldManager *fieldMgr = new G4FieldManager(fMagField);
        fieldMgr->SetDetectorField(fMagField);
        fieldMgr->CreateChordFinder(fMagField);
        Solenoid_Logic->SetFieldManager(fieldMgr, true);
    } else
        G4cout << "No Magnetic field " << G4endl << G4endl;
    attr_Solenoid = new G4VisAttributes(G4Color(0.1, 0, 0.1, 0.4));
    attr_Solenoid->SetLineWidth(1);
    attr_Solenoid->SetForceSolid(false);
    Solenoid_Logic->SetVisAttributes(attr_Solenoid);

#endif
    //===================================================================================
    //==                           HCAL  DETECTOR VOLUME  BARREL                       ==
    //===================================================================================
#ifdef USE_CB_HCAL
    cb_HCAL_GVol_RIn = Solenoid_ROut;
    cb_HCAL_GVol_ROut = Solenoid_ROut + 100. * cm;
    cb_HCAL_GVol_SizeZ = Solenoid_SizeZ + ce_ENDCAP_GVol_SizeZ;
    //cb_HCAL_det_Material = fMat->GetMaterial("StainlessSteel");
    cb_HCAL_GVol_Solid = new G4Tubs("cb_HCAL_GVol_Solid", cb_HCAL_GVol_RIn, cb_HCAL_GVol_ROut, cb_HCAL_GVol_SizeZ / 2., 0., 360 * deg);

    cb_HCAL_GVol_Logic = new G4LogicalVolume(cb_HCAL_GVol_Solid, World_Material, "cb_HCAL_GVol_Logic");

    cb_HCAL_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, World_ShiftVTX - ce_ENDCAP_GVol_SizeZ / 2), "cb_HCAL_GVol_Phys",
                                          cb_HCAL_GVol_Logic,
                                          World_Phys, false, 0);

    attr_cb_HCAL_GVol = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    attr_cb_HCAL_GVol->SetLineWidth(1);
    attr_cb_HCAL_GVol->SetForceSolid(false);
    cb_HCAL_GVol_Logic->SetVisAttributes(attr_cb_HCAL_GVol);
     //  cb_HCAL_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);
#endif // end HCALb
    //===================================================================================


#ifdef USE_CI_ENDCAP
    //===================================================================================
    //==                           HADRON-ENDCAP                                       ==
    //===================================================================================
    ci_ENDCAP_GVol_RIn = 20. * cm;
    ci_ENDCAP_GVol_ROut = cb_HCAL_GVol_ROut;
    ci_ENDCAP_GVol_SizeZ = 250 * cm;
    ci_ENDCAP_GVol_ShiftZ = 0. * cm;
    ci_ENDCAP_GVol_PosX = 0. * cm;
    ci_ENDCAP_GVol_PosZ = Solenoid_SizeZ / 2. + World_ShiftVTX + ci_ENDCAP_GVol_ShiftZ + ci_ENDCAP_GVol_SizeZ / 2.;

    ci_ENDCAP_GVol_Solid = new G4Tubs("ci_ENDCAP_GVol_Solid", ci_ENDCAP_GVol_RIn, ci_ENDCAP_GVol_ROut, ci_ENDCAP_GVol_SizeZ / 2., 0., 360 * deg);

    ci_ENDCAP_GVol_Logic = new G4LogicalVolume(ci_ENDCAP_GVol_Solid, World_Material, "ci_ENDCAP_GVol_Logic");

    ci_ENDCAP_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(ci_ENDCAP_GVol_PosX, 0, ci_ENDCAP_GVol_PosZ), "ci_ENDCAP_GVol_Phys",
                                            ci_ENDCAP_GVol_Logic,
                                            World_Phys, false, 0);

    attr_ci_ENDCAP_GVol = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    attr_ci_ENDCAP_GVol->SetLineWidth(1);
    attr_ci_ENDCAP_GVol->SetForceSolid(false);
    ci_ENDCAP_GVol_Logic->SetVisAttributes(attr_ci_ENDCAP_GVol);

#endif


#ifdef USE_E_ENDCAP

    //===================================================================================
    //==                           ELECTRON-ENDCAP                                     ==
    //===================================================================================

    ce_ENDCAP_GVol_RIn = 20 * cm;
    ce_ENDCAP_GVol_ROut = Solenoid_ROut;
    ce_ENDCAP_GVol_SizeZ = 60 * cm;
    ce_ENDCAP_GVol_PosZ = -ce_ENDCAP_GVol_SizeZ / 2 - Solenoid_SizeZ / 2 + World_ShiftVTX;

    ce_ENDCAP_GVol_Solid = new G4Tubs("ce_ENDCAP_GVol_Solid", ce_ENDCAP_GVol_RIn, ce_ENDCAP_GVol_ROut, ce_ENDCAP_GVol_SizeZ / 2., 0.,
                                 360 * deg);

    ce_ENDCAP_GVol_Logic = new G4LogicalVolume(ce_ENDCAP_GVol_Solid, World_Material, "ce_ENDCAP_GVol_Logic");

    ce_ENDCAP_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, ce_ENDCAP_GVol_PosZ), "ce_ENDCAP_GVol_Phys", ce_ENDCAP_GVol_Logic,
                                          World_Phys, false, 0);

    attr_ce_ENDCAP_GVol = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    attr_ce_ENDCAP_GVol->SetLineWidth(1);
    attr_ce_ENDCAP_GVol->SetForceSolid(false);
    ce_ENDCAP_GVol_Logic->SetVisAttributes(attr_ce_ENDCAP_GVol);

#endif


//===================================================================================


    //===================================================================================
//-------------------------------------HCAL Iron detector barrel-----------------------
    //===================================================================================
#ifdef USE_CB_HCAL
#ifdef USE_CB_HCAL_D
    cb_HCAL_det_SizeZ = cb_HCAL_GVol_SizeZ;
    cb_HCAL_det_Thickness = 2 * cm;

    cb_HCAL_det_Material = fMat->GetMaterial("Iron");

    int hlay = 0;
    int NLAY_HCALb = 25;
    for (hlay = 0; hlay < NLAY_HCALb; hlay++) {

        cb_HCAL_det_RIn = cb_HCAL_GVol_RIn + hlay * 2 * cb_HCAL_det_Thickness;
        cb_HCAL_det_ROut = cb_HCAL_det_RIn + cb_HCAL_det_Thickness;
        if (cb_HCAL_det_RIn > cb_HCAL_GVol_ROut || cb_HCAL_det_ROut > cb_HCAL_GVol_ROut) continue;

        sprintf(abname, "cb_HCAL_det_Solid_%d", hlay);
        cb_HCAL_det_Solid = new G4Tubs(abname, cb_HCAL_det_RIn, cb_HCAL_det_ROut, cb_HCAL_det_SizeZ / 2., 0., 360 * deg);
        sprintf(abname, "cb_HCAL_det_Logic_%d", hlay);
        cb_HCAL_det_Logic = new G4LogicalVolume(cb_HCAL_det_Solid, World_Material, abname);
        sprintf(abname, "cb_HCAL_det_Phys_%d", hlay);
        cb_HCAL_det_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), abname, cb_HCAL_det_Logic,
                                             cb_HCAL_GVol_Phys, false, hlay);
        attr_cb_HCAL_det = new G4VisAttributes(G4Color(0.6, 0, 0.6, 1));
        attr_cb_HCAL_det->SetLineWidth(1);
        attr_cb_HCAL_det->SetForceSolid(true);
        cb_HCAL_det_Logic->SetVisAttributes(attr_cb_HCAL_det);
    }


#endif // end HCALbdet
#endif // end HCALb

    //===================================================================================
    //==                          HADRON-ENDCAP    HCAL   DETECTOR VOLUME              ==
    //===================================================================================
#ifdef USE_CI_HCAL
                                                                                                                            // G4double ci_HCAL_GVol_RIn[2]={60*cm, 70*cm } ;
     //  G4double ci_HCAL_GVol_RIn[2]={0*cm, 0*cm }
     // G4double ci_HCAL_GVol_ROut[2]={cb_HCAL_GVol_ROut,cb_HCAL_GVol_ROut };
 ci_HCAL_GVol_RIn=0*cm ;
 ci_HCAL_GVol_ROut=cb_HCAL_GVol_ROut;
 ci_HCAL_GVol_SizeZ=195*cm;
 ci_HCAL_GVol_ShiftZ=5*cm;
 // G4double  fHCAL_HCAP_Zcone[2]= {ci_ENDCAP_GVol_PosZ+ ci_ENDCAP_GVol_SizeZ/2+ci_HCAL_GVol_ShiftZ,ci_ENDCAP_GVol_PosZ+ ci_ENDCAP_GVol_SizeZ/2+ci_HCAL_GVol_SizeZ + ci_HCAL_GVol_ShiftZ} ;



 // printf("HCAP_HCAL_det_sol::2  in= %lf %lf  out= %lf %lf z=%lf\n",ci_HCAL_GVol_RIn[0],ci_HCAL_GVol_RIn[1],ci_HCAL_GVol_ROut[0], ci_HCAL_GVol_ROut[1], ci_HCAL_GVol_SizeZ);

 //cb_HCAL_det_Material = fMat->GetMaterial("StainlessSteel");
 //   ci_HCAL_GVol_Solid = new G4Polycone("HCAP_HCAL_vol_sol",0.*deg,360.*deg,2,fHCAL_HCAP_Zcone, ci_HCAL_GVol_RIn, ci_HCAL_GVol_ROut);
 ci_HCAL_GVol_Solid  = new G4Tubs("ci_HCAL_GVol_Solid", ci_HCAL_GVol_RIn, ci_HCAL_GVol_ROut, ci_HCAL_GVol_SizeZ/2.,0.,360*deg);
 ci_HCAL_GVol_Logic = new G4LogicalVolume(ci_HCAL_GVol_Solid,  World_Material,  "ci_HCAL_GVol_Logic");
 //  ci_HCAL_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0,0,0), "HCAP_HCAL_vol_phys",ci_HCAL_GVol_Logic,
 //					  World_Phys, false,	0 );

 ci_HCAL_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0,0,ci_ENDCAP_GVol_PosZ+ ci_ENDCAP_GVol_SizeZ/2+ci_HCAL_GVol_ShiftZ+ci_HCAL_GVol_SizeZ/2.), "HCAP_HCAL_vol_phys",ci_HCAL_GVol_Logic,
					  World_Phys, false,	0 );

 attr_ci_HCAL_GVol= new G4VisAttributes(G4Color(0.3,0,3.,0.1));
 attr_ci_HCAL_GVol->SetLineWidth(1); attr_ci_HCAL_GVol->SetForceSolid(false);
 ci_HCAL_GVol_Logic->SetVisAttributes(attr_ci_HCAL_GVol);
 //  cb_HCAL_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);

 //---------------------------- HCAL IRON--------------------------------------

 #ifdef USE_CI_HCAL_D

 ci_HCAL_det_RIn= 80*cm ;
 ci_HCAL_det_ROut=ci_HCAL_GVol_ROut -1*cm;
 ci_HCAL_det_ThicknessZ=2*cm;
 ci_HCAL_det_GapZ=2*cm;
 ci_HCAL_Nlay=25;
 ci_HCAL_det_Material= fMat->GetMaterial("Iron");
  printf("ci_HCAL_det::1 %f %f ,%f\n",ci_HCAL_det_RIn, ci_HCAL_det_ROut, ci_HCAL_det_ThicknessZ);

 sprintf(abname,"ci_HCAL_det_Solid");
 ci_HCAL_det_Solid  = new G4Tubs(abname, ci_HCAL_det_RIn, ci_HCAL_det_ROut, ci_HCAL_det_ThicknessZ/2.,0.,360*deg);

 sprintf(abname,"ci_HCAL_det_Logic");
 ci_HCAL_det_Logic = new G4LogicalVolume(ci_HCAL_det_Solid,   World_Material,abname);

 attr_ci_HCAL_det= new G4VisAttributes(G4Color(0.6,0,0.6,1));
 attr_ci_HCAL_det->SetLineWidth(1); attr_ci_HCAL_det->SetForceSolid(true);
 ci_HCAL_det_Logic->SetVisAttributes(attr_ci_HCAL_det);

  int hhlay=0;

  for (hhlay=0;hhlay<ci_HCAL_Nlay;hhlay++){
    ci_HCAL_det_PosZ= -ci_HCAL_GVol_SizeZ/2   +(hhlay+1)*ci_HCAL_det_ThicknessZ +(hhlay+1)*5*cm;
    printf("ci_HCAL_det::2 %f %f ,%f\n", ci_HCAL_det_PosZ,ci_HCAL_det_PosZ- ci_HCAL_det_ThicknessZ,ci_HCAL_GVol_SizeZ);

   if( (ci_HCAL_det_PosZ- ci_HCAL_det_ThicknessZ) > ci_HCAL_GVol_SizeZ/2) continue;
    //   ci_HCAL_det_PosZ=-ci_HCAL_GVol_SizeZ/2+hhlay*ci_HCAL_det_ThicknessZ+ci_HCAL_det_GapZ*hhlay;
    // ci_HCAL_det_PosZ= hhlay*ci_HCAL_det_ThicknessZ+ci_HCAL_det_GapZ*hhlay;
     sprintf(abname,"ci_HCAL_det_Phys_%d",hhlay);
    ci_HCAL_det_Phys = new G4PVPlacement(0, G4ThreeVector(0,0, ci_HCAL_det_PosZ), abname,ci_HCAL_det_Logic,
				           ci_HCAL_GVol_Phys,  false, hhlay);
   }


#endif // end HCAL
#endif // end HCAL

    //===================================================================================

    //===================================================================================
    //===================================================================================


    //***********************************************************************************
    //***********************************************************************************
    //**                                DETECTOR VOLUMES                               **
    //***********************************************************************************
    //***********************************************************************************


#ifdef  USE_BARREL_det
    //***********************************************************************************
    //***********************************************************************************
    //**                               BARREL VOLUMES                                  **
    //***********************************************************************************
    //***********************************************************************************

#ifdef  USE_CB_VTX
    //===================================================================================
    //==                          VERTEX DETECTOR VOLUME                               ==
    //===================================================================================
    printf("Begin VERTEX volume \n");
    cb_VTX_GVol_RIn = 3.3 * cm;
    cb_VTX_GVol_ROut = 20 * cm;
    cb_VTX_GVol_SizeZ = 50 * cm;

    cb_VTX_GVol_Solid = new G4Tubs("cb_VTX_GVol_Solid", cb_VTX_GVol_RIn, cb_VTX_GVol_ROut, cb_VTX_GVol_SizeZ / 2., 0., 360 * deg);
    cb_VTX_GVol_Logic = new G4LogicalVolume(cb_VTX_GVol_Solid, World_Material, "cb_VTX_GVol_Logic");
    cb_VTX_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, -World_ShiftVTX), "VTX1", cb_VTX_GVol_Logic,
                                    Solenoid_Phys, false, 0);

    // cb_VTX_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);
    G4VisAttributes *attr_cb_VTX = new G4VisAttributes(G4Color(0.1, 0, 1., 0.1));
    attr_cb_VTX->SetLineWidth(1);
    attr_cb_VTX->SetForceSolid(false);
    cb_VTX_GVol_Logic->SetVisAttributes(attr_cb_VTX);

//===================================================================================
#endif  // end VERTEX

#ifdef  USE_CB_CTD

    //===================================================================================
    //==                          CTD DETECTOR VOLUME                                  ==
    //===================================================================================
    cb_CTD_GVol_RIn = 21 * cm;
    // for new magnet  
    //   cb_CTD_GVol_ROut = 95 * cm;
    // for CLEO and BABAR DIRC
    cb_CTD_GVol_ROut = 82 * cm;
    cb_CTD_GVol_SizeZ = Solenoid_SizeZ - 60 * cm;

    cb_CTD_GVol_Solid = new G4Tubs("cb_CTD_GVol_Solid", cb_CTD_GVol_RIn, cb_CTD_GVol_ROut, cb_CTD_GVol_SizeZ / 2., 0., 360 * deg);
    cb_CTD_GVol_Logic = new G4LogicalVolume(cb_CTD_GVol_Solid, World_Material, "cb_CTD_GVol_Logic");
    cb_CTD_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(), "CTD", cb_CTD_GVol_Logic,
                                    Solenoid_Phys, false, 0);

    // cb_CTD_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);
    G4VisAttributes *attr_cb_CTD = new G4VisAttributes(G4Color(0.1, 0, 1., 0.1));
    attr_cb_CTD->SetLineWidth(1);
    attr_cb_CTD->SetForceSolid(false);
    cb_CTD_GVol_Logic->SetVisAttributes(attr_cb_CTD);

//===================================================================================
#endif  // end CTD


#ifdef  USE_CB_EMCAL
    //===================================================================================
    //==                          EMCAL DETECTOR VOLUME                               ==
    //===================================================================================
    cb_EMCAL_GVol_RIn = 115. * cm;
    cb_EMCAL_GVol_ROut = 140. * cm;
    // cb_EMCAL_GVol_SizeZ=Solenoid_SizeZ -30*cm;

    // working

    //    G4double EMCALB_R_cone[4]={cb_EMCAL_GVol_RIn,  cb_EMCAL_GVol_ROut, cb_EMCAL_GVol_ROut, cb_EMCAL_GVol_RIn };
    //  G4double cb_EMCAL_GVol_ConeZ[4]={ -Solenoid_SizeZ/2, -Solenoid_SizeZ/2+60*cm, Solenoid_SizeZ/2-60*cm, Solenoid_SizeZ/2};

    G4double cb_EMCAL_GVol_ConeROut[4] = {cb_EMCAL_GVol_ROut, cb_EMCAL_GVol_ROut, cb_EMCAL_GVol_ROut, cb_EMCAL_GVol_ROut};
    G4double cb_EMCAL_GVol_ConeRIn[4] = {cb_EMCAL_GVol_ROut - 1. * cm, cb_EMCAL_GVol_RIn, cb_EMCAL_GVol_RIn, cb_EMCAL_GVol_ROut - 1. * cm};
    G4double cb_EMCAL_GVol_ConeZ[4] = {-Solenoid_SizeZ / 2, -Solenoid_SizeZ / 2 + 30 * cm, Solenoid_SizeZ / 2 - 30 * cm,
                                 Solenoid_SizeZ / 2};

    cb_EMCAL_GVol_Solid = new G4Polycone("cb_EMCAL_GVol_Solid", 0. * deg, 360. * deg, 4, cb_EMCAL_GVol_ConeZ, cb_EMCAL_GVol_ConeRIn,
                                  cb_EMCAL_GVol_ConeROut);
    cb_EMCAL_GVol_Logic = new G4LogicalVolume(cb_EMCAL_GVol_Solid, World_Material, "cb_EMCAL_GVol_Logic");
    cb_EMCAL_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(), "cb_EMCAL_GVol_Phys", cb_EMCAL_GVol_Logic,
                                       Solenoid_Phys, false, 0);


    G4VisAttributes *attr_cb_EMCAL = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 1.));
    attr_cb_EMCAL->SetLineWidth(1);
    attr_cb_EMCAL->SetForceSolid(false);
    cb_EMCAL_GVol_Logic->SetVisAttributes(attr_cb_EMCAL);
    // cb_EMCAL_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);
//........................EMCAL Barrel detector----------------------------------------------

    cb_EMCAL_det_Material = fMat->GetMaterial("PbWO4");
    // fSolidEMCAL = new G4Tubs("EMCALbSol",  cb_EMCAL_GVol_RIn,cb_EMCAL_GVol_ROut,cb_EMCAL_GVol_SizeZ/2.,0.,360*deg);
    cb_EMCAL_det_Solid = new G4Polycone("cb_EMCAL_det_Solid", 0. * deg, 360. * deg, 4, cb_EMCAL_GVol_ConeZ, cb_EMCAL_GVol_ConeRIn,
                                   cb_EMCAL_GVol_ConeROut);

    cb_EMCAL_det_Logic = new G4LogicalVolume(cb_EMCAL_det_Solid, cb_EMCAL_det_Material, "cb_EMCAL_det_Logic");

    // G4VisAttributes* attr_cb_EMCAL= new G4VisAttributes(G4Color(0.3,0.5,0.9,0.9));
    //  G4VisAttributes* attr_cb_EMCAL= new G4VisAttributes(G4Color(0.7,0.7,0.7,1.));
    attr_cb_EMCAL->SetLineWidth(1);
    attr_cb_EMCAL->SetForceSolid(true);
    cb_EMCAL_det_Logic->SetVisAttributes(attr_cb_EMCAL);

    //    fPhysicsEMCAL = new G4PVPlacement(0, G4ThreeVector(0,0,-40*cm), "EMCALbSolPhys",fLogicEMCAL,
    //                               cb_EMCAL_GVol_Phys, false,     0 );

    cb_EMCAL_det_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "cb_EMCAL_det_Phys", cb_EMCAL_det_Logic,
                                        cb_EMCAL_GVol_Phys, false, 0);

//...............................detector----------------------------------------------


#endif  // end EMCALb


#ifdef USE_GEMb
    //====================================================================================
    //==                          GEM DETECTOR VOLUME  CAP-barrel- Hadron side          ==
    //====================================================================================
    ci_GEM_GVol_RIn = 0 * cm;
    ci_GEM_GVol_ROut = 65 * cm + 50 * cm;
    ci_GEM_GVol_SizeZ = 30 * cm;
    ci_GEM_GVol_ShiftZ = 0 * cm;

    ci_GEM_GVol_Solid = new G4Tubs("ci_GEM_GVol_Solid", ci_GEM_GVol_RIn, ci_GEM_GVol_ROut, ci_GEM_GVol_SizeZ / 2., 0., 360 * deg);

    ci_GEM_GVol_Logic = new G4LogicalVolume(ci_GEM_GVol_Solid, World_Material, "ci_GEM_GVol_Logic");

    // ci_GEM_GVol_PosZ= Solenoid_SizeZ/2-abs(World_ShiftVTX)+ci_GEM_GVol_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed
    ci_GEM_GVol_PosZ = Solenoid_SizeZ / 2 -
               ci_GEM_GVol_SizeZ / 2;   // --- need to find out why this 5 cm are needed
    ci_GEM_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, ci_GEM_GVol_PosZ), "ci_GEM_GVol_Phys", ci_GEM_GVol_Logic,
                                      Solenoid_Phys, false, 0);

    //===================================================================================
    //==                          GEM DETECTOR VOLUME     CAP-barrel- Electron side    ==
    //===================================================================================
    ce_GEM_GVol_RIn = 0 * cm;
    ce_GEM_GVol_ROut = 65 * cm + 50 * cm;
    ce_GEM_GVol_SizeZ = 30 * cm;
    ce_GEM_GVol_ShiftZ = 0 * cm;
    ce_GEM_GVol_PosZ = -Solenoid_SizeZ / 2 + ce_GEM_GVol_SizeZ / 2;
    // ce_GEM_GVol_PosZ= -Solenoid_SizeZ/2+abs(World_ShiftVTX)- ce_GEM_GVol_SizeZ +5*cm;  // --- need to find out why this 5 cm are needed
    ce_GEM_GVol_Solid = new G4Tubs("ce_GEM_GVol_Solid", ce_GEM_GVol_RIn, ce_GEM_GVol_ROut, ce_GEM_GVol_SizeZ / 2., 0., 360 * deg);

    ce_GEM_GVol_Logic = new G4LogicalVolume(ce_GEM_GVol_Solid, World_Material, "ce_GEM_GVol_Logic");

    ce_GEM_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, ce_GEM_GVol_PosZ), "ce_GEM_GVol_Phys", ce_GEM_GVol_Logic,
                                      Solenoid_Phys, false, 0);

    //===================================================================================
#endif // end USE_GEMb


#endif  // end BARREL



    //***********************************************************************************
    //***********************************************************************************
    //**                               HADRON ENDCAP DETECTOR  VOLUMES                 **
    //***********************************************************************************
    //***********************************************************************************
#ifdef USE_CI_ENDCAP

#ifdef USE_CI_DRICH
//===================================================================================
// ==                       dRICH     Hadron endcap                                ==
//==================================================================================
    ci_DRICH_GVol_RIn = ci_ENDCAP_GVol_RIn;
    ci_DRICH_GVol_ROut = 150 * cm;
    ci_DRICH_GVol_ThicknessZ = 170 * cm;

    ci_DRICH_GVol_PosZ = -ci_ENDCAP_GVol_SizeZ / 2. + ci_DRICH_GVol_ThicknessZ / 2.;
    //    double ci_DRICH_GVol_PosZ= 0*cm;

    ci_DRICH_GVol_Solid = new G4Tubs("ci_DRICH_GVol_Solid", ci_DRICH_GVol_RIn, ci_DRICH_GVol_ROut, ci_DRICH_GVol_ThicknessZ / 2., 0., 360 * deg);

    ci_DRICH_GVol_Logic = new G4LogicalVolume(ci_DRICH_GVol_Solid, World_Material, "ci_DRICH_GVol_Logic");

    ci_DRICH_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, ci_DRICH_GVol_PosZ), "ci_DRICH_GVol_Phys", ci_DRICH_GVol_Logic,
                                        ci_ENDCAP_GVol_Phys, false, 0);

    //  fLogic_H->SetVisAttributes(G4VisAttributes::Invisible);

    attr_ci_DRICH_GVol = new G4VisAttributes(G4Color(1., 1., 0.2, 0.2));
    attr_ci_DRICH_GVol->SetLineWidth(1);
    attr_ci_DRICH_GVol->SetForceSolid(true);
    ci_DRICH_GVol_Logic->SetVisAttributes(attr_ci_DRICH_GVol);

//===================================================================================
#endif // end USE_CI_DRICH

//===================================================================================
//==== space for TRD =============================
//===================================================================================
   ci_TRD_ThicknessZ = 40 * cm;
 //   G4int i, j = 0;

//===================================================================================
//                         EMCAL Hadron endcap
//===================================================================================

#ifdef USE_CI_EMCAL

    ci_EMCAL_GVol_RIn = 20 * cm;
    ci_EMCAL_GVol_ROut = 200 * cm;
    ci_EMCAL_GVol_ThicknessZ = 40 * cm;
    ci_EMCAL_GVol_PosZ = -ci_ENDCAP_GVol_SizeZ / 2 + ci_DRICH_GVol_ThicknessZ + ci_TRD_ThicknessZ + ci_EMCAL_GVol_ThicknessZ / 2;
    ci_EMCAL_GVol_Solid = new G4Tubs("ci_EMCAL_GVol_Solid", ci_EMCAL_GVol_RIn, ci_EMCAL_GVol_ROut, ci_EMCAL_GVol_ThicknessZ / 2., 0.,
                               360 * deg);
    ci_EMCAL_GVol_Logic = new G4LogicalVolume(ci_EMCAL_GVol_Solid, World_Material, "ci_EMCAL_GVol_Logic");
    attr_ci_EMCAL_GVol = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
    attr_ci_EMCAL_GVol->SetLineWidth(1);
    attr_ci_EMCAL_GVol->SetForceSolid(false);
    ci_EMCAL_GVol_Logic->SetVisAttributes(attr_ci_EMCAL_GVol);

    ci_EMCAL_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, ci_EMCAL_GVol_PosZ), "H_CAP_EMCAL_Physics", ci_EMCAL_GVol_Logic,
                                        ci_ENDCAP_GVol_Phys, false, 0);


    //--------------------------------------------------------------------
    //                           Ecal modules
    //--------------------------------------------------------------------

    ci_EMCAL_det_Length = 40. * cm;
    ci_EMCAL_det_Width = 4. * cm;
    ci_EMCAL_det_Gap = 0.01 * mm;

    ci_EMCAL_det_Material = fMat->GetMaterial("PbWO4");
    ci_EMCAL_det_Solid = new G4Box("ci_EMCAL_det_Solid", ci_EMCAL_det_Width * 0.5, ci_EMCAL_det_Width * 0.5, ci_EMCAL_det_Length * 0.5);
    ci_EMCAL_det_Logic = new G4LogicalVolume(ci_EMCAL_det_Solid, World_Material, "ci_EMCAL_det_Logic");

    attr_ci_EMCAL_det = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
    // G4VisAttributes* attr_ci_EMCAL_det= new G4VisAttributes(G4Color(0.1, 1.0, 0.9,0.5));
    attr_ci_EMCAL_det->SetLineWidth(1);
    attr_ci_EMCAL_det->SetForceSolid(true);
    ci_EMCAL_det_Logic->SetVisAttributes(attr_ci_EMCAL_det);

    // Crystals

    ci_EMCAL_det_Rin1 = 20. * cm;
    ci_EMCAL_det_Rin2 = 55. * cm;
    G4double y_Ch = 0;
    G4double x_Ch;
    G4int kh = -1;

//============  For sectors =====
    for (int j = 0; j < 50; j++) {
        y_Ch -= ci_EMCAL_det_Width + ci_EMCAL_det_Gap;
        x_Ch = (ci_EMCAL_det_Width + ci_EMCAL_det_Gap) * 0.5;

        for (int i = 0; i < 50; i++) {
            double R_H = sqrt(x_Ch * x_Ch + y_Ch * y_Ch);

            printf("ci_EMCAL_det:: k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ", kh, j, i, x_Ch, y_Ch, R_H, ci_EMCAL_det_Rin1);

            //----------------------- Remove left side (small ring)----------------
            if (R_H < ci_EMCAL_GVol_ROut - ci_EMCAL_det_Width + ci_EMCAL_det_Gap && R_H > ci_EMCAL_det_Rin1) {
                printf("ci_EMCAL_det::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ", kh, j, i, x_Ch, y_Ch, R_H, ci_EMCAL_det_Rin1);


                kh++;
                sprintf(abname, "ci_EMCAL_det_Phys_%d", kh);

                new G4PVPlacement(0, G4ThreeVector(x_Ch, y_Ch, 0.), abname, ci_EMCAL_det_Logic,
                                  ci_EMCAL_GVol_Phys, false, kh);
                kh++;
                sprintf(abname, "ci_EMCAL_det_Phys_%d", kh);
                new G4PVPlacement(0, G4ThreeVector(x_Ch, -y_Ch, 0.), abname, ci_EMCAL_det_Logic,
                                  ci_EMCAL_GVol_Phys, false, kh);
            }

            //----------------------- Remove right side (large ring)----------------
            if (R_H < ci_EMCAL_GVol_ROut - ci_EMCAL_det_Width + ci_EMCAL_det_Gap && R_H > ci_EMCAL_det_Rin2) {
                printf("ci_EMCAL_det::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ", kh, j, i, x_Ch, y_Ch, R_H, ci_EMCAL_det_Rin2);

                kh++;
                sprintf(abname, "ci_EMCAL_det_Phys_%d", kh);
                new G4PVPlacement(0, G4ThreeVector(-x_Ch, y_Ch, 0.), abname, ci_EMCAL_det_Logic,
                                  ci_EMCAL_GVol_Phys, false, kh);
                kh++;
                sprintf(abname, "ci_EMCAL_detPhys_%d", kh);
                new G4PVPlacement(0, G4ThreeVector(-x_Ch, -y_Ch, 0.), abname, ci_EMCAL_det_Logic,
                                  ci_EMCAL_GVol_Phys, false, kh);
            }
            x_Ch += ci_EMCAL_det_Width + ci_EMCAL_det_Gap;

        }
    }

#endif
//===================================================================================



#endif // end USE_CI_ENDCAP


    //***********************************************************************************
    //***********************************************************************************
    //**                              ELECTRON  ENDCAP DETECTOR  VOLUMES               **
    //***********************************************************************************
    //***********************************************************************************

#ifdef USE_E_ENDCAP

//===================================================================================
//                         mRICH  electron endcap
//===================================================================================

#ifdef USE_CE_MRICH

    ce_MRICH_GVol_RIn = 12 * cm;
    // for new magnet     ce_MRICH_GVol_ROut = 120 * cm;
    ce_MRICH_GVol_ROut = 82 * cm;
    ce_MRICH_GVol_SizeZ = 15 * cm;
    ce_MRICH_GVol_PosZ = ce_ENDCAP_GVol_SizeZ / 2 - ce_MRICH_GVol_SizeZ / 2 - 2 * cm;
    ce_MRICH_GVol_Solid = new G4Tubs("ce_MRICH_GVol_Solid", ce_MRICH_GVol_RIn, ce_MRICH_GVol_ROut, ce_MRICH_GVol_SizeZ / 2., 0., 360 * deg);
    ce_MRICH_GVol_Logic = new G4LogicalVolume(ce_MRICH_GVol_Solid, World_Material, "ce_MRICH_GVol_Logic");
    attr_ce_MRICH_GVol = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
    attr_ce_MRICH_GVol->SetLineWidth(1);
    attr_ce_MRICH_GVol->SetForceSolid(false);
    ce_MRICH_GVol_Logic->SetVisAttributes(attr_ce_MRICH_GVol);

    ce_MRICH_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, ce_MRICH_GVol_PosZ), "ce_MRICH_GVol_Phys", ce_MRICH_GVol_Logic,
                                         ce_ENDCAP_GVol_Phys, false, 0);
    //.................................................................................
    //                                MRICH modules
    //.................................................................................

    ce_MRICH_mod_Thickness = 10. * cm;
    ce_MRICH_mod_Width = 10. * cm;
    ce_MRICH_mod_Gap = 1 * cm;

    ce_MRICH_mod_Solid = new G4Box("ce_MRICH_mod_Solid", ce_MRICH_mod_Width * 0.5, ce_MRICH_mod_Width * 0.5, ce_MRICH_mod_Thickness * 0.5);
    ce_MRICH_mod_Logic = new G4LogicalVolume(ce_MRICH_mod_Solid, World_Material, "ce_MRICH_mod_Logic");

    attr_ce_MRICH_mod = new G4VisAttributes(G4Color(1, 0.6, 0.1, 0.8));
    attr_ce_MRICH_mod->SetLineWidth(1);
    attr_ce_MRICH_mod->SetForceSolid(true);
    ce_MRICH_mod_Logic->SetVisAttributes(attr_ce_MRICH_mod);

    G4double ce_MRICH_GVol_InnerR = 15. * cm;
    G4double MRICH_R;
    G4double y_mrich = 0.;
    G4double x_mrich = 0;
    G4int kmrich = -1;

//============  For MRICH sectors =====
    for ( int j = 0; j < 10; j++) {
        y_mrich -= (ce_MRICH_mod_Width + ce_MRICH_mod_Gap);
        x_mrich = (ce_MRICH_mod_Width + ce_MRICH_mod_Gap) * 0.5;
	// printf("MRICH0:: x_mrich =%f,  y_mrich=%f\n", x_mrich, y_mrich);
        for (int i = 0; i < 10; i++) {
            double MRICH_R = sqrt(x_mrich * x_mrich + y_mrich * y_mrich);

	    //       printf("MRICH1::kmrich=%d  j=%d i =%d x=%f, y=%f  MRICH_R=%f ce_MRICH_GVol_InnerR=%f \n ", kmrich, j, i, x_mrich,
            //       y_mrich, MRICH_R, ce_MRICH_GVol_InnerR);


            if (MRICH_R < (ce_MRICH_GVol_ROut - ce_MRICH_mod_Width/2 - ce_MRICH_mod_Gap/2) && MRICH_R > ce_MRICH_GVol_InnerR) {
  
                kmrich++;
                sprintf(abname, "ce_MRICH_mod_Phys_%d", kmrich);
                new G4PVPlacement(0, G4ThreeVector(x_mrich, y_mrich, 0.), abname, ce_MRICH_mod_Logic,
                                  ce_MRICH_GVol_Phys, false, kmrich);
                kmrich++;
                sprintf(abname, "ce_MRICH_mod_Phys_%d", kmrich);
                new G4PVPlacement(0, G4ThreeVector(-x_mrich, y_mrich, 0.), abname, ce_MRICH_mod_Logic,
                                  ce_MRICH_GVol_Phys, false, kmrich);

                kmrich++;
                sprintf(abname, "ce_MRICH_mod_Phys_%d", kmrich);
                new G4PVPlacement(0, G4ThreeVector(x_mrich, -y_mrich, 0.), abname, ce_MRICH_mod_Logic,
                                  ce_MRICH_GVol_Phys, false, kmrich);

                kmrich++;
                sprintf(abname, "ce_MRICH_mod_Phys_%d", kmrich);
                new G4PVPlacement(0, G4ThreeVector(-x_mrich, -y_mrich, 0.), abname, ce_MRICH_mod_Logic,
                                  ce_MRICH_GVol_Phys, false, kmrich);
              printf("ce_MRICH::kmrich=%d  j=%d i =%d x=%f, y=%f  MRICH_R=%f ce_MRICH_GVol_InnerR=%f \n ", kmrich, j, i, x_mrich,
                       y_mrich, MRICH_R, ce_MRICH_GVol_InnerR);

            }
            x_mrich += (ce_MRICH_mod_Width + ce_MRICH_mod_Gap);

        }
    }
//----------------------------end MRICH --------------------------------------------
#endif


//===================================================================================
//                         EMCAL Electron endcap
//===================================================================================

#ifdef USE_CE_EMCAL

    ce_EMCAL_GVol_RIn = 12 * cm;
       ce_EMCAL_GVol_ROut = 150 * cm;
    //  ce_EMCAL_GVol_ROut = 60 * cm;
    ce_EMCAL_GVol_SizeZ = 40 * cm;
    ce_EMCAL_GVol_PosZ = -ce_ENDCAP_GVol_SizeZ / 2 + ce_EMCAL_GVol_SizeZ / 2;
    ce_EMCAL_GVol_Solid = new G4Tubs("ce_EMCAL_GVol_Solid", ce_EMCAL_GVol_RIn, ce_EMCAL_GVol_ROut, ce_EMCAL_GVol_SizeZ / 2., 0., 360 * deg);
    ce_EMCAL_GVol_Logic = new G4LogicalVolume(ce_EMCAL_GVol_Solid, World_Material, "ce_EMCAL_GVol_Logic");
    attr_ce_EMCAL_GVol = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
    attr_ce_EMCAL_GVol->SetLineWidth(1);
    attr_ce_EMCAL_GVol->SetForceSolid(false);
    ce_EMCAL_GVol_Logic->SetVisAttributes(attr_ce_EMCAL_GVol);

    //   my_z= 0*cm;
    ce_EMCAL_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, ce_EMCAL_GVol_PosZ), "ce_EMCAL_GVol_Phys", ce_EMCAL_GVol_Logic,
                                       ce_ENDCAP_GVol_Phys, false, 0);


    //------------------------------------------------------------------
    // Ecal module Crystals 
    //-------------------------------------------------------------------

    ce_EMCAL_detPWO_Thickness = 30. * cm;
    ce_EMCAL_detPWO_ROut= 82*cm ;
    ce_EMCAL_detPWO_Width = 2. * cm;
    ce_EMCAL_detPWO_Gap = 0.01 * mm;

    ce_EMCAL_detPWO_Material = fMat->GetMaterial("PbWO4");
    ce_EMCAL_detPWO_Solid = new G4Box("ce_EMCAL_detPWO_Solid", ce_EMCAL_detPWO_Width * 0.5, ce_EMCAL_detPWO_Width * 0.5, ce_EMCAL_detPWO_Thickness * 0.5);
    ce_EMCAL_detPWO_Logic = new G4LogicalVolume(ce_EMCAL_detPWO_Solid, ce_EMCAL_detPWO_Material, "ce_EMCAL_detPWO_Logic");

    attr_ce_EMCAL_detPWO = new G4VisAttributes(G4Color(0.1, 1.0, 0.9, 0.5));
    attr_ce_EMCAL_detPWO->SetLineWidth(1);
    attr_ce_EMCAL_detPWO->SetForceSolid(true);
    ce_EMCAL_detPWO_Logic->SetVisAttributes(attr_ce_EMCAL_detPWO);

    // Crystals

    G4double x0 = 0 * cm;
    G4double y0 = 0 * cm;
    ce_EMCAL_detPWO_InnerR = 15. * cm;
    G4double y_C = 0;
    G4double x_C;
    ce_EMCAL_detPWO_PosZ =ce_EMCAL_GVol_SizeZ/2-ce_EMCAL_detPWO_Thickness/2;
    G4int k = -1;

//============  For sectors =====
    for (j = 0; j < 50; j++) {
        y_C -= ce_EMCAL_detPWO_Width + ce_EMCAL_detPWO_Gap;
        x_C = (ce_EMCAL_detPWO_Width + ce_EMCAL_detPWO_Gap) * 0.5;

        for (i = 0; i < 50; i++) {
            double R = sqrt(x_C * x_C + y_C * y_C);

            //   printf("EMCALLL::k=%d  j=%d i =%d x=%f, y=%f  R=%f ce_EMCAL_detPWO_InnerR=%f \n ",k, j,i, x_C,y_C, R, ce_EMCAL_detPWO_InnerR);


            if (R < ce_EMCAL_detPWO_ROut - ce_EMCAL_detPWO_Width + ce_EMCAL_detPWO_Gap && R > ce_EMCAL_detPWO_InnerR) {
 
                k++;
                sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                new G4PVPlacement(0, G4ThreeVector(x_C, y_C, ce_EMCAL_detPWO_PosZ), abname, ce_EMCAL_detPWO_Logic,
                                  ce_EMCAL_GVol_Phys, false, k);
                k++;
                sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                new G4PVPlacement(0, G4ThreeVector(-x_C, y_C, ce_EMCAL_detPWO_PosZ ), abname, ce_EMCAL_detPWO_Logic,
                                  ce_EMCAL_GVol_Phys, false, k);

                k++;
                sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                new G4PVPlacement(0, G4ThreeVector(x_C, -y_C,  ce_EMCAL_detPWO_PosZ), abname, ce_EMCAL_detPWO_Logic,
                                  ce_EMCAL_GVol_Phys, false, k);

                k++;
                sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                new G4PVPlacement(0, G4ThreeVector(-x_C, -y_C,  ce_EMCAL_detPWO_PosZ), abname, ce_EMCAL_detPWO_Logic,
                                  ce_EMCAL_GVol_Phys, false, k);
                 printf("ce_EMCAL_detPWO::k=%d  j=%d i =%d x=%f, y=%f  R=%f ce_EMCAL_detPWO_InnerR=%f \n ",k, j,i, x_C,y_C, R, ce_EMCAL_detPWO_InnerR);

          }
            x_C += ce_EMCAL_detPWO_Width + ce_EMCAL_detPWO_Gap;

        }
    }

    //------------------------------------------------------------------
    // Ecal module GLASS 
    //-------------------------------------------------------------------

    ce_EMCAL_detGLASS_Thickness = 40. * cm;
    ce_EMCAL_detGLASS_OuterR= ce_EMCAL_GVol_ROut;

    ce_EMCAL_detGLASS_Width = 4. * cm;
    ce_EMCAL_detGLASS_Gap = 0.01 * mm;


    ce_EMCAL_detGLASS_Material= fMat->GetMaterial("DSBCe");
    //   ce_EMCAL_det_Material = fMat->GetMaterial("PbWO4");
    ce_EMCAL_detGLASS_Solid = new G4Box("ce_EMCAL_detGLASS_Solid", ce_EMCAL_detGLASS_Width * 0.5, ce_EMCAL_detGLASS_Width * 0.5, ce_EMCAL_detGLASS_Thickness * 0.5);
    ce_EMCAL_detGLASS_Logic = new G4LogicalVolume(ce_EMCAL_detGLASS_Solid, ce_EMCAL_detGLASS_Material, "ce_EMCAL_detGLASS_Logic");

    attr_ce_EMCAL_detGLASS = new G4VisAttributes(G4Color(0.3, 0.4, 1., 0.5));
     attr_ce_EMCAL_detGLASS->SetLineWidth(1);
     attr_ce_EMCAL_detGLASS->SetForceSolid(true);
    ce_EMCAL_detGLASS_Logic->SetVisAttributes(attr_ce_EMCAL_detGLASS);

    // GLASS

    x0 = 0 * cm;
    y0 = 0 * cm;
    ce_EMCAL_detGLASS_InnerR= ce_EMCAL_detPWO_ROut;
     y_C = 0;
     x_C=0.;
     ce_EMCAL_detGLASS_PosZ =ce_EMCAL_GVol_SizeZ/2-ce_EMCAL_detGLASS_Thickness/2;
     k = -1;

//============  For sectors =====
    for (j = 0; j < 50; j++) {
        y_C -= ce_EMCAL_detGLASS_Width + ce_EMCAL_detGLASS_Gap;
        x_C = (ce_EMCAL_detGLASS_Width + ce_EMCAL_detGLASS_Gap) * 0.5;

        for (i = 0; i < 50; i++) {
            double R = sqrt(x_C * x_C + y_C * y_C);

            //   printf("EMCALLL::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, R0);


            if (R <  ce_EMCAL_GVol_ROut  - ce_EMCAL_detGLASS_Width + ce_EMCAL_detGLASS_Gap && R > ce_EMCAL_detGLASS_InnerR) {
                printf("ce_EMCAL_detGLASS::GLASS k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, ce_EMCAL_detPWO_InnerR);


                k++;
                sprintf(abname, "ce_EMCAL_detGLASS_Phys_%d", k);
                new G4PVPlacement(0, G4ThreeVector(x_C, y_C, ce_EMCAL_detGLASS_PosZ), abname, ce_EMCAL_detGLASS_Logic,
                                  ce_EMCAL_GVol_Phys, false, k);
                k++;
                sprintf(abname, "ce_EMCAL_detGLASS_Phys_%d", k);
                new G4PVPlacement(0, G4ThreeVector(-x_C, y_C, ce_EMCAL_detGLASS_PosZ ), abname, ce_EMCAL_detGLASS_Logic,
                                  ce_EMCAL_GVol_Phys, false, k);

                k++;
                sprintf(abname, "ce_EMCAL_detGLASS_Phys_%d", k);
                new G4PVPlacement(0, G4ThreeVector(x_C, -y_C,  ce_EMCAL_detGLASS_PosZ), abname, ce_EMCAL_detGLASS_Logic,
                                  ce_EMCAL_GVol_Phys, false, k);

                k++;
                sprintf(abname, "ce_EMCAL_detGLASS_Phys_%d", k);
                new G4PVPlacement(0, G4ThreeVector(-x_C, -y_C,  ce_EMCAL_detGLASS_PosZ), abname, ce_EMCAL_detGLASS_Logic,
                                  ce_EMCAL_GVol_Phys, false, k);
            }
            x_C += ce_EMCAL_detGLASS_Width + ce_EMCAL_detGLASS_Gap;

        }
    }

#endif
//======================= END E-EMCAL =================================================

#endif





//===================================================================================
//                          START placement of BEAM ELEMENTS                       ==
//===================================================================================
#ifdef USE_FFQs


    Read_Di_File();
    Read_dE_File();

#endif


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
                                  Solenoid_Phys, false,  0 );

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
                                  Solenoid_Phys, false,  0 );

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
  G4double Z_cone[4]={ 0.*cm,  0.*cm,  (Solenoid_SizeZ/2),  (Solenoid_SizeZ/2)};
  //World_ShiftVTX
  // Z1=  fBeamSizeZ_Be

  //  printf("zPos=%f,fBeamSizeRin2Cone=%f fBeamSizeRout2Cone=%f fBeamSizeZCone+zPos=%f\n",zPos,
  //       fBeamSizeRin2Cone,fBeamSizeRout2Cone,fBeamSizeZCone+zPos);

  fSolidBeamCone = new G4Polycone("BeampipeCone", 0., 360., 4, R_cone , Z_cone );

   fLogicBeamCone = new G4LogicalVolume(fSolidBeamCone,  fBeamPipeConeMaterial,  "BeampipeCone");

   //  fPhysicsBeamCone = new G4PVPlacement(0, G4ThreeVector(0,0,-30*cm), "BeampipeCone",fLogicBeamCone,
   //                           Solenoid_Phys, false,    0 );



   bpr1.rotateY(-2.5*deg);
  fPhysicsBeamCone = new G4PVPlacement(G4Transform3D(bpr1, G4ThreeVector(0,0,0*cm)), "BeampipeCone",fLogicBeamCone,
                              Solenoid_Phys, false,    0 );

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
                                 Solenoid_Phys, false,  0 );


   vb1->SetForceSolid(true); fLogicBeamCone2->SetVisAttributes(vb1);fLogicBeamCone->SetVisAttributes(vb1);


  */

#endif

#ifdef  USE_VTX0    /*----------vtx barrel simple geometry--------------*/
                                                                                                                            //===================================================================================

  int FDIV=0;

  G4RotationMatrix rm[10][20], rm1[10][20], rm2[10][20];
   deltaphi1=0; cb_VTX_ladder_deltaphi=30.*deg; phi=0; x=3*cm; y=0*cm; z=0*cm;
  cb_VTX_ladder_deltashi=-7.*deg;
  G4double           fAbsorberDX=10*cm;
  G4double           cb_VTX_ladder_DY=2*cm;
  G4double fVTXZ;
  fVTXZ = fStartZ + fRadThick + 2*cm;  //-- Si at dist. 2cm
  int NLAYBARR=6;
  G4double   fVTXThickness0[6]=  {0.005,0.005,0.01,0.01,0.01,0.01};  // --- in cm ;
  G4double           VTXin[6] =  { 3.5 ,4.5, 6.5, 10.5, 14.5, 19.5 };  // --- in cm
  G4double           VTXout[6];
  G4double          fVTXZ[6]=  { 10., 11., 18.,24.,36.,48.}; // --- in cm  ;

  /* VTXin[0]=3.5; VTXout[0]=( VTXin[0]*cm; fVTXZ[0]=10*cm;
  VTXin[1]=4.5; VTXout[1]=4.505*cm; fVTXZ[1]=11*cm;
  VTXin[2]=6.5; VTXout[2]=6.51*cm;   fVTXZ[2]=18*cm;
  VTXin[3]=10.5; VTXout[3]=10.51*cm; fVTXZ[3]=24*cm;
  VTXin[4]=14.5; VTXout[4]=14.51*cm; fVTXZ[4]=36*cm;
  VTXin[5]=19.5; VTXout[5]=19.51*cm; fVTXZ[5]=48*cm;
  */



    // ------- layers of Si in VTX0
      for (int ia=0;ia<NLAYBARR;ia++) {

	VTXout[ia]=VTXin[ia]+fVTXThickness0[ia];
	sprintf(abname,"Solid_VTX_layer%d",ia);
	fSolidVTXBarrel[ia] = new  G4Tubs(abname, VTXin[ia]*cm, VTXout[ia]*cm,(fVTXZ[ia]/2.)*cm,0.,360*deg);

	sprintf(abname,"Logic_VTX_layer_%d",ia);
	fLogicVTXBarrel[ia] = new G4LogicalVolume(fSolidVTXBarrel[ia],
                                         fVTXMaterial, abname);
	G4VisAttributes* attr_cb_VTX;
	if(ia<2){      attr_cb_VTX= new G4VisAttributes(G4Color(0.0,0.2,0.8,2.0));}
	else{       attr_cb_VTX= new G4VisAttributes(G4Color(1.0-0.1*(ia-2), 1.0, 0.0+0.1*(ia-2),1.1));}
	attr_cb_VTX->SetLineWidth(1); attr_cb_VTX->SetForceSolid(true);
        fLogicVTXBarrel[ia]->SetVisAttributes(attr_cb_VTX);

	sprintf(abname,"BarrelVTX_layer_%d",ia);
	printf("Logic BarrelVTX_layer_%d \n ",ia);
	fPhysicsVTXBarrel[ia] = new G4PVPlacement(0,G4ThreeVector(),
					     abname,fLogicVTXBarrel[ia],
					     cb_VTX_GVol_Phys,false,0.);
	//	if (fLogicVTXBarrel[ia])  fLogicVTXBarrel[ia]->SetSenstiveDetector(fCalorimeterSD);
     }


     printf(" I use vtx0, nlay=%d\n",NLAYBARR);
    for (int ia=0;ia<NLAYBARR;ia++) {

	if (fLogicVTXBarrel[ia]) { 	printf("Sensitive BarrelVTX_layer_%d\n",ia); fLogicVTXBarrel[ia]->SetSensitiveDetector(fCalorimeterSD);  }
    }
 printf(" end of VERTEX detector \n");



#endif


//===================================================================================
#ifdef  USE_CB_VTX_LADDERS
    //--------------------------------------------------
    //----------vtx barrel ladder geometry--------------
    //--------------------------------------------------
//===================================================================================

    G4RotationMatrix rm[10][40];
   // deltaphi1 = 0;
    cb_VTX_ladder_deltaphi = 30. * deg;
    cb_VTX_ladder_deltashi = -7. * deg;
    G4double x, y, z;
     z = 0 * cm;
    //phi=26.*deg; x=0; y=0; z=fAbsorberZ;
    //phi=0.*deg; x=0; y=0; z=fAbsorberZ;

    int FDIV = 0;
    double dR;
    double myL;
    double phi=0;

    cb_VTX_ladder_Material = fMat->GetMaterial("Si");

    std::vector <cb_VTX_ladder_LayParam> Lays;
    cb_VTX_ladder_LayParam Lay;
    // Lay 0
    Lay.Dx=0.050 * mm; Lay.Dy=2*cm; Lay.Dz=10*cm; Lay.Rin=3.5 * cm; Lays.push_back(Lay);  
    // Lay 1
    Lay.Dx=0.050 * mm; Lay.Dy=2*cm; Lay.Dz=11*cm; Lay.Rin=4.5 * cm; Lays.push_back(Lay);
    // Lay 2
    Lay.Dx=0.050 * mm; Lay.Dy=4*cm; Lay.Dz=18*cm; Lay.Rin=6.5 * cm; Lays.push_back(Lay);
    // Lay 3
    Lay.Dx=0.050 * mm; Lay.Dy=4*cm; Lay.Dz=24*cm; Lay.Rin=10.5 * cm; Lays.push_back(Lay);
    // Lay 4
    Lay.Dx=0.050 * mm; Lay.Dy=4*cm; Lay.Dz=36*cm; Lay.Rin=13.5 * cm; Lays.push_back(Lay);
    // Lay 5
    Lay.Dx=0.050 * mm; Lay.Dy=4*cm; Lay.Dz=48*cm; Lay.Rin=15.5 * cm; Lays.push_back(Lay);

    if( Lays.size()>10) {printf("Nlayers in VERTEX >10 !!! \n"); exit(1); }

    for (int lay = 0; lay < Lays.size(); lay++) {

      printf("cb_VTX_ladder:: Layer loop:: %d\n", lay);
      cb_VTX_ladder_DZ = Lays[lay].Dz;
      cb_VTX_ladder_DY = Lays[lay].Dy;
      cb_VTX_ladder_Thickness = Lays[lay].Dx;
      dR =  Lays[lay].Rin;

      myL = 2*3.1415*dR;
      NUM = myL/cb_VTX_ladder_DY;

      for(int i=0;i<2; i++){ 
	double LN = cb_VTX_ladder_DY * NUM;
	double LN1 = cb_VTX_ladder_DY * (NUM+1+i);
	printf("cb_VTX_ladder:: LN= Orig NUM=%d\n",NUM);
	if (LN/LN1>0.8) NUM=NUM+1;
	printf("cb_VTX_ladder:: LN=%f, LN1=%f  delenie=%f NUM=%d \n",LN,LN1,LN/LN1,NUM);
      }
      cb_VTX_ladder_deltaphi = 2*3.1415926/NUM  ;
 
 
      sprintf(abname, "cb_VTX_ladder_Solid_%d", lay);
      cb_VTX_ladder_Solid[lay] = new G4Box(abname, cb_VTX_ladder_Thickness / 2.,   cb_VTX_ladder_DY / 2.,cb_VTX_ladder_DZ / 2.  );
      
      sprintf(abname, "cb_VTX_ladder_Logic_%d", lay);
      cb_VTX_ladder_Logic[lay] = new G4LogicalVolume(cb_VTX_ladder_Solid[lay], cb_VTX_ladder_Material,
						      abname);
      

      if (lay == 0 || lay == 1) { attr_cb_VTX_ladder = new G4VisAttributes(G4Color(0.0, 0.2, 0.8, 2.0)); }
      else if (lay == 2) { attr_cb_VTX_ladder = new G4VisAttributes(G4Color(0.0, 0.2, 0.8, 0.7)); }
      else {
	  attr_cb_VTX_ladder = new G4VisAttributes(G4Color(0.0 + 0.1 * double(lay - 3), 1., 1. - 0.1 * double(lay - 3), 1.0));
      }
      attr_cb_VTX_ladder->SetForceSolid(true);
      cb_VTX_ladder_Logic[lay]->SetVisAttributes(attr_cb_VTX_ladder);
      
      if( NUM>40) {printf("cb_VTX_ladder:: Nladders in VERTEX >40 lay=%f !!! \n",lay); exit(1); }

      
      for (int ia = 0; ia < NUM; ia++) {
	//for (int ia=0;ia<1;ia++) {
	printf("cb_VTX_ladder:: lay=%d  NUM=%d, dR=%f cb_VTX_ladder_deltaphi=%f %f \n",lay, NUM,  dR, cb_VTX_ladder_deltaphi,cb_VTX_ladder_deltashi);
	printf("cb_VTX_ladder:: Module  loop:: %d\n", ia);
	
	phi = (ia * (cb_VTX_ladder_deltaphi));
	x = - dR * cos(phi) ;
	y = - dR * sin(phi) ;
	rm[lay][ia].rotateZ(cb_VTX_ladder_deltaphi * ia);
	rm[lay][ia].rotateZ(cb_VTX_ladder_deltashi);

	printf("cb_VTX_ladder::  %d %d x=%f  y=%f  \n", lay, ia, x, y);
	sprintf(abname, "cb_VTX_ladder_Phys_%d_%d", lay, ia);
	fPhysicsAbsorber = new G4PVPlacement(G4Transform3D(rm[lay][ia], G4ThreeVector(x, y, z)),
					     abname, cb_VTX_ladder_Logic[lay],
					     cb_VTX_GVol_Phys, false, 0.);
      }
      //-------------------------------------------------------------------------
      //                          VTX  slices and pixels
      //-------------------------------------------------------------------------
      G4Box *pxdBox_slice[10];
      G4Box *pxdBox_pixel[10];
      G4double PixelDX, PixelDY;
      if (lay < 2) {
	      PixelDX = cb_VTX_ladder_DZ / 10.; //2000.*um;
	      PixelDY = cb_VTX_ladder_DY / 50.; //2000.*um;
      } else {
	      PixelDX = cb_VTX_ladder_DZ / 50.; //2000.*um;
	      PixelDY = cb_VTX_ladder_DY / 10.; //2000.*um;
	    
      }
      //G4double PixelDX=20.*um;
      //G4double PixelDY=20.*um;
      //G4double PixelDX=24.*um;
      //G4double PixelDY=24.*um;
      G4double PixelDZ = cb_VTX_ladder_Thickness; // 0.450*mm
      
        if (FDIV >= 1) {
	  printf("cb_VTX_ladder_pxdSlice_:: construct slices %d \n", lay);
	  
	  sprintf(abname, "cb_VTX_ladder_pxdSlice_%d", lay);
	  pxdBox_slice[lay] = new G4Box(abname,
					PixelDX / 2,                   //gD->GetPixelDX(),
					cb_VTX_ladder_DY / 2., // 10.*mm,  //gD->GetHalfMPXWaferDY(),
					cb_VTX_ladder_Thickness / 2.);    //gD->GetHalfMPXWaferDZ());
	  
	  pxdSlice_log[lay] = new G4LogicalVolume(pxdBox_slice[lay], fAbsorberMaterial, abname, 0, 0, 0);
	  
	  G4VisAttributes *pixelVisAtt = new G4VisAttributes(G4Color(0, 1, 1, 1));
	  pixelVisAtt->SetLineWidth(1);
	  pixelVisAtt->SetForceWireframe(true);
	  pxdSlice_log[lay]->SetVisAttributes(pixelVisAtt);
	  
	  
	  // divide in slices
	  sprintf(abname, "pxdSlice_%d", lay);
	  G4PVDivision *sliceDiv = new G4PVDivision(abname,
						    pxdSlice_log[lay],
						    cb_VTX_ladder_Logic[lay],
						    kXAxis,
						    PixelDX,
                                                      0);
	  printf("SetUpVertex16():: construct done\n");
	  
	  
	  if (FDIV >= 2) {
	    printf("SetUpVertex16():: construct pixels \n");
	    if (lay < 2) { sprintf(abname, "pxdPixel"); }
	    else {
	      sprintf(abname, "svdPixel");
	    }
	    
	    //sprintf(abname,"pxdPixel_%d",lay);
	    pxdBox_pixel[lay] = new G4Box(abname,
					  PixelDX / 2,
					  PixelDY / 2.,
					  PixelDZ / 2.);
	    pxdPixel_log[lay] = new G4LogicalVolume(pxdBox_pixel[lay], fAbsorberMaterial, abname, 0, 0, 0);
	    pxdPixel_log[lay]->SetVisAttributes(pixelVisAtt);

	    // divide in pixels
	    G4PVDivision *pixelDiv = new G4PVDivision(abname,
						      pxdPixel_log[lay],
						      pxdSlice_log[lay],
						      kYAxis,
						      PixelDY,
						      0);
	  } //-- end if pixel division
        } //-- end if slices division

        //	 };  // -- end loop over modules
	
    }; // --- end loop over layers

#endif



    /*--------------------------------------------------*/
    /*-----------VTX  End caps ladder geometry----------*/
    /*--------------------------------------------------*/


#ifdef  USE_VTX_ENDCAP

    lay = 0;
    int NUMF;
    G4RotationMatrix rme[10][20], rme1[10][20], rme2[10][20];
    G4double Fdeltaphi, Ftheta, F2theta;
    G4double RxF[10], RyF[10], RzF[10], RxF2[10], RyF2[10], RzF2[10];
    //for simple version     G4double Rzshift =24.;
    G4double Rzshift = 24.;

    //  Rx[lay]=(1.4)*cm; Ry[lay]=Rx[lay];
    G4double fVTX_END_EDY = 12 * cm;
    G4double fVTX_END_EDZ = 0.05 * cm;
    G4double fVTX_END_EDX1 = 6 * cm;
    G4double fVTX_END_EDX2 = 4 * cm;

    for (lay = 0; lay < 4; lay++) {
        if (lay == 3) {
            fVTX_END_EDY = 18 * cm;
            NUMF = 24;
            Fdeltaphi = 15. * deg;
            Ftheta = -40. * deg;
            RxF[lay] = (1.3) * cm;
            RyF[lay] = RxF[lay];
            RzF[lay] = -Rzshift * cm - 5.5 * cm;
            RxF2[lay] = (1.3) * cm;
            RyF2[lay] = RxF2[lay];
            RzF2[lay] = Rzshift * cm + 5.5 * cm;
        }
        if (lay == 2) {
            NUMF = 20;
            fVTX_END_EDY = 16 * cm;
            Fdeltaphi = 18. * deg;
            Ftheta = -38. * deg;
            RxF[lay] = (1.1) * cm;
            RyF[lay] = RxF[lay];
            RzF[lay] = -Rzshift * cm - 1. * cm;
            RxF2[lay] = (1.1) * cm;
            RyF2[lay] = RxF2[lay];
            RzF2[lay] = Rzshift * cm + 1. * cm;
        }
        if (lay == 1) {
            NUMF = 18;
            fVTX_END_EDY = 14 * cm;
            Fdeltaphi = 20. * deg;
            Ftheta = -45. * deg;
            RxF[lay] = (1.0) * cm;
            RyF[lay] = RxF[lay];
            RzF[lay] = -Rzshift * cm + 1.0 * cm;
            RxF2[lay] = (1.0) * cm;
            RyF2[lay] = RxF2[lay];
            RzF2[lay] = Rzshift * cm - 1.0 * cm;
        }
        if (lay == 0) {
            NUMF = 12;
            fVTX_END_EDY = 12 * cm;
            Fdeltaphi = 30. * deg;
            Ftheta = -55. * deg;
            RxF[lay] = (0.8) * cm;
            RyF[lay] = RxF[lay];
            RzF[lay] = -Rzshift * cm + 7. * cm;
            RxF2[lay] = (0.8) * cm;
            RyF2[lay] = RxF2[lay];
            RzF2[lay] = Rzshift * cm - 7. * cm;
        }
        printf("x1=%f x2=%f  ,y=%f ,z=%f \n", fVTX_END_EDZ, fVTX_END_EDY + lay * 2., fVTX_END_EDX1, fVTX_END_EDX2);
        sprintf(abname, "Solid_VTX_ladder_END_E%d", lay);
        fSolidVTXEndE[lay] = new G4Trap(abname, fVTX_END_EDZ,
                                        fVTX_END_EDY + lay * 2., fVTX_END_EDX1, fVTX_END_EDX2);

        sprintf(abname, "Logic_VTX_ladder_END_E%d", lay);
        fLogicVTXEndE[lay] = new G4LogicalVolume(fSolidVTXEndE[lay], fVTXMaterial,
                                                 abname);

        //      G4VisAttributes* vs2= new G4VisAttributes(G4Color(0.0+0.2*double(lay),0.2,0.8-0.2*double(lay),0.5));
        G4VisAttributes *vs2 = new G4VisAttributes(G4Color(1.0 - 0.1 * lay, 1.0, 0.0 + 0.1 * lay, 0.5));
        //vs2->SetForceWireframe(true);
        vs2->SetForceSolid(true);
        fLogicVTXEndE[lay]->SetVisAttributes(vs2);

        for (int ia = 0; ia < NUMF; ia++) {

            //       cb_VTX_ladder_deltaphi=0;cb_VTX_ladder_deltashi=0;
            phi = (ia * (Fdeltaphi));
            x = -RxF[lay] * cos(phi) * cm;
            y = -RyF[lay] * sin(phi) * cm;
            z = RzF[lay];
            rme1[lay][ia].rotateX(Ftheta);
            rme1[lay][ia].rotateZ(-90 + (Fdeltaphi * (ia + 1)));
            //WORKING       rm1[lay][ia].rotateX(-60*deg);
            //WORKING       rm1[lay][ia].rotateZ(-90+(cb_VTX_ladder_deltaphi*(ia+1)));
            sprintf(abname, "VTX_ladderEnd_%d_%d", lay, ia);
            fPhysicsVTXEndE = new G4PVPlacement(G4Transform3D(rme1[lay][ia], G4ThreeVector(x, y, z)),
                                                abname, fLogicVTXEndE[lay],
                                                cb_VTX_GVol_Phys, false, 0.);
        }


        printf("x1=%f x2=%f  ,y=%f ,z=%f \n", fVTX_END_EDZ, fVTX_END_EDY, fVTX_END_EDX1, fVTX_END_EDX2);
        sprintf(abname, "Solid_VTX_ladder_END_H%d", lay);
        fSolidVTXEndH[lay] = new G4Trap(abname, fVTX_END_EDZ,
                                        fVTX_END_EDY, fVTX_END_EDX1, fVTX_END_EDX2);

        sprintf(abname, "Logic_VTX_ladder_END_H%d", lay);
        fLogicVTXEndH[lay] = new G4LogicalVolume(fSolidVTXEndH[lay], fVTXMaterial,
                                                 abname);
        //      G4VisAttributes* vs3= new G4VisAttributes(G4Color(0.0+0.2*double(lay),0.2,0.8-0.2*double(lay),0.5));
        G4VisAttributes *vs3 = new G4VisAttributes(G4Color(1.0 - 0.1 * lay, 1.0, 0.0 + 0.1 * lay, 0.5));
        //      vs3->SetForceWireframe(true);
        vs3->SetForceSolid(true);
        fLogicVTXEndH[lay]->SetVisAttributes(vs3);

        for (int ia = 0; ia < NUMF; ia++) {


            //       cb_VTX_ladder_deltaphi=0;cb_VTX_ladder_deltashi=0;
            phi = (ia * (Fdeltaphi));
            x = -RxF2[lay] * cos(phi) * cm;
            y = -RyF2[lay] * sin(phi) * cm;
            z = RzF2[lay];
            rme2[lay][ia].rotateX(-Ftheta);
            rme2[lay][ia].rotateZ(-90 + (Fdeltaphi * (ia + 1)));
            //WORKING       rm1[lay][ia].rotateX(-60*deg);
            //WORKING       rm1[lay][ia].rotateZ(-90+(cb_VTX_ladder_deltaphi*(ia+1)));
            sprintf(abname, "VTX_ladderEnd2_%d_%d", lay, ia);
            fPhysicsVTXEndH = new G4PVPlacement(G4Transform3D(rme2[lay][ia], G4ThreeVector(x, y, z)),
                                                abname, fLogicVTXEndH[lay],
                                                cb_VTX_GVol_Phys, false, 0.);


        }

        if (fLogicVTXEndE[lay]) { fLogicVTXEndE[lay]->SetSensitiveDetector(fCalorimeterSD); }
        if (fLogicVTXEndH[lay]) { fLogicVTXEndH[lay]->SetSensitiveDetector(fCalorimeterSD); }


    }



    //-----------------------------------------------------------------------




#endif

    printf(" end of VERTEX volume \n");

//============================   end VERTEX    =====================


//===================================================================================
//                           cb_CTD_Si  detector
//===================================================================================
#ifdef USE_CB_CTD_Si

    cb_CTD_detSi_SizeZ = cb_CTD_GVol_SizeZ;
    cb_CTD_detSi_Steps = 5.;
    cb_CTD_detSi_Layers = 15;
    cb_CTD_detSi_Material = fMat->GetMaterial("Si");
    // ------- layers of Si in CTD
    for (int ia = 0; ia < cb_CTD_detSi_Layers; ia++) {
        cb_CTD_detSi_lay_Rin[ia] = cb_CTD_GVol_RIn + (cb_CTD_detSi_Steps * ia) * cm;
        cb_CTD_detSi_lay_Rout[ia] = cb_CTD_GVol_RIn + (0.01 + cb_CTD_detSi_Steps * ia) * cm;
        printf("cb_CTD_detSi %d  Rout=%f \n", ia, cb_CTD_detSi_lay_Rout[ia]);

        sprintf(abname, "cb_CTD_detSi_Solid_lay_%d", ia);
        cb_CTD_detSi_Solid[ia] = new G4Tubs(abname, cb_CTD_detSi_lay_Rin[ia], cb_CTD_detSi_lay_Rout[ia], cb_CTD_detSi_SizeZ / 2., 0.,
                                             360 * deg);

        sprintf(abname, "cb_CTD_detSi_Logic_lay_%d", ia);
        cb_CTD_detSi_Logic[ia] = new G4LogicalVolume(cb_CTD_detSi_Solid[ia],
                                                      cb_CTD_detSi_Material, abname);

        //   attr_cb_CTD_det    = new G4VisAttributes(G4Color(1,0,1,0.2));
        attr_cb_CTD_det= new G4VisAttributes(G4Color(1.0 - 0.1 * ia, 1.0, 0.0 + 0.1 * ia, 1));
        attr_cb_CTD_det->SetLineWidth(1);
        attr_cb_CTD_det->SetForceSolid(true);
        cb_CTD_detSi_Logic[ia]->SetVisAttributes(attr_cb_CTD_det);

        sprintf(abname, "cb_CTD_detSi_Phys_lay_%d", ia);
        cb_CTD_detSi_Phys[ia] = new G4PVPlacement(0, G4ThreeVector(),
                                                      abname, cb_CTD_detSi_Logic[ia],
                                                      cb_CTD_GVol_Phys, false, 0.);

        if (cb_CTD_detSi_Logic[ia]) cb_CTD_detSi_Logic[ia]->SetSensitiveDetector(fCalorimeterSD);
    }


#endif
//===================================================================================
//                               cb_CTD_Straw detector
//===================================================================================
#ifdef USE_cb_CTD_Straw
                                                                                                                            printf("begin STRAW volume \n");
 int NUMFs[100];
 G4double Stheta,Sphi,phi_s, Sx, Sy, Sz;;
 G4RotationMatrix rm1s;
 G4double  RxF_Straw[100],RyF_Straw[100],RzF_Straw[100];

    G4VisAttributes* vstr2[100];

 cb_CTD_detStraw_SizeZ=300*cm;
 cb_CTD_detStraw_Layers=10;

 cb_CTD_detStraw_Material=Air;

 cb_CTD_Straws_RIn=0.3 *cm;
 cb_CTD_Straws_ROut=0.31 *cm;

 sprintf(abname,"cb_CTD_Straws_Solid");
 cb_CTD_Straws_Solid = new  G4Tubs(abname,0., cb_CTD_Straws_ROut,cb_CTD_detStraw_SizeZ/2.,0.,360*deg);

 sprintf(abname,"cb_CTD_Straws_Logic");
 cb_CTD_Straws_Logic = new G4LogicalVolume(cb_CTD_Straws_Solid,
						   cb_CTD_detStraw_Material, abname);
 attr_cb_CTD_Straws= new G4VisAttributes(G4Color(0.5,0.5,0.5,1.));
 attr_cb_CTD_Straws->SetLineWidth(1); attr_cb_CTD_Straws->SetForceSolid(true);
 cb_CTD_Straws_Logic->SetVisAttributes(attr_cb_CTD_Straws);

 //-----Walls
 cb_CTD_Straws_Wall_Material=fMat->GetMaterial("Mylar");
 sprintf(abname,"cb_CTD_Straws_Wall_Solid");
 cb_CTD_Straws_Wall_Solid = new  G4Tubs(abname,0., cb_CTD_Straws_ROut,cb_CTD_detStraw_SizeZ/2.,0.,360*deg);

 sprintf(abname,"cb_CTD_Straws_Wall_Logic");
 cb_CTD_Straws_Wall_Logic = new G4LogicalVolume(cb_CTD_Straws_Wall_Solid,
						   cb_CTD_Straws_Wall_Material, abname);
sprintf(abname,"cb_CTD_Straws_Wall_Phys");
 cb_CTD_Straws_Wall_Phys = new G4PVPlacement(0, G4ThreeVector(0,0,0),
                                                  cb_CTD_Straws_Wall_Logic, abname,
                                                  cb_CTD_Straws_Logic, false,   0 );
 //-----Gas
 cb_CTD_Straws_Gas_Material=fMat->GetMaterial("Ar10CO2");
 sprintf(abname,"cb_CTD_Straws_Gas_Solid");
 cb_CTD_Straws_Gas_Solid = new  G4Tubs(abname,0., cb_CTD_Straws_RIn,cb_CTD_detStraw_SizeZ/2.,0.,360*deg);

 sprintf(abname,"cb_CTD_Straws_Gas_Logic");
 cb_CTD_Straws_Gas_Logic = new G4LogicalVolume(cb_CTD_Straws_Gas_Solid,
						   cb_CTD_Straws_Gas_Material, abname);

    attr_cb_CTD_Straws_Gas= new G4VisAttributes(G4Color(1.,0.5,0.,1.));
    attr_cb_CTD_Straws_Gas->SetLineWidth(1); attr_cb_CTD_Straws_Gas->SetForceSolid(true);
    cb_CTD_Straws_Gas_Logic->SetVisAttributes(attr_cb_CTD_Straws_Gas);

 sprintf(abname,"cb_CTD_Straws_Gas_Phys");
 cb_CTD_Straws_Gas_Phys = new G4PVPlacement(0, G4ThreeVector(0,0,0),
                                                  cb_CTD_Straws_Gas_Logic, abname,
                                                  cb_CTD_Straws_Wall_Logic, false,   0 );

    if (cb_CTD_Straws_Gas_Logic)  cb_CTD_Straws_Gas_Logic->SetSensitiveDetector(fCalorimeterSD);


       // ------- layers of Straw in CTD
    int counter=0;
    int mystr=-1;
    for (int lay=0;lay<cb_CTD_detStraw_Layers;lay++) {

 	//	RxF_Straw[lay]=cb_CTD_GVol_RIn;
	//     printf("Straw X=%f\n",	RxF_Straw[lay]);
       RxF_Straw[lay]=cb_CTD_GVol_RIn+5.+(cb_CTD_Straws_ROut*2*lay)+lay*cb_CTD_Straws_ROut; // position of first layer
	RyF_Straw[lay]=RxF_Straw[lay]+cb_CTD_Straws_ROut/2;
	RzF_Straw[lay]=0*cm;

	// cb_CTD_Straws_ROut[lay]=cb_CTD_GVol_RIn+(0.01+cb_CTD_detStraw_Steps*lay)*cm;
	//	if(lay==0) NUMFs[lay]=213;
	//	if(lay==1) NUMFs[lay]=213;
	NUMFs[lay]=213;
        int j=2*3.1415*RxF_Straw[lay]/2/cb_CTD_Straws_ROut+1;
        printf("NLAY=%d %d %f\n",lay, j ,2*3.1415*RxF_Straw[lay]/2/cb_CTD_Straws_ROut);
	NUMFs[lay]=j;


	Sphi= (cb_CTD_Straws_ROut*2/RxF_Straw[lay]);
	Stheta=0*deg;

	for (int ia=0;ia<NUMFs[lay];ia++) {

          phi_s=(ia*(Sphi));
	  Sx=- RxF_Straw[lay]*cos(phi_s);
	  Sy=- RyF_Straw[lay]*sin(phi_s);
	  Sz= 2*RzF_Straw[lay];
	   rm1s.rotateX(Stheta);
	   rm1s.rotateZ(0+(Sphi*(ia+1)));
	   mystr++;
	   printf("cb_CTD_Straws %d %d X=%f (%f,%f,%f \n", counter,	mystr, RxF_Straw[lay],Sx,Sy,Sz);


	  //       G4VisAttributes* vstr= new G4VisAttributes(G4Color(1,0,1,0.2));

	  //sprintf(abname,"Barrel_CTD_Straw_layer_%d_str_%d",lay,ia);
	  sprintf(abname,"cb_CTD_Straws_Phys");
	  cb_CTD_Straws_Phys = new G4PVPlacement(G4Transform3D(rm1s,G4ThreeVector(Sx,Sy,Sz)),
						       abname,
						       cb_CTD_Straws_Logic,
						       cb_CTD_GVol_Phys,
						       false,
						       counter);
	  counter++;
	  /* ------------------------------------------------
	    G4PVPlacement(  G4RotationMatrix*       pRot,
	                    const G4ThreeVector&    tlate,
	                    G4LogicalVolume*        pCurrentLogical,
	                    const G4String&         pName,
	                    G4LogicalVolume*   pMotherLogical,
	                    G4bool             pMany,
	                    G4int              pCopyNo,
	                    G4bool             pSurfChk=false );
	    -------------------------------------------------
	    physiHoleSF = new G4PVPlacement(
	                 rm3,
	                 G4ThreeVector(x,y,z),
	                 logicHoleSF,
	                 "HoleSF",
	                 logicZnScomp,
	                 false,
	                 counter);
	    counter++;
	    ------------------------------------------------
	  */

 	}
	//      vstr2[lay]= new G4VisAttributes(G4Color(1.0-0.01*lay, 1.0, 0.0+0.01*lay,1));
	//       G4VisAttributes *vtpc = new G4VisAttributes(G4Color(1.0 - 0.1 * ia, 1.0, 0.0 + 0.1 * ia, 1));
	//
        attr_cb_CTD_Straws = new G4VisAttributes(G4Color(1.0-0.01*lay, 1.0, 0.0+0.01*lay,1));
        attr_cb_CTD_Straws->SetLineWidth(1); attr_cb_CTD_Straws->SetForceSolid(true);
	    cb_CTD_Straws_Logic->SetVisAttributes(attr_cb_CTD_Straws);
       }


    printf("cb_CTD_Straws   number of Straws =%d\n",counter );


#endif


#ifdef USE_GEMb
#ifdef USE_GEM
//===================================================================================
//                          E-ENDCAP                                               ==
//===================================================================================

//===================================================================================
//                           GEM tracker                                           ==
//===================================================================================
    printf("Begin ci_GEM_lay_\n");
    //ci_GEM_lay_Material = fMat->GetMaterial("Si");
    ci_GEM_lay_Material = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----

    for (int lay = 0; lay < 8; lay++) {

        ci_GEM_lay_RIn[lay] = ci_GEM_GVol_RIn + 1 * cm + (double(lay) * 0.5) * cm;
        ci_GEM_lay_ROut[lay] = ci_GEM_GVol_ROut - 25 * cm + (double(lay) * 2.) * cm;;

        //      ci_GEM_lay_PosZ[lay]=-ci_GEM_GVol_PosZ/2+(double(lay)*5.)*cm;
        ci_GEM_lay_PosZ[lay] = -ci_GEM_GVol_SizeZ / 2 + 5 * cm + (double(lay) * 3.) * cm;
        ci_GEM_lay_SizeZ[lay] = 1 * cm;

        sprintf(abname, "ci_GEM_lay_Solid_%d", lay);
        ci_GEM_lay_Solid[lay] = new G4Tubs(abname, ci_GEM_lay_RIn[lay], ci_GEM_lay_ROut[lay],
                                         ci_GEM_lay_SizeZ[lay] / 2., 0., 360 * deg);

        sprintf(abname, "ci_GEM_lay_Logic_%d", lay);
        ci_GEM_lay_Logic[lay] = new G4LogicalVolume(ci_GEM_lay_Solid[lay],
                                                  ci_GEM_lay_Material, abname);

        attr_ci_GEM_lay = new G4VisAttributes(G4Color(0.8, 0.4, 0.3, 0.8));
        attr_ci_GEM_lay->SetLineWidth(1);
        attr_ci_GEM_lay->SetForceSolid(true);
        ci_GEM_lay_Logic[lay]->SetVisAttributes(attr_ci_GEM_lay);

        sprintf(abname, "ci_GEM_lay_Phys_%d", lay);
        ci_GEM_lay_Phys[lay] = new G4PVPlacement(0, G4ThreeVector(0, 0, ci_GEM_lay_PosZ[lay]),
                                                  abname, ci_GEM_lay_Logic[lay],
                                                  ci_GEM_GVol_Phys, false, 0);

        if (ci_GEM_lay_Logic[lay]) ci_GEM_lay_Logic[lay]->SetSensitiveDetector(fCalorimeterSD);


    }

    printf("END ci_GEM_lay_\n");

//===================================================================================

    printf("Begin ce_GEM_lay_\n");
    //ce_GEM_lay_Material = fMat->GetMaterial("Si");
    ce_GEM_lay_Material = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----

    for (int lay = 0; lay < 8; lay++) {

        ce_GEM_lay_RIn[lay] = ce_GEM_GVol_RIn + 1 * cm + (double(lay) * 0.5) * cm;
        ce_GEM_lay_ROut[lay] = ce_GEM_GVol_ROut - 25 * cm + (double(lay) * 2.) * cm;;

        //      ce_GEM_lay_PosZ[lay]=-ce_GEM_GVol_PosZ/2+(double(lay)*5.)*cm;
        ce_GEM_lay_PosZ[lay] = ce_GEM_GVol_SizeZ / 2 - 5 * cm - (double(lay) * 3.) * cm;
        ce_GEM_lay_SizeZ[lay] = 1 * cm;

        sprintf(abname, "ce_GEM_lay_Solid_%d", lay);
        ce_GEM_lay_Solid[lay] = new G4Tubs(abname, ce_GEM_lay_RIn[lay], ce_GEM_lay_ROut[lay],
                                         ce_GEM_lay_SizeZ[lay] / 2., 0., 360 * deg);

        sprintf(abname, "ce_GEM_lay_Logic_%d", lay);
        ce_GEM_lay_Logic[lay] = new G4LogicalVolume(ce_GEM_lay_Solid[lay],
                                                  ce_GEM_lay_Material, abname);

        attr_ce_GEM_lay = new G4VisAttributes(G4Color(0.8, 0.4, 0.3, 0.8));
        attr_ce_GEM_lay->SetLineWidth(1);
        attr_ce_GEM_lay->SetForceSolid(true);
        ce_GEM_lay_Logic[lay]->SetVisAttributes(attr_ce_GEM_lay);

        sprintf(abname, "ce_GEM_lay_Phys_%d", lay);
        ce_GEM_lay_Phys[lay] = new G4PVPlacement(0, G4ThreeVector(0, 0, ce_GEM_lay_PosZ[lay]),
                                                  abname, ce_GEM_lay_Logic[lay],
                                                  ce_GEM_GVol_Phys, false, 0);

        if (ce_GEM_lay_Logic[lay]) ce_GEM_lay_Logic[lay]->SetSensitiveDetector(fCalorimeterSD);


    }

    printf("END ce_GEM_lay_ \n");


#endif
#endif

    //*********************************************************************
    //====================================================================================
    //==                          DETECTOR VOLUME  H-FORWARD                           ==
    //====================================================================================
    //*********************************************************************

    //====================================================================================
    //==                          DIPOLE-1 Tracker and EMCAL                            ==
    //====================================================================================

#ifdef USE_FI_DIPOLE1_A
    //-------------------------------------------------------------------------------
    //                      Place Si_disks inside D1a
    //-------------------------------------------------------------------------------
    int mydipole_id;

    for (int id = 0; id < 20; id++) {
        if (strcmp(fSolid_BigDi_ffqsNAME[id], "iBDS1a") == 0) {
            printf("found D21=%s  Z=%f dZ=%f Rout=%f \n", fSolid_BigDi_ffqsNAME[id], fSolid_BigDi_ffqsZ[id],
                   fSolid_BigDi_ffqsSizeZDi[id],
                   fSolid_BigDi_ffqsRinDi[id]);
            mydipole_id = id;
        };
    };
    fi_D1A_GVol_RIn = 0 * cm;
    fi_D1A_GVol_ROut = fSolid_BigDi_ffqsRinDi[mydipole_id] * cm;
    fi_D1A_GVol_SizeZ = 30 * cm;


    fi_D1A_GVol_Solid = new G4Tubs("fi_D1A_GVol_Solid", fi_D1A_GVol_RIn, fi_D1A_GVol_ROut,
                                    fi_D1A_GVol_SizeZ / 2., 0., 360 * deg);

    fi_D1A_GVol_Logic = new G4LogicalVolume(fi_D1A_GVol_Solid, World_Material, "fi_D1A_GVol_Logic");

    // ci_GEM_GVol_PosZ= Solenoid_SizeZ/2-abs(World_ShiftVTX)+ci_GEM_GVol_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed
    fi_D1A_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "fi_D1A_GVol_Phys", fi_D1A_GVol_Logic,
                                             fPhysics_BigDi_m[mydipole_id], false, 0);

    //  G4VisAttributes* vgemff= new G4VisAttributes(G4Color(0.8,0.4,0.3,0.8));
    attr_fi_D1A_GVol = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    attr_fi_D1A_GVol->SetLineWidth(1);
    attr_fi_D1A_GVol->SetForceSolid(true);
    fi_D1A_GVol_Logic->SetVisAttributes(attr_fi_D1A_GVol);

    // ---------------------------------------------------------------------------
    //                     D1 tracking  detectors
    // ---------------------------------------------------------------------------
    fi_D1A_lay_RIn = 5 * cm;
    fi_D1A_lay_ROut = fi_D1A_GVol_ROut - 5 * cm;
    fi_D1A_lay_SizeZ = 1 * cm;
    f1_D1A_NLAY=5;
    fi_D1A_lay_Material = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----

    f1_D1A_lay_Solid = new G4Tubs("f1_D1A_lay_Solid", fi_D1A_lay_RIn, fi_D1A_lay_ROut,
                                        fi_D1A_lay_SizeZ / 2., 170., 330 * deg);
    f1_D1A_Lay_Logic = new G4LogicalVolume(f1_D1A_lay_Solid, fi_D1A_lay_Material, "f1_D1A_lay_Logic");

    if (f1_D1A_Lay_Logic) f1_D1A_Lay_Logic->SetSensitiveDetector(fCalorimeterSD);


    int ffsi_counter = 0;
    for (int fflay = 0; fflay < f1_D1A_NLAY; fflay++) {
        double Z = -fi_D1A_GVol_SizeZ / 2 + (fflay + 1) * fi_D1A_lay_SizeZ / 2 + (fflay + 1) * 5 * cm;
        f1_D1A_lay_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, Z),
                                                     "f1_D1A_lay_Phys", f1_D1A_Lay_Logic,
                                                     fi_D1A_GVol_Phys, false, ffsi_counter);
        ffsi_counter++;
        attr_fi_D1A_lay = new G4VisAttributes(G4Color(0.8, 0.4 + 0.1 * fflay, 0.3, 1.));
        attr_fi_D1A_lay->SetLineWidth(1);
        attr_fi_D1A_lay->SetForceSolid(true);
        f1_D1A_Lay_Logic->SetVisAttributes(attr_fi_D1A_lay);
    }

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
    fSI_FORWDD1b_SizeRin = 0 * cm;
    fSI_FORWDD1b_SizeRout = fSolid_BigDi_ffqsRinDi[mydipole_id] * cm;
    fSI_FORWDD1b_SizeZ = fSolid_BigDi_ffqsSizeZDi[mydipole_id] * m;


    fSolid_SI_FORWDD1b = new G4Tubs("SI_FORWDD1b_solid", fSI_FORWDD1b_SizeRin, fSI_FORWDD1b_SizeRout,
                                    fSI_FORWDD1b_SizeZ / 2., 0., 360 * deg);

    fLogic_SI_FORWDD1b = new G4LogicalVolume(fSolid_SI_FORWDD1b, World_Material, "SI_FORWDD1b_logic");

    // ci_GEM_GVol_PosZ= Solenoid_SizeZ/2-abs(World_ShiftVTX)+ci_GEM_GVol_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed
    fPhysics_SI_FORWDD1b = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "SI_FORWDD1b_physics", fLogic_SI_FORWDD1b,
                                             fPhysics_BigDi_m[mydipole_id], false, 0);

    vtpc1 = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    vtpc1->SetLineWidth(1);
    vtpc1->SetForceSolid(true);
    fLogic_SI_FORWDD1b->SetVisAttributes(vtpc1);

    // ---------------------------------------------------------------------------
    //                     Si detectors
    // ---------------------------------------------------------------------------
    fSI_FORWDD1b_SizeRin_Lay = 5 * cm;
    fSI_FORWDD1b_SizeRout_Lay = fSI_FORWDD1b_SizeRout - 1 * cm;
    fSI_FORWDD1b_SizeZ_Lay = 1 * cm;

    fi_D1A_lay_Material = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----

    fSolid_SI_FORWDD1b_Lay = new G4Tubs("fSI_FORWDD1b_Lay_solid", fSI_FORWDD1b_SizeRin_Lay, fSI_FORWDD1b_SizeRout_Lay,
                                        fSI_FORWDD1b_SizeZ_Lay / 2., 170., 330 * deg);
    fLogic_SI_FORWDD1b_Lay = new G4LogicalVolume(fSolid_SI_FORWDD1b_Lay, fi_D1A_lay_Material, "fSI_FORWDD1b_Lay_logic");

    if (fLogic_SI_FORWDD1b_Lay) fLogic_SI_FORWDD1b_Lay->SetSensitiveDetector(fCalorimeterSD);


    ffsi_counter = 0;
    for (int fflay = 0; fflay < 5; fflay++) {
        double Z = -fSI_FORWDD1b_SizeZ / 2 + (fflay + 1) * fSI_FORWDD1b_SizeZ_Lay / 2 + (fflay + 1) * 5 * cm;
        fPhysics_SI_FORWDD1b_Lay = new G4PVPlacement(0, G4ThreeVector(0, 0, Z),
                                                     "fSI_FORWDD1b_Lay__phys", fLogic_SI_FORWDD1b_Lay,
                                                     fPhysics_SI_FORWDD1b, false, ffsi_counter);
        ffsi_counter++;
        G4VisAttributes *vsiff = new G4VisAttributes(G4Color(0.8, 0.4 + 0.1 * fflay, 0.3, 1.));
        vsiff->SetLineWidth(1);
        vsiff->SetForceSolid(true);
        fLogic_SI_FORWDD1b_Lay->SetVisAttributes(vsiff);
    }

#endif

#ifdef USE_CI_HCAL

    //------------------------------------------------------------------
    // Ecal module  AFTER !!!   Dipole1 
    //-------------------------------------------------------------------

    double fEcalLength_D1 = 40. * cm;
    double fEcalWidth_D1 = 4. * cm;
    G4double gap_D1 = 0.01 * mm;
    G4double angle_D1=0.05;
    G4RotationMatrix brm_emd1;
     brm_emd1.rotateY(angle_D1 * rad);
 

    // ce_EMCAL_det_Material= fMat->GetMaterial("DSBCe");
    ci_EMCAL_det_Material = fMat->GetMaterial("PbWO4");
    G4Box *fSolid_EMCAL_D1 = new G4Box("EMCAL_D1_sol", fEcalWidth_D1 * 0.5, fEcalWidth_D1 * 0.5, fEcalLength_D1 / 2.);
    G4LogicalVolume *fLogic_EMCAL_D1 = new G4LogicalVolume(fSolid_EMCAL_D1, ci_EMCAL_det_Material, "EMCAL_D1_log");

    attr_ce_EMCAL_detGLASS = new G4VisAttributes(G4Color(0.1, 1.0, 0.9, 0.5));
    attr_ce_EMCAL_detGLASS->SetLineWidth(1);
    attr_ce_EMCAL_detGLASS->SetForceSolid(true);
    fLogic_EMCAL_D1->SetVisAttributes(attr_ce_EMCAL_detGLASS);

    // Crystals

    x0 =0*cm;
    y0 = 0 * cm;
    double R0 = 7. * cm;
    y_C = 0;
    x_C= 0*cm;
    k = -1;
    float x_C_shift= -35.*cm;
    //    z = fSI_FORWDD1b_SizeZ / 2 - fEcalLength_D1 / 2;
    z = ci_HCAL_GVol_SizeZ/ 2 - fEcalLength_D1 / 2;
//============  For sectors =====
    for (j = 0; j < 50; j++) {
        y_C -= ce_EMCAL_detPWO_Width + gap_D1;
        x_C = (ce_EMCAL_detPWO_Width + gap_D1) * 0.5 ;

        for (i = 0; i < 50; i++) {
            float R = sqrt(x_C * x_C + y_C * y_C);

            // printf("found D1b= EMCAL D1::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, R0);


            if (R < fSI_FORWDD1b_SizeRout - ce_EMCAL_detPWO_Width + gap_D1 && R > R0) {
                //  printf("found D1b= EMCAL D1::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, R0);


                k++;
                sprintf(abname, "EMCAL_D1_%d", k);

		//               new G4PVPlacement(0, G4ThreeVector(x_C, y_C, z), abname, fLogic_EMCAL_D1,
		//                fPhysics_SI_FORWDD1b, false, k);
                new G4PVPlacement(G4Transform3D(brm_emd1, G4ThreeVector(x_C+x_C_shift, y_C, z)), abname, fLogic_EMCAL_D1,
                                  ci_HCAL_GVol_Phys, false, k);

                k++;
                sprintf(abname, "EMCAL_D1_%d", k);
		new G4PVPlacement(G4Transform3D(brm_emd1, G4ThreeVector(-x_C+x_C_shift, y_C, z)), abname, fLogic_EMCAL_D1,
                                  ci_HCAL_GVol_Phys, false, k);

                k++;
                sprintf(abname, "EMCAL_D1_%d", k);
		 new G4PVPlacement(G4Transform3D(brm_emd1, G4ThreeVector(x_C+x_C_shift, -y_C, z)), abname, fLogic_EMCAL_D1,
                                  ci_HCAL_GVol_Phys, false, k);

                k++;
                sprintf(abname, "EMCAL_D1_%d", k);
		new G4PVPlacement(G4Transform3D(brm_emd1, G4ThreeVector(-x_C+x_C_shift, -y_C, z)), abname, fLogic_EMCAL_D1,
                                  ci_HCAL_GVol_Phys, false, k);
            }
            x_C += ce_EMCAL_detPWO_Width + gap_D1;

        }
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
            printf("found D1b=%s  Z=%f dZ=%f Rout=%f \n", fSolid_BigDi_ffqsNAME[id], fSolid_BigDi_ffqsZ[id],
                   fSolid_BigDi_ffqsSizeZDi[id],
                   fSolid_BigDi_ffqsRinDi[id]);
            mydipole_id = id;
        };
    };
    fSI_FORWDD2_SizeRin = 0 * cm;
    fSI_FORWDD2_SizeRout = fSolid_BigDi_ffqsRinDi[mydipole_id] * cm;
    fSI_FORWDD2_SizeZ = fSolid_BigDi_ffqsSizeZDi[mydipole_id] * m;


    fSolid_SI_FORWDD2 = new G4Tubs("SI_FORWDD2_solid", fSI_FORWDD2_SizeRin, fSI_FORWDD2_SizeRout,
                                   fSI_FORWDD2_SizeZ / 2., 0., 360 * deg);

    fLogic_SI_FORWDD2 = new G4LogicalVolume(fSolid_SI_FORWDD2, World_Material, "SI_FORWDD2_logic");

    // ci_GEM_GVol_PosZ= Solenoid_SizeZ/2-abs(World_ShiftVTX)+ci_GEM_GVol_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed
    fPhysics_SI_FORWDD2 = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "SI_FORWDD2_physics", fLogic_SI_FORWDD2,
                                            fPhysics_BigDi_m[mydipole_id], false, 0);

    vtpc1 = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    vtpc1->SetLineWidth(1);
    vtpc1->SetForceSolid(true);
    fLogic_SI_FORWDD2->SetVisAttributes(vtpc1);

    // ---------------------------------------------------------------------------
    //                     Si detectors
    // ---------------------------------------------------------------------------
    fSI_FORWDD2_SizeRin_Lay = 0 * cm;
    fSI_FORWDD2_SizeRout_Lay = fSI_FORWDD2_SizeRout - 1 * cm;
    fSI_FORWDD2_SizeZ_Lay = 1 * cm;

    ce_GEM_lay_Material = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----

    fSolid_SI_FORWDD2_Lay = new G4Tubs("fSI_FORWDD2_Lay_solid", fSI_FORWDD2_SizeRin_Lay, fSI_FORWDD2_SizeRout_Lay,
                                       fSI_FORWDD2_SizeZ_Lay / 2., 0., 360 * deg);
    fLogic_SI_FORWDD2_Lay = new G4LogicalVolume(fSolid_SI_FORWDD2_Lay, fi_D1A_lay_Material, "fSI_FORWDD2_Lay_logic");

    if (fLogic_SI_FORWDD2_Lay) fLogic_SI_FORWDD2_Lay->SetSensitiveDetector(fCalorimeterSD);


    ffsi_counter = 0;
    //------------------ in front ------
    for (int fflay = 0; fflay < 5; fflay++) {
        double Z = -fSI_FORWDD2_SizeZ / 2 + (fflay + 1) * fSI_FORWDD2_SizeZ_Lay / 2 + (fflay + 1) * 5 * cm;
        fPhysics_SI_FORWDD2_Lay = new G4PVPlacement(0, G4ThreeVector(0, 0, Z),
                                                    "fSI_FORWDD2_Lay_phys", fLogic_SI_FORWDD2_Lay, fPhysics_SI_FORWDD2,
                                                    false, ffsi_counter);
        ffsi_counter++;
        G4VisAttributes *vsiff = new G4VisAttributes(G4Color(0.8, 0.4 + 0.1 * fflay, 0.3, 1.));
        vsiff->SetLineWidth(1);
        vsiff->SetForceSolid(true);
        fLogic_SI_FORWDD2_Lay->SetVisAttributes(vsiff);
    }

    //------------------ in middle  ------
    for (int fflay = 0; fflay < 5; fflay++) {
        double Z = (fflay + 1) * fSI_FORWDD2_SizeZ_Lay / 2 + (fflay + 1) * 5 * cm;
        fPhysics_SI_FORWDD2_Lay = new G4PVPlacement(0, G4ThreeVector(0, 0, Z),
                                                    "fSI_FORWDD2_Lay_phys", fLogic_SI_FORWDD2_Lay, fPhysics_SI_FORWDD2,
                                                    false, ffsi_counter);
        ffsi_counter++;
        G4VisAttributes *vsiff = new G4VisAttributes(G4Color(0.8, 0.4 + 0.1 * fflay, 0.3, 1.));
        vsiff->SetLineWidth(1);
        vsiff->SetForceSolid(true);
        fLogic_SI_FORWDD2_Lay->SetVisAttributes(vsiff);
    }
#endif
    //====================================================================================
    //==                          GEM DETECTOR VOLUME  after  D2                      ==
    //====================================================================================

#ifdef USE_FARFORWARD_GEM

    fGEM_FARFORWD_SizeRin = 0 * cm;
    fGEM_FARFORWD_SizeRout = 90 * cm;
    fGEM_FARFORWD_SizeZ = 30 * cm;
    //double fGEM_FARFORWD_Zshift=0*cm;
    // fGEM_FARFORWD_Z= 4000*cm;

    for (int id = 0; id < 20; id++) {
        if (strcmp(fSolid_BigDi_ffqsNAME[id], "iBDS2") ==
            0) { //printf("found D2 =%s  Z=%f dZ=%f \n",fSolid_BigDi_ffqsNAME[id],fSolid_BigDi_ffqsZ[id], fSolid_BigDi_ffqsSizeZDi[id]);
            fGEM_FARFORWD_Z =
                    fSolid_BigDi_ffqsZ[id] * m + fSolid_BigDi_ffqsSizeZDi[id] / 2 * m + fGEM_FARFORWD_SizeZ / 2 +
                    10 * cm;
            fGEM_FARFORWD_X = fSolid_BigDi_ffqsX[id] * m - 10 * cm;
            //  printf("found D2 =%f \n", fGEM_FARFORWD_Z);
        };
    };


    fSolidGEM_FARFORWD = new G4Tubs("GEM_FARFORWD_solid", fGEM_FARFORWD_SizeRin, fGEM_FARFORWD_SizeRout,
                                    fGEM_FARFORWD_SizeZ / 2., 0., 360 * deg);

    fLogicGEM_FARFORWD = new G4LogicalVolume(fSolidGEM_FARFORWD, World_Material, "GEM_FARFORWD_logic");

    // ci_GEM_GVol_PosZ= Solenoid_SizeZ/2-abs(World_ShiftVTX)+ci_GEM_GVol_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed
    fPhysicsGEM_FARFORWD = new G4PVPlacement(0, G4ThreeVector(fGEM_FARFORWD_X, 0, fGEM_FARFORWD_Z),
                                             "GEM_FARFORWD_physics", fLogicGEM_FARFORWD,
                                             World_Phys, false, 0);
    vtpc1 = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
    vtpc1->SetLineWidth(1);
    vtpc1->SetForceSolid(true);
    fLogicGEM_FARFORWD->SetVisAttributes(vtpc1);
    // ---------------------------------------------------------------------------
    //                     GEM detectors
    // ---------------------------------------------------------------------------
    fGEM_FARFORWD_Lay_SizeRin = 0 * cm;
    fGEM_FARFORWD_Lay_SizeRout = 45 * cm;
    fGEM_FARFORWD_Lay_SizeZ = 1 * cm;
    ce_GEM_lay_Material = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----

    fSolidGEM_FARFORWD_Lay = new G4Tubs("GEM_FARFORWD_Lay_solid", fGEM_FARFORWD_Lay_SizeRin, fGEM_FARFORWD_Lay_SizeRout,
                                        fGEM_FARFORWD_Lay_SizeZ / 2., 0., 360 * deg);
    fLogicGEM_FARFORWD_Lay = new G4LogicalVolume(fSolidGEM_FARFORWD_Lay, fi_D1A_lay_Material, "GEM_FARFORWD_Lay_logic");

    if (fLogicGEM_FARFORWD_Lay) fLogicGEM_FARFORWD_Lay->SetSensitiveDetector(fCalorimeterSD);

    G4VisAttributes *vgemff = new G4VisAttributes(G4Color(0.8, 0.4, 0.3, 0.8));
    vgemff->SetLineWidth(1);
    vgemff->SetForceSolid(true);
    fLogicGEM_FARFORWD_Lay->SetVisAttributes(vgemff);

    //---------------------------- after D2-----------------------
    int ff_counter = 0;
    for (int fflay = 0; fflay < 5; fflay++) {
        double Z = -fGEM_FARFORWD_SizeZ / 2 + (fflay + 1) * fGEM_FARFORWD_Lay_SizeZ / 2 + (fflay + 1) * 5 * cm;
        fPhysicsGEM_FARFORWD_Lay = new G4PVPlacement(0, G4ThreeVector(0, 0, Z),
                                                     "GEM_FARFORWD_Lay_phys", fLogicGEM_FARFORWD_Lay,
                                                     fPhysicsGEM_FARFORWD, false, ff_counter);
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
    gGeoManager->CloseGeometry();
    gGeoManager->Export("geometryJLEIC.root");
    //
    // end VGM demo
    //---------------------------------------------------------------------------
#endif


    printf("exit Detector Construction\n");


    return World_Phys;
}



//==========================================================================================================
//                                      JLEIC   END
//==========================================================================================================



G4VPhysicalVolume *JLeicDetectorConstruction::SetUpJLEIC09() {
}



////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::PrintGeometryParameters() {
    G4cout << "\n The  WORLD   is made of "
           << World_SizeZ / mm << "mm of " << World_Material->GetName();
    G4cout << ", the transverse size (R) of the world is " << World_SizeR / mm << " mm. " << G4endl;
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
            fRadiatorMat = pttoMaterial;
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
    fAbsorberThickness = val;
    //  ComputeCalorParameters();
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetRadiatorThickness(G4double val) {
    // change XTR radiator thickness and recompute the calorimeter parameters
    fRadThickness = val;
    // ComputeCalorParameters();
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetGasGapThickness(G4double val) {
    // change XTR gas gap thickness and recompute the calorimeter parameters
    fGasGap = val;
    // ComputeCalorParameters();
}

/////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetAbsorberRadius(G4double val) {
    // change the transverse size and recompute the calorimeter parameters
    fAbsorberRadius = val;
    // ComputeCalorParameters();
}

////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetWorldSizeZ(G4double val) {
    fWorldChanged = true;
    World_SizeZ = val;
    // ComputeCalorParameters();
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetWorldSizeR(G4double val) {
    fWorldChanged = true;
    World_SizeR = val;
    // ComputeCalorParameters();
}

//////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetAbsorberZpos(G4double val) {
    fAbsorberZ = val;
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
    sprintf(fname, "i_ir.txt");
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
    fSolid_QUADS_hd_m[j] = new G4Tubs(abname, 0. * cm, ffqsRoutDi * cm, (ffqsSizeZDi / 2.) * m, 0., 360 * deg);
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
    G4double fieldStrength = qFIELSol * tesla;  // 0.01*tesla; // field strength in pipe
    G4double alphaB = 0. * degree;
    fMagField_ASOLENOID[j] = new G4UniformMagField(G4ThreeVector(fieldStrength * std::sin(alphaB),
                                                                 0., fieldStrength * std::cos(alphaB)));

    fieldMgrAS[j] = new G4FieldManager(fMagField_ASOLENOID[j]);
    fieldMgrAS[j]->SetDetectorField(fMagField_ASOLENOID[j]);
    fieldMgrAS[j]->CreateChordFinder(fMagField_ASOLENOID[j]);
    fLogic_ASOLENOID_hd_m[j]->SetFieldManager(fieldMgrAS[j], true);


}







