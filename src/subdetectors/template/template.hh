#ifndef G4E_TEMPLATE_HH
#define G4E_TEMPLATE_HH

struct TEMPLATE_Config
{
// define here Global volume parameters

};


class TEMPLATE_Design
{
public:
    inline void Construct(TEMPLATE_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume)
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
    TEMPLATE_Config ConstructionConfig;

private:

    // define here local variables and parameter of detectors
};


#endif //G4E_TEMPLATE_HH
