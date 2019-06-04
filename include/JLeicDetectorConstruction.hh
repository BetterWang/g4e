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

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ios.hh"

//class G4Box;
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Polycone.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
//class G4LogicalVolume;
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
//class G4VPhysicalVolume;
//class G4Material;
#include "G4Material.hh"
//class G4Region;
#include "G4Region.hh"
//class G4UniformMagField;
#include "G4UniformMagField.hh"
//class JLeicDetectorMessenger;
#include "JLeicDetectorMessenger.hh"

class JLeicCalorimeterSD;
//#include "JLeicCalorimeterSD.hh"
//class JLeicMaterials;
#include "JLeicMaterials.hh"

//-- Quadrupole --
#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4QuadrupoleMagField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ClassicalRK4.hh"
#include "G4FieldManager.hh"


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

    void SetFoilNumber(G4int i) { fFoilNumber = i; };

    void SetWorldMaterial(G4String);

    void SetWorldSizeZ(G4double);

    void SetWorldSizeR(G4double);

    void SetDetectorSetUp(G4String s) { fSetUp = s; };


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

    G4double GetWorldSizeZ() { return World_SizeZ; };

    G4double GetWorldSizeR() { return World_SizeR; };

    G4double GetAbsorberZpos() { return fAbsorberZ; };

    G4Material *GetAbsorberMaterial() { return fAbsorberMaterial; };

    G4double GetAbsorberThickness() { return fAbsorberThickness; };

    G4double GetAbsorberRadius() { return fAbsorberRadius; };

    const G4VPhysicalVolume *GetphysiWorld() { return World_Phys; };

    const G4VPhysicalVolume *GetAbsorber() { return fPhysicsAbsorber; };

    G4LogicalVolume *GetLogicalAbsorber() { return fLogicAbsorber; };

    G4LogicalVolume *GetLogicalRadiator() { return fLogicRadiator; };

    G4double GetFoilThick() { return fRadThickness; };

    G4double GetGasThick() { return fGasGap; };

    G4int GetFoilNumber() { return fFoilNumber; };

    G4Material *GetFoilMaterial() { return fFoilMat; };

    G4Material *GetGasMaterial() { return fGasMat; };


    G4String rootFileName;
    G4double fadc_slice;
    G4int NannVAR;
    //---  fsv move to public ---
    G4double fAbsorberZ;
    G4double fAbsorberThickness;
    G4String fSetUp;
    G4Material *fAbsorberMaterial;
    G4double fRadThick;
    G4int fModuleNumber;   // the number of Rad-et modules

private:

    G4VPhysicalVolume *ConstructDetectorXTR();

    G4FieldManager *SetQMagField(float field, float angle, float theta, G4ThreeVector fieldorigin);

    // G4FieldManager* SetQMagField(G4double field_x);
    G4FieldManager *SetDipoleMagField(G4double fx, G4double fy, G4double fz, float theta);


    G4VPhysicalVolume *SimpleSetUpALICE();

    G4VPhysicalVolume *SetUpALICE06();

    G4VPhysicalVolume *SetUpBari05();

    G4VPhysicalVolume *SetUpJLEIC2019();

    G4VPhysicalVolume *SetUpJLEIC09();

    G4VPhysicalVolume *SetUpHarris73();

    G4VPhysicalVolume *SetUpWatase86();

    G4VPhysicalVolume *SetUpBarr90();


    void TestOld();

private:

    G4bool fWorldChanged;
    //G4Material*        fAbsorberMaterial;
    //G4double           fAbsorberThickness;
    G4double fAbsorberRadius;

    G4Material *fPipeMat;

    G4Material *fWindowMat;
    G4double fWindowThick;

    G4Material *fElectrodeMat;
    G4double fElectrodeThick;

    G4Material *fGapMat;
    G4double fGapThick;

    //G4double           fAbsorberZ;
    //G4double           zstartAbs , zendAbs;
    //G4String           fSetUp;
    // World
    //=========================================================================
    //--------------World -------------------
    // =========================================================================
    G4Box *World_Solid;    //pointer to the solid World
    G4LogicalVolume *World_Logic;    //pointer to the logical World
    G4VPhysicalVolume *World_Phys;    //pointer to the physical World
    G4Material *World_Material;
    G4double World_SizeR;
    G4double World_SizeZ;
    G4double World_ShiftVTX;
    //--------------Solenoid BARREL  -------------------
    G4double Solenoid_RIn;
    G4double Solenoid_ROut;
    G4double Solenoid_SizeZ;
    G4double Solenoid_Field_Strength;
    G4double Solenoid_AlphaB;
    G4VisAttributes *attr_Solenoid;
    G4Tubs *Solenoid_Solid;      //pointer to the solid
    G4LogicalVolume *Solenoid_Logic;    //pointer to the logical
    G4VPhysicalVolume *Solenoid_Phys;  //pointer to the physical

//-----------------Hadron ENDCAP  volume--------------------
    G4double ci_ENDCAP_GVol_RIn;
    G4double ci_ENDCAP_GVol_ROut;
    G4double ci_ENDCAP_GVol_SizeZ;
    G4double ci_ENDCAP_GVol_ShiftZ ;
    G4double ci_ENDCAP_GVol_PosX ;
    G4double ci_ENDCAP_GVol_PosZ;
    //    G4double fENDCAP_H_Z;
    G4VisAttributes *attr_ci_ENDCAP_GVol;
    G4Tubs *ci_ENDCAP_GVol_Solid;    //pointer to the solid  ENDCAP-H volume
    G4LogicalVolume *ci_ENDCAP_GVol_Logic;    //pointer to the logical  ENDCAP-H  volume
    G4VPhysicalVolume *ci_ENDCAP_GVol_Phys;    //pointer to the physical ENDCAP-H  volume

//-----------------ENDCAP-E  volume--------------------
    G4double ce_ENDCAP_GVol_RIn;
    G4double ce_ENDCAP_GVol_ROut;
    G4double ce_ENDCAP_GVol_SizeZ;
    G4double ce_ENDCAP_GVol_PosZ;
    G4VisAttributes *attr_ce_ENDCAP_GVol;
    G4Tubs *ce_ENDCAP_GVol_Solid;    //pointer to the solid  ENDCAP-E volume
    G4LogicalVolume *ce_ENDCAP_GVol_Logic;    //pointer to the logical ENDCAP-E  volume
    G4VPhysicalVolume *ce_ENDCAP_GVol_Phys;    //pointer to the physical ENDCAP-E  volume
    // =========================================================================


    // =========================================================================
    // --------------- START DETECTOR VOLUMES ----------------------------------
    // =========================================================================


    // =========================================================================
    // ---------------  BARREL -------------------------------------------------
    // =========================================================================

    //*************************************************************
    //----------------VTX volume -------------------
    G4double cb_VTX_GVol_RIn;
    G4double cb_VTX_GVol_ROut;
    G4double cb_VTX_GVol_SizeZ;
    G4Tubs *cb_VTX_GVol_Solid;    //pointer to the solid VTX volume
    G4LogicalVolume *cb_VTX_GVol_Logic;    //pointer to the logical VTX volume
    G4VPhysicalVolume *cb_VTX_GVol_Phys;    //pointer to the physical VTX volume
    //--------------- VTX ladders geom-------
     G4Box *cb_VTX_ladder_Solid[10]; //pointer to the solid Absorber
    G4LogicalVolume *cb_VTX_ladder_Logic[10]; //pointer to the logical Absorber
     G4VPhysicalVolume *cb_VTX_ladder_Phys[10]; //pointer to the logical Absorber
    typedef struct {
        double Dx;
        double Dy;
        double Dz;
        double Rin;
    } cb_VTX_ladder_LayParam;
    G4double cb_VTX_ladder_DZ ;
    G4double cb_VTX_ladder_DY ;
    G4double cb_VTX_ladder_Thickness;

    G4VisAttributes *attr_cb_VTX_ladder;
    G4Material *cb_VTX_ladder_Material;
    int NUM;
    G4double cb_VTX_ladder_deltaphi;
    G4double cb_VTX_ladder_deltashi;

    //===================================================================================

    //-- slices and pixels

    G4LogicalVolume *pxdSlice_log[10]; //pointer to the logical slice
    G4LogicalVolume *pxdPixel_log[10]; //pointer to the logical pixel


    //*************************************************************
    //----------------CTD  volume -------------------
    G4double cb_CTD_GVol_RIn;
    G4double cb_CTD_GVol_ROut;
    G4double cb_CTD_GVol_SizeZ;
    G4Tubs *cb_CTD_GVol_Solid;    //pointer to the solid CTD  volume
    G4LogicalVolume *cb_CTD_GVol_Logic;    //pointer to the logical CTD  volume
    G4VPhysicalVolume *cb_CTD_GVol_Phys;    //pointer to the physical CTD  volume

//-------------------CTD_Si barrel detector ------------------
    G4double cb_CTD_detSi_RIn;
    G4double cb_CTD_detSi_ROut;
    G4double cb_CTD_detSi_SizeZ;
    G4double cb_CTD_detSi_Steps;
    G4int cb_CTD_detSi_Layers;
    G4Material *cb_CTD_detSi_Material;
    G4VisAttributes *attr_cb_CTD_det;
    G4double cb_CTD_detSi_lay_Rin[100];
    G4double cb_CTD_detSi_lay_Rout[100];
    G4Tubs *cb_CTD_detSi_Solid[100];    //pointer to the solid World
    G4LogicalVolume *cb_CTD_detSi_Logic[100];    //pointer to the logical World
    G4VPhysicalVolume *cb_CTD_detSi_Phys[100];    //pointer to the physical World
    //-------------------CTD_Straw barrel detector ------------------
    G4double cb_CTD_detStraw_RIn;
    G4double cb_CTD_detStraw_ROut;
    G4double cb_CTD_detStraw_SizeZ;
    G4double cb_CTD_detStraw_Steps;
    G4Material *cb_CTD_detStraw_Material;
    G4int cb_CTD_detStraw_Layers;
    G4double cb_CTD_Straws_RIn;
    G4double cb_CTD_Straws_ROut;
    G4VisAttributes* attr_cb_CTD_Straws;
    G4Tubs *cb_CTD_Straws_Solid;    //pointer to the solid World
    G4LogicalVolume *cb_CTD_Straws_Logic;    //pointer to the logical World
    G4VPhysicalVolume *cb_CTD_Straws_Phys;    //pointer to the physical World

    G4Material *cb_CTD_Straws_Wall_Material;
    G4Tubs *cb_CTD_Straws_Wall_Solid;    //pointer to the solid World
    G4LogicalVolume *cb_CTD_Straws_Wall_Logic;    //pointer to the logical World
    G4VPhysicalVolume *cb_CTD_Straws_Wall_Phys;    //pointer to the physical World

    G4VisAttributes* attr_cb_CTD_Straws_Gas;
    G4Material *cb_CTD_Straws_Gas_Material;
    G4Tubs *cb_CTD_Straws_Gas_Solid;    //pointer to the solid World
    G4LogicalVolume *cb_CTD_Straws_Gas_Logic;    //pointer to the logical World
    G4VPhysicalVolume *cb_CTD_Straws_Gas_Phys;    //pointer to the physical World

    //*************************************************************
  //----------------DIRC  volume -------------------
    G4double cb_DIRC_GVol_RIn;
    G4double cb_DIRC_GVol_ROut;
    G4double cb_DIRC_GVol_SizeZ;
    G4Tubs *cb_DIRC_GVol_Solid;    //pointer to the solid DIRC  volume
    G4VisAttributes* attr_cb_DIRC_GVol;
    G4LogicalVolume *cb_DIRC_GVol_Logic;    //pointer to the logical DIRC  volume
    G4VPhysicalVolume *cb_DIRC_GVol_Phys;    //pointer to the physical DIRC  volume
   //--------------DIRC bars detector----------------------

    G4double cb_DIRC_bars_RIn;
    G4double cb_DIRC_bars_ROut;
    G4double cb_DIRC_bars_DZ;
    G4double cb_DIRC_bars_DY;
    G4double cb_DIRC_bars_DX;
    G4double cb_DIRC_bars_deltaphi;

    G4Material *cb_DIRC_bars_Material;
    G4VisAttributes* attr_cb_DIRC_bars;
    // G4Tubs*            cb_DIRC_bars_Solid;    //pointer to the solid World
    G4Box *cb_DIRC_bars_Solid;    //pointer to the solid World
    G4LogicalVolume *cb_DIRC_bars_Logic;    //pointer to the logical World
    G4VPhysicalVolume *cb_DIRC_bars_Phys;    //pointer to the physical World
   //*************************************************************
    //-----------------EMCAL barrel volume--------------------
    G4double cb_EMCAL_GVol_RIn;
    G4double cb_EMCAL_GVol_ROut;
    G4double cb_EMCAL_GVol_Thickness;
    G4double cb_EMCAL_GVol_SizeZ;
    // G4Tubs*            cb_EMCAL_GVol_Solid;    //pointer to the solid  EMCAL barrel volume
    G4Polycone *cb_EMCAL_GVol_Solid;    //pointer to the solid  EMCAL barrel volume
    G4LogicalVolume *cb_EMCAL_GVol_Logic;    //pointer to the logical EMCAL barrel volume
    G4VPhysicalVolume *cb_EMCAL_GVol_Phys;    //pointer to the physical EMCAL barrel volume

    //--------------EMCAL barrel detector----------------------

    G4double cb_EMCAL_det_RIn;
    G4double cb_EMCAL_det_ROut;
    G4double cb_EMCAL_det_SizeZ;
    G4Material *cb_EMCAL_det_Material;
    // G4Tubs*            cb_EMCAL_det_Solid;    //pointer to the solid World
    G4Polycone *cb_EMCAL_det_Solid;    //pointer to the solid World
    G4LogicalVolume *cb_EMCAL_det_Logic;    //pointer to the logical World
    G4VPhysicalVolume *cb_EMCAL_det_Phys;    //pointer to the physical World

    //*************************************************************
    //--------------HCAL barrel volume  -------------------
    G4double cb_HCAL_GVol_RIn;
    G4double cb_HCAL_GVol_ROut;
    G4double cb_HCAL_GVol_SizeZ;

    G4Tubs *cb_HCAL_GVol_Solid;      //pointer to the solid
    G4LogicalVolume *cb_HCAL_GVol_Logic;    //pointer to the logical
    G4VPhysicalVolume *cb_HCAL_GVol_Phys;  //pointer to the physical
    G4VisAttributes *attr_cb_HCAL_GVol;

    G4double cb_HCAL_det_RIn;
    G4double cb_HCAL_det_ROut;
    G4double cb_HCAL_det_SizeZ;
    G4double cb_HCAL_det_Thickness;
    G4Material *cb_HCAL_det_Material;
    G4VisAttributes *attr_cb_HCAL_det;
    G4Tubs *cb_HCAL_det_Solid;      //pointer to the solid
    G4LogicalVolume *cb_HCAL_det_Logic;    //pointer to the logical
    G4VPhysicalVolume *cb_HCAL_det_Phys;  //pointer to the physical


    //***************** Included into BARREL***********************
    //-----------------ENDCAP-E GEM  volume--------------------

    G4double ce_GEM_GVol_RIn;
    G4double ce_GEM_GVol_ROut;
    G4double ce_GEM_GVol_SizeZ;
    G4double ce_GEM_GVol_PosZ;
    G4double ce_GEM_GVol_ShiftZ;
    G4Tubs *ce_GEM_GVol_Solid;    //pointer to the solid  GEM E-endcap  volume
    G4LogicalVolume *ce_GEM_GVol_Logic;    //pointer to the logical GEM E-endcap volume
    G4VPhysicalVolume *ce_GEM_GVol_Phys;    //pointer to the physical GEM E-endcap volume
    //--------------- ENDCAP-E GEM  detector ------------------
    G4double ce_GEM_lay_RIn[20];
    G4double ce_GEM_lay_ROut[20];
    G4double ce_GEM_lay_SizeZ[20];
    G4double ce_GEM_lay_PosZ[20];
    G4Material *ce_GEM_lay_Material;
    G4VisAttributes *attr_ce_GEM_lay;
    G4Tubs *ce_GEM_lay_Solid[20];    //pointer to the solid World
    G4LogicalVolume *ce_GEM_lay_Logic[20];    //pointer to the logical World
    G4VPhysicalVolume *ce_GEM_lay_Phys[20];    //pointer to the physical World

    //*************************************************************
    //-----------------ENDCAP-H GEM  volume--------------------

    G4double ci_GEM_GVol_RIn;
    G4double ci_GEM_GVol_ROut;
    G4double ci_GEM_GVol_SizeZ;
    G4double ci_GEM_GVol_PosZ;
    G4double ci_GEM_GVol_ShiftZ;
    G4Tubs *ci_GEM_GVol_Solid;    //pointer to the solid  GEM H-endcap  volume
    G4LogicalVolume *ci_GEM_GVol_Logic;    //pointer to the logical GEM H-endcap volume
    G4VPhysicalVolume *ci_GEM_GVol_Phys;    //pointer to the physical GEM H-endcap volume
    //-------------------ENDCAP-H GEM detector ------------------
    G4double ci_GEM_lay_RIn[20];
    G4double ci_GEM_lay_ROut[20];
    G4double ci_GEM_lay_SizeZ[20];
    G4double ci_GEM_lay_PosZ[20];
    G4Material *ci_GEM_lay_Material;
    G4VisAttributes *attr_ci_GEM_lay;
    G4Tubs *ci_GEM_lay_Solid[20];    //pointer to the solid World
    G4LogicalVolume *ci_GEM_lay_Logic[20];    //pointer to the logical World
    G4VPhysicalVolume *ci_GEM_lay_Phys[20];    //pointer to the physical World
    //--------------------------------------------------------------


    // =========================================================================
    // =========================================================================
    // ---------------  ENDCAP-E -------------------------------------------------
    // =========================================================================
    // =========================================================================


    // =========================================================================
    //------------- ENDCAP-E HCAL -------------------
    G4double ce_HCAL_GVol_RIn;
    G4double ce_HCAL_GVol_ROut;
    G4double  ce_HCAL_GVol_SizeZ;
    G4double  ce_HCAL_GVol_ShiftZ;


    //  G4Polycone*        ce_HCAL_GVol_Solid;    //pointer to the solid ECAP_HCAL volume

    G4Tubs *ce_HCAL_GVol_Solid;    //pointer to the solid ECAP_HCAL volume
    G4LogicalVolume *ce_HCAL_GVol_Logic;    //pointer to the logicalECAP_HCAL  volume
    G4VPhysicalVolume *ce_HCAL_GVol_Phys;    //pointer to the physical EMCAL barrel volume

    G4Tubs *ce_HCAL_det_Solid;    //pointer to the solid ECAP_HCAL volume
    G4LogicalVolume *ce_HCAL_det_Logic;    //pointer to the logicalECAP_HCAL  volume
    G4VPhysicalVolume *ce_HCAL_det_Phys;    //pointer to the physical EMCAL barrel volume

    // =========================================================================
    //------------- ENDCAP-E EMCAL -------------------

    G4double ce_EMCAL_GVol_RIn;
    G4double ce_EMCAL_GVol_ROut;
    G4double ce_EMCAL_GVol_SizeZ;
    G4double ce_EMCAL_GVol_PosZ;
    G4VisAttributes *attr_ce_EMCAL_GVol;
    G4Tubs *ce_EMCAL_GVol_Solid;    //pointer to the solid World
    G4LogicalVolume *ce_EMCAL_GVol_Logic;    //pointer to the logical World
    G4VPhysicalVolume *ce_EMCAL_GVol_Phys;    //pointer to the physical World

    //............... EMCAL Crystals modules ......................
    double ce_EMCAL_detPWO_Thickness;
    double ce_EMCAL_detPWO_Width;
    double ce_EMCAL_detPWO_ROut;
    G4double ce_EMCAL_detPWO_Gap ;
    G4double ce_EMCAL_detPWO_InnerR;
    G4double ce_EMCAL_detPWO_PosZ;

    G4Material *ce_EMCAL_detPWO_Material;
    G4VisAttributes *attr_ce_EMCAL_detPWO;
    G4Box *ce_EMCAL_detPWO_Solid;
    G4LogicalVolume *ce_EMCAL_detPWO_Logic;


    //............... EMCAL Glass modules ......................
    G4double ce_EMCAL_detGLASS_InnerR;
    G4double ce_EMCAL_detGLASS_Thickness;
    G4double ce_EMCAL_detGLASS_OuterR;
    G4double ce_EMCAL_detGLASS_PosZ;
    G4double ce_EMCAL_detGLASS_Width;
    double ce_EMCAL_detGLASS_Gap ;

    G4Material *ce_EMCAL_detGLASS_Material;
    G4VisAttributes *attr_ce_EMCAL_detGLASS;
    G4Box *ce_EMCAL_detGLASS_Solid;
    G4LogicalVolume *ce_EMCAL_detGLASS_Logic;
    // =========================================================================
    //----------------- ENDCAP-E  MRICH  -------------------
    G4double ce_MRICH_GVol_RIn;
    G4double ce_MRICH_GVol_ROut;
    G4double ce_MRICH_GVol_SizeZ;
    G4double ce_MRICH_GVol_PosZ;
    G4double ce_MRICH_GVol_InnerR;
    G4VisAttributes* attr_ce_MRICH_GVol;
    G4Tubs *ce_MRICH_GVol_Solid;    //pointer to the solid  e-endcap MRICH volume
    G4LogicalVolume *ce_MRICH_GVol_Logic;    //pointer to the logical  e-endcap MRICH volume
    G4VPhysicalVolume *ce_MRICH_GVol_Phys;    //pointer to the physical  e-endcap MRICH volume
    //............... MRICH modules ......................
    double ce_MRICH_mod_Thickness;
    double ce_MRICH_mod_Width;
    G4double ce_MRICH_mod_Gap;
    G4VisAttributes *attr_ce_MRICH_mod;
    G4Box *ce_MRICH_mod_Solid;
    G4LogicalVolume *ce_MRICH_mod_Logic;

    // =========================================================================



    // =========================================================================
    // =========================================================================
    // ---------------  ENDCAP-H -------------------------------------------------
    // =========================================================================
    // =========================================================================
    //
    // =========================================================================
    //--------------ENDCAP-H  HCAL vol -------------------
    // G4double           cb_EMCAL_GVol_RIn;
    // G4double           cb_EMCAL_GVol_ROut;
    // G4double           cb_EMCAL_GVol_SizeZ;

    //  G4Polycone*        ci_HCAL_GVol_Solid;    //pointer to the solid HCAP_HCAL volume
    G4double ci_HCAL_GVol_RIn;
    G4double ci_HCAL_GVol_ROut;
    G4double  ci_HCAL_GVol_SizeZ;
    G4double  ci_HCAL_GVol_ShiftZ;
    G4VisAttributes* attr_ci_HCAL_GVol;
    G4Tubs *ci_HCAL_GVol_Solid;    //pointer to the solid HCAP_HCAL volume
    G4LogicalVolume *ci_HCAL_GVol_Logic;    //pointer to the logicalHCAP_HCAL  volume
    G4VPhysicalVolume *ci_HCAL_GVol_Phys;    //pointer to the physical EMCAL barrel volume
    //---------------ENDCAP-H HCAL det iron-------------------
    G4double ci_HCAL_det_RIn;
    G4double ci_HCAL_det_ROut;
    G4double ci_HCAL_det_ThicknessZ;
    G4double ci_HCAL_det_GapZ;
    G4double ci_HCAL_det_PosZ;
    int ci_HCAL_Nlay;
    G4VisAttributes* attr_ci_HCAL_det;
    G4Material *ci_HCAL_det_Material;
    G4Tubs *ci_HCAL_det_Solid;    //pointer to the solid HCAP_HCAL volume
    G4LogicalVolume *ci_HCAL_det_Logic;    //pointer to the logicalHCAP_HCAL  volume
    G4VPhysicalVolume *ci_HCAL_det_Phys;    //pointer to the physical EMCAL barrel volume
    // =========================================================================
    //------------------ ENDCAP-H EMCAL vol ------------------
    G4double ci_EMCAL_GVol_RIn;
    G4double ci_EMCAL_GVol_ROut;
    G4double ci_EMCAL_GVol_ThicknessZ;
    double ci_EMCAL_GVol_PosZ;
    G4Material *ci_EMCAL_GVol_Material;
    G4VisAttributes *attr_ci_EMCAL_GVol;
    G4Tubs *ci_EMCAL_GVol_Solid;    //pointer to the solid World
    G4LogicalVolume *ci_EMCAL_GVol_Logic;    //pointer to the logical World
    G4VPhysicalVolume *ci_EMCAL_GVol_Phys;    //pointer to the physical World
    //  ------------ENDCAP-H  EMCAL modules -----
    double ci_EMCAL_det_Length ;
    double ci_EMCAL_det_Width ;
    G4double ci_EMCAL_det_Gap ;
    G4double ci_EMCAL_det_Rin1 ;
    G4double ci_EMCAL_det_Rin2 ;

    G4Material *ci_EMCAL_det_Material ;
    G4VisAttributes *attr_ci_EMCAL_det;
    G4Box *ci_EMCAL_det_Solid ;
    G4LogicalVolume *ci_EMCAL_det_Logic ;

    // =========================================================================
    //--------------ENDCAP-H  TRD vol -------------------

     double ci_TRD_GVol_RIn ;
     double ci_TRD_GVol_ROut ;
     double ci_TRD_GVol_ThicknessZ ;
     double ci_TRD_GVol_PosZ;
     G4VisAttributes *attr_ci_TRD_GVol;
     G4Tubs *ci_TRD_GVol_Solid;
     G4LogicalVolume *ci_TRD_GVol_Logic;
     G4PVPlacement *ci_TRD_GVol_Phys;
  //------------- det -----------
     double ci_TRD_det_RIn ;
     double ci_TRD_det_ROut ;
     double ci_TRD_det_ThicknessZ ;
     double ci_TRD_det_PosZ;
     G4Material *ci_TRD_det_Material ;
     G4VisAttributes *attr_ci_TRD_det;
     G4VisAttributes *attr_ci_TRD_rad;
     G4Tubs *ci_TRD_det_Solid;
     G4LogicalVolume *ci_TRD_det_Logic;
     G4PVPlacement *ci_TRD_det_Phys;

    // =========================================================================
    //--------------ENDCAP-H  HCAL vol -------------------
    double ci_DRICH_GVol_RIn ;
    double ci_DRICH_GVol_ROut ;
    double ci_DRICH_GVol_ThicknessZ ;

    double ci_DRICH_GVol_PosZ ;
    G4VisAttributes* attr_ci_DRICH_GVol;

    G4Tubs *ci_DRICH_GVol_Solid;
    G4LogicalVolume *ci_DRICH_GVol_Logic;
    G4PVPlacement *ci_DRICH_GVol_Phys;


    //===========================================================
    //===========================================================
    //-------------------------------------
    //-----------------FARFORWARD  AREA-------------------
    //===========================================================
    //===========================================================
   //-----------------FORWARD D1  volume--------------------
    // ----- A----
    G4double fi_D1A_GVol_RIn;
    G4double fi_D1A_GVol_ROut;
    G4double fi_D1A_GVol_SizeZ;
    G4VisAttributes* attr_fi_D1A_GVol;
    G4Tubs *fi_D1A_GVol_Solid;    //pointer to the solid  FARFORWD
    G4LogicalVolume *fi_D1A_GVol_Logic;    //pointer to the logical FARFORWD
    G4VPhysicalVolume *fi_D1A_GVol_Phys;    //pointer to the physical FARFORWD

    G4double fi_D1A_lay_RIn;
    G4double fi_D1A_lay_ROut;
    G4double fi_D1A_lay_SizeZ;
    int f1_D1A_NLAY;
    G4Material *fi_D1A_lay_Material;
    G4VisAttributes* attr_fi_D1A_lay;
    G4Tubs *f1_D1A_lay_Solid;    //pointer to the solid  FARFORWD
    G4LogicalVolume *f1_D1A_Lay_Logic;    //pointer to the logical FARFORWD
    G4VPhysicalVolume *f1_D1A_lay_Phys;    //pointer to the physical FARFORWD

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

    //----------------EMCAL D1 area---------------------

    double fi_D1B_EMCAL_SizeZ;
    double fi_D1B_EMCAL_Width;
    G4double fi_D1B_EMCAL_Gap ;
    G4double fi_D1B_EMCAL_Angle;
    float fi_D1B_EMCAL_Shift;
    G4RotationMatrix rotm_fi_D1B_EMCAL;
    G4Material *fi_D1B_EMCAL_Material;
    G4VisAttributes* attr_fi_D1B_EMCAL;

    G4Box *fi_D1B_EMCAL_Solid;
    G4LogicalVolume *fi_D1B_EMCAL_Logic;


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



    //-------------------VERTEX barrel detector ------------------
    G4Material *fVTXMaterial;
    G4Tubs *fSolidVTXBarrel[20];    //pointer to the solid World
    G4LogicalVolume *fLogicVTXBarrel[20];    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsVTXBarrel[20];    //pointer to the physical World
    //--------------------Vertex Endcaps ---------------------------

    G4Trap *fSolidVTXEndE[10]; //pointer to the solid Absorber
    G4LogicalVolume *fLogicVTXEndE[10]; //pointer to the logical Absorber
    G4VPhysicalVolume *fPhysicsVTXEndE; //pointer to the physical Absorber
    G4Trap *fSolidVTXEndH[10]; //pointer to the solid Absorber
    G4LogicalVolume *fLogicVTXEndH[10]; //pointer to the logical Absorber
    G4VPhysicalVolume *fPhysicsVTXEndH; //pointer to the physical Absorber

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
     G4Tubs *fSolidRadiator;
    G4LogicalVolume *fLogicRadiator;
    G4VPhysicalVolume *fPhysicsRadiator;

    G4Material *fRadiatorMat;        // pointer to the mixed TR radiator material
    G4Material *fFoilMat;            // pointer to the TR foil radiator material
    G4Material *fGasMat;             // pointer to the TR gas radiator material

    G4double fRadThickness;
    G4double fGasGap;
    G4double foilGasRatio;

    G4int fFoilNumber;

    G4double fDetThickness;
    G4double fDetLength;
    G4double fDetGap;

    G4double fStartR;
    G4double fStartZ;
    G4LogicalVolume *fLogicAbsorber;    //pointer to the logical Absorber
    G4VPhysicalVolume *fPhysicsAbsorber;    //pointer to the physical Absorber

    //G4int fModuleNumber;   // the number of Rad-Det modules

    //G4double fRadThick;
    G4double fMylarThick;
    G4double fPipeLength;
    G4bool fPipe;
    G4bool fPipeField;
    G4double fRadZ;
    G4double fWindowZ;
    G4double fGapZ;
    G4double fElectrodeZ;


    //----
    G4UniformMagField *fMagField;      //pointer to the magnetic field

    // G4double fElectronCut, fGammaCut, fPositronCut;

    JLeicDetectorMessenger *fDetectorMessenger;  //pointer to the Messenger
    JLeicCalorimeterSD *fCalorimeterSD;  //pointer to the sensitive detector
    G4Region *fRegGasDet;
    G4Region *fRadRegion;
    JLeicMaterials *fMat;

};

#endif







