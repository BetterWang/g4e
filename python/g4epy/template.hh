#ifndef G4E_TEMPLATE_HH
#define G4E_TEMPLATE_HH

#include <InitializationContext.hh>


struct /*{{detector_name}}*/_Config
{
    // define here Global volume parameters
    double ExampleParameter = 42;   /// This parameter is an example
};


class /*{{detector_name}}*/_Design
{
public:

    /// Constructs outer or 'mother' volume for the detector
    inline void ConstructOuter(/*{{detector_name}}*/_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume)
    {
        printf("Begin TEMPLATE volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors

        // Visual attributes
        VisAttr = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
        VisAttr->SetLineWidth(1);
        VisAttr->SetForceSolid(true);
        // Logic->SetVisAttributes(VisAttr);
    };


    /// Construct internal parts of the detector
    inline void ConstructDetectors()
    {
        printf("Begin TEMPLATE detector volumes \n");

        // construct here your detectors

    };


    // Public variables
    G4Tubs *Solid;              // pointer to the solid (!) Might be not G4Tubs
    G4LogicalVolume *Logic;     // pointer to the logical
    G4VPhysicalVolume *Phys;    // pointer to the physical
    G4VisAttributes *VisAttr;   // Visual attributes

    /// Parameters that was used in the moment of construction
    /*{{detector_name}}*/_Config ConstructionConfig;

    // constructor /*{{detector_name}}*/_Design(g4e::InitializationContext& context):fContext(context){}
private:

    // if initialization context is needed
    // g4e::InitializationContext &fContext;

    // define here local variables and parameter of detectors
};


#endif //G4E_TEMPLATE_HH
