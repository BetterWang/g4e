//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Geometry.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef Geometry_h
#define Geometry_h 1

#include "G4VUserDetectorConstruction.hh"
//#include "constants.hh"
#include "Alice_ZDC_constants.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
//------------------------------------------------------------------------------
  class Geometry : public G4VUserDetectorConstruction
//------------------------------------------------------------------------------
{
  public:
    Geometry();
   ~Geometry();

  G4VPhysicalVolume* Construct();
  G4VPhysicalVolume*  ConstructZDC(G4VPhysicalVolume *physVol_World);
	//Passive volumes
    G4LogicalVolume* GetScoringVol_EMCal1() const { return fScoringVol_EMCal1; }
    G4LogicalVolume* GetScoringVol_EMCal2() const { return fScoringVol_EMCal2; }
    G4LogicalVolume* GetScoringVol_HCal() const { return fScoringVol_HCal; }
	//Active volumes
    G4LogicalVolume* GetScoringVol_PAD() const { return fScoringVol_PAD; }
    G4LogicalVolume* GetScoringVol_PIX() const { return fScoringVol_PIX; }
    G4LogicalVolume* GetScoringVol_SCI() const { return fScoringVol_SCI; }
	//Leakage "detector"
    G4LogicalVolume* GetScoringVol_Leak() const { return fScoringVol_Leak; }
	
  protected:
	
	G4LogicalVolume*  fScoringVol_EMCal1;
	G4LogicalVolume*  fScoringVol_EMCal2;
	G4LogicalVolume*  fScoringVol_HCal;  
	  
    G4LogicalVolume*  fScoringVol_PAD;
    G4LogicalVolume*  fScoringVol_PIX;
    G4LogicalVolume*  fScoringVol_SCI;
	
    G4LogicalVolume*  fScoringVol_Leak;
};
#endif
