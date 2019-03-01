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

#define USE_VERTEX
//#define  USE_VTX0 1   // for simple vtx geom
#define USE_VTX_B
#define  USE_VTX_ENDCAP    // for vxt endcaps ladders
//#define  USE_VTX_DISKS    // for vxt disks along beampipe

//#define USE_VTX_E 1   // for vxt endcaps 
#define USE_CTD
#define USE_CTD_Si  1 // silicon version of CTD 
//#define USE_CTD_Straw 1 // straw version of CTD 

#define USE_EMCALb
#define USE_HCALb
#define USE_GEM   // volumes 
#define USE_GEMb  // detectors

#define USE_HCALbdet // hcal detector


//--------H-encap------
#define USE_H_ENDCAP
//------- subdetector-volumes H-encap ----- 
#define USE_H_dRICH
#define USE_H_EMCAL
//#define USE_H_ENDCAP_HCAL
//#define USE_H_ENDCAP_HCAL_D
//--------E-encap------
#define USE_E_ENDCAP
//#define USE_E_ENDCAP_HCAL
//#define USE_E_ENDCAP_HCAL_D
//------- subdetector-volumes E-encap ----- 
#define  USE_E_EMCAL
#define  USE_E_MRICH


//--------FARFORWARD HADRON------
#define USE_DIPOLE1_SI
#define USE_DIPOLE1a_SI
#define USE_DIPOLE1b_SI
#define USE_DIPOLE2_SI

#define USE_FARFORWARD_GEM

//--------barrel------
//#define  USE_VTX0 1   // for simple vtx geom
//#define USE_VTX_E 1   // for vxt endcaps 
//-------------CTD----------
//#define  USE_EMCALb 1 
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
  :fWorldChanged(false), fAbsorberMaterial(0),  fGapMat(0),fSetUp("simpleALICE"),
   fWorldMaterial(0),  fSolidWorld(0),      fLogicWorld(0),      fPhysicsWorld(0),
   fSolidRadSlice(0), fLogicRadSlice(0),  fPhysicRadSlice(0),
   fSolidRadiator(0),  fLogicRadiator(0),   fPhysicsRadiator(0),
   fRadiatorMat(0), fPipe(false), fPipeField(false),
// fSolidAbsorber(0),    fLogicAbsorber(0),   fPhysicsAbsorber(0), 
   fPhysicsCTD(0),
   fMagField(0),       fCalorimeterSD(0),   fRegGasDet(0), fRadRegion(0), fMat(0)
{
  fDetectorMessenger = new JLeicDetectorMessenger(this);
  fMat               = new JLeicMaterials();
}

//////////////////////////////////////////////////////////////////////////
//
//

JLeicDetectorConstruction::~JLeicDetectorConstruction()
{ 
  delete fDetectorMessenger;
  delete fMat;
}

//////////////////////////////////////////////////////////////////////////
//
//

G4VPhysicalVolume* JLeicDetectorConstruction::Construct()
{
  return ConstructDetectorXTR();  
}


/////////////////////////////////////////////////////////////////////////
//
//

G4VPhysicalVolume* JLeicDetectorConstruction::ConstructDetectorXTR()
{
 // Cleanup old geometry

  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4cout<<"USE Experimental setup : " << fSetUp <<G4endl;

  if( fSetUp == "jleic2019" )
  {
    return SetUpJLEIC2019();
  }
  else
  {
    G4cout<<"Experimental setup is unsupported.  "<<G4endl;
    return SetUpJLEIC09();

    //  return 0;
  }
}


//==========================================================================================================
//                              JLEIC 2018 
//==========================================================================================================

/////////////////////////////////////////////////////////////////////////////////
//


G4VPhysicalVolume* JLeicDetectorConstruction::SetUpJLEIC2019()
{
 
char abname[128];

   // Preparation of mixed radiator material

  G4Material* Mylar = fMat->GetMaterial("Mylar");
  G4Material* Air   = fMat->GetMaterial("Air");
  G4Material* Al   = fMat->GetMaterial("Al");
  G4Material* CH2   = fMat->GetMaterial("CH2");
  G4Material* He   = fMat->GetMaterial("He");

 
 
  fAbsorberMaterial = fMat->GetMaterial("Si");

  G4cout <<"G4Material* CH2   = "<<CH2<<G4endl;   G4cout <<"G4Material* Mylar = "<<Mylar<<G4endl;
  G4double foilDensity =  0.91*g/cm3;  // CH2 1.39*g/cm3; // Mylar //  0.534*g/cm3; //Li     
  G4double gasDensity  =  1.2928*mg/cm3; // Air // 1.977*mg/cm3; // CO2 0.178*mg/cm3; // He  
  G4double totDensity  = foilDensity*foilGasRatio + gasDensity*(1.0-foilGasRatio) ;
  G4cout<<"Rad totDensity = "<<totDensity/(g/cm3)<<" g/cm3 "<<G4endl ;
  G4cout<<"use for Rad totDensity = "<<totDensity/(g/cm3)<<" g/cm3 "<<G4endl ;


  G4double fractionFoil =  foilDensity*foilGasRatio/totDensity ; 
  G4double fractionGas  =  gasDensity*(1.0-foilGasRatio)/totDensity ;  
  G4Material* radiatorMat0 = new G4Material("radiatorMat0"  , totDensity, 2);
  radiatorMat0->AddMaterial( CH2, fractionFoil ) ;
  radiatorMat0->AddMaterial( Air, fractionGas  ) ;
  G4double NewDensity = 0.083*(g/cm3);
  G4Material* radiatorMat = new G4Material("radiatorMat",NewDensity,1);
  radiatorMat->AddMaterial(radiatorMat0, 1.) ;
  G4cout<<"new  Rad with totDensity = "<<NewDensity/(g/cm3)<<" g/cm3 "<<G4endl ;

  G4double XTR_density = radiatorMat->GetDensity();
  G4cout<<"Read back Rad totDensity = "<<XTR_density/(g/cm3)<<" g/cm3 "<<G4endl ;
 // default materials of the detector and TR radiator
  fRadiatorMat = radiatorMat; 
  fFoilMat     = CH2; // Kapton; // Mylar ; // Li ; // CH2 ;  
  fGasMat      = Air; // CO2; // He; //   

  G4VisAttributes* vtpc1, *vhcal;

  //===================================================================================
  //==                    create a world                                            ==
  //===================================================================================
 
  fWorldSizeZ = 20000.*cm; 
  fWorldSizeR = 10000.*cm;

  // fWorldMaterial    = Air; 
  fWorldMaterial  = fMat->GetMaterial("G4_Galactic");
  fSolidWorld = new G4Box("World", fWorldSizeR,fWorldSizeR,fWorldSizeZ/2.);
                         
  fLogicWorld = new G4LogicalVolume(fSolidWorld,  fWorldMaterial,  "World");	
                                   
  fPhysicsWorld = new G4PVPlacement(0, G4ThreeVector(), "World",		
                                 fLogicWorld, 0,  false, 0);

    
     //=========================================================================
     //                    Sensitive 
     //=========================================================================
     if( fRegGasDet != 0 ) delete fRegGasDet;
     if( fRegGasDet == 0 )        fRegGasDet = new G4Region("XTRdEdxDetector");  
     
      G4SDManager* SDman = G4SDManager::GetSDMpointer();
     
     if(!fCalorimeterSD)
       {
	 
	 fCalorimeterSD = new JLeicCalorimeterSD("CalorSD",this);
	 SDman->AddNewDetector( fCalorimeterSD );
       }
     
     printf("CalorSD done\n");

     //=========================================================================
     //                   TR radiator envelope 
     //=========================================================================
    printf("Radiator\n");
    fRadThickness = 0.020*mm;    // 16 um // ZEUS NIMA 323 (1992) 135-139, D=20um, dens.= 0.1 g/cm3
    fGasGap       = 0.600*mm;    // for ZEUS  300-publication
    fRadThick = .3*cm - fGasGap + fDetGap;
    fFoilNumber=fRadThick/(fRadThickness + fGasGap);
    fRadZ=-1000*cm;
    foilGasRatio  = fRadThickness/(fRadThickness+fGasGap);
    fAbsorberThickness = 0.050*mm;
    fAbsorberRadius   = 100.*mm;
    fAbsorberZ        = 136.*cm;
    fDetGap       =   0.01*mm ;
    fModuleNumber = 1      ;
    
    fSolidRadiator = new G4Box("Radiator",1*mm, 1*mm, 0.5*fRadThick );                           
    fLogicRadiator = new G4LogicalVolume(fSolidRadiator, fRadiatorMat,      
					 "Radiator");            
    fPhysicsRadiator = new G4PVPlacement(0,
					 G4ThreeVector(0,-60*cm,fRadZ),             
					 "Radiator", fLogicRadiator,                
					 fPhysicsWorld, false,      0 ); 
    
    if( fRadRegion != 0 ) delete fRadRegion; 
    if( fRadRegion == 0 )        fRadRegion = new G4Region("XTRradiator");
    fRadRegion->AddRootLogicalVolume(fLogicRadiator);                   
  
    printf("Radiator done \n");
			
#ifdef  USE_BARREL
  //===================================================================================
  //==                          Solenoid  magnetic field                             ==
  //===================================================================================
  fSolenoidSizeRin=0.0*cm ;
  fSolenoidSizeRout=160.*cm ;
  fSolenoidSizeZ= 400.*cm ;  
  fWorldVTXshift = 40.*cm;

  printf("fSolenoidSizeZ=%f",fSolenoidSizeZ);

  fSolidSolenoid = new G4Tubs("Solenoid", fSolenoidSizeRin,fSolenoidSizeRout,fSolenoidSizeZ/2.,0.,360*deg);                        
                         
  fLogicSolenoid = new G4LogicalVolume(fSolidSolenoid,  fWorldMaterial,  "Solenoid");	
                                   
  fPhysicsSolenoid = new G4PVPlacement(0, G4ThreeVector(0,0,fWorldVTXshift), "Solenoid",	fLogicSolenoid,	
				       fPhysicsWorld,  false, 0);

  // G4VisAttributes* vsol= new G4VisAttributes(G4Color(0.1,0,1.,0.1));
  //   vsol->SetLineWidth(1); vsol->SetForceSolid(true);
  //   fLogicSolenoid->SetVisAttributes(vvtx);
 
  // G4double fieldStrength = 3.0*tesla;  // 0.01*tesla; // field strength in pipe
  G4double fieldStrength = -2.0*tesla;  // 0.01*tesla; // field strength in pipe
  G4double alphaB        = 0.*degree;
  fPipeField     =  true;   // field in helium pipe used?
  if ( fPipeField )
    {
      G4cout<<"Set Magnetic field = " << fieldStrength <<G4endl<<G4endl;
      if( fMagField ) delete fMagField; //delete the existing mag field
      
      fMagField = new G4UniformMagField(G4ThreeVector(fieldStrength*std::sin(alphaB), 
					   0. , fieldStrength*std::cos(alphaB)));
 
      G4FieldManager* fieldMgr = new G4FieldManager(fMagField);
      fieldMgr->SetDetectorField(fMagField);
      fieldMgr->CreateChordFinder(fMagField);
      fLogicSolenoid->SetFieldManager(fieldMgr, true);
    }
  else   G4cout<<"No Magnetic field "<<G4endl<<G4endl;
      vtpc1= new G4VisAttributes(G4Color(0.1,0,0.1,0.4));
      vtpc1->SetLineWidth(1); vtpc1->SetForceSolid(true);
      fLogicSolenoid->SetVisAttributes(vtpc1);
			
#endif

 

  //===================================================================================
  //==                           ELECTRON-ENDCAP                                     ==
  //===================================================================================

  fENDCAP_E_SizeRin=20*cm;
  fENDCAP_E_SizeRout=fSolenoidSizeRout;
  fENDCAP_E_SizeZ=60*cm;
  fENDCAP_E_Z=-fENDCAP_E_SizeZ/2-fSolenoidSizeZ/2+fWorldVTXshift;

#ifdef USE_E_ENDCAP

  fSolid_ENDCAP_E = new G4Tubs("ENDCAP_E",  fENDCAP_E_SizeRin,fENDCAP_E_SizeRout,fENDCAP_E_SizeZ/2.,0.,360*deg);
                         
  fLogic_ENDCAP_E = new G4LogicalVolume(fSolid_ENDCAP_E,  fWorldMaterial,  "ENDCAP_E");	
                                   
  fPhysics_ENDCAP_E = new G4PVPlacement(0, G4ThreeVector(0,0,fENDCAP_E_Z), "ENDCAP_E",fLogic_ENDCAP_E, 		
                                 fPhysicsWorld, false,	0 );	

  vtpc1= new G4VisAttributes(G4Color(0.3,0,3.,0.1));
  vtpc1->SetLineWidth(1); vtpc1->SetForceSolid(true);
  fLogic_ENDCAP_E->SetVisAttributes(vtpc1);
 
 #endif


//===================================================================================

  //===================================================================================
  //==                           HCAL  DETECTOR VOLUME  BARREL                       ==
  //===================================================================================
 #ifdef USE_HCALb
  fHCALbSizeRin=fSolenoidSizeRout ;
  fHCALbSizeRout=fSolenoidSizeRout+100.*cm ;
  fHCALbSizeZ=  fSolenoidSizeZ+fENDCAP_E_SizeZ;
  //fHCALbMaterial = fMat->GetMaterial("StainlessSteel");
  fSolidHCALb = new G4Tubs("HCALb", fHCALbSizeRin,fHCALbSizeRout,fHCALbSizeZ/2.,0.,360*deg);                        
                         
  fLogicHCALb = new G4LogicalVolume(fSolidHCALb,   fWorldMaterial,  "HCALb");	
                                   
  fPhysicsHCALb = new G4PVPlacement(0, G4ThreeVector(0,0,fWorldVTXshift-fENDCAP_E_SizeZ/2), "HCALb",	fLogicHCALb,	
				        fPhysicsWorld,  false, 0);
  
      vtpc1= new G4VisAttributes(G4Color(0.3,0,3.,0.1));
     vtpc1->SetLineWidth(1); vtpc1->SetForceSolid(true);
     fLogicHCALb->SetVisAttributes(vtpc1);
  //  fLogicHCALb->SetVisAttributes(G4VisAttributes::Invisible);	
#endif // end HCALb
 //===================================================================================


 //===================================================================================
//-------------------------------------HCAL Iron ----------------------------------
 //===================================================================================
#ifdef USE_HCALb
#ifdef USE_HCALbdet
      double fHCALbdetSize_z=fHCALbSizeZ;
      double fHCALbdet_thinkess=2*cm;

      fHCALbMaterial= fMat->GetMaterial("Iron");  

  int hlay=0; 
  int NLAY_HCALb=25;
  for (hlay=0;hlay<NLAY_HCALb;hlay++){ 
  
    fHCALbdet_SizeRin =fHCALbSizeRin+ hlay*2*fHCALbdet_thinkess;
    fHCALbdet_SizeRout = fHCALbdet_SizeRin+fHCALbdet_thinkess;
    if(fHCALbdet_SizeRin>fHCALbSizeRout ||  fHCALbdet_SizeRout> fHCALbSizeRout) continue;

    sprintf(abname,"HCAL_B_sol_%d",hlay);
    fSolidHCALbdet = new G4Tubs(abname, fHCALbdet_SizeRin, fHCALbdet_SizeRout, fHCALbdetSize_z/2.,0.,360*deg);                        
    
    sprintf(abname,"HCAL_B_log_%d",hlay); 
    fLogicHCALbdet = new G4LogicalVolume(fSolidHCALbdet,   fWorldMaterial,abname);	
    sprintf(abname,"HCAL_B_ph_%d",hlay); 
    fPhysicsHCALbdet = new G4PVPlacement(0, G4ThreeVector(0,0,0), abname,fLogicHCALbdet,	
				         fPhysicsHCALb,  false, hlay);
     G4VisAttributes* vhcal1= new G4VisAttributes(G4Color(0.6,0,0.6,1));
    vhcal1->SetLineWidth(1); vhcal1->SetForceSolid(true);
    fLogicHCALbdet->SetVisAttributes(vhcal1);
  }


#endif // end HCALbdet
#endif // end HCALb
//===================================================================================


 
 //===================================================================================
  //==                          ELECTRON-ENDCAP    HCAL   DETECTOR VOLUME              ==
  //===================================================================================
#ifdef USE_E_ENDCAP_HCAL
     // G4double fHCAL_ECAP_SizeRi[2]={60*cm, 70*cm } ;
     //  G4double fHCAL_ECAP_SizeRi[2]={0*cm, 0*cm } ;
     // G4double fHCAL_ECAP_SizeRo[2]={fHCALbSizeRout,fHCALbSizeRout }; 
 G4double fHCAL_ECAP_SizeRi=0*cm ;
 G4double fHCAL_ECAP_SizeRo= fHCALbSizeRout; 
 G4double  fHCAL_ECAP_SizeZ=100*cm;
 G4double  fHCAL_ECAP_Zshift=5*cm;
 // G4double  fHCAL_ECAP_Zcone[2]= {fENDCAP_H_SizeZshift+ fENDCAP_H_SizeZ/2+fHCAL_ECAP_Zshift,fENDCAP_H_SizeZshift+ fENDCAP_H_SizeZ/2+fHCAL_ECAP_SizeZ + fHCAL_ECAP_Zshift} ;



 // printf("ECAP_HCAL_det_sol::2  in= %lf %lf  out= %lf %lf z=%lf\n",fHCAL_ECAP_SizeRi[0],fHCAL_ECAP_SizeRi[1],fHCAL_ECAP_SizeRo[0], fHCAL_ECAP_SizeRo[1], fHCAL_ECAP_SizeZ); 

   //fHCALbMaterial = fMat->GetMaterial("StainlessSteel");
 //   fSolid_ECAP_HCAL = new G4Polycone("ECAP_HCAL_vol_sol",0.*deg,360.*deg,2,fHCAL_ECAP_Zcone, fHCAL_ECAP_SizeRi, fHCAL_ECAP_SizeRo);
 fSolid_ECAP_HCAL  = new G4Tubs("ECAP_HCAL_vol_sol", fHCAL_ECAP_SizeRi, fHCAL_ECAP_SizeRo, fHCAL_ECAP_SizeZ/2.,0.,360*deg);                        
    fLogic_ECAP_HCAL = new G4LogicalVolume(fSolid_ECAP_HCAL,  fWorldMaterial,  "ECAP_HCAL_vol_log");	
    //  fPhysics_ECAP_HCAL = new G4PVPlacement(0, G4ThreeVector(0,0,0), "ECAP_HCAL_vol_phys",fLogic_ECAP_HCAL, 		
    //					  fPhysicsWorld, false,	0 );	

   fPhysics_ECAP_HCAL = new G4PVPlacement(0, G4ThreeVector(0,0,fENDCAP_E_Z- fENDCAP_E_SizeZ/2-fHCAL_ECAP_Zshift-fHCAL_ECAP_SizeZ/2.), "ECAP_HCAL_vol_phys",fLogic_ECAP_HCAL, 		
					  fPhysicsWorld, false,	0 );	
  
   vtpc1= new G4VisAttributes(G4Color(0.3,0,3.,0.1));
   vtpc1->SetLineWidth(1); vtpc1->SetForceSolid(true);
     fLogic_ECAP_HCAL->SetVisAttributes(vtpc1);
  //  fLogicHCALb->SetVisAttributes(G4VisAttributes::Invisible);	
#endif // end HCALb
     //---------------------------- ELECTRON-ENDCAP ----HCAL IRON--------------------------------------
#ifdef USE_E_ENDCAP_HCAL
#ifdef USE_E_ENDCAP_HCAL_D

  G4double fHCAL_ECAP_DET_SizeRi= 30*cm ;
  G4double fHCAL_ECAP_DET_SizeRo=fHCAL_ECAP_SizeRo -1*cm; 
 G4double  fHCAL_ECAP_DET_SizeZ=2*cm;
 G4double  fHCAL_ECAP_DET_Zgap=2*cm;
 G4double  fHCAL_ECAP_DET_Z;
 fHCALbMaterial= fMat->GetMaterial("IronAll");  
  printf("ECAP_HCAL_det_sol::1 %f %f ,%f\n",fHCAL_ECAP_DET_SizeRi, fHCAL_ECAP_DET_SizeRo, fHCAL_ECAP_DET_SizeZ); 

 sprintf(abname,"ECAP_HCAL_det_sol");
 fSolid_ECAP_HCAL_D  = new G4Tubs(abname, fHCAL_ECAP_DET_SizeRi, fHCAL_ECAP_DET_SizeRo, fHCAL_ECAP_DET_SizeZ/2.,0.,360*deg);                        

 sprintf(abname,"ECAP_HCAL_det_log"); 
 fLogic_ECAP_HCAL_D = new G4LogicalVolume(fSolid_ECAP_HCAL_D,   fWorldMaterial,abname);	
 
 G4VisAttributes* vehcal1= new G4VisAttributes(G4Color(0.6,0,0.6,1));
 vehcal1->SetLineWidth(1); vehcal1->SetForceSolid(true);
 fLogic_ECAP_HCAL_D->SetVisAttributes(vehcal1);
 
  int ehlay=0; 
  int NLAY_ECAP_HCAL=20;
  for (ehlay=0;ehlay<NLAY_ECAP_HCAL;ehlay++){ 
    fHCAL_ECAP_DET_Z= fHCAL_ECAP_SizeZ/2   -(ehlay+1)*fHCAL_ECAP_DET_SizeZ -(ehlay+1)*5*cm;
    printf("ECAP_HCAL_det_sol::2 %f %f ,%f\n", fHCAL_ECAP_DET_Z,abs(fHCAL_ECAP_DET_Z- fHCAL_ECAP_DET_SizeZ),fHCAL_ECAP_SizeZ/2); 

    if( ( abs (fHCAL_ECAP_DET_Z- fHCAL_ECAP_DET_SizeZ)) > fHCAL_ECAP_SizeZ/2) continue;
    //   fHCAL_ECAP_DET_Z=-fHCAL_ECAP_SizeZ/2+ehlay*fHCAL_ECAP_DET_SizeZ+fHCAL_ECAP_DET_Zgap*ehlay;
    // fHCAL_ECAP_DET_Z= ehlay*fHCAL_ECAP_DET_SizeZ+fHCAL_ECAP_DET_Zgap*ehlay;
     sprintf(abname,"ECAP_HCAL_det_phys_%d",ehlay); 
    fPhysics_ECAP_HCAL_D = new G4PVPlacement(0, G4ThreeVector(0,0, fHCAL_ECAP_DET_Z), abname,fLogic_ECAP_HCAL_D,	
				           fPhysics_ECAP_HCAL,  false, ehlay);
   }


#endif // end HCAL --- ELECTRON-ENDCAP
#endif // end HCAL--- ELECTRON-ENDCAP

 //===================================================================================

 



#ifdef USE_H_ENDCAP
  //===================================================================================
  //==                           HADRON-ENDCAP                                       ==
  //===================================================================================
  fENDCAP_H_SizeRin=20.*cm ;
  fENDCAP_H_SizeRout=fHCALbSizeRout;
  fENDCAP_H_SizeZ= 250*cm ;
  double fENDCAP_H_Zshift=0.*cm; 
  double fENDCAP_H_Xshift=0.*cm; 
  double fENDCAP_H_SizeZshift= fSolenoidSizeZ/2.+fWorldVTXshift+fENDCAP_H_Zshift+fENDCAP_H_SizeZ/2.;
 
  fSolid_ENDCAP_H = new G4Tubs("Hendcap", fENDCAP_H_SizeRin,fENDCAP_H_SizeRout,fENDCAP_H_SizeZ/2.,0.,360*deg);                        
                         
  fLogic_ENDCAP_H = new G4LogicalVolume(fSolid_ENDCAP_H, fWorldMaterial,  "ENDCAP_H");	
                                   
  fPhysics_ENDCAP_H = new G4PVPlacement(0, G4ThreeVector(fENDCAP_H_Xshift,0,fENDCAP_H_SizeZshift), "ENDCAP_H",fLogic_ENDCAP_H,	
				       fPhysicsWorld,  false, 0);

     vtpc1= new G4VisAttributes(G4Color(0.3,0,3.,0.1));
     vtpc1->SetLineWidth(1); vtpc1->SetForceSolid(true);
     fLogic_ENDCAP_H->SetVisAttributes(vtpc1);

 #endif
 //===================================================================================
  //==                          HADRON-ENDCAP    HCAL   DETECTOR VOLUME              ==
  //===================================================================================
#ifdef USE_H_ENDCAP_HCAL
     // G4double fHCAL_HCAP_SizeRi[2]={60*cm, 70*cm } ;
     //  G4double fHCAL_HCAP_SizeRi[2]={0*cm, 0*cm } ;
     // G4double fHCAL_HCAP_SizeRo[2]={fHCALbSizeRout,fHCALbSizeRout }; 
 G4double fHCAL_HCAP_SizeRi=0*cm ;
 G4double fHCAL_HCAP_SizeRo=fHCALbSizeRout; 
 G4double  fHCAL_HCAP_SizeZ=150*cm;
 G4double  fHCAL_HCAP_Zshift=5*cm;
 // G4double  fHCAL_HCAP_Zcone[2]= {fENDCAP_H_SizeZshift+ fENDCAP_H_SizeZ/2+fHCAL_HCAP_Zshift,fENDCAP_H_SizeZshift+ fENDCAP_H_SizeZ/2+fHCAL_HCAP_SizeZ + fHCAL_HCAP_Zshift} ;



 // printf("HCAP_HCAL_det_sol::2  in= %lf %lf  out= %lf %lf z=%lf\n",fHCAL_HCAP_SizeRi[0],fHCAL_HCAP_SizeRi[1],fHCAL_HCAP_SizeRo[0], fHCAL_HCAP_SizeRo[1], fHCAL_HCAP_SizeZ); 

   //fHCALbMaterial = fMat->GetMaterial("StainlessSteel");
 //   fSolid_HCAP_HCAL = new G4Polycone("HCAP_HCAL_vol_sol",0.*deg,360.*deg,2,fHCAL_HCAP_Zcone, fHCAL_HCAP_SizeRi, fHCAL_HCAP_SizeRo);
 fSolid_HCAP_HCAL  = new G4Tubs("HCAP_HCAL_vol_sol", fHCAL_HCAP_SizeRi, fHCAL_HCAP_SizeRo, fHCAL_HCAP_SizeZ/2.,0.,360*deg);                        
    fLogic_HCAP_HCAL = new G4LogicalVolume(fSolid_HCAP_HCAL,  fWorldMaterial,  "HCAP_HCAL_vol_log");	
    //  fPhysics_HCAP_HCAL = new G4PVPlacement(0, G4ThreeVector(0,0,0), "HCAP_HCAL_vol_phys",fLogic_HCAP_HCAL, 		
    //					  fPhysicsWorld, false,	0 );	

   fPhysics_HCAP_HCAL = new G4PVPlacement(0, G4ThreeVector(0,0,fENDCAP_H_SizeZshift+ fENDCAP_H_SizeZ/2+fHCAL_HCAP_Zshift+fHCAL_HCAP_SizeZ/2.), "HCAP_HCAL_vol_phys",fLogic_HCAP_HCAL, 		
					  fPhysicsWorld, false,	0 );	
  
   vtpc1= new G4VisAttributes(G4Color(0.3,0,3.,0.1));
   vtpc1->SetLineWidth(1); vtpc1->SetForceSolid(true);
     fLogic_HCAP_HCAL->SetVisAttributes(vtpc1);
  //  fLogicHCALb->SetVisAttributes(G4VisAttributes::Invisible);	
#endif // end HCALb
     //---------------------------- HADRON-ENDCAP ----HCAL IRON--------------------------------------
#ifdef USE_H_ENDCAP_HCAL
#ifdef USE_H_ENDCAP_HCAL_D

G4double fHCAL_HCAP_DET_SizeRi= 80*cm ;
G4double fHCAL_HCAP_DET_SizeRo=fHCAL_HCAP_SizeRo -1*cm; 
G4double  fHCAL_HCAP_DET_SizeZ=2*cm;
G4double  fHCAL_HCAP_DET_Zgap=2*cm;
G4double  fHCAL_HCAP_DET_Z;
 fHCALbMaterial= fMat->GetMaterial("Iron");  
  printf("HCAP_HCAL_det_sol::1 %f %f ,%f\n",fHCAL_HCAP_DET_SizeRi, fHCAL_HCAP_DET_SizeRo, fHCAL_HCAP_DET_SizeZ); 

 sprintf(abname,"HCAP_HCAL_det_sol");
 fSolid_HCAP_HCAL_D  = new G4Tubs(abname, fHCAL_HCAP_DET_SizeRi, fHCAL_HCAP_DET_SizeRo, fHCAL_HCAP_DET_SizeZ/2.,0.,360*deg);                        

 sprintf(abname,"HCAP_HCAL_det_log"); 
 fLogic_HCAP_HCAL_D = new G4LogicalVolume(fSolid_HCAP_HCAL_D,   fWorldMaterial,abname);	
 
G4VisAttributes* vhcal1= new G4VisAttributes(G4Color(0.6,0,0.6,1));
 vhcal1->SetLineWidth(1); vhcal1->SetForceSolid(true);
 fLogic_HCAP_HCAL_D->SetVisAttributes(vhcal1);
 
  int hhlay=0; 
  int NLAY_HCAP_HCAL=25;
  for (hhlay=0;hhlay<NLAY_HCAP_HCAL;hhlay++){ 
    fHCAL_HCAP_DET_Z= -fHCAL_HCAP_SizeZ/2   +(hhlay+1)*fHCAL_HCAP_DET_SizeZ +(hhlay+1)*5*cm;
    printf("HCAP_HCAL_det_sol::2 %f %f ,%f\n", fHCAL_HCAP_DET_Z,fHCAL_HCAP_DET_Z- fHCAL_HCAP_DET_SizeZ,fHCAL_HCAP_SizeZ); 

   if( (fHCAL_HCAP_DET_Z- fHCAL_HCAP_DET_SizeZ) > fHCAL_HCAP_SizeZ/2) continue;
    //   fHCAL_HCAP_DET_Z=-fHCAL_HCAP_SizeZ/2+hhlay*fHCAL_HCAP_DET_SizeZ+fHCAL_HCAP_DET_Zgap*hhlay;
    // fHCAL_HCAP_DET_Z= hhlay*fHCAL_HCAP_DET_SizeZ+fHCAL_HCAP_DET_Zgap*hhlay;
     sprintf(abname,"HCAP_HCAL_det_phys_%d",hhlay); 
    fPhysics_HCAP_HCAL_D = new G4PVPlacement(0, G4ThreeVector(0,0, fHCAL_HCAP_DET_Z), abname,fLogic_HCAP_HCAL_D,	
				           fPhysics_HCAP_HCAL,  false, hhlay);
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
 
  #ifdef  USE_VERTEX
 //===================================================================================
 //==                          VERTEX DETECTOR VOLUME                               ==
 //===================================================================================
   printf("Begin VERTEX volume \n");
   fVTXSizeRin=3.3*cm;
  fVTXSizeRout=20*cm;
  fVTXSizeZ=50*cm;

  fSolidVTX = new G4Tubs("VTX1",  fVTXSizeRin,fVTXSizeRout,fVTXSizeZ/2.,0.,360*deg);                         
  fLogicVTX = new G4LogicalVolume(fSolidVTX,  fWorldMaterial,  "VTX1");	                                   
  fPhysicsVTX = new G4PVPlacement(0, G4ThreeVector(0,0,-fWorldVTXshift), "VTX1",fLogicVTX, 		
                                 fPhysicsSolenoid, false,	0 );	

  // fLogicVTX->SetVisAttributes(G4VisAttributes::Invisible);	
  G4VisAttributes* vvtx= new G4VisAttributes(G4Color(0.1,0,1.,0.1));
  vvtx->SetLineWidth(1); vvtx->SetForceSolid(true); fLogicVTX->SetVisAttributes(vvtx);

//===================================================================================
  #endif  // end VERTEX 

 #ifdef  USE_CTD
 
 //===================================================================================
 //==                          CTD DETECTOR VOLUME                                  ==
 //===================================================================================
  fCTDSizeRin=21*cm;
  fCTDSizeRout=95*cm;
  fCTDSizeZ=fSolenoidSizeZ -60*cm;

  fSolidCTD = new G4Tubs("CTD",  fCTDSizeRin,fCTDSizeRout,fCTDSizeZ/2.,0.,360*deg);                         
  fLogicCTD = new G4LogicalVolume(fSolidCTD,  fWorldMaterial,  "CTD");	 
  fPhysicsCTD = new G4PVPlacement(0, G4ThreeVector(), "CTD",fLogicCTD, 		
                                 fPhysicsSolenoid, false,	0 );	

  // fLogicCTD->SetVisAttributes(G4VisAttributes::Invisible);	
    G4VisAttributes* vctd1= new G4VisAttributes(G4Color(0.1,0,1.,0.1));
    vctd1->SetLineWidth(1); vctd1->SetForceSolid(true); fLogicCTD->SetVisAttributes(vctd1);
 
//===================================================================================
  #endif  // end CTD


  #ifdef  USE_EMCALb
 //===================================================================================
 //==                          EMCAL DETECTOR VOLUME                               ==
 //===================================================================================
  fEMCALbSizeRin=115.*cm;
  fEMCALbSizeRout=140.*cm;
  // fEMCALbSizeZ=fSolenoidSizeZ -30*cm;
 
  // working   

  //    G4double EMCALB_R_cone[4]={fEMCALbSizeRin,  fEMCALbSizeRout, fEMCALbSizeRout, fEMCALbSizeRin };
  //  G4double EMCALB_Z_cone[4]={ -fSolenoidSizeZ/2, -fSolenoidSizeZ/2+60*cm, fSolenoidSizeZ/2-60*cm, fSolenoidSizeZ/2};
 
  G4double EMCALB_Ro_cone[4]={ fEMCALbSizeRout, fEMCALbSizeRout, fEMCALbSizeRout, fEMCALbSizeRout };
  G4double EMCALB_Ri_cone[4]={ fEMCALbSizeRout-1.*cm, fEMCALbSizeRin, fEMCALbSizeRin, fEMCALbSizeRout-1.*cm };
  G4double EMCALB_Z_cone[4]={-fSolenoidSizeZ/2, -fSolenoidSizeZ/2+30*cm, fSolenoidSizeZ/2-30*cm, fSolenoidSizeZ/2};
 
   fSolidEMCALb = new G4Polycone("EMCALb_vol_solid",0.*deg,360.*deg,4, EMCALB_Z_cone, EMCALB_Ri_cone, EMCALB_Ro_cone);
  fLogicEMCALb = new G4LogicalVolume(fSolidEMCALb,  fWorldMaterial,  "EMCALb_vol_logic");	
  fPhysicsEMCALb = new G4PVPlacement(0, G4ThreeVector(), "EMCALb_vol_phys",fLogicEMCALb, 		
                                 fPhysicsSolenoid, false,	0 );	


    G4VisAttributes* vemcal= new G4VisAttributes(G4Color(0.3,0.5,0.9,1.));
      vemcal->SetLineWidth(1); vemcal->SetForceSolid(true);
      fLogicEMCALb->SetVisAttributes(vemcal);     
  
 
      // fLogicEMCALb->SetVisAttributes(G4VisAttributes::Invisible);	
//...............................detector----------------------------------------------


     fEMCALMaterial = fMat->GetMaterial("PbWO4");
     // fSolidEMCAL = new G4Tubs("EMCALbSol",  fEMCALbSizeRin,fEMCALbSizeRout,fEMCALbSizeZ/2.,0.,360*deg);
     fSolidEMCALbd = new G4Polycone("EMCAL_barrel_solid",0.*deg,360.*deg,4, EMCALB_Z_cone, EMCALB_Ri_cone, EMCALB_Ro_cone);
  
     fLogicEMCALbd = new G4LogicalVolume(fSolidEMCALbd, fEMCALMaterial,  "EMCAL_barrel_Logic");    
    
     // G4VisAttributes* vemcal= new G4VisAttributes(G4Color(0.3,0.5,0.9,0.9));
     //  G4VisAttributes* vemcal= new G4VisAttributes(G4Color(0.7,0.7,0.7,1.));
      vemcal->SetLineWidth(1); vemcal->SetForceSolid(true);
      fLogicEMCALbd->SetVisAttributes(vemcal);     
  
      //    fPhysicsEMCAL = new G4PVPlacement(0, G4ThreeVector(0,0,-40*cm), "EMCALbSolPhys",fLogicEMCAL,          
      //                               fPhysicsEMCALb, false,     0 );    
     
      fPhysicsEMCALbd = new G4PVPlacement(0, G4ThreeVector(0,0,0), "EMCAL_barrel_Phys",fLogicEMCALbd,          
                                      fPhysicsEMCALb, false,     0 );    
  
//...............................detector----------------------------------------------


   #endif  // end EMCALb

 

 #ifdef USE_GEMb
 //====================================================================================
 //==                          GEM DETECTOR VOLUME  CAP-barrel- Hadron side          ==
 //====================================================================================
  fGEM_H_SizeRin=20*cm;
  fGEM_H_SizeRout=65*cm+50*cm;
  fGEM_H_SizeZ=30*cm;
  double fGEM_H_Zshift=0*cm;

  fSolidGEM_H = new G4Tubs("GEM_H",  fGEM_H_SizeRin,fGEM_H_SizeRout,fGEM_H_SizeZ/2.,0.,360*deg);
                         
  fLogicGEM_H = new G4LogicalVolume(fSolidGEM_H,  fWorldMaterial,  "GEM_H");	

  // fGEM_H_Z= fSolenoidSizeZ/2-abs(fWorldVTXshift)+fGEM_H_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed                
  fGEM_H_Z= fSolenoidSizeZ/2-fGEM_H_SizeZ/2;   // --- need to find out why this 5 cm are needed                             
  fPhysicsGEM_H = new G4PVPlacement(0, G4ThreeVector(0,0,fGEM_H_Z), "GEM_H",fLogicGEM_H, 		
                                 fPhysicsSolenoid, false,	0 );	
 
 //===================================================================================
 //==                          GEM DETECTOR VOLUME     CAP-barrel- Electron side    ==
 //===================================================================================
  fGEM_E_SizeRin=20*cm;
  fGEM_E_SizeRout=65*cm+50*cm;
  fGEM_E_SizeZ=30*cm;
  double fGEM_E_Zshift=0*cm;
  fGEM_E_Z=- fSolenoidSizeZ/2+fGEM_E_SizeZ/2;
  // fGEM_E_Z= -fSolenoidSizeZ/2+abs(fWorldVTXshift)- fGEM_E_SizeZ +5*cm;  // --- need to find out why this 5 cm are needed    
  fSolidGEM_E = new G4Tubs("GEM_E",  fGEM_E_SizeRin,fGEM_E_SizeRout,fGEM_E_SizeZ/2.,0.,360*deg);
                         
  fLogicGEM_E = new G4LogicalVolume(fSolidGEM_E,  fWorldMaterial,  "GEM_E");	
                                   
  fPhysicsGEM_E = new G4PVPlacement(0, G4ThreeVector(0,0,fGEM_E_Z), "GEM_E",fLogicGEM_E, 		
                                 fPhysicsSolenoid, false,	0 );	

 //===================================================================================
#endif // end GEMb


#endif  // end BARREL



 //***********************************************************************************
 //***********************************************************************************
 //**                               HADRON ENDCAP DETECTOR  VOLUMES                 **
 //***********************************************************************************
 //***********************************************************************************
 #ifdef USE_H_ENDCAP

 #ifdef USE_H_dRICH
//===================================================================================
 //==                       dRICH     Hadron endcap                                ==
 //==================================================================================
  double f_H_RICHSizeRin= fENDCAP_H_SizeRin;
  double f_H_RICHSizeRout=150*cm;
  double f_H_RICHSizeZ=170*cm;
  
  G4Tubs *fSolid_H_RICH;
  G4LogicalVolume *fLogic_H_RICH;
  G4PVPlacement *fPhysics_H_RICH;
  double f_H_RICHZ= -fENDCAP_H_SizeZ/2.+f_H_RICHSizeZ/2.;
  //    double f_H_RICHZ= 0*cm;

   fSolid_H_RICH = new G4Tubs("_H",  f_H_RICHSizeRin,f_H_RICHSizeRout,f_H_RICHSizeZ/2.,0.,360*deg);
                         
  fLogic_H_RICH = new G4LogicalVolume(fSolid_H_RICH,  fWorldMaterial,  "_H");	
                                   
  fPhysics_H_RICH = new G4PVPlacement(0, G4ThreeVector(0,0,f_H_RICHZ), "_H",fLogic_H_RICH, 		
                                 fPhysics_ENDCAP_H, false,	0 );	

  //  fLogic_H->SetVisAttributes(G4VisAttributes::Invisible);	
 
       vtpc1= new G4VisAttributes(G4Color(1.,1.,0.2,0.2));
       vtpc1->SetLineWidth(1); vtpc1->SetForceSolid(true);
      fLogic_H_RICH->SetVisAttributes(vtpc1);

//===================================================================================
#endif // end USE_H_dRICH

//===================================================================================
//==== space for TRD =============================
//===================================================================================
  double f_H_TRDSizeZ=40*cm;
  G4int i,j,lay=0;

//===================================================================================
//                         EMCAL Hadron endcap  
//===================================================================================
 
#ifdef USE_H_EMCAL

     fEMCAL_H_SizeRin=20*cm;
     fEMCAL_H_SizeRout=200*cm;
     fEMCAL_H_SizeZ=40*cm;   
     double my_z2=-fENDCAP_H_SizeZ/2+f_H_RICHSizeZ+f_H_TRDSizeZ+fEMCAL_H_SizeZ/2; 
     fSolidEMCAL_H = new G4Tubs("H_CAP_EMCAL_Solid",  fEMCAL_H_SizeRin,fEMCAL_H_SizeRout,fEMCAL_H_SizeZ/2.,0.,360*deg);
     fLogicEMCAL_H = new G4LogicalVolume(fSolidEMCAL_H,  fWorldMaterial,  "H_CAP_EMCAL_Logic");   
     G4VisAttributes* vemcal3= new G4VisAttributes(G4Color(0.3,0.5,0.9,0.9));
     vemcal3->SetLineWidth(1); vemcal3->SetForceSolid(false);
     fLogicEMCAL_H->SetVisAttributes(vemcal3);    
  
       fPhysicsEMCAL_H = new G4PVPlacement(0, G4ThreeVector(0,0,my_z2), "H_CAP_EMCAL_Physics",fLogicEMCAL_H,            
                                       fPhysics_ENDCAP_H , false,     0 );    

   
  //--------------------------------------------------------------------
  //                           Ecal modules 
  //--------------------------------------------------------------------
 
  double Ecal_H_Length   = 40.*cm;
  double Ecal_H_Width    = 4.*cm;
  G4double Ecal_H_gap    = 0.01*mm;
 
  fEMCALeMaterial= fMat->GetMaterial("PbWO4");
   G4Box* solidC1 = new G4Box("EMCAL_H_sol",Ecal_H_Width*0.5,Ecal_H_Width*0.5,Ecal_H_Length*0.5);
  G4LogicalVolume* fLogicCal1 = new G4LogicalVolume( solidC1, fWorldMaterial,"EMCAL_H_log");

      G4VisAttributes* vemcal4= new G4VisAttributes(G4Color(0.3,0.5,0.9,0.9));
      // G4VisAttributes* vemcal4= new G4VisAttributes(G4Color(0.1, 1.0, 0.9,0.5));
   vemcal4->SetLineWidth(1); vemcal4->SetForceSolid(true);
   fLogicCal1 ->SetVisAttributes(vemcal4);     
 
  // Crystals

  G4double R0_H1 =  20.*cm;
  G4double R0_H2 =  55.*cm;
  G4double y_Ch  = 0 ;
  G4double x_Ch;
  G4int kh = -1;
  
//============  For sectors =====
for (j=0; j<50; j++) {
   y_Ch -= Ecal_H_Width + Ecal_H_gap;
   x_Ch  = (Ecal_H_Width + Ecal_H_gap)*0.5;
 
   for (i=0; i<50; i++) {
    double R_H=sqrt(x_Ch*x_Ch + y_Ch*y_Ch);

    printf("EMCAL_H::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",kh, j,i, x_Ch,y_Ch, R_H, R0_H1);

    //----------------------- Remove left side (small ring)----------------
    if( R_H  <   fEMCAL_H_SizeRout -Ecal_H_Width + Ecal_H_gap && R_H > R0_H1 ) { 
     printf("EMCAL_H::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",kh, j,i, x_Ch,y_Ch, R_H, R0_H1);
 

      kh++; 	sprintf(abname,"EMCAL_H_ph_%d",kh);

      new G4PVPlacement(0,G4ThreeVector(x_Ch,y_Ch,0.),abname,fLogicCal1,
			fPhysicsEMCAL_H,false,kh);
    kh++;	sprintf(abname,"EMCAL_H_ph_%d",kh);
      new G4PVPlacement(0,G4ThreeVector(x_Ch,-y_Ch,0.),abname,fLogicCal1,
                                    fPhysicsEMCAL_H,false,kh);
    }

     //----------------------- Remove right side (large ring)----------------
     if( R_H  <   fEMCAL_H_SizeRout -Ecal_H_Width + Ecal_H_gap && R_H > R0_H2   ) { 
        printf("EMCAL_H::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",kh, j,i, x_Ch,y_Ch, R_H, R0_H2);
       
     kh++;	sprintf(abname,"EMCAL_H_ph_%d",kh);
      new G4PVPlacement(0,G4ThreeVector(-x_Ch,y_Ch,0.),abname,fLogicCal1,
                                    fPhysicsEMCAL_H,false,kh);
      kh++;	sprintf(abname,"EMCAL_H_ph_%d",kh);
       new G4PVPlacement(0,G4ThreeVector(-x_Ch,-y_Ch,0.),abname,fLogicCal1,
                                    fPhysicsEMCAL_H,false,kh);
    }
    x_Ch += Ecal_H_Width + Ecal_H_gap;
 
   }  
 }
     
#endif 
//===================================================================================



#endif // end USE_H_ENDCAP


 //***********************************************************************************
 //***********************************************************************************
 //**                              ELECTRON  ENDCAP DETECTOR  VOLUMES               **
 //***********************************************************************************
 //***********************************************************************************
 
#ifdef USE_E_ENDCAP

//===================================================================================
//                         mRICH  electron endcap  
//===================================================================================

#ifdef USE_E_MRICH

     fMRICHSizeRin=12*cm;
     fMRICHSizeRout=120*cm;
     fMRICHSizeZ=15*cm;   
     fMRICH_Z  = fENDCAP_E_SizeZ/2-fMRICHSizeZ/2-2*cm; 
     fSolid_E_MRICH = new G4Tubs("E_CAP_MRICH_Solid",  fMRICHSizeRin,fMRICHSizeRout,fMRICHSizeZ/2.,0.,360*deg);
     fLogic_E_MRICH = new G4LogicalVolume( fSolid_E_MRICH,  fWorldMaterial,  "E_CAP_MRICH_Logic");   
     vmrich1= new G4VisAttributes(G4Color(0.3,0.5,0.9,0.9));
     vmrich1->SetLineWidth(1); vmrich1->SetForceSolid(false);
     fLogic_E_MRICH->SetVisAttributes(vmrich1);    
  
     fPhysics_E_MRICH = new G4PVPlacement(0, G4ThreeVector(0,0,fMRICH_Z), "E_CAP_MRICH_Physics",fLogic_E_MRICH,            
                                       fPhysics_ENDCAP_E , false,     0 );    
  //.................................................................................
  //                                MRICH modules  
  //.................................................................................
 
  MRICHLength   = 10.*cm;
  MRICHWidth    = 10.*cm;
  MRICHgap    = 1*cm;
 
  mSolidMRICH = new G4Box("MRICH_sol",MRICHWidth*0.5,MRICHWidth*0.5,MRICHLength*0.5);
  mLogicMRICH = new G4LogicalVolume( mSolidMRICH, fWorldMaterial,"MRICH_log");

  vmrich2= new G4VisAttributes(G4Color(1, 0.6,0.1,0.8));
  vmrich2->SetLineWidth(1); vmrich2->SetForceSolid(true);
  mLogicMRICH ->SetVisAttributes(vmrich2);     
 
  G4double MRICH_R0 =  15.*cm;
  G4double MRICH_R;
  G4double y_mrich  = 0. ;
  G4double x_mrich =0;
  G4int kmrich = -1;
  
//============  For MRICH sectors =====
for (j=0; j<10; j++) {
   y_mrich -= ( MRICHWidth +  MRICHgap);
   x_mrich  = ( MRICHWidth +  MRICHgap)*0.5;
   printf("MRICH0:: x_mrich =%f,  y_mrich=%f\n", x_mrich, y_mrich);
   for (i=0; i<10; i++) {
    double MRICH_R=sqrt(x_mrich*x_mrich + y_mrich*y_mrich);

     printf("MRICH1::kmrich=%d  j=%d i =%d x=%f, y=%f  MRICH_R=%f MRICH_R0=%f \n ",kmrich, j,i, x_mrich,y_mrich, MRICH_R,MRICH_R0);


     if( MRICH_R  <  (fMRICHSizeRout - MRICHWidth - MRICHgap) && MRICH_R > MRICH_R0 ) { 

      printf("MRICH2::kmrich=%d  j=%d i =%d x=%f, y=%f  MRICH_R=%f MRICH_R0=%f \n ",kmrich, j,i, x_mrich,y_mrich, MRICH_R,MRICH_R0);


      kmrich++; 	sprintf(abname,"MRICH_ph_%d",kmrich);
      new G4PVPlacement(0,G4ThreeVector(x_mrich,y_mrich,0.),abname, mLogicMRICH,
			fPhysics_E_MRICH,false,kmrich);
      kmrich++; sprintf(abname,"MRICH_ph_%d",kmrich);
      new G4PVPlacement(0,G4ThreeVector(-x_mrich,y_mrich,0.),abname, mLogicMRICH,
                                    fPhysics_E_MRICH,false,kmrich);
 
      kmrich++; sprintf(abname,"MRICH_ph_%d",kmrich);
      new G4PVPlacement(0,G4ThreeVector(x_mrich,-y_mrich,0.),abname, mLogicMRICH,
                                    fPhysics_E_MRICH,false,kmrich);
      
      kmrich++;    sprintf(abname,"MRICH_ph_%d",kmrich);
       new G4PVPlacement(0,G4ThreeVector(-x_mrich,-y_mrich,0.),abname, mLogicMRICH,
                                    fPhysics_E_MRICH,false,kmrich);
    }
    x_mrich += ( MRICHWidth +  MRICHgap);
 
   }  
 }
//----------------------------end MRICH --------------------------------------------  
#endif 


//===================================================================================
//                         EMCAL Electron endcap  
//===================================================================================

#ifdef USE_E_EMCAL

      fEMCALeSizeRin=12*cm;
     fEMCALeSizeRout=130*cm;
     fEMCALeSizeZ=40*cm;   
     double my_z=-fENDCAP_E_SizeZ/2+fEMCALeSizeZ/2; 
     fSolidEMCALe = new G4Tubs("E_CAP_EMCAL_Solid",  fEMCALeSizeRin,fEMCALeSizeRout,fEMCALeSizeZ/2.,0.,360*deg);
     fLogicEMCALe = new G4LogicalVolume(fSolidEMCALe,  fWorldMaterial,  "E_CAP_EMCAL_Logic");   
     G4VisAttributes* vemcal1= new G4VisAttributes(G4Color(0.3,0.5,0.9,0.9));
     vemcal1->SetLineWidth(1); vemcal1->SetForceSolid(false);
      fLogicEMCALe->SetVisAttributes(vemcal1);    
  
    //   my_z= 0*cm;                                       
      fPhysicsEMCALe = new G4PVPlacement(0, G4ThreeVector(0,0,my_z), "E_CAP_EMCAL_Physics",fLogicEMCALe,            
                                       fPhysics_ENDCAP_E , false,     0 );    


  //------------------------------------------------------------------
  // Ecal module 
  //-------------------------------------------------------------------
 
  double fEcalLength   = 40.*cm;
  double fEcalWidth    = 4.*cm;
  G4double gap    = 0.01*mm;
 
 
  //fEMCALeMaterial= fMat->GetMaterial("DSBCe");
  fEMCALeMaterial= fMat->GetMaterial("PbWO4");
  G4Box* solidC = new G4Box("Ecal",fEcalWidth*0.5,fEcalWidth*0.5,fEcalLength*0.5);
  G4LogicalVolume* fLogicCal = new G4LogicalVolume( solidC, fEMCALeMaterial,"Ecal");

   G4VisAttributes* vemcal2= new G4VisAttributes(G4Color(0.1, 1.0, 0.9,0.5));
   vemcal2->SetLineWidth(1); vemcal2->SetForceSolid(true);
   fLogicCal ->SetVisAttributes(vemcal2);     
 
  // Crystals

  G4double x0 =  0*cm;
  G4double y0 =  0*cm;
  G4double R0 =  15.*cm;
  G4double y_C  = 0 ;
  G4double x_C;
  G4int k = -1;
  
//============  For sectors =====
for (j=0; j<50; j++) {
   y_C -= fEcalWidth + gap;
   x_C  = (fEcalWidth + gap)*0.5;
 
   for (i=0; i<50; i++) {
    double R=sqrt(x_C*x_C + y_C*y_C);

    //   printf("EMCALLL::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, R0);


    if( R  <  fEMCALeSizeRout -fEcalWidth + gap && R > R0 ) { 
      // printf("EMCALLL::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, R0);

      
      k++; 	sprintf(abname,"E_CAP_EMCAL_ph_%d",k);
      new G4PVPlacement(0,G4ThreeVector(x_C,y_C,0.),abname,fLogicCal,
			fPhysicsEMCALe,false,k);
      k++; 	sprintf(abname,"E_CAP_EMCAL_ph_%d",k);
      new G4PVPlacement(0,G4ThreeVector(-x_C,y_C,0.),abname,fLogicCal,
                                    fPhysicsEMCALe,false,k);
 
      k++; 	sprintf(abname,"E_CAP_EMCAL_ph_%d",k);
      new G4PVPlacement(0,G4ThreeVector(x_C,-y_C,0.),abname,fLogicCal,
                                    fPhysicsEMCALe,false,k);
      
      k++; 	sprintf(abname,"E_CAP_EMCAL_ph_%d",k);
       new G4PVPlacement(0,G4ThreeVector(-x_C,-y_C,0.),abname,fLogicCal,
                                    fPhysicsEMCALe,false,k);
    }
    x_C += fEcalWidth + gap;
 
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
  // fBeamSizeZ_Be=fVTXSizeZ;
  fBeamSizeZ_Be=1600*cm;

  /*--------------------------------------*/;
  /*-------Volume for Inner Berilium------*/;
  fBeampipeVacuum  = fMat->GetMaterial("G4_Galactic");
  fSolidBeamBe = new G4Tubs("BeampipeBe", 0,fBeamSizeRoutBe,fBeamSizeZ_Be/2.,0.,360*deg);                        
  fLogicBeamBe = new G4LogicalVolume(fSolidBeamBe, fBeampipeVacuum ,  "BeampipeBe");     
                                   
  G4RotationMatrix  bpr_h;                               
  bpr_h.rotateY((-0.05*180/3.1415)*deg);

  fPhysicsBeamBe = new G4PVPlacement(G4Transform3D(bpr_h, G4ThreeVector(0,0,-fWorldVTXshift)), "BeampipeBe1-b",fLogicBeamBe,                
                                  fPhysicsSolenoid, false,  0 );    
 
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
  fPhysicsBeamH = new G4PVPlacement(G4Transform3D(bpr_h, G4ThreeVector(0,0,-fWorldVTXshift)), "BeampipeH",fLogicBeamH,                
                                  fPhysicsSolenoid, false,  0 );    

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
  G4double Z_cone[4]={ 0.*cm,  0.*cm,  (fSolenoidSizeZ/2),  (fSolenoidSizeZ/2)};
  //fWorldVTXshift
  // Z1=  fBeamSizeZ_Be

  //  printf("zPos=%f,fBeamSizeRin2Cone=%f fBeamSizeRout2Cone=%f fBeamSizeZCone+zPos=%f\n",zPos, 
  //       fBeamSizeRin2Cone,fBeamSizeRout2Cone,fBeamSizeZCone+zPos);

  fSolidBeamCone = new G4Polycone("BeampipeCone", 0., 360., 4, R_cone , Z_cone );
                         
   fLogicBeamCone = new G4LogicalVolume(fSolidBeamCone,  fBeamPipeConeMaterial,  "BeampipeCone");  
 
   //  fPhysicsBeamCone = new G4PVPlacement(0, G4ThreeVector(0,0,-30*cm), "BeampipeCone",fLogicBeamCone,                 
   //                           fPhysicsSolenoid, false,    0 );    
  
  
 
   bpr1.rotateY(-2.5*deg);
  fPhysicsBeamCone = new G4PVPlacement(G4Transform3D(bpr1, G4ThreeVector(0,0,0*cm)), "BeampipeCone",fLogicBeamCone,                 
                              fPhysicsSolenoid, false,    0 );    

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
                                 fPhysicsSolenoid, false,  0 );    
  

   vb1->SetForceSolid(true); fLogicBeamCone2->SetVisAttributes(vb1);fLogicBeamCone->SetVisAttributes(vb1);


  */
 
#endif

//===================================================================================
//                                V E R T E X
//===================================================================================
 int NUM;
  G4double Rx[10],Ry[10]; 
 G4double phi,deltaphi1,deltaphi,deltashi, x, y, z;
 G4double  fVTXZ, fVTXThickness;
 fStartZ=0;
  fVTXMaterial = fMat->GetMaterial("Si");
 //===================================================================================
#ifdef  USE_VTX0    /*----------vtx barrel simple geometry--------------*/ 
//===================================================================================

  int FDIV=0;
 
  G4RotationMatrix rm[10][20], rm1[10][20], rm2[10][20];
   deltaphi1=0; deltaphi=30.*deg; phi=0; x=3*cm; y=0*cm; z=0*cm;
  deltashi=-7.*deg;
  G4double           fAbsorberDX=10*cm;
  G4double           fAbsorberDY=2*cm;
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
	G4VisAttributes* vvtx;
	if(ia<2){      vvtx= new G4VisAttributes(G4Color(0.0,0.2,0.8,2.0));}
	else{       vvtx= new G4VisAttributes(G4Color(1.0-0.1*(ia-2), 1.0, 0.0+0.1*(ia-2),1.1));}
	vvtx->SetLineWidth(1); vvtx->SetForceSolid(true);
        fLogicVTXBarrel[ia]->SetVisAttributes(vvtx);

	sprintf(abname,"BarrelVTX_layer_%d",ia);
	printf("Logic BarrelVTX_layer_%d \n ",ia);
	fPhysicsVTXBarrel[ia] = new G4PVPlacement(0,G4ThreeVector(),
					     abname,fLogicVTXBarrel[ia], 
					     fPhysicsVTX,false,0.);
	//	if (fLogicVTXBarrel[ia])  fLogicVTXBarrel[ia]->SetSenstiveDetector(fCalorimeterSD);
     }


     printf(" I use vtx0, nlay=%d\n",NLAYBARR);
    for (int ia=0;ia<NLAYBARR;ia++) { 

	if (fLogicVTXBarrel[ia]) { 	printf("Sensitive BarrelVTX_layer_%d\n",ia); fLogicVTXBarrel[ia]->SetSensitiveDetector(fCalorimeterSD);  } 
    }
 printf(" end of VERTEX detector \n");



#endif

  
//===================================================================================
#ifdef  USE_VTX_B
 //--------------------------------------------------
 //----------vtx barrel ladder geometry-------------- 
 //-------------------------------------------------- 
//===================================================================================

   G4RotationMatrix rm[10][20], rm1[10][20], rm2[10][20];
    deltaphi1=0; deltaphi=30.*deg; phi=0; x=3*cm; y=0*cm; z=0*cm;
    deltashi=-7.*deg;
    //phi=26.*deg; x=0; y=0; z=fAbsorberZ;
    //phi=0.*deg; x=0; y=0; z=fAbsorberZ;
    G4double           fAbsorberDX=10*cm;
    G4double           fAbsorberDY=2*cm;
     int FDIV=0;
 
    fVTXZ = fStartZ + fRadThick + 2*cm;  //-- Si at dist. 2cm
  int NLAYBARR=6;
  

    for (int lay=0;lay<NLAYBARR;lay++) { 
	//for (int lay=0;lay<1;lay++) { 
      printf("Layer loop:: %d\n",lay);
         if (lay==0) { 
           NUM  = 12;
	   fAbsorberDX=10*cm;
	   fAbsorberDY=2*cm;
	   fVTXThickness = 0.050*mm;
            deltaphi=30.*deg; 
           Rx[lay]=(0.35 +0.005)*cm;
	   Ry[lay]=Rx[lay];
	   
	 }  else if (lay==1)  {
	   NUM  = 14;
	   fAbsorberDX=11*cm;
	   fAbsorberDY=2*cm;
           fVTXThickness = 0.050*mm;
           deltaphi=26.*deg;
           Rx[lay]=(0.35 +0.1 + 0.005)*cm;
	   Ry[lay]=Rx[lay];
	 } else if (lay==2) {
	   NUM  = 12;
	   fAbsorberDX=18*cm;
	   fAbsorberDY=4*cm;
	   fVTXThickness = 0.300*mm;
 
	   deltaphi=30.*deg;
           Rx[lay]=(0.35 +0.3 + 0.005)*cm;
           Ry[lay]=Rx[lay];
	 } else if (lay==3) {
	   NUM  = 12;
	   fAbsorberDX=24*cm;
	   fAbsorberDY=6*cm;
	   fVTXThickness = 0.300*mm;
	   deltaphi=30.*deg;
           Rx[lay]=(0.35 +0.7 + 0.005)*cm;
           Ry[lay]=Rx[lay];
	 } else if (lay==4) {
	   NUM  = 18;
	   fAbsorberDX=36*cm;
	   fAbsorberDY=6*cm;
	   fVTXThickness = 0.300*mm;
	   deltaphi=20.*deg;
           Rx[lay]=(0.35 +1.1 + 0.005)*cm;
           Ry[lay]=Rx[lay];
	 } else if (lay==5) {
	   NUM  = 20;
	   fAbsorberDX=40*cm;
	   fAbsorberDY=6*cm;
	   fVTXThickness = 0.300*mm;
	   deltaphi=18.*deg;
           Rx[lay]=(0.35 +1.4 + 0.005)*cm;
           Ry[lay]=Rx[lay];
	 } else if (lay==6) {
	   NUM  = 24;
	   fAbsorberDX=48*cm;
	   fAbsorberDY=6*cm;
	   deltaphi=15.*deg;
           Rx[lay]=(0.35 +1.6 + 0.005)*cm;
           Ry[lay]=Rx[lay];
	   
	 } else {
	   NUM  = 14;
	   fAbsorberDX=12*cm;
	   fAbsorberDY=6*cm;
	   fVTXThickness = 0.300*mm;
	   deltaphi=26.*deg;
           Rx[lay]=(0.35 +2.0 + 0.005)*cm;
           Ry[lay]=Rx[lay];
	   
	 }

	 
	 sprintf(abname,"Solid_VTX_ladder%d",lay);
	 fSolidAbsorberBarrel[lay] = new G4Box(abname, 
				    fAbsorberDX/2., fAbsorberDY/2., 
				    //10.*mm,10.*mm,
				    fVTXThickness/2.); 
	 
	 sprintf(abname,"Logic_VTX_ladder_%d",lay);
	 fLogicAbsorberBarrel[lay] = new G4LogicalVolume(fSolidAbsorberBarrel[lay], fAbsorberMaterial, 
					      abname);     
	 
	 G4VisAttributes* vs1;
	 if(lay==0 || lay==1 ){  vs1= new G4VisAttributes(G4Color(0.0,0.2,0.8,2.0));}
	    else if(lay==2) {  vs1= new G4VisAttributes(G4Color(0.0,0.2,0.8,0.7));}
         else {	
	   //vs1= new G4VisAttributes(G4Color(1.0-0.1*lay, 1.0, 0.0+0.1*lay,0.1));
	   //	 vs1= new G4VisAttributes(G4Color(1.0-0.1*lay, 1.0, 0.0+0.1*lay,0.1));
	  vs1= new G4VisAttributes(G4Color(0.0+0.1*double(lay-3),1.,1.-0.1*double(lay-3),1.0));
	 }
	 // vs1->SetForceWireframe(true);
	 vs1->SetForceSolid(true);
	 fLogicAbsorberBarrel[lay]->SetVisAttributes(vs1);


	 printf(" %d Rx=%f  Ry=%f deltaphi=%f \n", lay,  Rx[lay], Ry[lay],deltaphi);
	 
	 for (int ia=0;ia<NUM;ia++) {
	   //for (int ia=0;ia<1;ia++) {
	   printf("Module  loop:: %d\n",ia);
	    
	    phi=(ia*(deltaphi));
	    x=-Rx[lay]*cos(phi)*cm;
	    y=-Ry[lay]*sin(phi)*cm;
	    rm[lay][ia].rotateX(-(deltaphi*ia+deltashi));
	    rm[lay][ia].rotateY(90*deg);
	    //WORKIN	rm[lay][ia].rotateX(-(deltaphi*ia+deltashi));
	    //WORKING      rm[lay][ia].rotateY(90*deg);
	    
	    printf(" %d %d x=%f  y=%f  \n", lay, ia, x, y);
	    sprintf(abname,"VTX_ladder%d_%d",lay,ia);
	    fPhysicsAbsorber = new G4PVPlacement(G4Transform3D(rm[lay][ia],G4ThreeVector(x,y,z)),
						 abname,fLogicAbsorberBarrel[lay], 
						 fPhysicsVTX,false,0.);
	    //rm.rotateX(+(deltaphi*ia+deltashi));
	    //   rm.rotateY(-90*deg);
	    //  rm.rotateX(0);
	 }
	    //-------------------------------------------------------------------------
	    //                          VTX  slices and pixels
	    //-------------------------------------------------------------------------
	    G4Box *pxdBox_slice[10];
            G4Box *pxdBox_pixel[10];
	    G4double PixelDX,PixelDY;
            if(lay<2) { 
	       PixelDX=fAbsorberDX/10.; //2000.*um;
	       PixelDY=fAbsorberDY/50.; //2000.*um;
	    } else {
	       PixelDX=fAbsorberDX/50.; //2000.*um;
	       PixelDY=fAbsorberDY/10.; //2000.*um;

	    }
	    //G4double PixelDX=20.*um;
	    //G4double PixelDY=20.*um;
	    //G4double PixelDX=24.*um;
	    //G4double PixelDY=24.*um;
	    G4double PixelDZ=fVTXThickness; // 0.450*mm
	    
	    if (FDIV>=1) {
	      printf("SetUpVertex16():: construct slices %d \n",lay);
	      
	      sprintf(abname,"pxdSlice_%d",lay);
	      pxdBox_slice[lay] = new G4Box(abname,
					      PixelDX/2,                   //gD->GetPixelDX(),
					      fAbsorberDY/2., // 10.*mm,  //gD->GetHalfMPXWaferDY(),
					      fVTXThickness/2.);    //gD->GetHalfMPXWaferDZ());

	      pxdSlice_log[lay] = new G4LogicalVolume(pxdBox_slice[lay], fAbsorberMaterial, abname,0,0,0);
	      
	      G4VisAttributes* pixelVisAtt= new G4VisAttributes(G4Color(0,1,1,1));
	      pixelVisAtt->SetLineWidth(1);
	      pixelVisAtt->SetForceWireframe(true);
	      pxdSlice_log[lay]->SetVisAttributes(pixelVisAtt);
	      
	      
	      // divide in slices
	      sprintf(abname,"pxdSlice_%d",lay);
	      G4PVDivision *sliceDiv = new G4PVDivision(abname,
							 pxdSlice_log[lay],
							 fLogicAbsorberBarrel[lay],
							 kXAxis,
							 PixelDX,
							 0);
	      printf("SetUpVertex16():: construct done\n");


	      if (FDIV>=2 ) {
		printf("SetUpVertex16():: construct pixels \n");
		if(lay<2)  { sprintf(abname,"pxdPixel"); }
		else{ sprintf(abname,"svdPixel");
		}
		
		//sprintf(abname,"pxdPixel_%d",lay);
		pxdBox_pixel[lay] = new G4Box(abname,
						PixelDX/2,
						PixelDY/2.,
						PixelDZ/2.);
		pxdPixel_log[lay]= new G4LogicalVolume(pxdBox_pixel[lay], fAbsorberMaterial, abname,0,0,0);
		pxdPixel_log[lay]->SetVisAttributes(pixelVisAtt);
		
		// divide in pixels
		G4PVDivision * pixelDiv = new G4PVDivision(abname,
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

    lay=0;    
    int NUMF;
   G4RotationMatrix rme[10][20], rme1[10][20], rme2[10][20];
    G4double Fdeltaphi,Ftheta, F2theta;
    G4double RxF[10],RyF[10], RzF[10], RxF2[10],RyF2[10], RzF2[10]; 
    //for simple version     G4double Rzshift =24.;
    G4double Rzshift =24.;

    //  Rx[lay]=(1.4)*cm; Ry[lay]=Rx[lay];
    G4double           fVTX_END_EDY=12*cm;
    G4double           fVTX_END_EDZ=0.05*cm;
    G4double           fVTX_END_EDX1=6*cm;
    G4double           fVTX_END_EDX2=4*cm;
  
     for (lay=0;lay<4;lay++) { 
         if (lay==3) { 
           fVTX_END_EDY=18*cm;
           NUMF  = 24;
           Fdeltaphi=15.*deg;
           Ftheta=-40.*deg;
           RxF[lay]=(1.3)*cm; RyF[lay]=RxF[lay];RzF[lay]=- Rzshift*cm-5.5*cm;
           RxF2[lay]=(1.3)*cm; RyF2[lay]=RxF2[lay];RzF2[lay]= Rzshift*cm+5.5*cm;
         }
         if (lay==2) { 
           NUMF  = 20;
           fVTX_END_EDY=16*cm;
           Fdeltaphi=18.*deg;
          Ftheta=-38.*deg;
           RxF[lay]=(1.1)*cm; RyF[lay]=RxF[lay];RzF[lay]=- Rzshift*cm-1.*cm;
           RxF2[lay]=(1.1)*cm; RyF2[lay]=RxF2[lay];RzF2[lay]= Rzshift*cm+1.*cm;
         }
         if (lay==1) { 
           NUMF  = 18;
           fVTX_END_EDY=14*cm;
           Fdeltaphi=20.*deg;
           Ftheta=-45.*deg;
           RxF[lay]=(1.0)*cm; RyF[lay]=RxF[lay];RzF[lay]=- Rzshift*cm+1.0*cm;
           RxF2[lay]=(1.0)*cm; RyF2[lay]=RxF2[lay];RzF2[lay]= Rzshift*cm-1.0*cm;
         }
          if (lay==0) { 
           NUMF  = 12;
           fVTX_END_EDY=12*cm;
           Fdeltaphi=30.*deg;
           Ftheta=-55.*deg;
           RxF[lay]=(0.8)*cm; RyF[lay]=RxF[lay];RzF[lay]=- Rzshift*cm+7.*cm;
           RxF2[lay]=(0.8)*cm; RyF2[lay]=RxF2[lay];RzF2[lay]= Rzshift*cm-7.*cm;
         }
        printf("x1=%f x2=%f  ,y=%f ,z=%f \n",fVTX_END_EDZ,fVTX_END_EDY+lay*2.,fVTX_END_EDX1,fVTX_END_EDX2);
        sprintf(abname,"Solid_VTX_ladder_END_E%d",lay);
        fSolidVTXEndE[lay] = new G4Trap(abname, fVTX_END_EDZ,
                                    fVTX_END_EDY+lay*2., fVTX_END_EDX1,fVTX_END_EDX2); 
         
	sprintf(abname,"Logic_VTX_ladder_END_E%d",lay);
	fLogicVTXEndE[lay] = new G4LogicalVolume(fSolidVTXEndE[lay], fVTXMaterial, 
						 abname);   
	
	//      G4VisAttributes* vs2= new G4VisAttributes(G4Color(0.0+0.2*double(lay),0.2,0.8-0.2*double(lay),0.5));
         G4VisAttributes* vs2= new G4VisAttributes(G4Color(1.0-0.1*lay, 1.0, 0.0+0.1*lay,0.5));
         //vs2->SetForceWireframe(true);
         vs2->SetForceSolid(true);
         fLogicVTXEndE[lay]->SetVisAttributes(vs2);

   for (int ia=0;ia<NUMF;ia++) { 
  
        //       deltaphi=0;deltashi=0;
        phi=(ia*(Fdeltaphi));
        x=- RxF[lay]*cos(phi)*cm;
        y=- RyF[lay]*sin(phi)*cm;
        z= RzF[lay];
        rme1[lay][ia].rotateX(Ftheta);
        rme1[lay][ia].rotateZ(-90+(Fdeltaphi*(ia+1)));
        //WORKING       rm1[lay][ia].rotateX(-60*deg);
        //WORKING       rm1[lay][ia].rotateZ(-90+(deltaphi*(ia+1)));
        sprintf(abname,"VTX_ladderEnd_%d_%d",lay,ia);
        fPhysicsVTXEndE = new G4PVPlacement(G4Transform3D(rme1[lay][ia],G4ThreeVector(x,y,z)),
                                             abname,fLogicVTXEndE[lay], 
                                             fPhysicsVTX,false,0.);
      }


      printf("x1=%f x2=%f  ,y=%f ,z=%f \n",fVTX_END_EDZ,fVTX_END_EDY,fVTX_END_EDX1,fVTX_END_EDX2);
      sprintf(abname,"Solid_VTX_ladder_END_H%d",lay);
      fSolidVTXEndH[lay] = new G4Trap(abname, fVTX_END_EDZ,
				      fVTX_END_EDY, fVTX_END_EDX1,fVTX_END_EDX2); 
      
      sprintf(abname,"Logic_VTX_ladder_END_H%d",lay);
      fLogicVTXEndH[lay] = new G4LogicalVolume(fSolidVTXEndH[lay], fVTXMaterial, 
					       abname);     
      //      G4VisAttributes* vs3= new G4VisAttributes(G4Color(0.0+0.2*double(lay),0.2,0.8-0.2*double(lay),0.5));
      G4VisAttributes* vs3= new G4VisAttributes(G4Color(1.0-0.1*lay, 1.0, 0.0+0.1*lay,0.5));
      //      vs3->SetForceWireframe(true);
      vs3->SetForceSolid(true);
      fLogicVTXEndH[lay]->SetVisAttributes(vs3);
      
      for (int ia=0;ia<NUMF;ia++) { 


        //       deltaphi=0;deltashi=0;
        phi=(ia*(Fdeltaphi));
        x=- RxF2[lay]*cos(phi)*cm;
        y=- RyF2[lay]*sin(phi)*cm;
        z= RzF2[lay];
        rme2[lay][ia].rotateX(-Ftheta);
        rme2[lay][ia].rotateZ(-90+(Fdeltaphi*(ia+1)));
        //WORKING       rm1[lay][ia].rotateX(-60*deg);
        //WORKING       rm1[lay][ia].rotateZ(-90+(deltaphi*(ia+1)));
        sprintf(abname,"VTX_ladderEnd2_%d_%d",lay,ia);
        fPhysicsVTXEndH = new G4PVPlacement(G4Transform3D(rme2[lay][ia],G4ThreeVector(x,y,z)),
                                             abname,fLogicVTXEndH[lay], 
                                             fPhysicsVTX,false,0.);

 
      }

    	if (fLogicVTXEndE[lay]) {fLogicVTXEndE[lay]->SetSensitiveDetector(fCalorimeterSD);} 
	if (fLogicVTXEndH[lay]) {fLogicVTXEndH[lay]->SetSensitiveDetector(fCalorimeterSD);} 
 

     }

 

        //-----------------------------------------------------------------------




#endif

 printf(" end of VERTEX volume \n");

//============================   end VERTEX    =====================
 

//===================================================================================
//                               _CTD_Si
//===================================================================================
#ifdef USE_CTD_Si

 fCTD_Si_SizeZ=fCTDSizeZ;
 fCTD_Si_steps=5.;
 fCTD_Si_layers=15;
 fCTD_Si_Material=fMat->GetMaterial("Si");
      // ------- layers of Si in CTD
      for (int ia=0;ia<fCTD_Si_layers;ia++) { 
        CTD_Si_Rin[ia]=fCTDSizeRin+(fCTD_Si_steps*ia)*cm;
        CTD_Si_Rout[ia]=fCTDSizeRin+(0.01+fCTD_Si_steps*ia)*cm;
	printf("CTD_Si %d  Rout=%f \n", ia, CTD_Si_Rout[ia]);

        sprintf(abname,"Solid_CTD_Si_layer%d",ia);
        fSolidCTD_Si_Barrel[ia] = new  G4Tubs(abname, CTD_Si_Rin[ia], CTD_Si_Rout[ia],fCTD_Si_SizeZ/2.,0.,360*deg);
         
        sprintf(abname,"Logic_CTD_Si_layer_%d",ia);
        fLogicCTD_Si_Barrel[ia] = new G4LogicalVolume(fSolidCTD_Si_Barrel[ia], 
                                       fCTD_Si_Material, abname);  
   
        //       G4VisAttributes* vtpc= new G4VisAttributes(G4Color(1,0,1,0.2));
        G4VisAttributes* vtpc= new G4VisAttributes(G4Color(1.0-0.1*ia, 1.0, 0.0+0.1*ia,1));
        vtpc->SetLineWidth(1); vtpc->SetForceSolid(true);
        fLogicCTD_Si_Barrel[ia]->SetVisAttributes(vtpc);

        sprintf(abname,"Barrel_CTD_Si_layer_%d",ia);
        fPhysicsCTD_Si_Barrel[ia] = new G4PVPlacement(0,G4ThreeVector(),
                                     abname,fLogicCTD_Si_Barrel[ia], 
                                             fPhysicsCTD,false,0.);
         
      if (fLogicCTD_Si_Barrel[ia])  fLogicCTD_Si_Barrel[ia]->SetSensitiveDetector(fCalorimeterSD);
     }


#endif
//===================================================================================
//                               _CTD_Straw
//===================================================================================
#ifdef USE_CTD_Straw
 printf("begin STRAW volume \n");
 int NUMFs[100];
 G4double Stheta,Sphi,phi_s, Sx, Sy, Sz;;
 G4RotationMatrix rm1s;
 G4double  RxF_Straw[100],RyF_Straw[100],RzF_Straw[100];
 G4double CTD_Str_in=0.3, CTD_Str_out=0.31;
 G4VisAttributes* vstr2[100];

 fCTD_Straw_SizeZ=200*cm;
 fCTD_Straw_layers=20;

 fCTD_Straw_Material=Air;

 CTD_Straw_Rin=CTD_Str_in*cm;
 CTD_Straw_Rout=CTD_Str_out*cm;
 
 sprintf(abname,"Solid_CTD_Straw");
 fSolidCTD_Straw_Barrel = new  G4Tubs(abname,0., CTD_Straw_Rout,fCTD_Straw_SizeZ/2.,0.,360*deg);
 
 sprintf(abname,"Logic_CTD_Straw_layer");
 fLogicCTD_Straw_Barrel = new G4LogicalVolume(fSolidCTD_Straw_Barrel, 
						   fCTD_Straw_Material, abname);  
 G4VisAttributes* vstr= new G4VisAttributes(G4Color(0.5,0.5,0.5,1.));
 vstr->SetLineWidth(1); vstr->SetForceSolid(true);
 fLogicCTD_Straw_Barrel->SetVisAttributes(vstr);

 //-----Walls 
 fCTD_Straw_Wall_Material=fMat->GetMaterial("Mylar");
 sprintf(abname,"Solid_CTD_Straw_Wall");
 fSolidCTD_Straw_Wall_Barrel = new  G4Tubs(abname,0., CTD_Straw_Rout,fCTD_Straw_SizeZ/2.,0.,360*deg);
 
 sprintf(abname,"Logic_CTD_Straw_Wall");
 fLogicCTD_Straw_Wall_Barrel = new G4LogicalVolume(fSolidCTD_Straw_Wall_Barrel, 
						   fCTD_Straw_Wall_Material, abname);  
sprintf(abname,"Phys_CTD_Straw_layer_Wall");
 fPhysicsCTD_Straw_Wall_Barrel = new G4PVPlacement(0, G4ThreeVector(0,0,0),
                                                  fLogicCTD_Straw_Wall_Barrel, abname,
                                                  fLogicCTD_Straw_Barrel, false,   0 );    
 //-----Gas 
 fCTD_Straw_Gas_Material=fMat->GetMaterial("Ar10CO2");
 sprintf(abname,"Solid_CTD_Straw_gas");
 fSolidCTD_Straw_Gas_Barrel = new  G4Tubs(abname,0., CTD_Straw_Rin,fCTD_Straw_SizeZ/2.,0.,360*deg);
 
 sprintf(abname,"Logic_CTD_Straw_layer_gas");
 fLogicCTD_Straw_Gas_Barrel = new G4LogicalVolume(fSolidCTD_Straw_Gas_Barrel, 
						   fCTD_Straw_Gas_Material, abname);  
 
 G4VisAttributes* vstr1= new G4VisAttributes(G4Color(1.,0.5,0.,1.));
 vstr1->SetLineWidth(1); vstr1->SetForceSolid(true);
 fLogicCTD_Straw_Gas_Barrel->SetVisAttributes(vstr1);

 sprintf(abname,"Phys_CTD_Straw_layer_gas");
 fPhysicsCTD_Straw_Gas_Barrel = new G4PVPlacement(0, G4ThreeVector(0,0,0),
                                                  fLogicCTD_Straw_Gas_Barrel, abname,
                                                  fLogicCTD_Straw_Wall_Barrel, false,   0 );    

    if (fLogicCTD_Straw_Gas_Barrel)  fLogicCTD_Straw_Gas_Barrel->SetSensitiveDetector(fCalorimeterSD);


       // ------- layers of Straw in CTD
    int counter=0;
    for (int lay=0;lay<fCTD_Straw_layers;lay++) { 

 	//	RxF_Straw[lay]=fCTDSizeRin;
	//     printf("Straw X=%f\n",	RxF_Straw[lay]);
       RxF_Straw[lay]=fCTDSizeRin+30.+(CTD_Straw_Rout*2*lay)+lay*CTD_Straw_Rout; // position of first layer
	RyF_Straw[lay]=RxF_Straw[lay]+CTD_Straw_Rout/2;
	RzF_Straw[lay]=0*cm;

	// CTD_Straw_Rout[lay]=fCTDSizeRin+(0.01+fCTD_Straw_steps*lay)*cm;
	//	if(lay==0) NUMFs[lay]=213;
	//	if(lay==1) NUMFs[lay]=213;
	NUMFs[lay]=213;
        int j=2*3.1415*RxF_Straw[lay]/2/CTD_Straw_Rout+1;
        printf("NLAY=%d %d %f\n",lay, j ,2*3.1415*RxF_Straw[lay]/2/CTD_Straw_Rout);
	NUMFs[lay]=j;


	Sphi= (CTD_Straw_Rout*2/RxF_Straw[lay]);
	Stheta=0*deg;
 
	for (int ia=0;ia<NUMFs[lay];ia++) { 
  
          phi_s=(ia*(Sphi));
	  Sx=- RxF_Straw[lay]*cos(phi_s);
	  Sy=- RyF_Straw[lay]*sin(phi_s);
	  Sz= 2*RzF_Straw[lay];
	   rm1s.rotateX(Stheta);
	   rm1s.rotateZ(0+(Sphi*(ia+1)));

	   printf("Straw X=%f (%f,%f,%f \n",	RxF_Straw[lay],Sx,Sy,Sz);

	  
	  //       G4VisAttributes* vstr= new G4VisAttributes(G4Color(1,0,1,0.2));
	  
	  //sprintf(abname,"Barrel_CTD_Straw_layer_%d_str_%d",lay,ia);
	  sprintf(abname,"Barrel_CTD_Straw");
	  fPhysicsCTD_Straw_Barrel = new G4PVPlacement(G4Transform3D(rm1s,G4ThreeVector(Sx,Sy,Sz)),
						       abname,
						       fLogicCTD_Straw_Barrel, 
						       fPhysicsCTD,
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
        vstr2[lay]= new G4VisAttributes(G4Color(1.0-0.1*lay, 1.0, 0.0+0.1*lay,1));
        vstr2[lay]->SetLineWidth(1); vstr2[lay]->SetForceSolid(true);
	fLogicCTD_Straw_Barrel->SetVisAttributes(vstr2[lay]);
       }

    
    printf("STRAW end  number=%d\n",counter );

#endif



#ifdef USE_GEMb
#ifdef USE_GEM
//===================================================================================
//                          E-ENDCAP                                               ==
//===================================================================================

//===================================================================================
//                           GEM tracker                                           ==
//===================================================================================
    printf("Begin GEM tracker H-endcap \n");
    //fGEM_H_Material = fMat->GetMaterial("Si");
    fGEM_H_Material  = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----
     printf("GEM tracker E-endcap: end material \n");
   
    for (int lay=0;lay<8;lay++) {

      fGEMlay_H_SizeRin[lay]=fGEM_H_SizeRin+1*cm+(double(lay)*0.5)*cm;
      fGEMlay_H_SizeRout[lay]=fGEM_H_SizeRout-25*cm+(double(lay)*2.)*cm;;

      //      fGEMlay_H_Z[lay]=-fGEM_H_Z/2+(double(lay)*5.)*cm;
        fGEMlay_H_Z[lay]=-fGEM_H_SizeZ/2+5*cm+(double(lay)*3.)*cm;
       fGEMlay_H_SizeZ[lay]=1*cm;

       sprintf(abname,"Solid_GEM_H_layer%d",lay);
       fSolidGEMlay_H[lay] = new  G4Tubs(abname, fGEMlay_H_SizeRin[lay],fGEMlay_H_SizeRout[lay],fGEMlay_H_SizeZ[lay]/2.,0.,360*deg);
 
       sprintf(abname,"Logic_GEM_H_layer_%d",lay);
       fLogicGEMlay_H[lay] = new G4LogicalVolume(fSolidGEMlay_H[lay], 
                                       fGEM_H_Material, abname);  
 
        G4VisAttributes* vgem= new G4VisAttributes(G4Color(0.8,0.4,0.3,0.8));
        vgem->SetLineWidth(1); vgem->SetForceSolid(true);
        fLogicGEMlay_H[lay]->SetVisAttributes(vgem);

        sprintf(abname,"GEM_H_layer_%d",lay);
        fPhysicsGEMlay_H[lay] = new G4PVPlacement(0, G4ThreeVector(0,0,fGEMlay_H_Z[lay]),
                                                  abname,fLogicGEMlay_H[lay], 
                                                  fPhysicsGEM_H, false,   0 );    
         
        if (fLogicGEMlay_H[lay])  fLogicGEMlay_H[lay]->SetSensitiveDetector(fCalorimeterSD);


     }
   
   printf("END GEM tracker H-endcap \n");



    printf("Begin GEM tracker E-endcap \n");
    //fGEM_E_Material = fMat->GetMaterial("Si");
    fGEM_E_Material  = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----
     printf("GEM tracker H-endcap: end material \n");
   
    for (int lay=0;lay<8;lay++) {

      fGEMlay_E_SizeRin[lay]=fGEM_E_SizeRin+1*cm+(double(lay)*0.5)*cm;
      fGEMlay_E_SizeRout[lay]=fGEM_E_SizeRout-25*cm+(double(lay)*2.)*cm;;

      //      fGEMlay_E_Z[lay]=-fGEM_E_Z/2+(double(lay)*5.)*cm;
        fGEMlay_E_Z[lay]=fGEM_E_SizeZ/2-5*cm-(double(lay)*3.)*cm;
       fGEMlay_E_SizeZ[lay]=1*cm;

       sprintf(abname,"Solid_GEM_E_layer%d",lay);
       fSolidGEMlay_E[lay] = new  G4Tubs(abname, fGEMlay_E_SizeRin[lay],fGEMlay_E_SizeRout[lay],fGEMlay_E_SizeZ[lay]/2.,0.,360*deg);
 
       sprintf(abname,"Logic_GEM_E_layer_%d",lay);
       fLogicGEMlay_E[lay] = new G4LogicalVolume(fSolidGEMlay_E[lay], 
                                       fGEM_E_Material, abname);  
 
       G4VisAttributes* vgem= new G4VisAttributes(G4Color(0.8,0.4,0.3,0.8));
        vgem->SetLineWidth(1); vgem->SetForceSolid(true);
        fLogicGEMlay_E[lay]->SetVisAttributes(vgem);

        sprintf(abname,"GEM_E_layer_%d",lay);
        fPhysicsGEMlay_E[lay] = new G4PVPlacement(0, G4ThreeVector(0,0,fGEMlay_E_Z[lay]),
                                                  abname,fLogicGEMlay_E[lay], 
                                                  fPhysicsGEM_E, false,   0 );    
         
        if (fLogicGEMlay_E[lay])  fLogicGEMlay_E[lay]->SetSensitiveDetector(fCalorimeterSD);


     }
   
   printf("END GEM tracker E-endcap \n");



#endif
#endif


  //====================================================================================
 //==                          DIPOLE-1 Tracker and EMCAL                            ==
 //====================================================================================

#ifdef USE_DIPOLE1a_SI
     //-------------------------------------------------------------------------------
     //                      Place Si_disks inside D1a
     //-------------------------------------------------------------------------------
   int mydipole_id;

  for(int id=0;id<20;id++){ 
     if(strcmp(fSolid_BigDi_ffqsNAME[id],"iBDS1a")==0) 
       { printf("found D21=%s  Z=%f dZ=%f Rout=%f \n",fSolid_BigDi_ffqsNAME[id],fSolid_BigDi_ffqsZ[id], fSolid_BigDi_ffqsSizeZDi[id],
                                                      fSolid_BigDi_ffqsRinDi[id]);
         mydipole_id=id;
       };
  };
  fSI_FORWDD1a_SizeRin=0*cm;
  fSI_FORWDD1a_SizeRout=fSolid_BigDi_ffqsRinDi[mydipole_id]*cm;
  fSI_FORWDD1a_SizeZ=30*cm;
 

  fSolid_SI_FORWDD1a = new G4Tubs("SI_FORWDD1a_solid",  fSI_FORWDD1a_SizeRin,fSI_FORWDD1a_SizeRout,fSI_FORWDD1a_SizeZ/2.,0.,360*deg);
                         
  fLogic_SI_FORWDD1a = new G4LogicalVolume(fSolid_SI_FORWDD1a,  fWorldMaterial,  "SI_FORWDD1a_logic");	

  // fGEM_H_Z= fSolenoidSizeZ/2-abs(fWorldVTXshift)+fGEM_H_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed                
   fPhysics_SI_FORWDD1a  = new G4PVPlacement(0, G4ThreeVector(0,0,0), "SI_FORWDD1a_physics",fLogic_SI_FORWDD1a, 		
                                 fPhysics_BigDi_m[mydipole_id], false,	0 );
	
   //  G4VisAttributes* vgemff= new G4VisAttributes(G4Color(0.8,0.4,0.3,0.8));
       vtpc1= new G4VisAttributes(G4Color(0.3,0,3.,0.1));
      vtpc1->SetLineWidth(1); vtpc1->SetForceSolid(true);
    fLogic_SI_FORWDD1a->SetVisAttributes(vtpc1);

 // ---------------------------------------------------------------------------
 //                     Si detectors  
 // ---------------------------------------------------------------------------
  fSI_FORWDD1a_SizeRin_Lay=5*cm;
  fSI_FORWDD1a_SizeRout_Lay= fSI_FORWDD1a_SizeRout-1*cm;
  fSI_FORWDD1a_SizeZ_Lay=1*cm;
 
  fGEM_E_Material  = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----
  
   fSolid_SI_FORWDD1a_Lay = new G4Tubs("fSI_FORWDD1a_Lay_solid",   fSI_FORWDD1a_SizeRin_Lay, fSI_FORWDD1a_SizeRout_Lay, fSI_FORWDD1a_SizeZ_Lay/2.,170.,330*deg);
   fLogic_SI_FORWDD1a_Lay= new G4LogicalVolume(fSolid_SI_FORWDD1a_Lay,  fGEM_E_Material,  "fSI_FORWDD1a_Lay_logic");	
 
 if ( fLogic_SI_FORWDD1a_Lay)   fLogic_SI_FORWDD1a_Lay->SetSensitiveDetector(fCalorimeterSD);
 

  int ffsi_counter=0;
  for(int fflay=0;fflay<5;fflay++) {
    double  Z=- fSI_FORWDD1a_SizeZ/2+(fflay+1)* fSI_FORWDD1a_SizeZ_Lay/2 +(fflay+1)*5*cm;
    fPhysics_SI_FORWDD1a_Lay = new G4PVPlacement(0, G4ThreeVector(0,0,Z),
			       "fSI_FORWDD1a_Lay__phys", fLogic_SI_FORWDD1a_Lay , fPhysics_SI_FORWDD1a, false, ffsi_counter);
	  ffsi_counter++;
	  G4VisAttributes* vsiff= new G4VisAttributes(G4Color(0.8,0.4+0.1*fflay,0.3,1.));
	  vsiff->SetLineWidth(1); vsiff->SetForceSolid(true);
	  fLogic_SI_FORWDD1a_Lay->SetVisAttributes(vsiff);
  }

#endif

#ifdef USE_DIPOLE1b_SI

     //-------------------------------------------------------------------------------
     //                      Place Si_disks inside D1b
     //-------------------------------------------------------------------------------
 
  for(int id=0;id<20;id++){ 
     if(strcmp(fSolid_BigDi_ffqsNAME[id],"iBDS1b")==0) 
       { printf("found D1b=%s  Z=%f dZ=%f Rout=%f \n",fSolid_BigDi_ffqsNAME[id],fSolid_BigDi_ffqsZ[id], fSolid_BigDi_ffqsSizeZDi[id],
                                                      fSolid_BigDi_ffqsRinDi[id]);
         mydipole_id=id;
       };
  };
  fSI_FORWDD1b_SizeRin=0*cm;
  fSI_FORWDD1b_SizeRout=fSolid_BigDi_ffqsRinDi[mydipole_id]*cm;
  fSI_FORWDD1b_SizeZ= fSolid_BigDi_ffqsSizeZDi[mydipole_id]*m;
 

  fSolid_SI_FORWDD1b = new G4Tubs("SI_FORWDD1b_solid",  fSI_FORWDD1b_SizeRin,fSI_FORWDD1b_SizeRout,fSI_FORWDD1b_SizeZ/2.,0.,360*deg);
                         
  fLogic_SI_FORWDD1b = new G4LogicalVolume(fSolid_SI_FORWDD1b,  fWorldMaterial,  "SI_FORWDD1b_logic");	

  // fGEM_H_Z= fSolenoidSizeZ/2-abs(fWorldVTXshift)+fGEM_H_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed                
   fPhysics_SI_FORWDD1b  = new G4PVPlacement(0, G4ThreeVector(0,0,0), "SI_FORWDD1b_physics",fLogic_SI_FORWDD1b, 		
                                 fPhysics_BigDi_m[mydipole_id], false,	0 );
	
   vtpc1= new G4VisAttributes(G4Color(0.3,0,3.,0.1));
   vtpc1->SetLineWidth(1); vtpc1->SetForceSolid(true);
   fLogic_SI_FORWDD1b->SetVisAttributes(vtpc1);

 // ---------------------------------------------------------------------------
 //                     Si detectors  
 // ---------------------------------------------------------------------------
  fSI_FORWDD1b_SizeRin_Lay=5*cm;
  fSI_FORWDD1b_SizeRout_Lay= fSI_FORWDD1b_SizeRout-1*cm;
  fSI_FORWDD1b_SizeZ_Lay=1*cm;
 
  fGEM_E_Material  = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----
  
   fSolid_SI_FORWDD1b_Lay = new G4Tubs("fSI_FORWDD1b_Lay_solid",   fSI_FORWDD1b_SizeRin_Lay, fSI_FORWDD1b_SizeRout_Lay, fSI_FORWDD1b_SizeZ_Lay/2.,170.,330*deg);
   fLogic_SI_FORWDD1b_Lay= new G4LogicalVolume(fSolid_SI_FORWDD1b_Lay,  fGEM_E_Material,  "fSI_FORWDD1b_Lay_logic");	
 
 if ( fLogic_SI_FORWDD1b_Lay)   fLogic_SI_FORWDD1b_Lay->SetSensitiveDetector(fCalorimeterSD);
 

   ffsi_counter=0;
  for(int fflay=0;fflay<5;fflay++) {
    double  Z=- fSI_FORWDD1b_SizeZ/2+(fflay+1)* fSI_FORWDD1b_SizeZ_Lay/2 +(fflay+1)*5*cm;
    fPhysics_SI_FORWDD1b_Lay = new G4PVPlacement(0, G4ThreeVector(0,0,Z),
			       "fSI_FORWDD1b_Lay__phys", fLogic_SI_FORWDD1b_Lay , fPhysics_SI_FORWDD1b, false, ffsi_counter);
	  ffsi_counter++;
	  G4VisAttributes* vsiff= new G4VisAttributes(G4Color(0.8,0.4+0.1*fflay,0.3,1.));
	  vsiff->SetLineWidth(1); vsiff->SetForceSolid(true);
	  fLogic_SI_FORWDD1b_Lay->SetVisAttributes(vsiff);
  }



  //------------------------------------------------------------------
  // Ecal module  Dipole1 
  //-------------------------------------------------------------------
 
  double fEcalLength_D1    = 40.*cm;
  double fEcalWidth_D1     = 4.*cm;
  G4double gap_D1     = 0.01*mm;
 
 
  // fEMCALeMaterial= fMat->GetMaterial("DSBCe");
  fEMCALeMaterial= fMat->GetMaterial("PbWO4");
  G4Box* fSolid_EMCAL_D1 = new G4Box("EMCAL_D1_sol",fEcalWidth_D1*0.5,fEcalWidth_D1*0.5,fEcalLength_D1/2.);
  G4LogicalVolume* fLogic_EMCAL_D1 = new G4LogicalVolume(fSolid_EMCAL_D1, fEMCALeMaterial,"EMCAL_D1_log");

   vemcal2= new G4VisAttributes(G4Color(0.1, 1.0, 0.9,0.5));vemcal2->SetLineWidth(1); vemcal2->SetForceSolid(true);
   fLogic_EMCAL_D1 ->SetVisAttributes(vemcal2);     
 
  // Crystals

   x0 =  0*cm;
   y0 =  0*cm;
   R0 =  5.*cm;
   y_C  = 0 ;
   x_C;
   k = -1;
   z=fSI_FORWDD1b_SizeZ/2-fEcalLength_D1/2;
//============  For sectors =====
for (j=0; j<50; j++) {
   y_C -= fEcalWidth + gap_D1;
   x_C  = (fEcalWidth + gap_D1)*0.5;
   
   for (i=0; i<50; i++) {
     float R=sqrt(x_C*x_C + y_C*y_C);

     // printf("found D1b= EMCAL D1::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, R0);


    if( R  <  fSI_FORWDD1b_SizeRout -fEcalWidth + gap_D1 && R > R0 ) { 
      //  printf("found D1b= EMCAL D1::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, R0);


      k++;	sprintf(abname,"EMCAL_D1_%d",k);

      new G4PVPlacement(0,G4ThreeVector(x_C,y_C,z),abname,fLogic_EMCAL_D1,
			fPhysics_SI_FORWDD1b ,false,k);
      k++;	sprintf(abname,"EMCAL_D1_%d",k);
      new G4PVPlacement(0,G4ThreeVector(-x_C,y_C,z),abname,fLogic_EMCAL_D1,
                                    fPhysics_SI_FORWDD1b,false,k);
 
      k++; 	sprintf(abname,"EMCAL_D1_%d",k);
      new G4PVPlacement(0,G4ThreeVector(x_C,-y_C,z),abname,fLogic_EMCAL_D1,
                                    fPhysics_SI_FORWDD1b,false,k);
      
      k++;	sprintf(abname,"EMCAL_D1_%d",k);
       new G4PVPlacement(0,G4ThreeVector(-x_C,-y_C,z),abname,fLogic_EMCAL_D1,
                                    fPhysics_SI_FORWDD1b,false,k);
    }
    x_C += fEcalWidth + gap_D1;
 
   }  
 }




#endif
 //====================================================================================
 //==                          DETECTOR VOLUME  FAR-FORWARD                           ==
 //====================================================================================


 //====================================================================================
 //==                          Si DETECTOR VOLUME  inside D2                      ==
 //====================================================================================
#ifdef USE_DIPOLE2_SI

     //-------------------------------------------------------------------------------
     //                      Place Si_disks inside D2
     //-------------------------------------------------------------------------------
 
  for(int id=0;id<20;id++){ 
     if(strcmp(fSolid_BigDi_ffqsNAME[id],"iBDS2")==0) 
       { printf("found D1b=%s  Z=%f dZ=%f Rout=%f \n",fSolid_BigDi_ffqsNAME[id],fSolid_BigDi_ffqsZ[id], fSolid_BigDi_ffqsSizeZDi[id],
                                                      fSolid_BigDi_ffqsRinDi[id]);
         mydipole_id=id;
       };
  };
  fSI_FORWDD2_SizeRin=0*cm;
  fSI_FORWDD2_SizeRout=fSolid_BigDi_ffqsRinDi[mydipole_id]*cm;
  fSI_FORWDD2_SizeZ= fSolid_BigDi_ffqsSizeZDi[mydipole_id]*m;
 

  fSolid_SI_FORWDD2 = new G4Tubs("SI_FORWDD2_solid",  fSI_FORWDD2_SizeRin,fSI_FORWDD2_SizeRout,fSI_FORWDD2_SizeZ/2.,0.,360*deg);
                         
  fLogic_SI_FORWDD2 = new G4LogicalVolume(fSolid_SI_FORWDD2,  fWorldMaterial,  "SI_FORWDD2_logic");	

  // fGEM_H_Z= fSolenoidSizeZ/2-abs(fWorldVTXshift)+fGEM_H_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed                
   fPhysics_SI_FORWDD2  = new G4PVPlacement(0, G4ThreeVector(0,0,0), "SI_FORWDD2_physics",fLogic_SI_FORWDD2, 		
                                 fPhysics_BigDi_m[mydipole_id], false,	0 );
	
   vtpc1= new G4VisAttributes(G4Color(0.3,0,3.,0.1));
   vtpc1->SetLineWidth(1); vtpc1->SetForceSolid(true);
   fLogic_SI_FORWDD2->SetVisAttributes(vtpc1);

 // ---------------------------------------------------------------------------
 //                     Si detectors  
 // ---------------------------------------------------------------------------
  fSI_FORWDD2_SizeRin_Lay=0*cm;
  fSI_FORWDD2_SizeRout_Lay= fSI_FORWDD2_SizeRout-1*cm;
  fSI_FORWDD2_SizeZ_Lay=1*cm;
 
  fGEM_E_Material  = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----
  
   fSolid_SI_FORWDD2_Lay = new G4Tubs("fSI_FORWDD2_Lay_solid",   fSI_FORWDD2_SizeRin_Lay, fSI_FORWDD2_SizeRout_Lay, fSI_FORWDD2_SizeZ_Lay/2.,0.,360*deg);
   fLogic_SI_FORWDD2_Lay= new G4LogicalVolume(fSolid_SI_FORWDD2_Lay,  fGEM_E_Material,  "fSI_FORWDD2_Lay_logic");	
 
 if ( fLogic_SI_FORWDD2_Lay)   fLogic_SI_FORWDD2_Lay->SetSensitiveDetector(fCalorimeterSD);
 

   ffsi_counter=0;
   //------------------ in front ------ 
  for(int fflay=0;fflay<5;fflay++) {
    double  Z=- fSI_FORWDD2_SizeZ/2+(fflay+1)* fSI_FORWDD2_SizeZ_Lay/2 +(fflay+1)*5*cm;
    fPhysics_SI_FORWDD2_Lay = new G4PVPlacement(0, G4ThreeVector(0,0,Z),
			       "fSI_FORWDD2_Lay_phys", fLogic_SI_FORWDD2_Lay , fPhysics_SI_FORWDD2, false, ffsi_counter);
	  ffsi_counter++;
	  G4VisAttributes* vsiff= new G4VisAttributes(G4Color(0.8,0.4+0.1*fflay,0.3,1.));
	  vsiff->SetLineWidth(1); vsiff->SetForceSolid(true);
	  fLogic_SI_FORWDD2_Lay->SetVisAttributes(vsiff);
  }

  //------------------ in middle  ------ 
  for(int fflay=0;fflay<5;fflay++) {
    double  Z= (fflay+1)* fSI_FORWDD2_SizeZ_Lay/2 +(fflay+1)*5*cm;
    fPhysics_SI_FORWDD2_Lay = new G4PVPlacement(0, G4ThreeVector(0,0,Z),
			       "fSI_FORWDD2_Lay_phys", fLogic_SI_FORWDD2_Lay , fPhysics_SI_FORWDD2, false, ffsi_counter);
	  ffsi_counter++;
	  G4VisAttributes* vsiff= new G4VisAttributes(G4Color(0.8,0.4+0.1*fflay,0.3,1.));
	  vsiff->SetLineWidth(1); vsiff->SetForceSolid(true);
	  fLogic_SI_FORWDD2_Lay->SetVisAttributes(vsiff);
  }
#endif
 //====================================================================================
 //==                          GEM DETECTOR VOLUME  after  D2                      ==
 //====================================================================================
#
#ifdef USE_FARFORWARD_GEM

  fGEM_FARFORWD_SizeRin=0*cm;
  fGEM_FARFORWD_SizeRout=90*cm;
  fGEM_FARFORWD_SizeZ=30*cm;
   //double fGEM_FARFORWD_Zshift=0*cm;
  // fGEM_FARFORWD_Z= 4000*cm;
 
  for(int id=0;id<20;id++){ 
     if(strcmp(fSolid_BigDi_ffqsNAME[id],"iBDS2")==0) 
       { //printf("found D2 =%s  Z=%f dZ=%f \n",fSolid_BigDi_ffqsNAME[id],fSolid_BigDi_ffqsZ[id], fSolid_BigDi_ffqsSizeZDi[id]);
	 fGEM_FARFORWD_Z=fSolid_BigDi_ffqsZ[id] *m + fSolid_BigDi_ffqsSizeZDi[id]/2*m+ fGEM_FARFORWD_SizeZ/2+10*cm; 
  	 fGEM_FARFORWD_X=fSolid_BigDi_ffqsX[id] *m -10*cm; 
	 //  printf("found D2 =%f \n", fGEM_FARFORWD_Z);
    };
  };
                         

  fSolidGEM_FARFORWD = new G4Tubs("GEM_FARFORWD_solid",  fGEM_FARFORWD_SizeRin,fGEM_FARFORWD_SizeRout,fGEM_FARFORWD_SizeZ/2.,0.,360*deg);
                         
  fLogicGEM_FARFORWD = new G4LogicalVolume(fSolidGEM_FARFORWD,  fWorldMaterial,  "GEM_FARFORWD_logic");	

  // fGEM_H_Z= fSolenoidSizeZ/2-abs(fWorldVTXshift)+fGEM_H_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed                
   fPhysicsGEM_FARFORWD  = new G4PVPlacement(0, G4ThreeVector(fGEM_FARFORWD_X,0,fGEM_FARFORWD_Z), "GEM_FARFORWD_physics",fLogicGEM_FARFORWD, 		
                                 fPhysicsWorld, false,	0 );	
     vtpc1= new G4VisAttributes(G4Color(0.3,0,3.,0.1));
     vtpc1->SetLineWidth(1); vtpc1->SetForceSolid(true);
     fLogicGEM_FARFORWD->SetVisAttributes(vtpc1);
 // ---------------------------------------------------------------------------
 //                     GEM detectors  
 // ---------------------------------------------------------------------------
  fGEM_FARFORWD_Lay_SizeRin=0*cm;
  fGEM_FARFORWD_Lay_SizeRout=45*cm;
  fGEM_FARFORWD_Lay_SizeZ=1*cm;
  fGEM_E_Material  = fMat->GetMaterial("Ar10CO2");  //----   !!!!! ----
  
  fSolidGEM_FARFORWD_Lay = new G4Tubs("GEM_FARFORWD_Lay_solid",  fGEM_FARFORWD_Lay_SizeRin,fGEM_FARFORWD_Lay_SizeRout,fGEM_FARFORWD_Lay_SizeZ/2.,0.,360*deg);
   fLogicGEM_FARFORWD_Lay= new G4LogicalVolume(fSolidGEM_FARFORWD_Lay,  fGEM_E_Material,  "GEM_FARFORWD_Lay_logic");	
 
 if ( fLogicGEM_FARFORWD_Lay)   fLogicGEM_FARFORWD_Lay->SetSensitiveDetector(fCalorimeterSD);
 
  G4VisAttributes* vgemff= new G4VisAttributes(G4Color(0.8,0.4,0.3,0.8));
  vgemff->SetLineWidth(1); vgemff->SetForceSolid(true);
   fLogicGEM_FARFORWD_Lay->SetVisAttributes(vgemff);

   //---------------------------- after D2-----------------------
  int ff_counter=0;
  for(int fflay=0;fflay<5;fflay++) {
    double  Z=-fGEM_FARFORWD_SizeZ/2+(fflay+1)*fGEM_FARFORWD_Lay_SizeZ/2 +(fflay+1)*5*cm;
    fPhysicsGEM_FARFORWD_Lay = new G4PVPlacement(0, G4ThreeVector(0,0,Z),
			       "GEM_FARFORWD_Lay_phys",fLogicGEM_FARFORWD_Lay , fPhysicsGEM_FARFORWD, false, ff_counter);
	  ff_counter++;
  }
 
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
  g4Factory.Import(fPhysicsWorld);
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


  return fPhysicsWorld;
}



//==========================================================================================================
//                                      JLEIC   END
//==========================================================================================================



G4VPhysicalVolume* JLeicDetectorConstruction::SetUpJLEIC09()
{
 }



////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::PrintGeometryParameters()
{
  G4cout << "\n The  WORLD   is made of " 
       << fWorldSizeZ/mm << "mm of " << fWorldMaterial->GetName() ;
  G4cout << ", the transverse size (R) of the world is " << fWorldSizeR/mm << " mm. " << G4endl;
   G4cout<<"WorldMaterial = "<<fWorldMaterial->GetName()<<G4endl ;
  //  G4cout<<"fVTX_END_Z = "<<fVTX_END_Z/mm<<" mm"<<G4endl;
  G4cout << G4endl;
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetAbsorberMaterial(G4String materialChoice)
{
  // get the pointer to the material table
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name   
  G4Material* pttoMaterial;

  for (size_t J=0 ; J<theMaterialTable->size() ; J++)
  { 
    pttoMaterial = (*theMaterialTable)[J]; 
    
    if(pttoMaterial->GetName() == materialChoice)
    {
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

  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name
   
  G4Material* pttoMaterial;
  for (size_t J=0 ; J<theMaterialTable->size() ; J++)
  { 
    pttoMaterial = (*theMaterialTable)[J];
     
    if(pttoMaterial->GetName() == materialChoice)
    {
      fRadiatorMat = pttoMaterial;
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
  const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();

  // search the material by its name   
  G4Material* pttoMaterial;

  for (size_t J=0 ; J<theMaterialTable->size() ; J++)
  { 
    pttoMaterial = (*theMaterialTable)[J];
     
    if(pttoMaterial->GetName() == materialChoice)
    {
      fWorldMaterial = pttoMaterial;
      fLogicWorld->SetMaterial(pttoMaterial); 
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
  fAbsorberThickness = val;
  //  ComputeCalorParameters();
}  

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetRadiatorThickness(G4double val)
{
  // change XTR radiator thickness and recompute the calorimeter parameters
  fRadThickness = val;
  // ComputeCalorParameters();
}
  
///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetGasGapThickness(G4double val)
{
  // change XTR gas gap thickness and recompute the calorimeter parameters
  fGasGap = val;
  // ComputeCalorParameters();
}  

/////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetAbsorberRadius(G4double val)
{
  // change the transverse size and recompute the calorimeter parameters
  fAbsorberRadius = val;
  // ComputeCalorParameters();
}  

////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetWorldSizeZ(G4double val)
{
  fWorldChanged=true;
  fWorldSizeZ = val;
  // ComputeCalorParameters();
}  

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetWorldSizeR(G4double val)
{
  fWorldChanged=true;
  fWorldSizeR = val;
  // ComputeCalorParameters();
}  

//////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetAbsorberZpos(G4double val)
{
  fAbsorberZ  = val;
  // ComputeCalorParameters();
}  

//////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorConstruction::SetMagField(G4double)
{
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
G4FieldManager* JLeicDetectorConstruction::SetQMagField(float field, float skew, float theta)
//
//
////////////////////////////////////////////////////////////////////////////

{
  G4RotationMatrix *qrm_f;
  printf("SetQMagField:: got parameters %f %f \n",field, skew);
 
   
  //G4double fGradient = -223.*tesla/m;
  G4double fGradient = sqrt(field*field+  skew*skew)*tesla/m;
  G4double angle;
  if( field!=0) {angle = 0.5*atan2(skew,field);} 
  else { 
    if (skew!=0) { angle=0.5*(3.1415/2-atan2(field,skew));}
      else {angle=0;}
  } 

  qrm_f= new G4RotationMatrix();
  qrm_f->rotateZ(angle*rad);
  qrm_f->rotateY(theta*rad);

  //G4ThreeVector fieldValue = G4ThreeVector( fGradient_x,fGradient_y,fGradient_z);
  // old  G4QuadrupoleMagField* pipeField = new G4QuadrupoleMagField(fGradient);
  
  G4ThreeVector fieldorigin = G4ThreeVector(0,0,0);

  G4QuadrupoleMagField* pipeField = new G4QuadrupoleMagField(fGradient,fieldorigin,qrm_f);

  G4Mag_UsualEqRhs* myEquation = new G4Mag_UsualEqRhs(pipeField);
  G4MagIntegratorStepper* myStepper = new G4ClassicalRK4(myEquation);
  G4ChordFinder* myChordFinder = new G4ChordFinder(pipeField,0.1*mm,myStepper);
  
  G4FieldManager* fieldMgr = new G4FieldManager(pipeField,myChordFinder,false);
  
  //logicSpace_Pipe->SetFieldManager(fieldMgr,true);

/*  
G4Box *solidB = new G4Bo"solidB", 1*m, 1*m, 1*m);
G4LogicalVolume* logicB = new G4LogicalVolume(solidB, Mat("G4_Galactic"), "logicB",fieldMgrq);
new G4PVPlacement(0, G4ThreeVector(5*m,5*m,-7*m), logicB,
                                           "physiB", world->getLogic(), false, 0);
*/
  delete qrm_f;
  return fieldMgr;
}
G4FieldManager*  JLeicDetectorConstruction::SetDipoleMagField(G4double fx, G4double fy, G4double fz,float theta)
{
  // G4Box  *sMagField = new G4Bo "dipole_magfield", 10/2*cm, 10/2*cm, 10/2*cm);
  //  G4LogicalVolume  *lMagField = new G4LogicalVolume(sMagField, fWorldMaterial, "logical_magnet");

  double ffx, ffy,ffz;
  ffx=fx*cos(theta*rad)+fz*sin(theta*rad); 
  ffy=fy;
  ffz=-fx*sin(theta*rad)+fz*cos(theta*rad);

  G4ThreeVector fieldValue = G4ThreeVector(ffx*tesla,ffy*tesla,ffz*tesla);
  G4UniformMagField *magField = new G4UniformMagField(fieldValue);
  G4FieldManager *fieldMgr = new G4FieldManager(magField);
  fieldMgr->SetDetectorField(magField);
  fieldMgr->CreateChordFinder(magField);
  //accuracy mag.
  G4double minEps= 1.0e-5; // Minimum & value for smallest steps
  G4double maxEps= 1.0e-4; // Maximum & value for largest steps
  fieldMgr->SetMinimumEpsilonStep(minEps);
  fieldMgr->SetMaximumEpsilonStep(maxEps);
  fieldMgr->SetDeltaOneStep(0.5*um); // 0.5 micrometer
  return fieldMgr;
//  new G4PVPlacement(0, G4ThreeVector(0,0,zpos*cm), lMagField, "Magnet",fLogicWorld, false, 0);
}
///////////////////////////////////////////////////////////////////////////////
//
//
  
void JLeicDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructDetectorXTR());
}

//
//
////////////////////////////////////////////////////////////////////////////



void  JLeicDetectorConstruction::Read_Di_File(){
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
FILE* rc;
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
  float   ffqsRinDiG;
   float   ffqsRinDi;
   float   ffqsRoutDi;
  float  ffqsSizeZDi;
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

 printf("read Di file\n");
 // sprintf(fname,"ion_ir_06feb19.txt");
 sprintf(fname,"i_ir.txt");
 rc=fopen(fname,"r");
 if (rc == NULL) return;


 // int iq=0,ik=0,is=0;
 printf("read Di file %s\n",fname);
 while(fgets(buffer, 512, (FILE*) rc)) {
   
   printf("*********************************************************************************\n");
   printf("%s\n", buffer);
   sscanf(buffer, "%s",mychar); 
   printf("mychar=%s\n ",mychar);
   if(mychar[0] !='i' || mychar[0] =='\n')  { printf("SKIP LINE %s\n", buffer); continue;}

   sscanf(buffer, "%s %s %f %f %f %f %f %f  %f %f %f %f %f  %f %f %f %f", ffqnameDi,ffqtype,&ffqsSizeZDi, &ffqsRinDiG,&ffqsRinDi,&ffqsRoutDi,&qFIELDx, &qFIELDy,
	  &qFIELQn, &qFIELQs,&qFIELSek,&qFIELSol,&ffqsX,&ffqsY,&ffqsZ,&ffqsTheta,&ffqsPhi); 
   
   printf(" NUM: |%s| %s Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", ffqnameDi, ffqtype,ffqsSizeZDi, ffqsRinDiG,ffqsRinDi,ffqsRoutDi,
	  qFIELDx,qFIELDy, qFIELQn, qFIELQs,qFIELSek,qFIELSol,ffqsX,ffqsY,ffqsZ,ffqsTheta,ffqsPhi);

   // ----------- create volumes for QUADRUPOLE----------
   if(strcmp(ffqtype,"QUADRUPOLE")== 0 ) { 
     printf(" found QUAD %s iq=%d \n",ffqtype,iq); 
     CreateQuad(iq,ffqnameDi,ffqsSizeZDi, ffqsRinDiG,ffqsRinDi,ffqsRoutDi,qFIELDx, qFIELDy,qFIELQn, qFIELQs,qFIELSek,qFIELSol,ffqsX,ffqsY,ffqsZ,ffqsTheta,ffqsPhi);
     iq++;
   } 
  // ----------- create volumes for kickers and rbend----------
   if((strcmp(ffqtype,"KICKER")== 0) || ( strcmp(ffqtype,"RBEND")== 0)) { 
     printf(" found KICKER %s \n",ffqtype); 
     CreateDipole(ik,ffqnameDi,ffqsSizeZDi, ffqsRinDiG,ffqsRinDi,ffqsRoutDi,qFIELDx, qFIELDy,qFIELQn, qFIELQs,qFIELSek,qFIELSol,ffqsX,ffqsY,ffqsZ,ffqsTheta,ffqsPhi);
     ik++;
   } 
 // ----------- create volumes for solenoid  ----------
   if((strcmp(ffqtype,"SOLENOID")== 0) && ((strcmp(ffqnameDi,"iASDS")== 0)|| (strcmp(ffqnameDi,"iASUS")== 0))) { 
     printf(" found SOLENOID %s \n",ffqtype); 

     CreateASolenoid(is,ffqnameDi,ffqsSizeZDi, ffqsRinDiG,ffqsRinDi,ffqsRoutDi,qFIELDx, qFIELDy,qFIELQn, qFIELQs,qFIELSek,qFIELSol,ffqsX,ffqsY,ffqsZ,ffqsTheta,ffqsPhi);
     is++;
     }
 }
 


 fclose(rc);
 return;
}


void  JLeicDetectorConstruction::Read_dE_File(){
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

FILE* rc;
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
  float   ffqsRinDiG;
   float   ffqsRinDi;
   float   ffqsRoutDi;
  float  ffqsSizeZDi;
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
  sprintf(fname,"e_ir.txt");
rc=fopen(fname,"r");
 if (rc == NULL) return;
 //int iq=0,ik=0,is=0;
 printf("read Di file %s\n",fname);
 while(fgets(buffer, 512, (FILE*) rc)) {
   
   printf("Read_dE*********************************************************************************\n");
   printf("Read_dE %s\n", buffer);
   sscanf(buffer, "%s",mychar); 
   printf("Read_dE:: mychar=%s\n ",mychar);
   if(mychar[0] !='e' || mychar[0] =='\n')  { printf("SKIP LINE %s\n", buffer); continue;}

   sscanf(buffer, "%s %s %f %f %f %f %f %f  %f %f %f %f %f  %f %f %f %f", ffqnameDi,ffqtype,&ffqsSizeZDi, &ffqsRinDiG,&ffqsRinDi,&ffqsRoutDi,&qFIELDx, &qFIELDy,
	  &qFIELQn, &qFIELQs,&qFIELSek,&qFIELSol,&ffqsX,&ffqsY,&ffqsZ,&ffqsTheta,&ffqsPhi); 
   
   printf("Read_dE dE NUM: |%s| %s Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", ffqnameDi, ffqtype,ffqsSizeZDi, ffqsRinDiG,ffqsRinDi,ffqsRoutDi,
	  qFIELDx,qFIELDy, qFIELQn, qFIELQs,qFIELSek,qFIELSol,ffqsX,ffqsY,ffqsZ,ffqsTheta,ffqsPhi);

   // ----------- create volumes for QUADRUPOLE----------
   if(strcmp(ffqtype,"QUADRUPOLE")== 0 ) { 
     printf("Read_dE found QUAD %s iq=%d\n",ffqtype,iq); 
     CreateQuad(iq,ffqnameDi,ffqsSizeZDi, ffqsRinDiG,ffqsRinDi,ffqsRoutDi,qFIELDx, qFIELDy,qFIELQn, qFIELQs,qFIELSek,qFIELSol,ffqsX,ffqsY,ffqsZ,ffqsTheta,ffqsPhi);
     iq++;
   } 
  // ----------- create volumes for kickers and rbend----------
   if((strcmp(ffqtype,"KICKER")== 0) || ( strcmp(ffqtype,"RBEND")== 0)) { 
     printf("Read_dE found KICKER %s ik=%d \n",ffqtype,ik); 
     CreateDipoleChicane(ik,ffqnameDi,ffqsSizeZDi, ffqsRinDiG,ffqsRinDi,ffqsRoutDi,qFIELDx, qFIELDy,qFIELQn, qFIELQs,qFIELSek,qFIELSol,ffqsX,ffqsY,ffqsZ,ffqsTheta,ffqsPhi);
     ik++;
   } 
 // ----------- create volumes for solenoid  ----------
   if((strcmp(ffqtype,"SOLENOID")== 0) && ((strcmp(ffqnameDi,"iASDS")== 0)|| (strcmp(ffqnameDi,"iASUS")== 0))) { 
     printf("Read_dE found SOLENOID %s \n",ffqtype); 

     CreateASolenoid(is,ffqnameDi,ffqsSizeZDi, ffqsRinDiG,ffqsRinDi,ffqsRoutDi,qFIELDx, qFIELDy,qFIELQn, qFIELQs,qFIELSek,qFIELSol,ffqsX,ffqsY,ffqsZ,ffqsTheta,ffqsPhi);
     is++;
     }
 }
 


 fclose(rc);
 return;
}




//==============================================================================================================
//                          Quadrupole
//==============================================================================================================

void  JLeicDetectorConstruction::CreateQuad(int j, char *ffqsNAME, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi,float ffqsRoutDi,
	  float qFIELDx,float qFIELDy, float qFIELQn, float qFIELQs,float qFIELSek,float qFIELSol,float ffqsX,float ffqsY,float ffqsZ,float ffqsTheta,float ffqsPhi ){
  char abname[256];

  printf("CreateQuad:: NUM: %d |%s|  Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", j, ffqsNAME, ffqsSizeZDi, ffqsRinDiG,ffqsRinDi,ffqsRoutDi,
	  qFIELDx,qFIELDy, qFIELQn, qFIELQs,qFIELSek,qFIELSol,ffqsX,ffqsY,ffqsZ,ffqsTheta,ffqsPhi);

  //return;

  vb1= new G4VisAttributes(G4Color(0.8,0.3,0.1,0.9)); vb1->SetForceSolid(true); 

  printf("CreateQuad:: theta =%f %f %f \n",ffqsTheta, ffqsTheta*rad,(ffqsTheta*180/3.1415)*deg);
  brm_hd[j].rotateY(ffqsTheta*rad);
  // brm_hd.rotateY((0*180/3.1415)*deg);
  
  ffqsMaterial = fMat->GetMaterial("IronAll");
  ffqsMaterial_G= fMat->GetMaterial("G4_Galactic");

     //fZ1=ffqsZ1Di[j]; 
  // printf(" FFQs name %s, Z=%f, L=%f, R=%f \n",ffqnameDi[j], ffqsZ1Di[j],  ffqsSizeZDi[j], ffqsRoutDi[j]);
 

    //--------------------Volumes ---------
    sprintf(abname,"Solid_QUADS_hd_v_%s",ffqsNAME);  
    fSolid_QUADS_hd_v[j] = new G4Tubs(abname, 0., (ffqsRoutDi+0.01)*cm,(ffqsSizeZDi/2.)*m,0.,360*deg);                        
    sprintf(abname,"Logic_QUADS_hd_v_%s",ffqsNAME);
    fLogic_QUADS_hd_v[j] = new G4LogicalVolume(fSolid_QUADS_hd_v[j], fWorldMaterial, abname);
    sprintf(abname,"Physics_QUADS_hd_v_%s",ffqsNAME);                                      
    fPhysics_QUADS_hd_v[j] = new G4PVPlacement(G4Transform3D(brm_hd[j], G4ThreeVector(ffqsX*m,ffqsY*m,ffqsZ*m)),abname,  
                                            fLogic_QUADS_hd_v[j],  fPhysicsWorld, false,  0 );  
    //printf("create %s ");
    //--------------------Iron---------   
    sprintf(abname,"Solid_QUADS_hd_ir_%s",ffqsNAME); 
    fSolid_QUADS_hd_ir[j] = new G4Tubs(abname,  ffqsRinDi*cm, ffqsRoutDi*cm,(ffqsSizeZDi/2.)*m,0.,360*deg);                    
    sprintf(abname,"Logic_QUADS_hd_ir_%s",ffqsNAME);
    fLogic_QUADS_hd_ir[j] = new G4LogicalVolume(fSolid_QUADS_hd_ir[j],   ffqsMaterial, abname);
    sprintf(abname,"Physics_QUADS_hd_ir_%s",ffqsNAME);                                      
    fPhysics_QUADS_hd_ir[j] = new G4PVPlacement(0,G4ThreeVector(),abname,fLogic_QUADS_hd_ir[j], fPhysics_QUADS_hd_v[j], false,  0 );     
    fLogic_QUADS_hd_ir[j]->SetVisAttributes(vb1);
    
    //---------------- set magnetic field ---------------
    sprintf(abname,"Solid_QUADS_hd_m_%s",ffqsNAME); 
    fSolid_QUADS_hd_m[j] = new G4Tubs(abname,  0.*cm, ffqsRoutDi*cm,(ffqsSizeZDi/2.)*m,0.,360*deg);                    
    sprintf(abname,"Logic_QUADS_hd_m_%s",ffqsNAME);
    fLogic_QUADS_hd_m[j] = new G4LogicalVolume(fSolid_QUADS_hd_m[j],   ffqsMaterial_G, abname);
    sprintf(abname,"Physics_QUADS_hd_m_%s",ffqsNAME);                                      
    fPhysics_QUADS_hd_m[j] = new G4PVPlacement(0,G4ThreeVector(),abname,fLogic_QUADS_hd_m[j], fPhysics_QUADS_hd_v[j], false,  0 );     
   

    //    G4FieldManager* fieldMgr = SetQMagField(qFIELDx[j],qFIELDy[j]);   // gradient tesla/m;
    // fLogic_QUADSm[j]->SetFieldManager(fieldMgr,true);

    printf("CreateQuad:: FIELD =%f %f --  %f %f -- %f %f \n", qFIELDx,qFIELDy, qFIELQn, qFIELQs,qFIELSek,qFIELSol);
    fieldMgr_QUADS_hd[j] = SetQMagField(qFIELQn, qFIELQs,ffqsTheta);   // gradient tesla/m;
    fLogic_QUADS_hd_m[j]->SetFieldManager(fieldMgr_QUADS_hd[j],true);
   
}


//==============================================================================================================
//                          DIPOLE
//==============================================================================================================

void  JLeicDetectorConstruction::CreateDipole(int j, char *ffqsNAME, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi,float ffqsRoutDi,
	  float qFIELDx,float qFIELDy, float qFIELQn, float qFIELQs,float qFIELSek,float qFIELSol,float ffqsX,float ffqsY,float ffqsZ,float ffqsTheta,float ffqsPhi ){
  char abname[256];
  //G4RotationMatrix brm_hk;

  printf("CreateKicker:: NUM: %d |%s|  Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", j, ffqsNAME, ffqsSizeZDi, ffqsRinDiG,ffqsRinDi,ffqsRoutDi,
	  qFIELDx,qFIELDy, qFIELQn, qFIELQs,qFIELSek,qFIELSol,ffqsX,ffqsY,ffqsZ,ffqsTheta,ffqsPhi);

 /*----------FFQs ions--------------*/ 
  
   ffqsMaterial = fMat->GetMaterial("IronAll");
   ffqsMaterial_G= fMat->GetMaterial("G4_Galactic");
 
   brm_hk[j].rotateY(ffqsTheta*rad);
 
   sprintf(fSolid_BigDi_ffqsNAME[j],"%s",ffqsNAME);
   fSolid_BigDi_ffqsSizeZDi[j]= ffqsSizeZDi;
   fSolid_BigDi_ffqsRinDi[j]=ffqsRinDi;
   fSolid_BigDi_ffqsRoutDi[j]=ffqsRoutDi;
   fSolid_BigDi_ffqsX[j]=ffqsX;
   fSolid_BigDi_ffqsY[j]=ffqsY;
   fSolid_BigDi_ffqsZ[j]=ffqsZ;
 
     //-------------------------- Magnetic volume------------------------
    sprintf(abname,"Solid_DIPOLE_v_%s",ffqsNAME);
      fSolid_BigDi_v[j] = new G4Tubs(abname, 0., (ffqsRoutDi+0.01)*cm,(ffqsSizeZDi/2.)*m,0.,360*deg); 
   
    // fSolid_BigDi_v[j] = new G4Box(abname, (double)(ffqsRoutDi+0.01)*cm, (double)(ffqsRoutDi+0.01)*cm,(double)(ffqsSizeZDi/2.)*m); 
     sprintf(abname,"Logic_DIPOLE_v_%s",ffqsNAME);
    fLogic_BigDi_v[j] = new G4LogicalVolume(fSolid_BigDi_v[j], fWorldMaterial, abname);
    sprintf(abname,"Physics_DIPOLE_v_%s",ffqsNAME);                                      
    fPhysics_BigDi_v[j] = new G4PVPlacement(G4Transform3D(brm_hk[j], G4ThreeVector(ffqsX*m,ffqsY*m,ffqsZ*m)),
                                     abname,fLogic_BigDi_v[j],  fPhysicsWorld, false,  0 );     
    printf(" Finish magnetic volume and start physics volume \n");
   
    //-------------------------- Magnet iron------------------------
    sprintf(abname,"Solid_DIPOLE_i_%s",ffqsNAME);
    fSolid_BigDi_ir[j] = new G4Tubs(abname,  ffqsRinDi*cm,ffqsRoutDi*cm,(ffqsSizeZDi/2.)*m,0.,360*deg); 
     // fSolid_BigDi_ir[j] = new G4Box(abname, (double) ffqsRoutDi*cm, (double)ffqsRoutDi*cm,((double)ffqsSizeZDi/2.)*m); 
    sprintf(abname,"Logic_DIPOLE_i_%s",ffqsNAME);
    fLogic_BigDi_ir[j] = new G4LogicalVolume(fSolid_BigDi_ir[j],   ffqsMaterial, abname);
    sprintf(abname,"Physics_DIPOLE_i_%s",ffqsNAME);                                      
    fPhysics_BigDi_ir[j] = new G4PVPlacement(0, G4ThreeVector(),
                                     abname,fLogic_BigDi_ir[j],  fPhysics_BigDi_v[j] , false,  0 );     
 
    vb1a= new G4VisAttributes(G4Color(0.2,0.8,0.2,1.)); vb1a->SetForceSolid(true); 
    fLogic_BigDi_ir[j]->SetVisAttributes(vb1a);
    
  //-------------------------- Magnet field------------------------
    sprintf(abname,"Solid_DIPOLE_m_%s",ffqsNAME);
       fSolid_BigDi_m[j] = new G4Tubs(abname, 0., ffqsRinDi*cm,(ffqsSizeZDi/2.)*m,0.,360*deg); 
    //  fSolid_BigDi_m[j] = new G4Box(abname,(double)ffqsRinDi*cm,(double)ffqsRinDi*cm,(double)(ffqsSizeZDi/2.)*m); 
    sprintf(abname,"Logic_DIPOLE_m_%s",ffqsNAME);
    fLogic_BigDi_m[j] = new G4LogicalVolume(fSolid_BigDi_m[j],   ffqsMaterial_G, abname);
    sprintf(abname,"Physics_DIPOLE_m_%s",ffqsNAME);                                      
    fPhysics_BigDi_m[j] = new G4PVPlacement(0, G4ThreeVector(),
                                     abname,fLogic_BigDi_m[j],  fPhysics_BigDi_v[j] , false,  0 );     
 
    vb1b= new G4VisAttributes(G4Color(1.,1.,0.8,0.5)); vb1b->SetForceSolid(true); 
    fLogic_BigDi_m[j]->SetVisAttributes(vb1b);
 

     printf(" Start assign magnet  \n");
     fieldMgr_BigDi[j] = SetDipoleMagField(qFIELDx,qFIELDy,0.,ffqsTheta);   // gradient tesla/m;
     fLogic_BigDi_m[j]->SetFieldManager(fieldMgr_BigDi[j], true);

 

}

//==============================================================================================================
//                         Electron Chicane Dipoles
//==============================================================================================================


void  JLeicDetectorConstruction::CreateDipoleChicane(int j, char *ffqsNAME, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi,float ffqsRoutDi,
	  float qFIELDx,float qFIELDy, float qFIELQn, float qFIELQs,float qFIELSek,float qFIELSol,float ffqsX,float ffqsY,float ffqsZ,float ffqsTheta,float ffqsPhi ){
  char abname[256];
  //G4RotationMatrix brm_hk;

  printf("CreateKicker:: NUM: %d |%s|  Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", j, ffqsNAME, ffqsSizeZDi, ffqsRinDiG,ffqsRinDi,ffqsRoutDi,
	  qFIELDx,qFIELDy, qFIELQn, qFIELQs,qFIELSek,qFIELSol,ffqsX,ffqsY,ffqsZ,ffqsTheta,ffqsPhi);

 /*----------FFQs ions--------------*/ 
  
   ffqsMaterial = fMat->GetMaterial("IronAll");
   ffqsMaterial_G= fMat->GetMaterial("G4_Galactic");
 
   brm_hk[j].rotateY(ffqsTheta*rad);
 
     //-------------------------- Magnetic volume------------------------
    sprintf(abname,"Solid_v_%s",ffqsNAME);
    //   fSolid_Chicane_v[j] = new G4Tubs(abname, 0., (ffqsRoutDi+0.01)*cm,(ffqsSizeZDi/2.)*m,0.,360*deg); 
   
    fSolid_Chicane_v[j] = new G4Box(abname, (double)(ffqsRoutDi+0.01)*cm, (double)(ffqsRoutDi+0.01)*cm,(double)(ffqsSizeZDi/2.)*m); 
     sprintf(abname,"Logic_Chicane_v_%s",ffqsNAME);
    fLogic_Chicane_v[j] = new G4LogicalVolume(fSolid_Chicane_v[j], fWorldMaterial, abname);
    sprintf(abname,"Physics_Chicane_v_%s",ffqsNAME);                                      
    fPhysics_Chicane_v[j] = new G4PVPlacement(G4Transform3D(brm_hk[j], G4ThreeVector(ffqsX*m,ffqsY*m,ffqsZ*m)),
                                     abname,fLogic_Chicane_v[j],  fPhysicsWorld, false,  0 );     
    printf(" Finish magnetic volume and start physics volume \n");
    
    //-------------------------- Magnet iron------------------------
    sprintf(abname,"Solid_i_%s",ffqsNAME);
    fSolid_Chicane_ir[j] = new G4Box(abname, (double) ffqsRoutDi*cm, (double)ffqsRoutDi*cm,((double)ffqsSizeZDi/2.)*m); 
    sprintf(abname,"Logic_Chicane_i_%d",j);
    fLogic_Chicane_ir[j] = new G4LogicalVolume(fSolid_Chicane_ir[j],   ffqsMaterial, abname);
    sprintf(abname,"Physics_Chicane_i_%d",j);                                      
    fPhysics_Chicane_ir[j] = new G4PVPlacement(0, G4ThreeVector(),
                                     abname,fLogic_Chicane_ir[j],  fPhysics_Chicane_v[j] , false,  0 );     
 
    vb1a= new G4VisAttributes(G4Color(0.2,0.8,0.2,1.)); vb1a->SetForceSolid(true); 
    fLogic_Chicane_ir[j]->SetVisAttributes(vb1a);
    
  //-------------------------- Magnet field------------------------
    sprintf(abname,"Solid_m_%s",ffqsNAME);
    //    fSolid_Chicane_m[j] = new G4Tubs(abname, 0., ffqsRinDi*cm,(ffqsSizeZDi/2.)*m,0.,360*deg); 
    fSolid_Chicane_m[j] = new G4Box(abname,(double)ffqsRinDi*cm,(double)ffqsRinDi*cm,(double)(ffqsSizeZDi/2.)*m); 
    sprintf(abname,"Logic_Chicane_m_%d",j);
    fLogic_Chicane_m[j] = new G4LogicalVolume(fSolid_Chicane_m[j],   ffqsMaterial_G, abname);
    sprintf(abname,"Physics_Chicane_m_%d",j);                                      
    fPhysics_Chicane_m[j] = new G4PVPlacement(0, G4ThreeVector(),
                                     abname,fLogic_Chicane_m[j],  fPhysics_Chicane_v[j] , false,  0 );     
 
    vb1b= new G4VisAttributes(G4Color(1.,1.,0.8,0.5)); vb1b->SetForceSolid(true); 
    fLogic_Chicane_m[j]->SetVisAttributes(vb1b);
 

     printf(" Start assign magnet  \n");
     fieldMgr_Chicane[j] = SetDipoleMagField(qFIELDx,qFIELDy,0.,ffqsTheta);   // gradient tesla/m;
     fLogic_Chicane_m[j]->SetFieldManager(fieldMgr_Chicane[j], true);

   
 

}

//==============================================================================================================
//                          ASOLENOID
//==============================================================================================================

void  JLeicDetectorConstruction::CreateASolenoid(int j, char *ffqsNAME, float ffqsSizeZDi, float ffqsRinDiG, float ffqsRinDi,float ffqsRoutDi,
	  float qFIELDx,float qFIELDy, float qFIELQn, float qFIELQs,float qFIELSek,float qFIELSol,float ffqsX,float ffqsY,float ffqsZ,float ffqsTheta,float ffqsPhi ){

  char abname[256];
  // G4RotationMatrix brm_as;

  printf("CreateQuad:: NUM: %d |%s|  Dz=%f %f Rin=%f Rout=%f  Dipole =%f, %f Quads =%f, %f sec =%f sol= %f xyz= %f %f %f %f %f \n ", j, ffqsNAME, ffqsSizeZDi, ffqsRinDiG,ffqsRinDi,ffqsRoutDi,
	  qFIELDx,qFIELDy, qFIELQn, qFIELQs,qFIELSek,qFIELSol,ffqsX,ffqsY,ffqsZ,ffqsTheta,ffqsPhi);

  //return;

  vb1= new G4VisAttributes(G4Color(0.8,0.3,0.1,0.9)); vb1->SetForceSolid(true); 

  printf("CreateQuad:: theta =%f %f %f \n",ffqsTheta, ffqsTheta*rad,(ffqsTheta*180/3.1415)*deg);
  brm_as[j].rotateY(ffqsTheta*rad);
    
  ffqsMaterial = fMat->GetMaterial("IronAll");
  ffqsMaterial_G= fMat->GetMaterial("G4_Galactic");

     //fZ1=ffqsZ1Di[j]; 
  // printf(" FFQs name %s, Z=%f, L=%f, R=%f \n",ffqnameDi[j], ffqsZ1Di[j],  ffqsSizeZDi[j], ffqsRoutDi[j]);
 

    //--------------------Volumes ---------
    sprintf(abname,"Solid_ASOLENOID_hd_v_%s",ffqsNAME);  
    fSolid_ASOLENOID_hd_v[j] = new G4Tubs(abname, 0., (ffqsRoutDi+0.01)*cm,(ffqsSizeZDi/2.)*m,0.,360*deg);                        
    sprintf(abname,"Logic_ASOLENOID_hd_v_%s",ffqsNAME);
    fLogic_ASOLENOID_hd_v[j] = new G4LogicalVolume(fSolid_ASOLENOID_hd_v[j], fWorldMaterial, abname);
    sprintf(abname,"Physics_ASOLENOID_hd_v_%s",ffqsNAME);                                      
    fPhysics_ASOLENOID_hd_v[j] = new G4PVPlacement(G4Transform3D(brm_as[j], G4ThreeVector(ffqsX*m,ffqsY*m,ffqsZ*m)),abname,  
                                            fLogic_ASOLENOID_hd_v[j],  fPhysicsWorld, false,  0 );  
    //printf("create %s ");
    //--------------------Iron---------   
    sprintf(abname,"Solid_ASOLENOID_hd_ir_%s",ffqsNAME); 
    fSolid_ASOLENOID_hd_ir[j] = new G4Tubs(abname,  ffqsRinDi*cm, ffqsRoutDi*cm,(ffqsSizeZDi/2.)*m,0.,360*deg);                    
    sprintf(abname,"Logic_ASOLENOID_hd_ir_%s",ffqsNAME);
    fLogic_ASOLENOID_hd_ir[j] = new G4LogicalVolume(fSolid_ASOLENOID_hd_ir[j],   ffqsMaterial, abname);
    sprintf(abname,"Physics_ASOLENOID_hd_ir_%s",ffqsNAME);                                      
    fPhysics_ASOLENOID_hd_ir[j] = new G4PVPlacement(0,G4ThreeVector(),abname,fLogic_ASOLENOID_hd_ir[j], fPhysics_ASOLENOID_hd_v[j], false,  0 );     
    //  fLogic_ASOLENOID_hd_ir[j]->SetVisAttributes(vb1);
    
    //---------------- set magnetic field ---------------
    sprintf(abname,"Solid_ASOLENOID_hd_m_%s",ffqsNAME); 
    fSolid_ASOLENOID_hd_m[j] = new G4Tubs(abname,  0.*cm, ffqsRoutDi*cm,(ffqsSizeZDi/2.)*m,0.,360*deg);                    
    sprintf(abname,"Logic_ASOLENOID_hd_m_%s",ffqsNAME);
    fLogic_ASOLENOID_hd_m[j] = new G4LogicalVolume(fSolid_ASOLENOID_hd_m[j],   ffqsMaterial_G, abname);
    sprintf(abname,"Physics_ASOLENOID_hd_m_%s",ffqsNAME);                                      
    fPhysics_ASOLENOID_hd_m[j] = new G4PVPlacement(0,G4ThreeVector(),abname,fLogic_ASOLENOID_hd_m[j], fPhysics_ASOLENOID_hd_v[j], false,  0 );     
   

    vb1as= new G4VisAttributes(G4Color(1.,0.5,0.7,1.)); vb1as->SetForceSolid(true); 
    fLogic_ASOLENOID_hd_ir[j]->SetVisAttributes(vb1as);
 

    //    G4FieldManager* fieldMgr = SetQMagField(qFIELDx[j],qFIELDy[j]);   // gradient tesla/m;
    // fLogic_ASOLENOIDm[j]->SetFieldManager(fieldMgr,true);

   // G4double fieldStrength = 3.0*tesla;  // 0.01*tesla; // field strength in pipe
    G4double fieldStrength =  qFIELSol*tesla;  // 0.01*tesla; // field strength in pipe
    G4double alphaB        = 0.*degree;
     fMagField_ASOLENOID[j]= new G4UniformMagField(G4ThreeVector(fieldStrength*std::sin(alphaB), 
					   0. , fieldStrength*std::cos(alphaB)));
 
     fieldMgrAS[j] = new G4FieldManager(fMagField_ASOLENOID[j]);
     fieldMgrAS[j]->SetDetectorField(fMagField_ASOLENOID[j]);
     fieldMgrAS[j]->CreateChordFinder(fMagField_ASOLENOID[j]);
     fLogic_ASOLENOID_hd_m[j]->SetFieldManager(fieldMgrAS[j], true);


   
}







