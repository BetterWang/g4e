
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

    G4VisAttributes *outerVolumeVisAttr = new G4VisAttributes(G4Color(0., 0., 0.9, 0.5));

    outerVolumeVisAttr->SetForceSolid(true);
    outerVolumeVisAttr->SetVisibility(true);
    outerVolumeVisAttr->SetLineWidth(1);
    outerVolumeVisAttr->SetForceSolid(true);

    // Make this global volume - invisible
    Logic->SetVisAttributes(G4VisAttributes::Invisible);

    //Logic->SetVisAttributes(outerVolumeVisAttr);
}

void ci_DRICH_Design::ConstructDetectors() {


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

    RichWorld = g4Factory.World();

    // The first vessel in Rich World is ci_DRICH_Vessel
    RichVessel = RichWorld->GetLogicalVolume()->GetDaughter(0);

    RichVessel->SetMotherLogical(Logic);
    Logic->AddDaughter(RichVessel);

    G4VisAttributes *RichVolumeVisAttr = new G4VisAttributes(G4Color(0.1, 0.1, 0.1, 0.5));

    RichVolumeVisAttr->SetForceWireframe();
    RichVolumeVisAttr->SetVisibility(true);
    RichVolumeVisAttr->SetLineWidth(2);
    RichVessel->GetLogicalVolume()->SetVisAttributes(RichVolumeVisAttr);

    int pdetCounter = 0;
    auto richLogical = RichVessel->GetLogicalVolume();
    for (int i=0; i < richLogical->GetNoDaughters(); i++) {
        auto dLV = richLogical->GetDaughter(i)->GetLogicalVolume();

        // This one is basically coloring
        if(dLV->GetName() == "ci_DRICH_vcenter") {
            G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.0, 0.9, 0., 0.5));
            visAttr->SetForceSolid();
            visAttr->SetVisibility(true);
            dLV->SetVisAttributes(visAttr);
        }

        if(dLV->GetName() == "ci_DRICH_aerogel") {
            G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.0, 0.5, 5., 0.5));
            visAttr->SetForceCloud();
            visAttr->SetVisibility(true);
            dLV->SetVisAttributes(visAttr);
        }

        if(dLV->GetName() == "ci_DRICH_acrylic") {
            G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.8, 0.1, 0.8, 0.8));
            visAttr->SetForceSolid();
            visAttr->SetVisibility(true);
            dLV->SetVisAttributes(visAttr);
        }

        if(dLV->GetName() == "ci_DRICH_mirror") {
            G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.5, 0.5, 0.5, 1));
            visAttr->SetForceLineSegmentsPerCircle(100);
            visAttr->SetForceWireframe();
            visAttr->SetVisibility(true);
            visAttr->SetLineWidth(3);
            dLV->SetVisAttributes(visAttr);
        }

        if(dLV->GetName() == "ci_DRICH_rotm") {
            G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.7, 0, 0.7, 0.5));
            visAttr->SetForceSolid();
            visAttr->SetVisibility(true);
            visAttr->SetLineWidth(1);
            dLV->SetVisAttributes(visAttr);
        }

        if(dLV->GetName() == "ci_DRICH_phdet") {
            G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0, 0, 0.6, 0.5));
            visAttr->SetForceSolid();
            visAttr->SetVisibility(true);
            visAttr->SetLineWidth(1);
            visAttr->SetForceSolid(true);
            dLV->SetVisAttributes(visAttr);

            // (!) Save pdets so one can add sensitive detector to them later
            auto pdetPhysical=richLogical->GetDaughter(i);
            PhotoDets.push_back(pdetPhysical);

            // Add pdet index to its name
            pdetPhysical->SetName(fmt::format("ci_DRICH_phdet_{}", pdetCounter));
            pdetPhysical->GetLogicalVolume()->SetName(fmt::format("ci_DRICH_phdet_{}", pdetCounter));
        }
    }
}

