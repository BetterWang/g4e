/**
 * This file defines ce_GEM_Config, ce_GEM_Design classes to construct ce_GEM geometry
 */
#ifndef G4E_CE_GEM_HH
#define G4E_CE_GEM_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"


/// Geometry construction configuration
struct ce_GEM_Config {
// define here Global volume parameters
    double RIn = 0 * cm;                // Inner diameter
    double ROut = 45 * cm + 50 * cm;    // Outer diameter
    double SizeZ = 30 * cm;             // Size in Z direction
    double PosZ =0*cm;                  // Absolute Z position (set by DetectorConst.)
    int Nlayers =8;                     // Number of layers
};

/// Holds information for each GEM layer
struct ce_GEM_Layer {
    G4double RIn;
    G4double ROut;
    G4double SizeZ;
    G4double PosZ;
    G4Tubs *Solid;             // pointer to the solid World
    G4LogicalVolume *Logic;    // pointer to the logical World
    G4VPhysicalVolume *Phys;   // pointer to the physical World
    G4String LogicName;        // name of the logic volume to set SD
};


/// Geometry construction
class ce_GEM_Design {
public:

    /// This function constructs the outer GEM volume
    inline void Construct(ce_GEM_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        G4cout << "Constructing ce_GEM volume \n";

        ConstructionConfig = cfg;   // Config is copy constructed and this is OK as we capture the state on the moment of construction

        // Create outer volume for GEM detector
        Solid = new G4Tubs("ce_GEM_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
        Logical = new G4LogicalVolume(Solid, worldMaterial, "ce_GEM_GVol_Logic");
        PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.PosZ), "ce_GEM_GVol_Phys", Logical, motherVolume, false, 0);
    };

    /// This function construct all GEM details
    inline void ConstructDetectors() {
        G4cout << "Begin ce_GEM detector geometry \n";
        static char abname[256];
        auto cfg = ConstructionConfig;

        // construct here your detectors
        //===================================================================================

        printf("Begin layer.\n");
        //layer.Material = fMat->GetMaterial("Si");
        fMaterial = G4Material::GetMaterial("Ar10CO2");  //----   !!!!! ----

        fLayerVisualAttributes = new G4VisAttributes(G4Color(0.8, 0.4, 0.3, 0.8));
        fLayerVisualAttributes->SetLineWidth(1);
        fLayerVisualAttributes->SetForceSolid(true);

        for (int layerIndex = 0; layerIndex < cfg.Nlayers; layerIndex++) {

            ce_GEM_Layer layer;

            layer.RIn = cfg.RIn + 1 * cm + (double(layerIndex) * 0.5) * cm;
            layer.ROut = cfg.ROut - 25 * cm + (double(layerIndex) * 2.) * cm;;

            //      layer.PosZ[lay]=-ce_GEM_GVol_PosZ/2+(double(lay)*5.)*cm;
            layer.PosZ = cfg.SizeZ / 2 - 5 * cm - (double(layerIndex) * 3.) * cm;
            layer.SizeZ = 1 * cm;

            sprintf(abname, "ce_GEM_l%d_Solid", layerIndex);
            layer.Solid = new G4Tubs(abname, layer.RIn, layer.ROut, layer.SizeZ / 2., 0., 360 * deg);

            sprintf(abname, "ce_GEM_l%d_Logic", layerIndex);
            layer.Logic = new G4LogicalVolume(layer.Solid, fMaterial, abname);
            layer.LogicName = abname;

            layer.Logic->SetVisAttributes(fLayerVisualAttributes);

            sprintf(abname, "ce_GEM_l%d_Phys", layerIndex);
            layer.Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, layer.PosZ), abname, layer.Logic, PhysicalVolume, false, 0);

            Layers.push_back(layer);
        }
    };

    G4Tubs *Solid;                      //pointer to the solid
    G4LogicalVolume *Logical;           //pointer to the logical
    G4VPhysicalVolume *PhysicalVolume;  //pointer to the physical
    std::vector<ce_GEM_Layer> Layers;

    /// Parameters that was used in the moment of construction
    ce_GEM_Config  ConstructionConfig;

private:
    G4VisAttributes *fLayerVisualAttributes;
    G4Material *fMaterial;

};



#endif //G4E_CE_GEM_HH
