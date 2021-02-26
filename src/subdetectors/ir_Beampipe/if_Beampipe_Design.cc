#include "CADMesh.hh"
#include <G4TessellatedSolid.hh>
#include <spdlog/common.h>
#include "ir_Beampipe_Design.hh"



void ir_Beampipe_Design::Construct(ir_Beampipe_Config cfg, G4VPhysicalVolume *motherVolume, G4String cadDir) {
    ConstructionConfig = cfg;

    auto  beMaterial = G4Material::GetMaterial("Be");

    // Electron IR tube (-440mm, 0)
    IrElectronSolid = new G4Tubs("ir_Beampipe_IrElectron_Solid", cfg.IrInnerR, cfg.IrOuterR, cfg.IrElectronSizeZ / 2., 0., 360 * deg);
    IrElectronLogic = new G4LogicalVolume(IrElectronSolid, beMaterial, "ir_Beampipe_IrElectron_Logic");
    IrElectronPhysical = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -cfg.IrElectronSizeZ / 2.),  // Rotation translation
                                           "ir_Beampipe_IrElectron_Phys", IrElectronLogic, motherVolume,           // Names and logical
                                           false, 0);                                                 // How many, copy number

    IrElectronVisual = new G4VisAttributes(G4Color(1., 1., 0., 1.));
    IrElectronVisual->SetForceSolid(true);
    IrElectronLogic->SetVisAttributes(IrElectronVisual);


//    // Hadron part of Interaction region [0, 1000mm)
//    IrHadronSolid = new G4Tubs("ir_Beampipe_IrHadron_Solid", cfg.IrInnerR, cfg.IrOuterR, cfg.IrElectronSizeZ / 2., 0., 360 * deg);
//    IrHadronLogic = new G4LogicalVolume(IrElectronSolid, beMaterial, "ir_Beampipe_IrHadron_Logic");
//    IrHadronPhysical = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -cfg.IrElectronSizeZ / 2.),  // Rotation translation
//                                         "ir_Beampipe_IrHadron_Phys", IrElectronLogic, motherVolume,           // Names and logical
//                                         false, 0);                                               // How many, copy number
//
//    IrHadronVisual = new G4VisAttributes(G4Color(1., 1., 0., 1.));
//    IrHadronVisual->SetForceSolid(true);
//    IrHadronLogic->SetVisAttributes(IrElectronVisual);


    G4NistManager * nistManager = G4NistManager::Instance();

    auto  beryllium = G4Material::GetMaterial("Be");
    auto  aluminium = G4Material::GetMaterial("Al");

    /*
     *
     DetectorChamberElectronExitRound.stl
     DetectorChamberHadronForward.stl
     DetectorChamberInnerHadronForward.stl
     DetectorChamberInnerHadronRearRound.stl
     DN63 CF.stl
     NEXTorr.bin.stl
     NEXTorr.stl
     PI-FLG-0007.stl
     PI-FLG-0061.stl
     PI-FLG-0153.stl

     */
//
//    ci_ENDCAP_GVol_Solid = new G4Tubs("ci_ENDCAP_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
//    ci_ENDCAP_GVol_Logic = new G4LogicalVolume(ci_ENDCAP_GVol_Solid, World_Material, "ci_ENDCAP_GVol_Logic");
//    ci_ENDCAP_GVol_Phys = new G4PVPlacement(nullptr, G4ThreeVector(cfg.PosX, 0, cfg.PosZ), "ci_ENDCAP_GVol_Phys", ci_ENDCAP_GVol_Logic, World_Phys, false, 0);
//

    auto hadronForwardChamberName = fmt::format("{}/DetectorChamberHadronForward.stl", cadDir);
    hadronForwardChamberMesh = CADMesh::TessellatedMesh::FromSTL(hadronForwardChamberName);

    auto chamberLogical = new G4LogicalVolume( hadronForwardChamberMesh->GetSolid(), beryllium, "HadronForwardChamberLogical", nullptr, nullptr, nullptr);
    new G4PVPlacement(nullptr, G4ThreeVector(), chamberLogical, "HadronForwardChamberPhysical", motherVolume->GetLogicalVolume(), false, 0);

    // Visual attributes
    static G4VisAttributes *VisAttr;
    VisAttr = new G4VisAttributes(G4Color(0.9, 0.9, 0.1, 0.3));
    VisAttr->SetLineWidth(1);
    VisAttr->SetForceSolid(true);
    chamberLogical->SetVisAttributes(VisAttr);




}



