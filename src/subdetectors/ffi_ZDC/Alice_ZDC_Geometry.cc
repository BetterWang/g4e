
//mini-FoCal at SPS setup
//with trigger Scintillators at 0X0 & 20X0

//edited i 20190805  Isshiki
//This code was modified the the length of materialals
//include the 2 Scintirator front of the 1layer and
//
//2020 Modifed by Quan Wang for EIC ZDC quan.wang@cern.ch

//LGL Layers
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Geometry.cc
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//#include "Geometry.hh"
#include "Alice_ZDC_Geometry.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"
#include "G4Element.hh"
#include "G4Material.hh"
//#include "SensitiveVolume.hh"
#include "G4SDManager.hh"
//#include "constants.hh"
#include "Alice_ZDC_constants.hh"

//------------------------------------------------------------------------------
Geometry::Geometry(bool bAbsorber) {
    cfg.bAbsorber = bAbsorber;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
Geometry::~Geometry() {}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
G4VPhysicalVolume* Geometry::Construct()
{
	//------------------------------------------------------------------------------
	G4NistManager* material_Man = G4NistManager::Instance();  //NistManager: start element destruction


	G4Box* solid_World =
		new G4Box("Solid_World", World_X/2.0, World_Y/2.0, World_Z/2.0);

	G4Material* material_World = material_Man->FindOrBuildMaterial("G4_AIR");
	G4LogicalVolume* logVol_World = new G4LogicalVolume(solid_World, material_World, "LogVol_World");
	//logVol_World->SetVisAttributes (G4VisAttributes::Invisible);

	G4PVPlacement* physVol_World  =
		new G4PVPlacement(G4Transform3D(), "PhysVol_World", logVol_World, 0,

				  				false, 0);
	//ConstructZDC(physVol_World);
 }

 G4VPhysicalVolume* Geometry::ConstructZDC(G4VPhysicalVolume *physVol_World) {

   G4NistManager* material_Man = G4NistManager::Instance();  //NistManager: start element destruction
   G4Material* material_World = material_Man->FindOrBuildMaterial("G4_AIR");

   printf("======> Construct ALICE ZDC ....\n");
   G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.0, 1., 0., 0.5));   visAttr->SetLineWidth(1);   visAttr->SetForceSolid(true);
   G4VisAttributes *visAttrAbs = new G4VisAttributes(G4Color(1., 0., 0., 0.5));   visAttrAbs->SetLineWidth(1);   visAttrAbs->SetForceSolid(true);
   G4VisAttributes *visAttrScin = new G4VisAttributes(G4Color(0., 0.0, 0.1, 0.5));   visAttrScin->SetLineWidth(1);   visAttrScin->SetForceSolid(true);

	//*****************************************************************************************
	//G4box is the material
	//PAD layer
	G4Box* PAD_W 		= new G4Box("PPAD_W",		PAD_MiniFoCal_X/2.0, PAD_MiniFoCal_Y/2.0, PAD_Absorber_Z/2.0);
	G4Box* PAD_Glue1 	= new G4Box("PPAD_Glue1",	PAD_MiniFoCal_X/2.0, PAD_MiniFoCal_Y/2.0, PAD_Glue1_Z/2.0);
	G4Box* PAD_Silicon 	= new G4Box("PPAD_Silicon",	PAD_X/2.0, 			 PAD_Y/2.0, 		  PAD_Z/2.0);
	G4Box* PAD_Glue2 	= new G4Box("PPAD_Glue2",	PAD_MiniFoCal_X/2.0, PAD_MiniFoCal_Y/2.0, PAD_Glue2_Z/2.0);
	G4Box* PAD_FPC 		= new G4Box("PPAD_FPC",		PAD_MiniFoCal_X/2.0, PAD_MiniFoCal_Y/2.0, PAD_FPC_Z/2.0);
	//PIX layer
	G4Box* PIX_W 		= new G4Box("PPIX_W",		PIX_MiniFoCal_X/2.0, PIX_MiniFoCal_Y/2.0, PIX_Absorber_Z/2.0);
	G4Box* PIX_Glue1 	= new G4Box("PPIX_Glue1",	PIX_MiniFoCal_X/2.0, PIX_MiniFoCal_Y/2.0, PIX_Glue1_Z/2.0);
	G4Box* PIX_Silicon 	= new G4Box("PPIX_Silicon",	PIX_X/2.0, 			 PIX_Y/2.0, 		  PIX_Z/2.0);
	G4Box* PIX_Glue2 	= new G4Box("PPIX_Glue2",	PIX_MiniFoCal_X/2.0, PIX_MiniFoCal_Y/2.0, PIX_Glue2_Z/2.0);
	G4Box* PIX_FPC 		= new G4Box("PPIX_FPC",		PIX_MiniFoCal_X/2.0, PIX_MiniFoCal_Y/2.0, PIX_FPC_Z/2.0);
	//HCal tower
	G4Box* HCal_Absorber	= new G4Box("HCal_Absorber",	HCAL_X_Tower/2.0, HCAL_Y_Tower/2.0, HCAL_Z_Absorber/2.0);
	G4Box* HCal_Scintilator	= new G4Box("HCal_Scintilator",	HCAL_X_Tower/2.0, HCAL_Y_Tower/2.0, HCAL_Z_Scintilator/2.0);
	G4Box* HCal_Gap			= new G4Box("HCal_Gap",			HCAL_X_Tower/2.0, HCAL_Y_Tower/2.0, HCAL_Z_Gap/2.0);
	//A large box after the HCal to study the leakage from the HCal
	G4Box* HCal_Leakage  	= new G4Box("HCal_Leakage",	1300*mm, 1300*mm, 2000.00*mm);
	//*****************************************************************************************
	//Unique materials
	//*****************************************************************************************
	//The definition of the W alloy
	G4Material* material_tungsten = new G4Material("tungsten",18.73 *g/cm3,3);
	//G4Material* material_tungsten = new G4Material("tungsten",19.3 *g/cm3,1);
	G4Element* W  = material_Man->FindOrBuildElement(74);//density: 19.3  I:727
	G4Element* Ni = material_Man->FindOrBuildElement(28);//density: 8.902   I:311
	G4Element* Cu = material_Man->FindOrBuildElement(29);//G4_Cu  8.96   I:322
	material_tungsten->AddElement(W,100*perCent);    //the percentage of materialal
	material_tungsten->AddElement(Ni,4*perCent);
	material_tungsten->AddElement(Cu,2*perCent);

	//Definition of the Epoxy Glue
	G4Material* material_PET = new G4Material("PET",1.38*g/cm3,3);
	G4Element* O = material_Man->FindOrBuildElement(8);
	G4Element* elH = new G4Element("Hydrogen","H", 1, 1.00794 *g/mole);
	G4Element* elC = new G4Element("Carbon","C", 6, 12.011 *g/mole);
	material_PET->AddElement(elC,10);
	material_PET->AddElement(elH,8);
	material_PET->AddElement(O,4);

	//Definition of the scintillator
	G4double density= 1.032 *g/cm3;       //to define the dencity on my own
	G4Material* material_Scintilator = new G4Material("Scintilator",density,2);   //
	material_Scintilator->AddElement(elC,8);
	material_Scintilator->AddElement(elH,8);

	//Other materials
	G4Material* material_Si = material_Man->FindOrBuildMaterial("G4_Si");
	G4Material* material_Pb = material_Man->FindOrBuildMaterial("G4_Pb");
	G4Material* material_Cu = material_Man->FindOrBuildMaterial("G4_Cu");
	G4Material* material_Fe = material_Man->FindOrBuildMaterial("G4_Fe");
	//*****************************************************************************************
	//   Logical volumes
	//*****************************************************************************************
	//PAD
	G4LogicalVolume* logVol_PAD_W 		= new G4LogicalVolume( PAD_W, 		material_tungsten, "LogVol_PAD_W" );
	G4LogicalVolume* logVol_PAD_Glue1 	= new G4LogicalVolume( PAD_Glue1, 	material_PET, "LogVol_PAD_Glue1", 0, 0, 0 );
	G4LogicalVolume* logVol_PAD_Silicon	= new G4LogicalVolume( PAD_Silicon, material_Si, "LogVol_PAD_Silicon", 0, 0, 0 );
	G4LogicalVolume* logVol_PAD_Glue2 	= new G4LogicalVolume( PAD_Glue2, 	material_PET, "LogVol_PAD_Glue2", 0, 0, 0 );
	G4LogicalVolume* logVol_PAD_FPC 	= new G4LogicalVolume( PAD_FPC, 	material_PET, "LogVol_PAD_FPC", 0, 0, 0 );
	fScoringVol_PAD = logVol_PAD_Silicon;
	fScoringVol_EMCal2 = logVol_PAD_W;
	//PIX
	G4LogicalVolume* logVol_PIX_W 		= new G4LogicalVolume( PIX_W, 		material_tungsten, "LogVol_PIX_W", 0, 0, 0 );
	G4LogicalVolume* logVol_PIX_Glue1 	= new G4LogicalVolume( PIX_Glue1, 	material_PET, "LogVol_PIX_Glue1", 0, 0, 0 );
	G4LogicalVolume* logVol_PIX_Silicon	= new G4LogicalVolume( PIX_Silicon, material_Si, "LogVol_PIX_Silicon", 0, 0, 0 );
	G4LogicalVolume* logVol_PIX_Glue2 	= new G4LogicalVolume( PIX_Glue2, 	material_PET, "LogVol_PIX_Glue2", 0, 0, 0 );
	G4LogicalVolume* logVol_PIX_FPC 	= new G4LogicalVolume( PIX_FPC, 	material_PET, "LogVol_PIX_FPC", 0, 0, 0 );
	fScoringVol_PIX = logVol_PIX_Silicon;
	fScoringVol_EMCal1 = logVol_PIX_W;
    logVol_PAD_W->SetVisAttributes(visAttr);
    logVol_PAD_FPC->SetVisAttributes(visAttr);
    logVol_PAD_Silicon->SetVisAttributes(visAttrAbs);
    logVol_PIX_Silicon->SetVisAttributes(visAttrScin);
	//HCal volumes
	G4LogicalVolume* logVol_HCal_Absorber	= new G4LogicalVolume( HCal_Absorber, 	 material_Pb, "logVol_HCal_Absorber");
	G4LogicalVolume* logVol_HCal_Scintilator= new G4LogicalVolume( HCal_Scintilator, material_Scintilator,"logVol_HCal_Scintilator",0,0,0);
	G4LogicalVolume* logVol_HCal_Gap		= new G4LogicalVolume( HCal_Gap, 		 material_World,	"logVol_HCal_Gap",0,0,0);
	fScoringVol_HCal = logVol_HCal_Absorber;
	fScoringVol_SCI  = logVol_HCal_Scintilator;
	//EndCap
	G4LogicalVolume* logVol_HCal_Leakage 	= new G4LogicalVolume( HCal_Leakage, 	material_tungsten, "logVol_HCal_Leakage",0,0,0);
	fScoringVol_Leak = logVol_HCal_Leakage;
	//*****************************************************************************************
	G4int IPADlayer = 0;
	G4int IPIXlayer = 0;
	//The PAD and PIX layer thicknesses can be different - this variable remembers the previous layer thickness either PIX or PAD
	//It is changed at the end of the loops
	G4double TotalLayerThickness = 0;
	G4cout << PAD_Layer_Thickness << "  " << PIX_Layer_Thickness << G4endl;
	for(G4int ilayer=0; ilayer<NumberOfLayers; ilayer++){
		//This is the PAD layer
		if(LayerLayout[ilayer] == 0){
			G4double position_Z_PAD_Absorber	= Start_Z + PAD_Absorber_Z/2 + TotalLayerThickness;
			G4double position_Z_PAD_Glue1		= Start_Z + PAD_Absorber_Z + PAD_Glue1_Z/2 + TotalLayerThickness;
			G4double position_Z_PAD_Silicon		= Start_Z + PAD_Absorber_Z + PAD_Glue1_Z + PAD_Z/2 + TotalLayerThickness;
			G4cout << ilayer << " PAD " << position_Z_PAD_Silicon << "  " << TotalLayerThickness << G4endl;
			for(int iy = 0; iy < NpadY; iy++){
				for(int ix = 0; ix < NpadX; ix++){
					G4double position_X_PAD_Silicon = (Start_X - PAD_MiniFoCal_X/2 + ix*PAD_X + PAD_X/2)*mm;
					G4double position_Y_PAD_Silicon = (Start_Y - PAD_MiniFoCal_Y/2 + iy*PAD_Y + PAD_Y/2)*mm;
					G4ThreeVector    threeVect_PAD_Silicon	= G4ThreeVector(position_X_PAD_Silicon, position_Y_PAD_Silicon, position_Z_PAD_Silicon);
					G4RotationMatrix rotMtrx_PAD_Silicon	= G4RotationMatrix();
					G4Transform3D    trans3D_PAD_W			= G4Transform3D(rotMtrx_PAD_Silicon, threeVect_PAD_Silicon);

					G4int IDnumber_PAD = IDnumber_PAD_First + ix + NpadX*iy + NpadX*NpadY*IPADlayer;
					//G4cout << "PAD " << IDnumber_PAD << "  " << ix << "  " << iy << "  " << IPADlayer << G4endl;
					new G4PVPlacement(trans3D_PAD_W,
										"PhysVol_Si",
										logVol_PAD_Silicon,
										physVol_World,
										false,
										IDnumber_PAD);
				}
			}
			G4double position_Z_PAD_Glue2	= Start_Z + PAD_Absorber_Z + PAD_Glue1_Z + PAD_Z + PAD_Glue2_Z/2 + TotalLayerThickness;
			G4double position_Z_PAD_FPC		= Start_Z + PAD_Absorber_Z + PAD_Glue1_Z + PAD_Z + PAD_Glue2_Z + PAD_FPC_Z/2 + TotalLayerThickness;

			G4ThreeVector threeVect_PAD_W 		= G4ThreeVector(Start_X, Start_Y, position_Z_PAD_Absorber);
			G4ThreeVector threeVect_PAD_Glue1 	= G4ThreeVector(Start_X, Start_Y, position_Z_PAD_Glue1);
			G4ThreeVector threeVect_PAD_Glue2 	= G4ThreeVector(Start_X, Start_Y, position_Z_PAD_Glue2);
			G4ThreeVector threeVect_PAD_FPC 	= G4ThreeVector(Start_X, Start_Y, position_Z_PAD_FPC);

			G4RotationMatrix rotMtrx_PAD_W 		= G4RotationMatrix();
			G4RotationMatrix rotMtrx_PAD_Glue1 	= G4RotationMatrix();
			G4RotationMatrix rotMtrx_PAD_Glue2 	= G4RotationMatrix();
			G4RotationMatrix rotMtrx_PAD_FPC 	= G4RotationMatrix();

			G4Transform3D trans3D_PAD_W 	= G4Transform3D(rotMtrx_PAD_W, 		threeVect_PAD_W);
			G4Transform3D trans3D_PAD_Glue1 = G4Transform3D(rotMtrx_PAD_Glue1, 	threeVect_PAD_Glue1);
			G4Transform3D trans3D_PAD_Glue2 = G4Transform3D(rotMtrx_PAD_Glue2, 	threeVect_PAD_Glue2);
			G4Transform3D trans3D_PAD_FPC 	= G4Transform3D(rotMtrx_PAD_FPC, 	threeVect_PAD_FPC);

			G4int IDnumber_PAD_Absorber 	= IDnumber_PAD_Absorber_First	+ilayer;
			G4int IDnumber_PAD_Glue1 		= IDnumber_PAD_Glue1_First		+ilayer;
			G4int IDnumber_PAD_Glue2 		= IDnumber_PAD_Glue2_First		+ilayer;
			G4int IDnumber_PAD_FPC 			= IDnumber_PAD_FPC_First		+ilayer;

			//G4cout << "PAD_ABS " << IDnumber_PAD_Absorber << "  " << ilayer << G4endl;
			//G4cout << "PAD_GLU1 " << IDnumber_PAD_Glue1 << "  " << ilayer << G4endl;
			//G4cout << "PAD_GLU2 " << IDnumber_PAD_Glue2 << "  " << ilayer << G4endl;
			//G4cout << "PAD_FPC " << IDnumber_PAD_FPC << "  " << ilayer << G4endl;

			new G4PVPlacement(trans3D_PAD_W, 		"PhysVol_Absorber", logVol_PAD_W,  		physVol_World, false, IDnumber_PAD_Absorber);
			new G4PVPlacement(trans3D_PAD_Glue1, 	"PhysVol_Glue1", 	logVol_PAD_Glue1, 	physVol_World, false, IDnumber_PAD_Glue1);
			new G4PVPlacement(trans3D_PAD_Glue2, 	"PhysVol_Glue2", 	logVol_PAD_Glue2, 	physVol_World, false, IDnumber_PAD_Glue2);
			new G4PVPlacement(trans3D_PAD_FPC, 		"PhysVol_FPC", 		logVol_PAD_FPC,     physVol_World, false, IDnumber_PAD_FPC);
			IPADlayer++;
			//Which layer thickness
			TotalLayerThickness += PAD_Layer_Thickness;
		}
		else if(LayerLayout[ilayer] == 1){
			G4double position_Z_PIX_Absorber	= Start_Z + PIX_Absorber_Z/2 + TotalLayerThickness;
			G4double position_Z_PIX_Glue1		= Start_Z + PIX_Absorber_Z + PIX_Glue1_Z/2 + TotalLayerThickness;
			G4double position_Z_PIX_Silicon		= Start_Z + PIX_Absorber_Z + PIX_Glue1_Z + PIX_Z/2 + TotalLayerThickness;
			G4cout << ilayer << " PIX " << position_Z_PIX_Silicon << "  " << TotalLayerThickness << G4endl;
			for(int iy = 0; iy < NpixY; iy++){
				for(int ix = 0; ix < NpixX; ix++){
					G4double position_X_PIX_Silicon = Start_X - PIX_MiniFoCal_X/2 + ix*PIX_X + PIX_X/2;
					G4double position_Y_PIX_Silicon = Start_Y - PIX_MiniFoCal_Y/2 + iy*PIX_Y + PIX_Y/2;
					G4ThreeVector    threeVect_PIX_Silicon	= G4ThreeVector(position_X_PIX_Silicon, position_Y_PIX_Silicon, position_Z_PIX_Silicon);
					G4RotationMatrix rotMtrx_PIX_Silicon	= G4RotationMatrix();
					G4Transform3D    trans3D_PIX_W			= G4Transform3D(rotMtrx_PIX_Silicon, threeVect_PIX_Silicon);

					G4int IDnumber_PIX = IDnumber_PIX_First + ix + NpixX*iy + NpixX*NpixY*IPIXlayer;
					//G4cout << "PIX " << IDnumber_PIX << "  " << ix << "  " << iy << "  " << IPIXlayer << G4endl;
					new G4PVPlacement(trans3D_PIX_W,
										"PhysVol_Si",
										logVol_PIX_Silicon,
										physVol_World,
										false,
										IDnumber_PIX);
				}
			}
			G4double position_Z_PIX_Glue2	= Start_Z + PIX_Absorber_Z + PIX_Glue1_Z + PIX_Z + PIX_Glue2_Z/2 + TotalLayerThickness;
			G4double position_Z_PIX_FPC		= Start_Z + PIX_Absorber_Z + PIX_Glue1_Z + PIX_Z + PIX_Glue2_Z + PIX_FPC_Z/2 + TotalLayerThickness;

			G4ThreeVector threeVect_PIX_W 		= G4ThreeVector(Start_X, Start_Y, position_Z_PIX_Absorber);
			G4ThreeVector threeVect_PIX_Glue1 	= G4ThreeVector(Start_X, Start_Y, position_Z_PIX_Glue1);
			G4ThreeVector threeVect_PIX_Glue2 	= G4ThreeVector(Start_X, Start_Y, position_Z_PIX_Glue2);
			G4ThreeVector threeVect_PIX_FPC 	= G4ThreeVector(Start_X, Start_Y, position_Z_PIX_FPC);

			G4RotationMatrix rotMtrx_PIX_W 		= G4RotationMatrix();
			G4RotationMatrix rotMtrx_PIX_Glue1 	= G4RotationMatrix();
			G4RotationMatrix rotMtrx_PIX_Glue2 	= G4RotationMatrix();
			G4RotationMatrix rotMtrx_PIX_FPC 	= G4RotationMatrix();

			G4Transform3D trans3D_PIX_W 	= G4Transform3D(rotMtrx_PIX_W, 		threeVect_PIX_W);
			G4Transform3D trans3D_PIX_Glue1 = G4Transform3D(rotMtrx_PIX_Glue1, 	threeVect_PIX_Glue1);
			G4Transform3D trans3D_PIX_Glue2 = G4Transform3D(rotMtrx_PIX_Glue2, 	threeVect_PIX_Glue2);
			G4Transform3D trans3D_PIX_FPC 	= G4Transform3D(rotMtrx_PIX_FPC, 	threeVect_PIX_FPC);

			G4int IDnumber_PIX_Absorber 	= IDnumber_PAD_Absorber_First + ilayer;
			G4int IDnumber_PIX_Glue1 		= IDnumber_PAD_Glue1_First	  + ilayer;
			G4int IDnumber_PIX_Glue2 		= IDnumber_PAD_Glue2_First	  + ilayer;
			G4int IDnumber_PIX_FPC 			= IDnumber_PAD_FPC_First	  + ilayer;

			//G4cout << "PIX_ABS  " << IDnumber_PIX_Absorber << "  " << ilayer << G4endl;
			//G4cout << "PIX_GLU1 " << IDnumber_PIX_Glue1 << "  " << ilayer << G4endl;
			//G4cout << "PIX_GLU2 " << IDnumber_PIX_Glue2 << "  " << ilayer << G4endl;
			//G4cout << "PIX_FPC  " << IDnumber_PIX_FPC << "  " << ilayer << G4endl;

			new G4PVPlacement(trans3D_PIX_W, 		"PhysVol_Absorber", logVol_PIX_W,  		physVol_World, false, IDnumber_PIX_Absorber);
			new G4PVPlacement(trans3D_PIX_Glue1, 	"PhysVol_Glue1", 	logVol_PIX_Glue1, 	physVol_World, false, IDnumber_PIX_Glue1);
			new G4PVPlacement(trans3D_PIX_Glue2, 	"PhysVol_Glue2", 	logVol_PIX_Glue2, 	physVol_World, false, IDnumber_PIX_Glue2);
			new G4PVPlacement(trans3D_PIX_FPC, 		"PhysVol_FPC", 		logVol_PIX_FPC,     physVol_World, false, IDnumber_PIX_FPC);
			IPIXlayer++;
			//Which layer thickness
			TotalLayerThickness += PIX_Layer_Thickness;
		}
	}
	G4double HCal_startZ = Start_Z + NumberPIX*PIX_Layer_Thickness + NumberPAD*PAD_Layer_Thickness + AirGapFromFoCalE;
	G4double HCal_X = (HCALNumberOfTowersX*HCAL_X_Tower)/2;
	G4double HCal_Y = (HCALNumberOfTowersY*HCAL_Y_Tower)/2;
	for(G4int iHcalLayer = 0; iHcalLayer < HCALNumberOfLayers; iHcalLayer++){
		G4double position_Z_HCal_Absorber    = HCal_startZ + HCAL_Z_Absorber/2 + iHcalLayer*HCal_Layer_Thickness;
		G4double position_Z_HCal_Scintilator = HCal_startZ + HCAL_Z_Absorber + HCAL_Z_Scintilator/2 + iHcalLayer*HCal_Layer_Thickness;
		G4double position_Z_HCal_Gap     	 = HCal_startZ + HCAL_Z_Absorber + HCAL_Z_Scintilator + HCAL_Z_Gap/2 + iHcalLayer*HCal_Layer_Thickness;
		for(G4int TowerX = 0; TowerX < HCALNumberOfTowersX; TowerX++){
			for(G4int TowerY = 0; TowerY < HCALNumberOfTowersY; TowerY++){

				G4double position_X_Tower = Start_X - HCal_X + TowerX*HCAL_X_Tower + HCAL_X_Tower/2;
				G4double position_Y_Tower = Start_Y - HCal_Y + TowerY*HCAL_Y_Tower + HCAL_Y_Tower/2;

				G4ThreeVector threeVect_LogV_HCal_Absorber 		= G4ThreeVector(position_X_Tower, position_Y_Tower, position_Z_HCal_Absorber);
				G4ThreeVector threeVect_LogV_HCal_Scintilator 	= G4ThreeVector(position_X_Tower, position_Y_Tower, position_Z_HCal_Scintilator);
				G4ThreeVector threeVect_LogV_HCal_Gap			= G4ThreeVector(position_X_Tower, position_Y_Tower, position_Z_HCal_Gap);

				G4RotationMatrix rotMtrx_LogV_HCal_Absorber		= G4RotationMatrix();
				G4RotationMatrix rotMtrx_LogV_HCal_Scintilator 	= G4RotationMatrix();
				G4RotationMatrix rotMtrx_LogV_HCal_Gap			= G4RotationMatrix();

				G4Transform3D trans3D_LogV_HCal_Absorber 		= G4Transform3D(rotMtrx_LogV_HCal_Absorber, 	threeVect_LogV_HCal_Absorber);
				G4Transform3D trans3D_LogV_HCal_Scintilator 	= G4Transform3D(rotMtrx_LogV_HCal_Scintilator,  threeVect_LogV_HCal_Scintilator);
				G4Transform3D trans3D_LogV_HCal_Gap 			= G4Transform3D(rotMtrx_LogV_HCal_Gap, 			threeVect_LogV_HCal_Gap);

				G4int IDnumber_HCal_Scintilator = IDnumber_HCal_Scintilator_First + iHcalLayer + TowerX*HCALNumberOfLayers + TowerY*HCALNumberOfTowersX*HCALNumberOfLayers;
				G4int IDnumber_HCal_Absorber 	= IDnumber_HCal_Absorber_First	+ TowerY + TowerX*HCALNumberOfTowersY;
				G4int IDnumber_HCal_Gap 		= IDnumber_HCal_Gap_First		+ TowerY + TowerX*HCALNumberOfTowersY;

				//G4cout << iHcalLayer << " HCal " << IDnumber_HCal_Scintilator << ", " << IDnumber_HCal_Absorber <<  ", " << IDnumber_HCal_Gap << G4endl;
				//G4cout << "HCAL_SCI " << IDnumber_HCal_Scintilator 	<< "  " << TowerX << "  " << TowerY << "  " << iHcalLayer << G4endl;
				//G4cout << "HCAL_ABS " << IDnumber_HCal_Absorber  	<< "  " << TowerX << "  " << TowerY << "  " << iHcalLayer << G4endl;
				//G4cout << "HCAL_GAP " << IDnumber_HCal_Gap  		<< "  " << TowerX << "  " << TowerY << "  " << iHcalLayer << G4endl;
				//new G4PVPlacement(trans3D_LogV_HCal_Absorber, 	 "PhysVol_HCal_Absorber", 	 logVol_HCal_Absorber, 		physVol_World,false, IDnumber_HCal_Absorber);
				//new G4PVPlacement(trans3D_LogV_HCal_Scintilator, "PhysVol_HCal_Scintilator", logVol_HCal_Scintilator, 	physVol_World,false, IDnumber_HCal_Scintilator);
				//new G4PVPlacement(trans3D_LogV_HCal_Gap, 		 "PhysVol_HCal_Gap", 		 logVol_HCal_Gap, 			physVol_World,false, IDnumber_HCal_Gap);
			}
		}
	}

	//This endcap is to study the particles which penetrate through all the detectors
	G4double HCal_Z_Leakage = HCal_startZ + HCal_Layer_Thickness*HCALNumberOfLayers + 3000.0;
	G4ThreeVector threeVect_LogV_HCal_Leakage 		= G4ThreeVector(Start_X, Start_Y, HCal_Z_Leakage);
	G4RotationMatrix rotMtrx_LogV_HCal_Leakage		= G4RotationMatrix();
	G4Transform3D trans3D_LogV_HCal_Leakage 		= G4Transform3D(rotMtrx_LogV_HCal_Leakage, 	threeVect_LogV_HCal_Leakage);
	G4int IDnumber_HCal_Leakage = IDnumber_HCal_Leakage_First;
	G4cout << "LEAK " << IDnumber_HCal_Leakage << G4endl;
	//new G4PVPlacement(trans3D_LogV_HCal_Leakage, 	 "PhysVol_HCal_Leakage", 	 logVol_HCal_Leakage, 		physVol_World,false, IDnumber_HCal_Leakage);

	return physVol_World;
}

G4VPhysicalVolume* Geometry::ConstructZDCPrototype(G4VPhysicalVolume *physVol_World) {
    G4NistManager* material_Man = G4NistManager::Instance();  //NistManager: start element destruction
    G4Material* material_World = material_Man->FindOrBuildMaterial("G4_AIR");
    G4Material* material_tungsten = material_Man->FindOrBuildMaterial("G4_W");

    //Definition of the Epoxy Glue
    G4Material* material_PET = new G4Material("PET",1.38*g/cm3,3);
    G4Element* O = material_Man->FindOrBuildElement(8);
    G4Element* elH = new G4Element("Hydrogen","H", 1, 1.00794 *g/mole);
    G4Element* elC = new G4Element("Carbon","C", 6, 12.011 *g/mole);
    material_PET->AddElement(elC,10);
    material_PET->AddElement(elH,8);
    material_PET->AddElement(O,4);

    //Definition of the scintillator
    G4double density= 1.032 *g/cm3;       //to define the dencity on my own
    G4Material* material_Scintilator = new G4Material("Scintilator",density,2);   //
    material_Scintilator->AddElement(elC,8);
    material_Scintilator->AddElement(elH,8);

    //Other materials
    G4Material* material_Si = material_Man->FindOrBuildMaterial("G4_Si");
    G4Material* material_Pb = material_Man->FindOrBuildMaterial("G4_Pb");
    G4Material* material_Cu = material_Man->FindOrBuildMaterial("G4_Cu");
    G4Material* material_Fe = material_Man->FindOrBuildMaterial("G4_Fe");
    G4Material* material_PbWO4 = material_Man->FindOrBuildMaterial("G4_PbWO4");

    printf("======> Construct ALICE ZDC Prototype....\n");
    G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.1, 1., 0.9, 1.0));   visAttr->SetLineWidth(1);   visAttr->SetForceSolid(true);
    G4VisAttributes *visAttrAbs = new G4VisAttributes(G4Color(1., 0., 0., 0.5));   visAttrAbs->SetLineWidth(1);   visAttrAbs->SetForceSolid(true);
    G4VisAttributes *visAttrScin = new G4VisAttributes(G4Color(0., 0.0, 0.1, 0.5));   visAttrScin->SetLineWidth(1);   visAttrScin->SetForceSolid(true);
    //
    //*****************************************************************************************
    //G4box is the material
    //*****************************************************************************************
    //PAD layer
    G4Box* PAD_W 		= new G4Box("PPAD_W",		cfg.Pad_Width/2.0, cfg.Pad_Width/2.0, cfg.Pad_Thickness/2.0);
    G4Box* PAD_Glue1 	= new G4Box("PPAD_Glue1",	cfg.Pad_Width/2.0, cfg.Pad_Width/2.0, cfg.Pad_Glue1_Thickness/2.0);
    G4Box* PAD_Silicon 	= new G4Box("PPAD_Silicon", cfg.Pad_Width/2.0, cfg.Pad_Width/2.0, cfg.Pad_Silicon_Thickness/2.0);
    G4Box* PAD_Glue2 	= new G4Box("PPAD_Glue2",	cfg.Pad_Width/2.0, cfg.Pad_Width/2.0, cfg.Pad_Glue2_Thickness/2.0);
    G4Box* PAD_FPC 		= new G4Box("PPAD_FPC",		cfg.Pad_Width/2.0, cfg.Pad_Width/2.0, cfg.Pad_FPC_Thickness/2.0);
    //*****************************************************************************************
    //   Logical volumes
    //*****************************************************************************************
    //PAD
    G4LogicalVolume* logVol_PAD_W 		= new G4LogicalVolume( PAD_W, 		material_tungsten, "LogVol_PAD_W" );
    G4LogicalVolume* logVol_PAD_Glue1 	= new G4LogicalVolume( PAD_Glue1, 	material_PET,      "LogVol_PAD_Glue1", 0, 0, 0 );
    G4LogicalVolume* logVol_PAD_Silicon	= new G4LogicalVolume( PAD_Silicon, material_Si,       "LogVol_PAD_Silicon", 0, 0, 0 );
    G4LogicalVolume* logVol_PAD_Glue2 	= new G4LogicalVolume( PAD_Glue2, 	material_PET,      "LogVol_PAD_Glue2", 0, 0, 0 );
    G4LogicalVolume* logVol_PAD_FPC 	= new G4LogicalVolume( PAD_FPC, 	material_PET,      "LogVol_PAD_FPC", 0, 0, 0 );

    fScoringVol_SCI = logVol_PAD_Silicon;

    logVol_PAD_W->SetVisAttributes(visAttr);
    logVol_PAD_FPC->SetVisAttributes(visAttr);
    logVol_PAD_Silicon->SetVisAttributes(visAttrAbs);

    double module_Thickness = cfg.Pad_Thickness + cfg.Pad_Glue1_Thickness + cfg.Pad_Silicon_Thickness + cfg.Pad_Glue2_Thickness + cfg.Pad_FPC_Thickness + cfg.Pad_AirGap;
    for ( int i = 0; i < cfg.NLayers; i++ ) {
        G4ThreeVector threeVect_PAD_W       (0., 0., module_Thickness * i);
        G4ThreeVector threeVect_PAD_Glue1   (0., 0., module_Thickness * i + cfg.Pad_Thickness/2. + cfg.Pad_Glue1_Thickness/2.);
        G4ThreeVector threeVect_PAD_Silicon (0., 0., module_Thickness * i + cfg.Pad_Thickness/2. + cfg.Pad_Glue1_Thickness + cfg.Pad_Silicon_Thickness/2.);
        G4ThreeVector threeVect_PAD_Glue2   (0., 0., module_Thickness * i + cfg.Pad_Thickness/2. + cfg.Pad_Glue1_Thickness + cfg.Pad_Silicon_Thickness + cfg.Pad_Glue2_Thickness/2.);
        G4ThreeVector threeVect_PAD_FPC     (0., 0., module_Thickness * i + cfg.Pad_Thickness/2. + cfg.Pad_Glue1_Thickness + cfg.Pad_Silicon_Thickness + cfg.Pad_Glue2_Thickness + cfg.Pad_FPC_Thickness/2.);

        G4Transform3D trans3D_PAD_W       = G4Transform3D(G4RotationMatrix(), threeVect_PAD_W);
        G4Transform3D trans3D_PAD_Glue1   = G4Transform3D(G4RotationMatrix(), threeVect_PAD_Glue1);
        G4Transform3D trans3D_PAD_Silicon = G4Transform3D(G4RotationMatrix(), threeVect_PAD_Silicon);
        G4Transform3D trans3D_PAD_Glue2   = G4Transform3D(G4RotationMatrix(), threeVect_PAD_Glue2);
        G4Transform3D trans3D_PAD_FPC     = G4Transform3D(G4RotationMatrix(), threeVect_PAD_FPC);

        new G4PVPlacement(trans3D_PAD_W,        "PhysVol_Absorber", logVol_PAD_W,       physVol_World, false, i);
        new G4PVPlacement(trans3D_PAD_Glue1,    "PhysVol_Glue1",    logVol_PAD_Glue1,   physVol_World, false, i);
        new G4PVPlacement(trans3D_PAD_Silicon,  "PhysVol_Si",       logVol_PAD_Silicon, physVol_World, false, i);
        new G4PVPlacement(trans3D_PAD_Glue2,    "PhysVol_Glue2",    logVol_PAD_Glue2,   physVol_World, false, i);
        new G4PVPlacement(trans3D_PAD_FPC,      "PhysVol_FPC",      logVol_PAD_FPC,     physVol_World, false, i);
    }

    if ( cfg.bAbsorber ) {
        G4Box*           Absorber_PbWO4         = new G4Box("PPAD_PbWO4",   cfg.Pad_Width/2.0, cfg.Pad_Width/2.0, cfg.Absorber_Thickness/2.0);
        G4LogicalVolume* logVol_Absorber_PbWO4  = new G4LogicalVolume( Absorber_PbWO4, material_PbWO4, "logVol_Absorber_PbWO4" );

        G4ThreeVector threeVect_Abs(0., 0., -(cfg.Pad_Thickness/2 + cfg.Pad_AirGap + cfg.Absorber_Thickness) );
        G4Transform3D trans3D_Absorber = G4Transform3D(G4RotationMatrix(), threeVect_Abs);
        new G4PVPlacement(trans3D_Absorber,  "PhysVol_Absorber", logVol_Absorber_PbWO4,  physVol_World, false, 0);

        fScoringVol_PAD = logVol_Absorber_PbWO4;
    }

    return physVol_World;
}
