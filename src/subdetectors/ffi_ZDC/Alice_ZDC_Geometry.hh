//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Geometry.hh
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef Geometry_h
#define Geometry_h 1

#include "G4VUserDetectorConstruction.hh"
//#include "constants.hh"
#include "Alice_ZDC_constants.hh"


struct ffi_ZDCPrototype_Config {
    double Pad_Width             = 600.*mm;
    double Pad_Thickness         = 3.5*mm;
    double Pad_Glue1_Thickness   = 0.11*mm;
    double Pad_Silicon_Thickness = 0.32*mm;
    double Pad_Glue2_Thickness   = 0.13*mm;
    double Pad_FPC_Thickness     = 0.28*mm;
    double Pad_AirGap            = 1.0*mm;
    double Absorber_Thickness    = 200.*mm;
    int    NLayers               = 40;
    bool   bAbsorber             = false;
};


class G4VPhysicalVolume;
class G4LogicalVolume;
//------------------------------------------------------------------------------
class Geometry : public G4VUserDetectorConstruction
//------------------------------------------------------------------------------
{
    public:
        Geometry(bool bAbsorber=true);
        ~Geometry();

        G4VPhysicalVolume*  Construct();
        G4VPhysicalVolume*  ConstructZDC(G4VPhysicalVolume*);
        G4VPhysicalVolume*  ConstructZDCPrototype(G4VPhysicalVolume*);
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

        ffi_ZDCPrototype_Config  cfg;
//        G4LogicalVolume*  logVol_PAD_W;
//        G4LogicalVolume*  logVol_PAD_Glue1;
//        G4LogicalVolume*  logVol_PAD_Silicon;
//        G4LogicalVolume*  logVol_PAD_Glue2;
//        G4LogicalVolume*  logVol_PAD_FPC;
};
#endif
