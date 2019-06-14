//
// Created by yulia on 6/14/19.
//

#ifndef G4E_CI_EMCAL_HH
#define G4E_CI_EMCAL_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

struct ci_EMCAL_Config {
// define here Global volume parameters

};


class ci_EMCAL_Design {
public:
    inline void Construct(ci_EMCAL_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin ci_EMCAL volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors



    };

    inline void ConstructDetectors() {
        printf("Begin ci_EMCAL detector volumes \n");

        // construct here your detectors

    };

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ci_EMCAL_Config  ConstructionConfig;

private:

    // define here local variables and parameter of detectors
};
#endif //G4E_CI_EMCAL_HH
