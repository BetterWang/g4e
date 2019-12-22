//
// Created by yulia on 6/11/19.
//

#ifndef G4E_CB_HCAL_HH
#define G4E_CB_HCAL_HH


#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"
#include <spdlog/spdlog.h>

//#include "../../../design_main/jleic/JLeicDetectorConfig.hh"

struct cb_HCAL_Config {

    G4double RIn;
    G4double ROut;
    G4double Thickness = 100. * cm;

    G4double SizeZ;
    G4double ShiftZ;

};


class cb_HCAL_Design {
public:
    inline void Construct(cb_HCAL_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume)
    {
        namespace log = spdlog;

        log::debug("Begin cb_HCAL volume \n");
        ConstructionConfig = cfg;

        //cb_HCAL_det_Material = fMat->GetMaterial("StainlessSteel");
        Solid = new G4Tubs("cb_HCAL_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "cb_HCAL_GVol_Logic");

        // Phys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, cfg.ShiftZ - cfg.SizeZ / 2), "cb_HCAL_GVol_Phys", Logic,motherVolume, false, 0);
        Phys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, cfg.ShiftZ), "cb_HCAL_GVol_Phys", Logic, motherVolume, false, 0);

        auto visAttr = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
        visAttr->SetLineWidth(1);
        visAttr->SetForceSolid(false);
        Logic->SetVisAttributes(visAttr);
        //  cb_HCAL_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);

    }


    inline void ConstructLayers()
    {
        namespace log = spdlog;

        auto cfg = ConstructionConfig;

        log::debug("Create HCAL  Size Z = {} \n ", cfg.SizeZ);

        fSizeZ = cfg.SizeZ;
        fLayerThickness = 2 * cm;

        fMaterial = G4Material::GetMaterial("Iron");

        for (size_t hlay = 0; hlay < LayersTotal; hlay++) {

            double rIn = cfg.RIn + hlay * 2 * fLayerThickness;
            double rOut = rIn + fLayerThickness;
            if (rIn > cfg.ROut || rOut > cfg.ROut) continue;

            auto solidName = fmt::format("cb_HCAL_det_Solid_{}", hlay);
            auto logicName = fmt::format("cb_HCAL_det_Logic_{}", hlay);
            auto physName = fmt::format("cb_HCAL_det_Phys_{}", hlay);

            fDetailedSolid = new G4Tubs(solidName, rIn, rOut, fSizeZ / 2., 0., 360 * deg);
            fDetailedLogic = new G4LogicalVolume(fDetailedSolid, fMaterial, logicName);
            fDetailedPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), physName, fDetailedLogic, Phys, false, hlay);

            auto visAttr = new G4VisAttributes(G4Color(0.6, 0, 0.6, 1));
            visAttr->SetLineWidth(1);
            visAttr->SetForceSolid(true);
            fDetailedLogic->SetVisAttributes(visAttr);
        }
    }



    inline void ConstructRings()
    {
        namespace log = spdlog;
        auto cfg = ConstructionConfig;
        log::debug("Create HCAL Rings\n");

        fSizeZ = cfg.SizeZ;
        fLayerThickness = 2 * cm;

        fMaterial = G4Material::GetMaterial("Iron");

        for (size_t hlay = 0; hlay < LayersTotal-1; hlay++) {

            double rIn = cfg.RIn + hlay * 2 * fLayerThickness + fLayerThickness + 0.2*cm;
            double rOut = rIn + fLayerThickness - 0.4*cm;
            if (rIn > cfg.ROut || rOut > cfg.ROut) continue;

            auto solidName = fmt::format("cb_HCAL_det_RingSolid_{}", hlay);
            auto logicName = fmt::format("cb_HCAL_det_RingLogic_{}", hlay);
            auto physName = fmt::format("cb_HCAL_det_RingPhys_{}", hlay);

            fDetailedSolid = new G4Tubs(solidName, rIn, rOut, fSizeZ / 2., 0., 360 * deg);
            fDetailedLogic = new G4LogicalVolume(fDetailedSolid, fMaterial, logicName);
            fDetailedPhys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), physName, fDetailedLogic, Phys, false, hlay);

            auto visAttr = new G4VisAttributes(G4Color(0.0, 0.6, 0.6, 1));
            visAttr->SetLineWidth(1);
            visAttr->SetForceSolid(true);
            fDetailedLogic->SetVisAttributes(visAttr);
        }
    }

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    cb_HCAL_Config ConstructionConfig;
private:

    const size_t LayersTotal = 25;
    G4double fSizeZ;
    G4double fLayerThickness;
    G4Material *fMaterial;
    G4Tubs *fDetailedSolid;             // pointer to the solid
    G4LogicalVolume *fDetailedLogic;    // pointer to the logical
    G4VPhysicalVolume *fDetailedPhys;   // pointer to the physical
};

#endif //G4E_CB_HCAL_HH
