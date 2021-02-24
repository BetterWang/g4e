
#include "ci_DRICH_Design.hh"
#include "ci_DRICH_Config.hh"

#include <G4LogicalVolume.hh>
#include <G4Tubs.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <spdlog/common.h>

TGeoManager *BuildDRichModel(Double_t r_entr=210., Double_t r_exit=210.);

void ci_DRICH_Design::Construct(ci_DRICH_Config cfg, G4Material* worldMaterial,  G4VPhysicalVolume *motherVolume) {
    printf("Begin ci_DRICH volume \n");

    ConstructionConfig = cfg;

    // Create mother volume
    Solid = new G4Tubs("ci_DRICH_GVol_Solid", cfg.RIn, cfg.ROut, cfg.ThicknessZ / 2., 0., 360 * deg);
    Logic = new G4LogicalVolume(Solid, worldMaterial, "ci_DRICH_GVol_Logic");
    Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.PosZ), "ci_DRICH_GVol_Phys", Logic, motherVolume, false, 0);
    fmt::print("Begin ci_DRICH volume {} {} {} {} \n",cfg.RIn, cfg.ROut, cfg.ThicknessZ, cfg.PosZ );

    // Make this global volume - invisible
    Logic->SetVisAttributes(G4VisAttributes::Invisible);

    // Build detector geometry using ROOT function
    auto rich = BuildDRichModel();

    // Import Geant4 geometry to VGM
    Geant4GM::Factory g4Factory;
    RootGM::Factory rtFactory;
    rich->cd("world/Vessel");
    auto vessel = rich->GetTopNode();

    rtFactory.Import(vessel);
    rtFactory.Export(&g4Factory);
    g4Factory.PrintVolumes();

    RichPhysical = g4Factory.World();
    RichPhysical->SetMotherLogical(Logic);
    Logic->AddDaughter(RichPhysical);
    Logic = Phys->GetLogicalVolume();


    G4VisAttributes *attr_ci_DRICH_GVol = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.1));

    attr_ci_DRICH_GVol->SetVisibility(false);
    attr_ci_DRICH_GVol->SetLineWidth(1);
    attr_ci_DRICH_GVol->SetForceSolid(true);
    RichPhysical->GetLogicalVolume()->SetVisAttributes(attr_ci_DRICH_GVol);
}

void ci_DRICH_Design::ConstructDetectors() {
//        printf("Begin ci_DRICH detector volumes \n");
//
//
//
//        InternalsLogic = new G4LogicalVolume(Logic, fWorldMaterial, "WorldLogic");
//        fWorldPhysicalVolume = new G4PVPlacement(nullptr, G4ThreeVector(), "WorldPhys", fWorldLogicVol, nullptr, false, 0);
//
//        //  fLogic_H->SetVisAttributes(G4VisAttributes::Invisible);
//
//        //    G4VisAttributes *attr_ci_DRICH_GVol = new G4VisAttributes(G4Color(1., 1., 0.2, 1.));
//
//        G4VisAttributes *attr_ci_DRICH_GVol = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
//
//        attr_ci_DRICH_GVol->SetLineWidth(1);
//        attr_ci_DRICH_GVol->SetForceSolid(true);
//        Logic->SetVisAttributes(attr_ci_DRICH_GVol);



//#include "TGeoOpticalSurface.h"


    // construct here your detectors

//        G4VisAttributes *attr_ci_DRICH_GVol = new G4VisAttributes(G4Color(1., 0.9, 0.6, 0.1));
//
//        attr_ci_DRICH_GVol->SetLineWidth(1);
//        attr_ci_DRICH_GVol->SetForceSolid(true);
//        Logic->SetVisAttributes(attr_ci_DRICH_GVol);

}
