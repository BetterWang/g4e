//
// Created by yulia on 6/12/19.
//

#ifndef G4E_CI_DRICH_HH
#define G4E_CI_DRICH_HH


#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

struct ci_DRICH_Config {
// define here Global volume parameters
    double RIn ;
    double ROut  = 150 * cm;
    double ThicknessZ = 170 * cm;
    double PosZ ;
};

class ci_DRICH_Design {
public:
    inline void Construct(ci_DRICH_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin ci_DRICH volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors

        Solid = new G4Tubs("ci_DRICH_GVol_Solid", cfg.RIn, cfg.ROut, cfg.ThicknessZ / 2., 0., 360 * deg);

        Logic = new G4LogicalVolume(Solid, worldMaterial, "ci_DRICH_GVol_Logic");

        Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.PosZ), "ci_DRICH_GVol_Phys", Logic,
                                               Phys, false, 0);

        //  fLogic_H->SetVisAttributes(G4VisAttributes::Invisible);

        G4VisAttributes *attr_ci_DRICH_GVol = new G4VisAttributes(G4Color(1., 1., 0.2, 0.2));
        attr_ci_DRICH_GVol->SetLineWidth(1);
        attr_ci_DRICH_GVol->SetForceSolid(true);
        Logic->SetVisAttributes(attr_ci_DRICH_GVol);



    };

    inline void ConstructDetectors() {
        printf("Begin ci_DRICH detector volumes \n");

        // construct here your detectors

    };

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ci_DRICH_Config  ConstructionConfig;

private:

    // define here local variables and parameter of detectors
};


#endif //G4E_CI_DRICH_HH
