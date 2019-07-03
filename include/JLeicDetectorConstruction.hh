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
// $Id: JLeicDetectorConstruction.hh,v 1.19 2006-06-29 16:37:46 gunter Exp $
// GEANT4 tag $Name: geant4-09-04-patch-01 $
//
// 

#ifndef JLeicDetectorConstruction_h
#define JLeicDetectorConstruction_h 1

#include "globals.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4ios.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Polycone.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4Region.hh"
#include "G4UniformMagField.hh"

//-- Quadrupole --
#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4QuadrupoleMagField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ClassicalRK4.hh"
#include "G4FieldManager.hh"


#include "JLeicMaterials.hh"
#include "JLeicDetectorMessenger.hh"
#include "JLeicDetectorConfig.hh"

//--------------CB---------------
#include "cb_Solenoid/cb_Solenoid.hh"   // Central Barrel - Solenoid
#include "cb_VTX/cb_VTX.hh"             // Central Barrel - Vertex
#include "cb_CTD/cb_CTD.hh"             // Central Barrel - Tracker
#include "cb_DIRC/cb_DIRC.hh"           // Central Barrel - DIRC
#include "cb_EMCAL/cb_EMCAL.hh"         // Central Barrel - EMCAL
#include "cb_HCAL/cb_HCAL.hh"           // Central Barrel - HCAL

//--------------CE---------------
#include "ce_GEM/ce_GEM.hh"             // Central Ion Endcap - TRD
#include "ce_MRICH/ce_MRICH.hh"         // Central Electron Endcap - MRICH
#include "ce_EMCAL/ce_EMCAL.hh"         // Central Electron Endcap - EMCAL

//--------------FFe---------------
#include "ffe_CPOL/ffe_CPOL.hh"         //  Far-forward electron direction ePolarimeter
//--------------CI---------------
#include "ci_GEM/ci_GEM.hh"             // Central Ion Endcap - TRD
#include "ci_DRICH/ci_DRICH.hh"         // Central Ion Endcap - DRICH
#include "ci_TRD/ci_TRD.hh"             // Central Ion Endcap - TRD
#include "ci_EMCAL/ci_EMCAL.hh"         // Central Ion Endcap - EMCAL
#include "ci_HCAL/ci_HCAL.hh"           // Central Ion Endcap - HCAL
//--------------FFi---------------
#include "fi_TRKD1/fi_TRKD1.hh"         // Far-forward Ion D1  - Tracking
#include "fi_EMCAL/fi_EMCAL.hh"         // Far-forward Ion D1  - EMCAL

#include "ffi_ZDC/ffi_ZDC.hh"         // Far-forward Ion   - HCAL ZDC


class JLeicCalorimeterSD;

class JLeicDetectorConstruction : public G4VUserDetectorConstruction {
public:

    JLeicDetectorConstruction();

    ~JLeicDetectorConstruction();

public:

    void SetAbsorberMaterial(G4String);

    void SetAbsorberThickness(G4double);

    void SetAbsorberRadius(G4double);

    void SetAbsorberZpos(G4double);

    void SetRadiatorMaterial(G4String);

    void SetRadiatorThickness(G4double);

    void SetGasGapThickness(G4double);

 //   void SetFoilNumber(G4int i) { fFoilNumber = i; };

    void SetWorldMaterial(G4String);

    void SetWorldSizeZ(G4double);

    void SetWorldSizeR(G4double);

    void SetDetectorSetUp(G4String setUp) { fSetUp = setUp; };


    void SetMagField(G4double);

    G4VPhysicalVolume *Construct();

    void UpdateGeometry();

    void Read_Di_File();

    void Read_dE_File();


    void CreateQuad(int i, char *MyChar, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi, float ffqsRoutDi,
                    float qFIELDx, float qFIELDy, float qFIELQn, float qFIELQs, float qFIELSek, float qFIELSol,
                    float ffqsX, float ffqsY, float ffqsZ, float ffqsTheta, float ffqsPhi);

    void CreateDipole(int i, char *MyChar, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi, float ffqsRoutDi,
                      float qFIELDx, float qFIELDy, float qFIELQn, float qFIELQs, float qFIELSek, float qFIELSol,
                      float ffqsX, float ffqsY, float ffqsZ, float ffqsTheta, float ffqsPhi);

    void
    CreateDipoleChicane(int i, char *MyChar, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi, float ffqsRoutDi,
                        float qFIELDx, float qFIELDy, float qFIELQn, float qFIELQs, float qFIELSek, float qFIELSol,
                        float ffqsX, float ffqsY, float ffqsZ, float ffqsTheta, float ffqsPhi);

    void CreateASolenoid(int i, char *MyChar, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi, float ffqsRoutDi,
                         float qFIELDx, float qFIELDy, float qFIELQn, float qFIELQs, float qFIELSek, float qFIELSol,
                         float ffqsX, float ffqsY, float ffqsZ, float ffqsTheta, float ffqsPhi);

    int iq = 0, ik = 0, is = 0;


public:

    void PrintGeometryParameters();

    G4Material *GetWorldMaterial() { return World_Material; };

    G4double GetWorldSizeZ() { return fConfig.World.SizeZ; };

    G4double GetWorldSizeR() { return fConfig.World.SizeR; };


    const G4VPhysicalVolume *GetphysiWorld() { return World_Phys; };

    const G4VPhysicalVolume *GetAbsorber() { return fPhysicsAbsorber; };

    G4LogicalVolume *GetLogicalAbsorber() { return fLogicAbsorber; };
    double  GetAbsorberThickness(){ return fConfig.ci_TRD.fAbsorberThickness;};
    G4Material *GetAbsorberMaterial(){ return fConfig.ci_TRD.det_Material;};

   //TRD- related ---- needs to be moved move

   G4LogicalVolume *GetLogicalRadiator() { return ci_TRD.fLogicRadiator; };
   G4Material *GetFoilMaterial() { return ci_TRD.fFoilMat; };
   G4Material *GetGasMaterial() { return ci_TRD.fGasMat; };
   G4double GetFoilThick() { return fConfig.ci_TRD.fRadThickness; };
   G4double GetGasThick() {  return fConfig.ci_TRD.fGasGap; };

   G4int   GetFoilNumber() {
       std::cout << " foil number4 = " << ci_TRD.ConstructionConfig.fFoilNumber << std::endl;
       return ci_TRD.ConstructionConfig.fFoilNumber;}

    G4String rootFileName;
    G4double fadc_slice;
    G4int NannVAR;
    //---  fsv move to public ---
    G4String fSetUp;
    G4Material *fAbsorberMaterial;

    G4int fModuleNumber;   // the number of Rad-et modules

    JLeicDetectorConfig fConfig;

private:

    G4VPhysicalVolume *ConstructDetectorXTR();

    G4FieldManager *SetQMagField(float field, float angle, float theta, G4ThreeVector fieldorigin);

    // G4FieldManager* SetQMagField(G4double field_x);
    G4FieldManager *SetDipoleMagField(G4double fx, G4double fy, G4double fz, float theta);



    G4VPhysicalVolume *SetUpJLEIC2019();

    G4VPhysicalVolume *SetUpJLEIC09();

    void Create_ci_Endcap(JLeicDetectorConfig::ci_Endcap_Config cfg);

    void Create_ce_Endcap(JLeicDetectorConfig::ce_Endcap_Config dfg);

private:

// ----------------------------------------------
//           R E F A C T O R I N G


     //=========================================================================
    //--------------World -------------------
    // =========================================================================
    G4Box *World_Solid;    //pointer to the solid World
    G4LogicalVolume *World_Logic;    //pointer to the logical World
    G4VPhysicalVolume *World_Phys;    //pointer to the physical World
    G4Material *World_Material;

//-----------------Hadron ENDCAP volume--------------------
    G4VisAttributes *attr_ci_ENDCAP_GVol;
    G4Tubs *ci_ENDCAP_GVol_Solid;    //pointer to the solid  ENDCAP-H volume
    G4LogicalVolume *ci_ENDCAP_GVol_Logic;    //pointer to the logical  ENDCAP-H  volume
    G4VPhysicalVolume *ci_ENDCAP_GVol_Phys;    //pointer to the physical ENDCAP-H  volume

//-----------------Electron ENDCAP  volume--------------------
    G4VisAttributes *attr_ce_ENDCAP_GVol;
    G4Tubs *ce_ENDCAP_GVol_Solid;    //pointer to the solid  ENDCAP-E volume
    G4LogicalVolume *ce_ENDCAP_GVol_Logic;    //pointer to the logical ENDCAP-E  volume
    G4VPhysicalVolume *ce_ENDCAP_GVol_Phys;    //pointer to the physical ENDCAP-E  volume
    //==============================================
    //----------------BARREL -----------------------
    cb_Solenoid_Design cb_Solenoid;
    //----------------VTX  volume ------------------
    cb_VTX_Design      cb_VTX;
    //----------------CTD  volume -------------------
    cb_CTD_Design      cb_CTD;
    //----------------DIRC  volume -------------------
    cb_DIRC_Design     cb_DIRC;
    //----------------EMCAL  volume -------------------
    cb_EMCAL_Design   cb_EMCAL;
    //----------------HCAL  volume -------------------
    cb_HCAL_Design     cb_HCAL;
    //==============================================
    //----------------E-ENDCAP -----------------------
    //----------------GEM volume ---------------------
    ce_GEM_Design     ce_GEM;
    //----------------MRICH volume -----------------------
    ce_MRICH_Design     ce_MRICH;
    //----------------EMCAL volume -----------------------
    ce_EMCAL_Design     ce_EMCAL;

    //==============================================
    //----------------Far-forward Electron  ---------

    //----------------FFe  CPOL volume --------------
    ffe_CPOL_Design    ffe_CPOL;
    //==============================================
    //----------------Ion-ENDCAP -----------------------
    //----------------GEM volume ---------------------
    ci_GEM_Design     ci_GEM;
    //----------------DRICH volume ---------------------
    ci_DRICH_Design     ci_DRICH;
    //----------------TRD volume ---------------------
    ci_TRD_Design       ci_TRD;
    //----------------EMCAL volume ---------------------
    ci_EMCAL_Design     ci_EMCAL;
    //----------------HCAL volume ---------------------
    ci_HCAL_Design      ci_HCAL;

    //==============================================
    //----------------Far-forward ION  --------------
    //----------------Tracking volume ---------------------
    fi_TRKD1_Design     fi_TRKD1;
    //----------------EMCAL volume ---------------------
    fi_EMCAL_Design     fi_EMCAL;
    //---------------HCAL -ZDC -------------------------
    ffi_ZDC_Design  ffi_ZDC;

// ----------------------------------------------



    G4bool fWorldChanged;

    G4Material *fPipeMat;

    G4Material *fWindowMat;
    G4double fWindowThick;

    G4Material *fElectrodeMat;
    G4double fElectrodeThick;

    G4Material *fGapMat;
    G4double fGapThick;


     // =========================================================================


    //  G4Polycone*        ce_HCAL_GVol_Solid;    //pointer to the solid ECAP_HCAL volume

    G4Tubs *ce_HCAL_GVol_Solid;    //pointer to the solid ECAP_HCAL volume
    G4LogicalVolume *ce_HCAL_GVol_Logic;    //pointer to the logicalECAP_HCAL  volume
    G4VPhysicalVolume *ce_HCAL_GVol_Phys;    //pointer to the physical EMCAL barrel volume

    G4Tubs *ce_HCAL_det_Solid;    //pointer to the solid ECAP_HCAL volume
    G4LogicalVolume *ce_HCAL_det_Logic;    //pointer to the logicalECAP_HCAL  volume
    G4VPhysicalVolume *ce_HCAL_det_Phys;    //pointer to the physical EMCAL barrel volume

    // =========================================================================
    //------------- ENDCAP-E EMCAL -------------------

    // =========================================================================




    //===========================================================
    //===========================================================
    //-------------------------------------
    //-----------------FARFORWARD  AREA-------------------
    //===========================================================
    //===========================================================
   //-----------------FORWARD D1  volume--------------------

    // ----- B----
    G4double fi_D1B_GVol_RIn;
    G4double fi_D1B_GVol_ROut;
    G4double fi_D1B_GVol_SizeZ;
    G4Material *fi_D1B_GVol_Material;
    G4VisAttributes* attr_fi_D1B_GVol;

    G4Tubs *fi_D1B_GVol_Solid;    //pointer to the solid  FARFORWD
    G4LogicalVolume *fi_D1B_GVol_Logic;    //pointer to the logical FARFORWD
    G4VPhysicalVolume *fi_D1B_GVol_Phys;    //pointer to the physical FARFORWD

    G4double fi_D1B_lay_RIn;
    G4double fi_D1B_lay_ROut;
    G4double fi_D1B_lay_SizeZ;
    G4Material *fi_D1B_lay_Material;
    G4VisAttributes* attr_fi_D1B_lay;

    G4Tubs *fi_D1B_lay_Solid;    //pointer to the solid  FARFORWD
    G4LogicalVolume *fi_D1B_lay_Logic;    //pointer to the logical FARFORWD
    G4VPhysicalVolume *fi_D1B_lay_Phys;    //pointer to the physical FARFORWD



    //-----------------FORWARD TRD inside D2  volume--------------------
    //
    G4double ffi_D2_GVol_RIn;
    G4double ffi_D2_GVol_ROut;
    G4double ffi_D2_GVol_SizeZ;
    G4VisAttributes* attr_ffi_D2_GVol;
    G4Tubs *ffi_D2_GVol_Solid;    //pointer to the solid  FARFORWD
    G4LogicalVolume *ffi_D2_GVol_Logic;    //pointer to the logical FARFORWD
    G4VPhysicalVolume *ffi_D2_GVol_Phys;    //pointer to the physical FARFORWD
    //-------------- Si layers---------------
    G4double ffi_D2_TRK_Lay_RIn;
    G4double ffi_D2_TRK_Lay_ROut;
    G4double ffi_D2_TRK_Lay_SizeZ;
    G4Material *ffi_D2_TRK_Lay_Material;
    G4VisAttributes *attr_ffi_D2_TRK_Lay;
    G4Tubs *ffi_D2_TRK_Lay_Solid;    //pointer to the solid  FARFORWD
    G4LogicalVolume *ffi_D2_TRK_Lay_Logic;    //pointer to the logical FARFORWD
    G4VPhysicalVolume *ffi_D2_TRK_Lay_Phys;    //pointer to the physical FARFORWD


    //===========================================================
    //===========================================================
    //-------------------------------------
    //-----------------FARFORWARD GEM  volume--------------------
    //===========================================================
    //===========================================================

    G4double ffi_D2AFTER_GVol_RIn;
    G4double ffi_D2AFTER_GVol_ROut;
    G4double ffi_D2AFTER_GVol_SizeZ;
    G4double ffi_D2AFTER_GVol_PosZ, ffi_D2AFTER_GVol_PosX;
    G4VisAttributes *attr_ffi_D2AFTER_GVol;
    G4Tubs *ffi_D2AFTER_GVol_Solid;    //pointer to the solid  FARFORWD
    G4LogicalVolume *ffi_D2AFTER_GVol_Logic;    //pointer to the logical FARFORWD
    G4VPhysicalVolume *ffi_D2AFTER_GVol_Phys;    //pointer to the physical FARFORWD


    G4double ffi_D2AFTER_TRK_Lay_RIn;
    G4double ffi_D2AFTER_TRK_Lay_ROut;
    G4double ffi_D2AFTER_TRK_Lay_SizeZ;

    G4Material *ffi_D2AFTER_TRK_Lay_Material;
    G4VisAttributes *attr_ffi_D2AFTER_TRK_Lay;

    G4Tubs *ffi_D2AFTER_TRK_Lay_Solid;    //pointer to the solid  FARFORWD lay
    G4LogicalVolume *ffi_D2AFTER_TRK_Lay_Logic;    //pointer to the logical FARFORWD  lay
    G4VPhysicalVolume *ffi_D2AFTER_TRK_Lay_Phys;    //pointer to the physical FARFORWD  lay



    //-----------------FARFORWARD VIRTUAL VOLUMES--------------------
    G4VisAttributes *vvpf1;


    G4double fFARFORWARD_VP_Rout, fFARFORWARD_VP_angle;
    G4double fFARFORWARD_VP_SizeZ;
    G4double fFARFORWARD_VP_X, fFARFORWARD_VP_Z;

    G4Tubs *fSolid_FARFORWARD_VP;    //pointer to the solid  FARFORWD
    G4LogicalVolume *fLogic_FARFORWARD_VP;    //pointer to the logical FARFORWD
    G4VPhysicalVolume *fPhysics_FARFORWARD_VP;    //pointer to the physical FARFORWD



    //-------------------TPC barrel detector ------------------
    G4double fTPCSizeRin;
    G4double fTPCSizeRout;
    G4double fTPCSizeZ;
    G4double fTPCsteps;
    G4Material *fTPCMaterial;

    G4double TPCRin[20];
    G4double TPCRout[20];
    G4Tubs *fSolidTPCBarrel[20];    //pointer to the solid World
    G4LogicalVolume *fLogicTPCBarrel[20];    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsTPCBarrel[20];    //pointer to the physical World

    //--------------------------------------------------------------


    //----------------BEAM  elements ----------------------------------------------
    /*
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

    G4Material *ffqsMaterial;
    G4VisAttributes *vb1, *vb1a, *vb1b, *vb1as;
    G4Material *ffqsMaterial_G;
    double fZ1;
    //---------------- BEAM QUADS -------------------------------------------
    G4Tubs *fSolid_QUADS_hd_v[100], *fSolid_QUADS_hd_ir[100], *fSolid_QUADS_hd_m[100];
    G4LogicalVolume *fLogic_QUADS_hd_v[100], *fLogic_QUADS_hd_ir[100], *fLogic_QUADS_hd_m[100];
    G4PVPlacement *fPhysics_QUADS_hd_v[100], *fPhysics_QUADS_hd_ir[100], *fPhysics_QUADS_hd_m[100];
    G4FieldManager *fieldMgr_QUADS_hd[100];
    G4RotationMatrix brm_hd[100];

    char fSolid_ffqsNAME[100][256];
    float fSolid_ffqsSizeZ[100], fSolid_ffqsRin[100], fSolid_ffqsRout[100];
    float fSolid_ffqsX[100], fSolid_ffqsY[100], fSolid_ffqsZ[100];


//---------------- BEAM ASOLENOID -------------------------------------------
    G4Tubs *fSolid_ASOLENOID_hd_v[40], *fSolid_ASOLENOID_hd_ir[40], *fSolid_ASOLENOID_hd_m[40];
    G4LogicalVolume *fLogic_ASOLENOID_hd_v[40], *fLogic_ASOLENOID_hd_ir[40], *fLogic_ASOLENOID_hd_m[40];
    G4PVPlacement *fPhysics_ASOLENOID_hd_v[40], *fPhysics_ASOLENOID_hd_ir[40], *fPhysics_ASOLENOID_hd_m[40];
    G4FieldManager *fieldMgr_ASOLENOID_hd[40];
    G4UniformMagField *fMagField_ASOLENOID[40];
    //  G4UniformMagField* fMagField_ASOLENOID[40];      //pointer to the magnetic field
    G4FieldManager *fieldMgrAS[40];
    G4RotationMatrix brm_as[40];

    //---------------- BEAM KICKERS/DIPOLES -------------------------------------------
    G4Tubs *fSolid_BigDi_v[20], *fSolid_BigDi_ir[20], *fSolid_BigDi_m[20];
    //  G4Box *fSolid_BigDi_v[20],*fSolid_BigDi_ir[20],*fSolid_BigDi_m[20];
    G4LogicalVolume *fLogic_BigDi_v[20], *fLogic_BigDi_ir[20], *fLogic_BigDi_m[20];
    G4PVPlacement *fPhysics_BigDi_v[20], *fPhysics_BigDi_ir[20], *fPhysics_BigDi_m[20];
    G4RotationMatrix brmBigDi[20];
    G4FieldManager *fieldMgr_BigDi[20];
    float fSolid_BigDi_ffqsX[20], fSolid_BigDi_ffqsY[20], fSolid_BigDi_ffqsZ[20];
    float fSolid_BigDi_ffqsSizeZDi[20], fSolid_BigDi_ffqsRinDi[20], fSolid_BigDi_ffqsRoutDi[20];
    char fSolid_BigDi_ffqsNAME[20][256];
    //----------------DIPOLES CHICANE -------------------------------------------
    G4Box *fSolid_Chicane_v[20], *fSolid_Chicane_ir[20], *fSolid_Chicane_m[20];
    G4LogicalVolume *fLogic_Chicane_v[20], *fLogic_Chicane_ir[20], *fLogic_Chicane_m[20];
    G4PVPlacement *fPhysics_Chicane_v[20], *fPhysics_Chicane_ir[20], *fPhysics_Chicane_m[20];
    G4RotationMatrix brmChicane[20];
    G4FieldManager *fieldMgr_Chicane[20];


    G4RotationMatrix brm_hk[20];


    //----------------BEAMPIPE elements ----------------------------------------------
    G4Material *fBeampipeVacuum;

    // ------------inner Be beampipe ------------
    G4double fBeamSizeRinBe;
    G4double fBeamSizeRoutBe;
    G4double fBeamSizeZ_Be;
    G4Material *fBeamPipeMaterialBe;

    //------------Volume -----------
    G4Tubs *fSolidBeamBe;    //pointer to the solid World
    G4LogicalVolume *fLogicBeamBe;    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsBeamBe;    //pointer to the physical World

    //------------Wall -----------
    G4Tubs *fSolidBeamBe1;    //pointer to the solid World
    G4LogicalVolume *fLogicBeamBe1;    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsBeamBe1;    //pointer to the physical World
    //------------Vacuum-----------
    G4Tubs *fSolidBeamBe2;    //pointer to the solid World
    G4LogicalVolume *fLogicBeamBe2;    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsBeamBe2;    //pointer to the physical World

    //-----------------------------------------------
    // ------------HAdron beampipe ------------
    G4double fBeamSizeRinH;
    G4double fBeamSizeRoutH;
    G4double fBeamSizeZ_H;
    G4Material *fBeamPipeMaterialH;

    //------------Volume -----------
    G4Tubs *fSolidBeamH;    //pointer to the solid World
    G4LogicalVolume *fLogicBeamH;    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsBeamH;    //pointer to the physical World

    // ------------Cone inner  beampipe ------------
    G4double fBeamSizeRin;
    G4double fBeamSizeRout;
    G4double fBeamSizeZ;
    G4Material *fBeamPipeMaterial;


    G4double fBeamSizeRin1Cone;
    G4double fBeamSizeRin2Cone;
    G4double fBeamSizeRout1Cone;
    G4double fBeamSizeRout2Cone;
    G4double fBeamSizeZCone;
    G4Material *fBeamPipeConeMaterial;

    G4Polycone *fSolidBeamCone;    //pointer to the solid World
    G4LogicalVolume *fLogicBeamCone;    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsBeamCone;    //pointer to the physical World


    G4Cons *fSolidBeamCone2;    //pointer to the solid World
    G4LogicalVolume *fLogicBeamCone2;    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsBeamCone2;    //pointer to the physical World


    // TR radiator volumes and dimensions

    G4Box *fSolidRadSlice;   // pointer to the solid  z-slice
    G4LogicalVolume *fLogicRadSlice;   // pointer to the logical z-slide
    G4VPhysicalVolume *fPhysicRadSlice;  // pointer to the physical z-slide

    G4Box *fSolidRadRing;    // pointer to the solid  R-slice
    G4LogicalVolume *fLogicRadRing;    // pointer to the logical R-slide
    G4VPhysicalVolume *fPhysicRadRing;   // pointer to the physical R-slide

  //   G4Box *fSolidRadiator;







    G4double fStartR;
    G4double fStartZ;
    G4LogicalVolume *fLogicAbsorber;    //pointer to the logical Absorber
    G4VPhysicalVolume *fPhysicsAbsorber;    //pointer to the physical Absorber

    //G4int fModuleNumber;   // the number of Rad-Det modules


    G4double fMylarThick;
    G4double fPipeLength;
    G4bool fPipe;


    G4double fWindowZ;
    G4double fGapZ;
    G4double fElectrodeZ;




    // G4double fElectronCut, fGammaCut, fPositronCut;

    JLeicDetectorMessenger *fDetectorMessenger;  //pointer to the Messenger
    JLeicCalorimeterSD *fCalorimeterSD;  //pointer to the sensitive detector

    JLeicMaterials *fMat;

};

#endif







