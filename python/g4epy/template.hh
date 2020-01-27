#ifndef G4E_TEMPLATE_HH
#define G4E_TEMPLATE_HH

#include <G4GenericMessenger.hh>


struct /*{{detector_name}}*/_Config
{
    // define here Global volume parameters
    double ExampleParameter;   // This parameter is example

    /*{{detector_name}}*/_Config():
        fMessenger(this, "/subdetectors//*{{detector_name}}*//")
    {
        // Example of crating a messenger for exampleParameter
        fMessenger.DeclareProperty("exampleParameter", ExampleParameter, "Example of adding a messenger for detector config");
    }

private:
    G4GenericMessenger fMessenger;
};


class /*{{detector_name}}*/_Design
{
public:
    inline void Construct(/*{{detector_name}}*/_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume)
    {
        printf("Begin TEMPLATE volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors

        // Visual attributes
        G4VisAttributes *VisAttr = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
        VisAttr->SetLineWidth(1);
        VisAttr->SetForceSolid(true);
        // Logic->SetVisAttributes(VisAttr);
    };

    inline void ConstructDetectors()
    {
        printf("Begin TEMPLATE detector volumes \n");

        // construct here your detectors

    };

    G4Tubs *Solid;              // pointer to the solid
    G4LogicalVolume *Logic;     // pointer to the logical
    G4VPhysicalVolume *Phys;    // pointer to the physical

    /// Parameters that was used in the moment of construction
    /*{{detector_name}}*/_Config ConstructionConfig;

private:

    // define here local variables and parameter of detectors
};


#endif //G4E_TEMPLATE_HH
