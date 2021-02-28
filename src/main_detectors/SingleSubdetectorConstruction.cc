#include "SingleSubdetectorConstruction.hh"

#include <G4SystemOfUnits.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4NistManager.hh>
#include <G4Tubs.hh>

#include "Materials.hh"
#include "DetectorConfig.hh"
#include "StringHelpers.hh"


SingleSubdetectorConstruction::SingleSubdetectorConstruction(g4e::InitializationContext *initContext, DetectorConfig &config):
    fInitContext(initContext),
    fConfig(config),
    fMessenger(this, "/eic/subdetector/", "EIC Single subdetector construction messenger")
{
    fMaterials = new g4e::Materials();
    fMessenger.DeclareProperty("name", fSubdetectorName, "Sub-Detector name");
}

G4VPhysicalVolume *SingleSubdetectorConstruction::Construct() {
    using namespace fmt;

    //===================================================================================
    //==                    create a world                                            ==
    //===================================================================================

    // If this function is called, this means that there is no volume

    double SizeR = 300. * cm;
    double SizeZ = 50000. * cm;
    fWorldMaterial = fMaterials->GetMaterial("G4_Galactic");
    fWorldSolidVol = new G4Box("WorldSolid", SizeR, SizeR, SizeZ / 2.);
    fWorldLogicVol = new G4LogicalVolume(fWorldSolidVol, fWorldMaterial, "WorldLogic");
    fWorldPhysicalVolume = new G4PVPlacement(nullptr, G4ThreeVector(), "WorldPhys", fWorldLogicVol, nullptr, false, 0);
    fmt::print("Initializing WORLD. x:{:<10}m y:{:<10}m z:{:<10}m", SizeR * 2 / m, SizeR * 2 / m, SizeZ / m);

    auto name = g4e::ToLowerCopy(fSubdetectorName);
    if(name == "ci_drich") {
        return Construct_ci_DRICH();
    }
    if(name == "ir_beampipe" || name == "beampipe") {
        return Construct_Beampipe();
    }

    if(fSubdetectorName == "ir_beamline" || fSubdetectorName == "beamline") {
        return Construct_Beamline();
    }


    G4Exception("SingleSubdetectorConstruction::Construct",
                "InvalidSubdetectorName", FatalException,
                "Subdetector name doesn't correspond to any known sub-detector");

}


void SingleSubdetectorConstruction::ConstructSDandField() {
    G4VUserDetectorConstruction::ConstructSDandField();
}

G4VPhysicalVolume *SingleSubdetectorConstruction::Construct_Beampipe() {
    /** The function constructs BeamPipe standalone */
    auto bpCadDirectory = fmt::format("{}/resources/ip6/beampipe/", fInitContext->Arguments->HomePath);
    ir_Beampipe.Construct(fConfig.ir_Beampipe, fWorldPhysicalVolume, bpCadDirectory);
    return fWorldPhysicalVolume;
}

G4VPhysicalVolume *SingleSubdetectorConstruction::Construct_Beamline() {
    return fWorldPhysicalVolume;
}

G4VPhysicalVolume *SingleSubdetectorConstruction::Construct_ci_DRICH() {
    ci_DRICH.Construct(fConfig.ci_DRICH, fWorldMaterial,  fWorldPhysicalVolume);
    ci_DRICH.ConstructDetectors();

    return fWorldPhysicalVolume;
}
