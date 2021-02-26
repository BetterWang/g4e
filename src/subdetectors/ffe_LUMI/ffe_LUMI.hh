//
// Created by yulia on 6/12/19.
//

#ifndef G4E_FFE_LUMI_HH
#define G4E_FFE_LUMI_HH


#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "main_detectors/DetectorConfig.hh"

struct ffe_LUMI_Config
{
// define here Global volume parameters
    double SizeX = 2. * m;
    double SizeY = 2. * m;
  // old   double SizeZ = 30 * m;
    double SizeZ = 10 * m;
    double PosX = 2 * m;
    double PosY = 2 * m;
    double PosZ = 2 * m;
};


class ffe_LUMI_Design
{
public:
    inline void Construct(ffe_LUMI_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume)
    {
        printf("Begin ffe_LUMI volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors

        //Global volume ---- ( check that your world is smaller then this volume
        Solid = new G4Box("ffe_LUMI_GVol_Solid", cfg.SizeX / 2., cfg.SizeY / 2., cfg.SizeZ / 2.);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "ffe_LUMI_GVol_Logic");
        //top_l->SetVisAttributes( G4VisAttributes::GetInvisible() );
        Phys = new G4PVPlacement(nullptr, G4ThreeVector(cfg.PosX, cfg.PosY, cfg.PosZ), "ffe_LUMI_GVol_Phys", Logic, motherVolume, false, 0);
        // Visual attributes
        auto *VisAttr = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
        VisAttr->SetLineWidth(1);
        VisAttr->SetForceSolid(true);
        Logic->SetVisAttributes(VisAttr);

    };

    inline void ConstructDetectors()
    {
        printf("Begin ffe_LUMI detector volumes \n");
        auto cfg = ConstructionConfig;
        // construct here your detectors

    };

    G4Box *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ffe_LUMI_Config ConstructionConfig;

private:

    // define here local variables and parameter of detectors
};


#endif //G4E_FFE_LUMI_HH
