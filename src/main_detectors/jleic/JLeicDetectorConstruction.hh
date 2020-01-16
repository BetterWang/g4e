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


#include "Materials.hh"
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

#include "ffi_TRKD2/ffi_TRKD2.hh"     // Far-forward Ion D2 - Tracking
#include "ffi_ZDC/ffi_ZDC.hh"         // Far-forward Ion   - HCAL ZDC
#include "ffi_RPOT_D2/ffi_RPOT_D2.hh"         // Far-forward Ion   - RomanPots system
#include "ffi_RPOT_D3/ffi_RPOT_D3.hh"         // Far-forward Ion   - RomanPots system
//---------Beam lattice ------------------------
//#include "ir_Lattice/ir_Lattice.hh"     // IR Lattice import from file
#include "ir_Lattice/AccMag.hh"   // New IR Lattice import from file
#include "ir_Beampipe/ir_Beampipe.hh"     // IR Lattice import from file


class JLeicCalorimeterSD;

class JLeicVertexSD;

class JLeicDetectorConstruction : public G4VUserDetectorConstruction
{
public:

    //--------BEAM elements------
    bool USE_JLEIC = false;  // use either JLEIC or ERHIC optics
    bool USE_ERHIC =true;   // use either JLEIC or ERHIC optics
    // -----
    bool USE_FFQs = true;
    //bool USE_FFQs_iu
    //bool USE_FFQs_ed
    //bool USE_FFQs_eu

    //===========================================
    //--------BARREL------
    bool USE_BARREL = true;
    bool USE_BARREL_det = true;

    //------- subdetector-volumes  barrel ----- 

    bool USE_CB_VTX = true;
    //bool  USE_VTX0 1   // for simple vtx geom
    bool USE_CB_VTX_LADDERS = true;
    bool USE_CB_VTX_ENDCAPS = false;  // for vxt endcaps ladders
    //bool  USE_VTX_DISKS    // for vxt disks along beampipe
    //bool USE_VTX_E 1   // for vxt endcaps 


    bool USE_CB_CTD = true;
    // -------- use one of the following options for CTD ---------------
    bool USE_CB_CTD_Si = true;//  silicon version of CTD
    bool USE_CB_CTD_Straw = false; // straw version of CTD
    //--------------------------------------------------------

    bool USE_CB_DIRC = true;
    bool USE_CB_DIRC_bars = false;  // bars for DIRC

    bool USE_CB_EMCAL = true ;
    bool USE_CB_HCAL = true;
    bool USE_CB_HCAL_D = true; // hcal detector ( granularity)

     // ==============================================
    //--------E-encap------
    bool USE_E_ENDCAP = true;
    //------- subdetector-volumes E-encap ----- 
    bool USE_CE_GEM = true;
    bool USE_CE_EMCAL = true;
    bool USE_CE_MRICH = true;

    // -------- polarimeter ------------
    bool USE_FFE_CPOL = false;
    //==============================================
    //--------H-encap------
    bool USE_CI_ENDCAP = true;
    //------- subdetector-volumes H-encap -----
    bool USE_CI_GEM = true;
    bool USE_CI_DRICH = true;
    bool USE_CI_TRD = true;
    bool USE_CI_EMCAL = false;
    bool USE_CI_HCAL = true;
    bool USE_CI_HCAL_D = false;
    //--------- Forward D1 ------
    bool USE_FI_EMCAL = false;
    bool USE_FI_TRKD1 = false ;
    //--------FARFORWARD HADRON------
    //bool USE_DIPOLE1_SI
    //
    //bool USE_FI_DIPOLE1_B
    //bool USE_FI_DIPOLE2
    bool USE_FFI_TRKD2 = false;
    bool USE_FFI_ZDC = false;
    bool USE_FFI_RPOT_D2 = false;
    bool USE_FFI_RPOT_D3 = false;
    //bool USE_FARFORWARD_GEM
    //bool USE_FARFORWARD_VP

    //--------barrel------
    //bool  USE_VTX0 1   // for simple vtx geom
    //bool USE_VTX_E 1   // for vxt endcaps 
    //-------------CTD----------
    //bool  USE_CB_EMCAL 1
    //--------Endcap ------
    //bool USE_ENDCAP 1
    //bool  USE_EMCALe
    //bool  USE_VTXB 1

    JLeicDetectorConstruction();

    ~JLeicDetectorConstruction();

    void EnableHCalRings();

public:
    void SetElectronBeamEnergy(G4int);

    void SetIonBeamEnergy(G4int);

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

    G4VPhysicalVolume *Construct() override;

    void UpdateGeometry();

public:

    void PrintGeometryParameters();

    G4Material *GetWorldMaterial() { return World_Material; };

    G4double GetWorldSizeZ() { return fConfig.World.SizeZ; };

    G4double GetWorldSizeR() { return fConfig.World.SizeR; };

    void checkVolumeOverlap();

    const G4VPhysicalVolume *GetWorldPhysicalVolume() { return World_Phys; };

    const G4VPhysicalVolume *GetAbsorberPhysicalVolume() { return fPhysicsAbsorber; };

    G4LogicalVolume *GetLogicalAbsorber() { return fLogicAbsorber; };

    double GetAbsorberThickness() { return fConfig.ci_TRD.fAbsorberThickness; };

    G4Material *GetAbsorberMaterial() { return fConfig.ci_TRD.det_Material; };

    //TRD- related ---- needs to be moved move

    G4LogicalVolume *GetLogicalRadiator() { return ci_TRD.fLogicRadiator; };

    G4Material *GetFoilMaterial() { return ci_TRD.fFoilMat; };

    G4Material *GetGasMaterial() { return ci_TRD.fGasMat; };

    G4double GetFoilThick() { return fConfig.ci_TRD.fRadThickness; };

    G4double GetGasThick() { return fConfig.ci_TRD.fGasGap; };

    G4int GetFoilNumber()
    {
        std::cout << " foil number4 = " << ci_TRD.ConstructionConfig.fFoilNumber << std::endl;
        return ci_TRD.ConstructionConfig.fFoilNumber;
    }

    G4String rootFileName;
    G4double fadc_slice;

    //---  fsv move to public ---
    G4Material *fAbsorberMaterial;

    G4int fModuleNumber;   // the number of Rad-et modules

    JLeicDetectorConfig fConfig;

    //----------------VTX  volume ------------------
    cb_VTX_Design cb_VTX;

    void SetUpJLEIC2019();

private:

    void Create_ci_Endcap(JLeicDetectorConfig::ci_Endcap_Config cfg);

    void Create_ce_Endcap(JLeicDetectorConfig::ce_Endcap_Config dfg);


private:

// ----------------------------------------------
//           R E F A C T O R I N G


    //=========================================================================
    //--------------World -------------------
    // =========================================================================
    G4Box *World_Solid;                 // pointer to the solid World
    G4LogicalVolume *World_Logic;       // pointer to the logical World
    G4VPhysicalVolume *World_Phys;      // pointer to the physical World
    G4Material *World_Material;

    // Interaction region
   // ir_LatticeDesign ir_Lattice;
    ir_Beampipe_Design ir_Beampipe;

//-----------------Hadron ENDCAP volume--------------------
    G4VisAttributes *ci_ENDCAP_GVol_VisAttr;
    G4Tubs *ci_ENDCAP_GVol_Solid;               // pointer to the solid  ENDCAP-H volume
    G4LogicalVolume *ci_ENDCAP_GVol_Logic;      // pointer to the logical  ENDCAP-H  volume
    G4VPhysicalVolume *ci_ENDCAP_GVol_Phys;     // pointer to the physical ENDCAP-H  volume

//-----------------Electron ENDCAP  volume--------------------
    G4VisAttributes *ce_ENDCAP_VisAttr;
    G4Tubs *ce_ENDCAP_GVol_Solid;               // pointer to the solid  ENDCAP-E volume
    G4LogicalVolume *ce_ENDCAP_GVol_Logic;      // pointer to the logical ENDCAP-E  volume
    G4VPhysicalVolume *ce_ENDCAP_GVol_Phys;     // pointer to the physical ENDCAP-E  volume
    //==============================================
    //----------------BARREL -----------------------
    cb_Solenoid_Design cb_Solenoid;
    //----------------VTX  volume ------------------
    // cb_VTX_Design      cb_VTX;
    //----------------CTD  volume -------------------
    cb_CTD_Design cb_CTD;
    //----------------DIRC  volume -------------------
    cb_DIRC_Design cb_DIRC;
    //----------------EMCAL  volume -------------------
    cb_EMCAL_Design cb_EMCAL;
    //----------------HCAL  volume -------------------
    cb_HCAL_Design cb_HCAL;
    //==============================================
    //----------------E-ENDCAP -----------------------
    //----------------GEM volume ---------------------
    ce_GEM_Design ce_GEM;
    //----------------MRICH volume -----------------------
    ce_MRICH_Design ce_MRICH;
    //----------------EMCAL volume -----------------------
    ce_EMCAL_Design ce_EMCAL;

    //==============================================
    //----------------Far-forward Electron  ---------

    //----------------FFe  CPOL volume --------------
    ffe_CPOL_Design ffe_CPOL;
    //==============================================
    //----------------Ion-ENDCAP -----------------------
    //----------------GEM volume ---------------------
    ci_GEM_Design ci_GEM;
    //----------------DRICH volume ---------------------
    ci_DRICH_Design ci_DRICH;
    //----------------TRD volume ---------------------
    ci_TRD_Design ci_TRD;
    //----------------EMCAL volume ---------------------
    ci_EMCAL_Design ci_EMCAL;
    //----------------HCAL volume ---------------------
    ci_HCAL_Design ci_HCAL;

    //==============================================
    //----------------Far-forward ION  --------------
    //----------------Tracking volume ---------------------
    fi_TRKD1_Design fi_TRKD1;
    //----------------EMCAL volume ---------------------
    fi_EMCAL_Design fi_EMCAL;

    //----------------D2 Tracking volume  ---------------------
    ffi_TRKD2_Design ffi_TRKD2;

    //---------------HCAL -ZDC -------------------------
    ffi_ZDC_Design ffi_ZDC;
    //---------------Roman Pot system -------------------------
    ffi_RPOT_D2_Design ffi_RPOT_D2;
    ffi_RPOT_D3_Design ffi_RPOT_D3;

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
    G4VisAttributes *attr_fi_D1B_GVol;

    G4Tubs *fi_D1B_GVol_Solid;    //pointer to the solid  FARFORWD
    G4LogicalVolume *fi_D1B_GVol_Logic;    //pointer to the logical FARFORWD
    G4VPhysicalVolume *fi_D1B_GVol_Phys;    //pointer to the physical FARFORWD

    G4double fi_D1B_lay_RIn;
    G4double fi_D1B_lay_ROut;
    G4double fi_D1B_lay_SizeZ;
    G4Material *fi_D1B_lay_Material;
    G4VisAttributes *attr_fi_D1B_lay;

    G4Tubs *fi_D1B_lay_Solid;    //pointer to the solid  FARFORWD
    G4LogicalVolume *fi_D1B_lay_Logic;    //pointer to the logical FARFORWD
    G4VPhysicalVolume *fi_D1B_lay_Phys;    //pointer to the physical FARFORWD



    //-----------------FORWARD TRD inside D2  volume--------------------
    //
    G4double ffi_D2_GVol_RIn;
    G4double ffi_D2_GVol_ROut;
    G4double ffi_D2_GVol_SizeZ;
    G4VisAttributes *attr_ffi_D2_GVol;
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
    JLeicVertexSD *fVertexSD;  //pointer to the sensitive detector

    g4e::Materials *fMat;

};

#endif







