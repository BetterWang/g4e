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

#include "../../../design_main/jleic/JLeicDetectorConfig.hh"

struct ffe_LUMI_Config {
// define here Global volume parameters

};


class ffe_LUMI_Design {
public:
    inline void Construct(ffe_LUMI_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin ffe_LUMI volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors



    };

    inline void ConstructDetectors() {
        printf("Begin ffe_LUMI detector volumes \n");
        auto cfg = ConstructionConfig;
        // construct here your detectors

    };

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ffe_LUMI_Config  ConstructionConfig;

private:

    // define here local variables and parameter of detectors
};


#endif //G4E_FFE_LUMI_HH
