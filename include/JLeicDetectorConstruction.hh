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


class JLeicDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    JLeicDetectorConstruction();
   ~JLeicDetectorConstruction();
  
  public:
       
    void SetAbsorberMaterial (G4String);     
     void SetAbsorberThickness(G4double);     
     void SetAbsorberRadius(G4double);                
     void SetAbsorberZpos(G4double);

     void SetRadiatorMaterial (G4String);     
     void SetRadiatorThickness(G4double);
 
     void SetGasGapThickness(G4double);     
   
     void SetFoilNumber (G4int    i)  {fFoilNumber = i;  };     

     void SetWorldMaterial(G4String);
     void SetWorldSizeZ(G4double);
     void SetWorldSizeR(G4double);
     void SetDetectorSetUp(G4String s) {fSetUp = s;};


     void SetMagField(G4double);
     
     G4VPhysicalVolume* Construct();

     void UpdateGeometry();
     void Read_Di_File();
     void  Read_dE_File();


  void  CreateQuad(int i, char *MyChar, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi,float ffqsRoutDi,
	  float qFIELDx,float qFIELDy, float qFIELQn, float qFIELQs,float qFIELSek,float qFIELSol,float ffqsX,float ffqsY,float ffqsZ,float ffqsTheta,float ffqsPhi );
   void  CreateDipole(int i, char *MyChar, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi,float ffqsRoutDi,
	  float qFIELDx,float qFIELDy, float qFIELQn, float qFIELQs,float qFIELSek,float qFIELSol,float ffqsX,float ffqsY,float ffqsZ,float ffqsTheta,float ffqsPhi );
   void  CreateDipoleChicane(int i, char *MyChar, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi,float ffqsRoutDi,
	  float qFIELDx,float qFIELDy, float qFIELQn, float qFIELQs,float qFIELSek,float qFIELSol,float ffqsX,float ffqsY,float ffqsZ,float ffqsTheta,float ffqsPhi );
   void  CreateASolenoid(int i, char *MyChar, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi,float ffqsRoutDi,
	  float qFIELDx,float qFIELDy, float qFIELQn, float qFIELQs,float qFIELSek,float qFIELSol,float ffqsX,float ffqsY,float ffqsZ,float ffqsTheta,float ffqsPhi );

 int iq=0,ik=0,is=0;


 public:
  
     void PrintGeometryParameters(); 
                    
     G4Material* GetWorldMaterial()    {return fWorldMaterial;};
     G4double GetWorldSizeZ()          {return fWorldSizeZ;}; 
     G4double GetWorldSizeR()          {return fWorldSizeR;};
     
     G4double GetAbsorberZpos()        {return fAbsorberZ;}; 

     G4Material* GetAbsorberMaterial()  {return fAbsorberMaterial;};
     G4double    GetAbsorberThickness() {return fAbsorberThickness;};      
     G4double GetAbsorberRadius()       {return fAbsorberRadius;};
     
     const G4VPhysicalVolume* GetphysiWorld() {return fPhysicsWorld;};           
     const G4VPhysicalVolume* GetAbsorber()   {return fPhysicsAbsorber;};
     G4LogicalVolume* GetLogicalAbsorber()    {return fLogicAbsorber;};

     G4LogicalVolume* GetLogicalRadiator()    {return fLogicRadiator;};
     G4double         GetFoilThick()          {return fRadThickness;};      
     G4double         GetGasThick()           {return fGasGap;};      
     G4int            GetFoilNumber()         {return fFoilNumber;};      
     G4Material* GetFoilMaterial()  {return fFoilMat;};
     G4Material* GetGasMaterial()  {return fGasMat;};


G4String  rootFileName;
  G4double fadc_slice;
  G4int NannVAR;
  //---  fsv move to public ---
G4double           fAbsorberZ;
G4double           fAbsorberThickness;
G4String           fSetUp;
G4Material*        fAbsorberMaterial;
G4double fRadThick;
G4int fModuleNumber;   // the number of Rad-et modules

private:
    
  G4VPhysicalVolume* ConstructDetectorXTR(); 
  G4FieldManager* SetQMagField(float field,float angle,float theta);  
  // G4FieldManager* SetQMagField(G4double field_x);  
  G4FieldManager* SetDipoleMagField(G4double fx, G4double fy, G4double fz,float theta);

    
  G4VPhysicalVolume* SimpleSetUpALICE();     
  G4VPhysicalVolume* SetUpALICE06();     
  G4VPhysicalVolume* SetUpBari05();     
  G4VPhysicalVolume* SetUpJLEIC2019();
  G4VPhysicalVolume* SetUpJLEIC09();
  G4VPhysicalVolume* SetUpHarris73(); 
  G4VPhysicalVolume* SetUpWatase86(); 
  G4VPhysicalVolume* SetUpBarr90(); 



  void TestOld();    
                
private:
     
  G4bool             fWorldChanged;
  //G4Material*        fAbsorberMaterial;
  //G4double           fAbsorberThickness;
  G4double           fAbsorberRadius;

  G4Material*        fPipeMat;

  G4Material*        fWindowMat;
  G4double           fWindowThick;

  G4Material*        fElectrodeMat;
  G4double           fElectrodeThick;

  G4Material*        fGapMat;
  G4double           fGapThick;
 
  //G4double           fAbsorberZ;
  //G4double           zstartAbs , zendAbs;
  //G4String           fSetUp;
  // World
  //--------------World -------------------
  G4Box*             fSolidWorld;    //pointer to the solid World 
  G4LogicalVolume*   fLogicWorld;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsWorld;    //pointer to the physical World
  G4Material*        fWorldMaterial;
  G4double           fWorldSizeR;
  G4double           fWorldSizeZ;
  G4double           fWorldVTXshift;
  //--------------solenoid -------------------
  G4double  fSolenoidSizeRin ;
  G4double  fSolenoidSizeRout;
  G4double  fSolenoidSizeZ ;
  G4Tubs*           fSolidSolenoid;      //pointer to the solid 
  G4LogicalVolume*   fLogicSolenoid;    //pointer to the logical 
  G4VPhysicalVolume* fPhysicsSolenoid;  //pointer to the physical 

//-----------------ENDCAP-H  volume--------------------
  G4double           fENDCAP_H_SizeRin;
  G4double           fENDCAP_H_SizeRout;
  G4double           fENDCAP_H_SizeZ;
   G4double          fENDCAP_H_Z;
  G4Tubs*            fSolid_ENDCAP_H;    //pointer to the solid  ENDCAP-H volume
  G4LogicalVolume*   fLogic_ENDCAP_H;    //pointer to the logical  ENDCAP-H  volume
  G4VPhysicalVolume* fPhysics_ENDCAP_H;    //pointer to the physical ENDCAP-H  volume
//-----------------ENDCAP-E  volume--------------------
  G4double           fENDCAP_E_SizeRin;
  G4double           fENDCAP_E_SizeRout;
  G4double           fENDCAP_E_SizeZ;
   G4double          fENDCAP_E_Z;

  G4Tubs*            fSolid_ENDCAP_E;    //pointer to the solid  ENDCAP-E volume
  G4LogicalVolume*   fLogic_ENDCAP_E;    //pointer to the logical ENDCAP-E  volume
  G4VPhysicalVolume* fPhysics_ENDCAP_E;    //pointer to the physical ENDCAP-E  volume

//-----------------E-endcap MRICH  -------------------
  G4double           fMRICHSizeRin;
  G4double           fMRICHSizeRout;
  G4double            fMRICHSizeZ;
   G4double           fMRICH_Z;

  G4Tubs*            fSolid_E_MRICH;    //pointer to the solid  e-endcap MRICH volume
  G4LogicalVolume*   fLogic_E_MRICH;    //pointer to the logical  e-endcap MRICH volume
  G4VPhysicalVolume* fPhysics_E_MRICH;    //pointer to the physical  e-endcap MRICH volume
  //............... MRICH modules ...................... 
  double MRICHLength;
  double MRICHWidth;
  G4double MRICHgap;
  G4VisAttributes *vmrich1,*vmrich2;

  G4Box*  mSolidMRICH;
  G4LogicalVolume* mLogicMRICH;

  //--------------HCAL -------------------
  G4double  fHCALSizeRin ;
  G4double  fHCALSizeRout;
  G4double  fHCALSizeZ ;
  G4Tubs*           fSolidHCAL;      //pointer to the solid 
  G4LogicalVolume*   fLogicHCAL;    //pointer to the logical 
  G4VPhysicalVolume* fPhysicsHCAL;  //pointer to the physical 
    
 //--------------H-endcap -------------------
  G4double  fHendcapSizeRin ;
  G4double  fHendcapSizeRout;
  G4double  fHendcapSizeZ ;
  G4Tubs*           fSolidHendcap;      //pointer to the solid 
  G4LogicalVolume*   fLogicHendcap;    //pointer to the logical 
  G4VPhysicalVolume* fPhysicsHendcap;  //pointer to the physical 
    
   //----------------VTX volume -------------------
  G4double           fVTXSizeRin;
  G4double           fVTXSizeRout;
  G4double           fVTXSizeZ; 
  G4Tubs*           fSolidVTX;    //pointer to the solid VTX volume
  G4LogicalVolume*   fLogicVTX;    //pointer to the logical VTX volume
  G4VPhysicalVolume* fPhysicsVTX;    //pointer to the physical VTX volume
 //----------------CTD  volume -------------------
 G4double           fCTDSizeRin;
  G4double           fCTDSizeRout;
  G4double           fCTDSizeZ; 
  G4Tubs*           fSolidCTD;    //pointer to the solid CTD  volume 
  G4LogicalVolume*   fLogicCTD;    //pointer to the logical CTD  volume
  G4VPhysicalVolume* fPhysicsCTD;    //pointer to the physical CTD  volume


 //-----------------EMCAL barrel volume--------------------
  G4double           fEMCALbSizeRin;
  G4double           fEMCALbSizeRout;
  G4double           fEMCALbSizeZ;
  // G4Tubs*            fSolidEMCALb;    //pointer to the solid  EMCAL barrel volume
  G4Polycone*            fSolidEMCALb;    //pointer to the solid  EMCAL barrel volume
  G4LogicalVolume*   fLogicEMCALb;    //pointer to the logical EMCAL barrel volume
  G4VPhysicalVolume* fPhysicsEMCALb;    //pointer to the physical EMCAL barrel volume
 

  G4double           fEMCALSizeRin;
  G4double           fEMCALSizeRout;
  G4double           fEMCALSizeZ;
  G4Material*        fEMCALMaterial;
  // G4Tubs*            fSolidEMCALbd;    //pointer to the solid World 
    G4Polycone*            fSolidEMCALbd;    //pointer to the solid World 
  G4LogicalVolume*   fLogicEMCALbd;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsEMCALbd;    //pointer to the physical World
 

 //--------------HCAL barrel volume  -------------------
  G4double  fHCALbSizeRin ;
  G4double  fHCALbSizeRout;
  G4double  fHCALbSizeZ ;
  G4Material*        fHCALbMaterial;
  G4Tubs*           fSolidHCALb;      //pointer to the solid 
  G4LogicalVolume*   fLogicHCALb;    //pointer to the logical 
  G4VPhysicalVolume* fPhysicsHCALb;  //pointer to the physical 
    
//-----------------ENDCAP-E GEM  volume--------------------

  G4double            fGEM_E_SizeRin;
  G4double            fGEM_E_SizeRout;
  G4double            fGEM_E_SizeZ;
  G4double            fGEM_E_Z;
  G4Tubs*             fSolidGEM_E;    //pointer to the solid  GEM E-endcap  volume
  G4LogicalVolume*    fLogicGEM_E;    //pointer to the logical GEM E-endcap volume
  G4VPhysicalVolume*  fPhysicsGEM_E;    //pointer to the physical GEM E-endcap volume


  
 //-----------------ENDCAP-H GEM  volume--------------------

  G4double            fGEM_H_SizeRin;
  G4double            fGEM_H_SizeRout;
  G4double            fGEM_H_SizeZ;
  G4double            fGEM_H_Z;
  G4Tubs*             fSolidGEM_H;    //pointer to the solid  GEM H-endcap  volume
  G4LogicalVolume*    fLogicGEM_H;    //pointer to the logical GEM H-endcap volume
  G4VPhysicalVolume*  fPhysicsGEM_H;    //pointer to the physical GEM H-endcap volume
 //-------------------------------------
 //-----------------FARFORWARD GEM  volume--------------------

  G4double            fGEM_FARFORWD_SizeRin;
  G4double            fGEM_FARFORWD_SizeRout;
  G4double            fGEM_FARFORWD_SizeZ;
  G4double            fGEM_FARFORWD_Z,fGEM_FARFORWD_X;
  G4double            fGEM_FARFORWD_Lay_SizeRin;
  G4double            fGEM_FARFORWD_Lay_SizeRout;
  G4double            fGEM_FARFORWD_Lay_SizeZ;
 
  G4Tubs*             fSolidGEM_FARFORWD;    //pointer to the solid  FARFORWD
  G4LogicalVolume*    fLogicGEM_FARFORWD;    //pointer to the logical FARFORWD
  G4VPhysicalVolume*  fPhysicsGEM_FARFORWD;    //pointer to the physical FARFORWD

  G4Tubs*             fSolidGEM_FARFORWD_Lay;    //pointer to the solid  FARFORWD lay
  G4LogicalVolume*    fLogicGEM_FARFORWD_Lay;    //pointer to the logical FARFORWD  lay
  G4VPhysicalVolume*  fPhysicsGEM_FARFORWD_Lay;    //pointer to the physical FARFORWD  lay
 //-----------------FORWARD Si D1  volume--------------------
   // ----- A----
 G4double            fSI_FORWDD1a_SizeRin;
  G4double            fSI_FORWDD1a_SizeRout;
  G4double            fSI_FORWDD1a_SizeZ;

  G4Tubs*             fSolid_SI_FORWDD1a;    //pointer to the solid  FARFORWD
  G4LogicalVolume*    fLogic_SI_FORWDD1a;    //pointer to the logical FARFORWD
  G4VPhysicalVolume*  fPhysics_SI_FORWDD1a;    //pointer to the physical FARFORWD
 
  G4double            fSI_FORWDD1a_SizeRin_Lay;
  G4double            fSI_FORWDD1a_SizeRout_Lay;
  G4double            fSI_FORWDD1a_SizeZ_Lay;

  G4Tubs*             fSolid_SI_FORWDD1a_Lay;    //pointer to the solid  FARFORWD
  G4LogicalVolume*    fLogic_SI_FORWDD1a_Lay;    //pointer to the logical FARFORWD
  G4VPhysicalVolume*  fPhysics_SI_FORWDD1a_Lay;    //pointer to the physical FARFORWD

  // ----- B----
  G4double            fSI_FORWDD1b_SizeRin;
  G4double            fSI_FORWDD1b_SizeRout;
  G4double            fSI_FORWDD1b_SizeZ;

  G4Tubs*             fSolid_SI_FORWDD1b;    //pointer to the solid  FARFORWD
  G4LogicalVolume*    fLogic_SI_FORWDD1b;    //pointer to the logical FARFORWD
  G4VPhysicalVolume*  fPhysics_SI_FORWDD1b;    //pointer to the physical FARFORWD
 
  G4double            fSI_FORWDD1b_SizeRin_Lay;
  G4double            fSI_FORWDD1b_SizeRout_Lay;
  G4double            fSI_FORWDD1b_SizeZ_Lay;

  G4Tubs*             fSolid_SI_FORWDD1b_Lay;    //pointer to the solid  FARFORWD
  G4LogicalVolume*    fLogic_SI_FORWDD1b_Lay;    //pointer to the logical FARFORWD
  G4VPhysicalVolume*  fPhysics_SI_FORWDD1b_Lay;    //pointer to the physical FARFORWD

  //-----------------FORWARD Si D2  volume--------------------
 // ----- B----
  G4double            fSI_FORWDD2_SizeRin;
  G4double            fSI_FORWDD2_SizeRout;
  G4double            fSI_FORWDD2_SizeZ;

  G4Tubs*             fSolid_SI_FORWDD2;    //pointer to the solid  FARFORWD
  G4LogicalVolume*    fLogic_SI_FORWDD2;    //pointer to the logical FARFORWD
  G4VPhysicalVolume*  fPhysics_SI_FORWDD2;    //pointer to the physical FARFORWD
 
  G4double            fSI_FORWDD2_SizeRin_Lay;
  G4double            fSI_FORWDD2_SizeRout_Lay;
  G4double            fSI_FORWDD2_SizeZ_Lay;

  G4Tubs*             fSolid_SI_FORWDD2_Lay;    //pointer to the solid  FARFORWD
  G4LogicalVolume*    fLogic_SI_FORWDD2_Lay;    //pointer to the logical FARFORWD
  G4VPhysicalVolume*  fPhysics_SI_FORWDD2_Lay;    //pointer to the physical FARFORWD

//-------------------------------------
 
  G4double           fEMCALeSizeRin;
  G4double           fEMCALeSizeRout;
  G4double           fEMCALeSizeZ;
  G4Material*        fEMCALeMaterial;
  G4Tubs*            fSolidEMCALe;    //pointer to the solid World 
  G4LogicalVolume*   fLogicEMCALe;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsEMCALe;    //pointer to the physical World
  
  //------------------ EMCAL HAdron endcap ------------------
  G4double           fEMCAL_H_SizeRin;
  G4double           fEMCAL_H_SizeRout;
  G4double           fEMCAL_H_SizeZ;
  G4Material*        fEMCAL_H_Material;
  G4Tubs*            fSolidEMCAL_H;    //pointer to the solid World 
  G4LogicalVolume*   fLogicEMCAL_H;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsEMCAL_H;    //pointer to the physical World
   

   //-------------------TPC barrel detector ------------------
 G4double           fTPCSizeRin;
  G4double           fTPCSizeRout;
  G4double           fTPCSizeZ;
  G4double            fTPCsteps;
  G4Material*        fTPCMaterial;
  
  G4double           TPCRin[20];
  G4double           TPCRout[20];
  G4Tubs*            fSolidTPCBarrel[20];    //pointer to the solid World 
  G4LogicalVolume*   fLogicTPCBarrel[20];    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsTPCBarrel[20];    //pointer to the physical World


   //-------------------CTD_Si barrel detector ------------------
  G4double           fCTD_Si_SizeRin;
  G4double           fCTD_Si_SizeRout;
  G4double           fCTD_Si_SizeZ;
  G4double            fCTD_Si_steps;
  G4Material*        fCTD_Si_Material;
   G4int              fCTD_Si_layers;
   G4double           CTD_Si_Rin[100];
  G4double           CTD_Si_Rout[100];
  G4Tubs*            fSolidCTD_Si_Barrel[100];    //pointer to the solid World 
  G4LogicalVolume*   fLogicCTD_Si_Barrel[100];    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsCTD_Si_Barrel[100];    //pointer to the physical World
 //-------------------CTD_Straw barrel detector ------------------
  G4double           fCTD_Straw_SizeRin;
  G4double           fCTD_Straw_SizeRout;
  G4double           fCTD_Straw_SizeZ;
  G4double            fCTD_Straw_steps;
  G4Material*        fCTD_Straw_Material;
  G4int              fCTD_Straw_layers;
   G4double           CTD_Straw_Rin;
  G4double           CTD_Straw_Rout;
  G4Tubs*            fSolidCTD_Straw_Barrel;    //pointer to the solid World 
  G4LogicalVolume*   fLogicCTD_Straw_Barrel;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsCTD_Straw_Barrel;    //pointer to the physical World
 
  G4Material*        fCTD_Straw_Wall_Material;
  G4Tubs*            fSolidCTD_Straw_Wall_Barrel;    //pointer to the solid World 
  G4LogicalVolume*   fLogicCTD_Straw_Wall_Barrel;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsCTD_Straw_Wall_Barrel;    //pointer to the physical World

  G4Material*        fCTD_Straw_Gas_Material;
  G4Tubs*            fSolidCTD_Straw_Gas_Barrel;    //pointer to the solid World 
  G4LogicalVolume*   fLogicCTD_Straw_Gas_Barrel;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsCTD_Straw_Gas_Barrel;    //pointer to the physical World

  //-------------------VERTEX barrel detector ------------------
   G4Material       *fVTXMaterial;
  G4Tubs*            fSolidVTXBarrel[20];    //pointer to the solid World 
  G4LogicalVolume*   fLogicVTXBarrel[20];    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsVTXBarrel[20];    //pointer to the physical World
 //--------------------Vertex Endcaps ---------------------------

  G4Trap*            fSolidVTXEndE[10]; //pointer to the solid Absorber
  G4LogicalVolume*   fLogicVTXEndE[10]; //pointer to the logical Absorber
  G4VPhysicalVolume* fPhysicsVTXEndE; //pointer to the physical Absorber
  G4Trap*            fSolidVTXEndH[10]; //pointer to the solid Absorber
  G4LogicalVolume*   fLogicVTXEndH[10]; //pointer to the logical Absorber
  G4VPhysicalVolume* fPhysicsVTXEndH; //pointer to the physical Absorber

  //--------------------------------------------------------------


 //-------------------GEM Endcap E  detector ------------------
  G4double            fGEMlay_E_SizeRin[20];
  G4double            fGEMlay_E_SizeRout[20];
  G4double            fGEMlay_E_SizeZ[20];
  G4double             fGEMlay_E_Z[20];
  G4Material          *fGEM_E_Material;
  G4Tubs*            fSolidGEMlay_E[20];    //pointer to the solid World 
  G4LogicalVolume*   fLogicGEMlay_E[20];    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsGEMlay_E[20];    //pointer to the physical World
  //--------------------------------------------------------------
 //-------------------GEM Endcap H  detector ------------------
  G4double            fGEMlay_H_SizeRin[20];
  G4double            fGEMlay_H_SizeRout[20];
  G4double            fGEMlay_H_SizeZ[20];
  G4double             fGEMlay_H_Z[20];
  G4Material          *fGEM_H_Material;
  G4Tubs*            fSolidGEMlay_H[20];    //pointer to the solid World 
  G4LogicalVolume*   fLogicGEMlay_H[20];    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsGEMlay_H[20];    //pointer to the physical World
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

    G4Material*ffqsMaterial;
  G4VisAttributes* vb1, *vb1a,*vb1b,*vb1as;
  G4Material*ffqsMaterial_G;
  double fZ1; 
  //---------------- BEAM QUADS -------------------------------------------
  G4Tubs *fSolid_QUADS_hd_v[40], *fSolid_QUADS_hd_ir[40],*fSolid_QUADS_hd_m[40];  
  G4LogicalVolume *fLogic_QUADS_hd_v[40],*fLogic_QUADS_hd_ir[40],*fLogic_QUADS_hd_m[40]; 
  G4PVPlacement *fPhysics_QUADS_hd_v[40],*fPhysics_QUADS_hd_ir[40],*fPhysics_QUADS_hd_m[40];
  G4FieldManager* fieldMgr_QUADS_hd[40];
   G4RotationMatrix brm_hd[40];
//---------------- BEAM ASOLENOID -------------------------------------------
  G4Tubs *fSolid_ASOLENOID_hd_v[40], *fSolid_ASOLENOID_hd_ir[40],*fSolid_ASOLENOID_hd_m[40];  
  G4LogicalVolume *fLogic_ASOLENOID_hd_v[40],*fLogic_ASOLENOID_hd_ir[40],*fLogic_ASOLENOID_hd_m[40]; 
  G4PVPlacement *fPhysics_ASOLENOID_hd_v[40],*fPhysics_ASOLENOID_hd_ir[40],*fPhysics_ASOLENOID_hd_m[40];
  G4FieldManager* fieldMgr_ASOLENOID_hd[40];
  G4UniformMagField* fMagField_ASOLENOID[40];
  //  G4UniformMagField* fMagField_ASOLENOID[40];      //pointer to the magnetic field
  G4FieldManager* fieldMgrAS[40];
  G4RotationMatrix brm_as[40];

  //---------------- BEAM KICKERS/DIPOLES -------------------------------------------
   G4Tubs *fSolid_BigDi_v[20],*fSolid_BigDi_ir[20],*fSolid_BigDi_m[20];  
  //  G4Box *fSolid_BigDi_v[20],*fSolid_BigDi_ir[20],*fSolid_BigDi_m[20];  
 G4LogicalVolume *fLogic_BigDi_v[20],*fLogic_BigDi_ir[20],*fLogic_BigDi_m[20]; 
  G4PVPlacement *fPhysics_BigDi_v[20],*fPhysics_BigDi_ir[20],*fPhysics_BigDi_m[20];
  G4RotationMatrix brmBigDi[20];
  G4FieldManager *fieldMgr_BigDi[20];
  float fSolid_BigDi_ffqsX[20],fSolid_BigDi_ffqsY[20],fSolid_BigDi_ffqsZ[20];
  float fSolid_BigDi_ffqsSizeZDi[20],fSolid_BigDi_ffqsRinDi[20],fSolid_BigDi_ffqsRoutDi[20];
  char fSolid_BigDi_ffqsNAME[20][256];
  //----------------DIPOLES CHICANE -------------------------------------------
    G4Box *fSolid_Chicane_v[20],*fSolid_Chicane_ir[20],*fSolid_Chicane_m[20];  
 G4LogicalVolume *fLogic_Chicane_v[20],*fLogic_Chicane_ir[20],*fLogic_Chicane_m[20]; 
  G4PVPlacement *fPhysics_Chicane_v[20],*fPhysics_Chicane_ir[20],*fPhysics_Chicane_m[20];
  G4RotationMatrix brmChicane[20];
  G4FieldManager *fieldMgr_Chicane[20];




 G4RotationMatrix brm_hk[20];

 
  //----------------BEAMPIPE elements ----------------------------------------------
  G4Material*  fBeampipeVacuum;

  // ------------inner Be beampipe ------------
  G4double           fBeamSizeRinBe;
  G4double           fBeamSizeRoutBe;
  G4double           fBeamSizeZ_Be;
  G4Material*        fBeamPipeMaterialBe;
 
  //------------Volume -----------
  G4Tubs*           fSolidBeamBe;    //pointer to the solid World 
  G4LogicalVolume*   fLogicBeamBe;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsBeamBe;    //pointer to the physical World
 
   //------------Wall -----------
 G4Tubs*           fSolidBeamBe1;    //pointer to the solid World 
  G4LogicalVolume*   fLogicBeamBe1;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsBeamBe1;    //pointer to the physical World
  //------------Vacuum-----------
  G4Tubs*           fSolidBeamBe2;    //pointer to the solid World 
  G4LogicalVolume*   fLogicBeamBe2;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsBeamBe2;    //pointer to the physical World

  //-----------------------------------------------
  // ------------HAdron beampipe ------------
  G4double           fBeamSizeRinH;
  G4double           fBeamSizeRoutH;
  G4double           fBeamSizeZ_H;
  G4Material*        fBeamPipeMaterialH;
 
  //------------Volume -----------
  G4Tubs*           fSolidBeamH;    //pointer to the solid World 
  G4LogicalVolume*   fLogicBeamH;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsBeamH;    //pointer to the physical World
 
   // ------------Cone inner  beampipe ------------
  G4double           fBeamSizeRin;
  G4double           fBeamSizeRout;
  G4double           fBeamSizeZ;
  G4Material*        fBeamPipeMaterial;

 
  G4double           fBeamSizeRin1Cone;
  G4double           fBeamSizeRin2Cone;
  G4double           fBeamSizeRout1Cone;
  G4double           fBeamSizeRout2Cone;
  G4double           fBeamSizeZCone;
  G4Material*        fBeamPipeConeMaterial;

  G4Polycone*            fSolidBeamCone;    //pointer to the solid World 
  G4LogicalVolume*   fLogicBeamCone;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsBeamCone;    //pointer to the physical World
  

  G4Cons*            fSolidBeamCone2;    //pointer to the solid World 
  G4LogicalVolume*   fLogicBeamCone2;    //pointer to the logical World
  G4VPhysicalVolume* fPhysicsBeamCone2;    //pointer to the physical World
          

  // TR radiator volumes and dimensions
          
  G4Box*             fSolidRadSlice;   // pointer to the solid  z-slice 
  G4LogicalVolume*   fLogicRadSlice;   // pointer to the logical z-slide
  G4VPhysicalVolume* fPhysicRadSlice;  // pointer to the physical z-slide

  G4Box*             fSolidRadRing;    // pointer to the solid  R-slice 
  G4LogicalVolume*   fLogicRadRing;    // pointer to the logical R-slide
  G4VPhysicalVolume* fPhysicRadRing;   // pointer to the physical R-slide

  G4Box*             fSolidRadiator;
  G4LogicalVolume*   fLogicRadiator; 
  G4VPhysicalVolume* fPhysicsRadiator;

  G4Material* fRadiatorMat;        // pointer to the mixed TR radiator material
  G4Material* fFoilMat;            // pointer to the TR foil radiator material
  G4Material* fGasMat;             // pointer to the TR gas radiator material

  G4double fRadThickness;
  G4double fGasGap;
  G4double foilGasRatio;

  G4int fFoilNumber;

  G4double fDetThickness;
  G4double fDetLength;
  G4double fDetGap;

  G4double fStartR;
  G4double fStartZ;

  //G4int fModuleNumber;   // the number of Rad-Det modules

  //G4double fRadThick;
  G4double fMylarThick;
  G4double fPipeLength;
  G4bool   fPipe;
  G4bool   fPipeField;
  G4double fRadZ;
  G4double fWindowZ;
  G4double fGapZ;
  G4double fElectrodeZ;
 
  G4LogicalVolume*   fLogicAbsorber;    //pointer to the logical Absorber
  G4VPhysicalVolume* fPhysicsAbsorber;    //pointer to the physical Absorber
  G4Box*             fSolidAbsorberBarrel[10]; //pointer to the solid Absorber
  G4LogicalVolume*   fLogicAbsorberBarrel[10]; //pointer to the logical Absorber
 
  
  //-- slices and pixels

  G4LogicalVolume*   pxdSlice_log[10]; //pointer to the logical slice
  G4LogicalVolume*   pxdPixel_log[10]; //pointer to the logical pixel
     
  //----
  G4UniformMagField* fMagField;      //pointer to the magnetic field

  // G4double fElectronCut, fGammaCut, fPositronCut;
       
  JLeicDetectorMessenger* fDetectorMessenger;  //pointer to the Messenger
  JLeicCalorimeterSD*     fCalorimeterSD;  //pointer to the sensitive detector
  G4Region*             fRegGasDet;
  G4Region*             fRadRegion;
  JLeicMaterials*        fMat;  

};

#endif







