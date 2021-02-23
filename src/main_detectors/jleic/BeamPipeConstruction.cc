#include "BeamPipeConstruction.hh"

#include <G4SystemOfUnits.hh>
#include <G4PVPlacement.hh>
#include <G4VisAttributes.hh>
#include <G4NistManager.hh>
#include <G4Tubs.hh>



#include "Materials.hh"


BeamPipeConstruction::BeamPipeConstruction(g4e::InitializationContext *initContext):
    fInitContext(initContext)
{
    fMaterials = new g4e::Materials();
}

G4VPhysicalVolume *BeamPipeConstruction::Construct() {
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

    Construct(fWorldPhysicalVolume);
}

G4VPhysicalVolume *BeamPipeConstruction::Construct(G4VPhysicalVolume *worldPhysVol) {
    // Materials //

    auto bpCadDirectory = fmt::format("{}/resources/ip6/beampipe/", fInitContext->Arguments->HomePath);
    ir_Beampipe.Construct(ir_BeampipeConfig, worldPhysVol, bpCadDirectory);
    m_ci_DRICH.Construct(m_ci_DRICH_Config, fWorldMaterial, fWorldPhysicalVolume);
    m_ci_DRICH.ConstructDetectors();

    return worldPhysVol;
}

void BeamPipeConstruction::ConstructSDandField() {
    G4VUserDetectorConstruction::ConstructSDandField();
}
