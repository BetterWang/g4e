//
// Created by yulia on 6/14/19.
//

#ifndef G4E_CI_HCAL_HH
#define G4E_CI_HCAL_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include <spdlog/spdlog.h>

#include "../../../design_main/jleic/JLeicDetectorConfig.hh"


struct ci_HCAL_Config
{
    // define here Global volume parameters
    double RIn = 0 * cm;
    double ROut = 300 * cm;
    double SizeZ = 195 * cm;
    double ShiftZ = 5 * cm;
    double PosZ = 0. * cm;
};


class ci_HCAL_Design
{
public:
    inline void Construct(ci_HCAL_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume)
    {
        namespace log = spdlog;

        log::debug("Begin ci_HCAL volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors
        // cb_HCAL_det_Material = fMat->GetMaterial("StainlessSteel");
        // ci_HCAL_GVol_Solid = new G4Polycone("HCAP_HCAL_vol_sol",0.*deg,360.*deg,2,fHCAL_HCAP_Zcone, ci_HCAL_GVol_RIn, ci_HCAL_GVol_ROut);

        Solid = new G4Tubs("ci_HCAL_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "ci_HCAL_GVol_Logic");

        //  ci_HCAL_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0,0,0), "HCAP_HCAL_vol_phys",ci_HCAL_GVol_Logic, World_Phys, false,	0 );

        Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.PosZ), "ci_HCAL_GVol_Phys", Logic, motherVolume, false, 0);

        G4VisAttributes *attr_ci_HCAL_GVol = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
        attr_ci_HCAL_GVol->SetLineWidth(1);
        attr_ci_HCAL_GVol->SetForceSolid(false);
        Logic->SetVisAttributes(attr_ci_HCAL_GVol);
        //  cb_HCAL_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);
    };

    inline void ConstructDetectors()
    {
        namespace log = spdlog;

        log::debug("Begin ci_HCAL detector volumes \n");

        // construct here your detectors
        auto cfg = ConstructionConfig;

        ci_HCAL_det_RIn = 80 * cm;
        ci_HCAL_det_ROut = cfg.ROut - 1 * cm;
        ci_HCAL_det_ThicknessZ = 2 * cm;
        ci_HCAL_det_GapZ = 2 * cm;
        ci_HCAL_TotalLayers = 25;
        ci_HCAL_det_Material = G4Material::GetMaterial("Iron");
        log::debug("ci_HCAL_det::1 {} {}, {}\n", ci_HCAL_det_RIn, ci_HCAL_det_ROut, ci_HCAL_det_ThicknessZ);

        ci_HCAL_det_Solid = new G4Tubs("ci_HCAL_det_Solid", ci_HCAL_det_RIn, ci_HCAL_det_ROut, ci_HCAL_det_ThicknessZ / 2., 0., 360 * deg);
        ci_HCAL_det_Logic = new G4LogicalVolume(ci_HCAL_det_Solid, ci_HCAL_det_Material, "ci_HCAL_det_Logic");

        attr_ci_HCAL_det = new G4VisAttributes(G4Color(0.6, 0, 0.6, 1));
        attr_ci_HCAL_det->SetLineWidth(1);
        attr_ci_HCAL_det->SetForceSolid(true);
        ci_HCAL_det_Logic->SetVisAttributes(attr_ci_HCAL_det);

        for (size_t layerIndex = 0; layerIndex < ci_HCAL_TotalLayers; layerIndex++) {
            ci_HCAL_det_PosZ = -cfg.SizeZ / 2 + (layerIndex + 1) * ci_HCAL_det_ThicknessZ + (layerIndex + 1) * 5 * cm;

            log::debug("ci_HCAL_det::2 {} {} ,{}\n", ci_HCAL_det_PosZ, ci_HCAL_det_PosZ - ci_HCAL_det_ThicknessZ, cfg.SizeZ);

            if ((ci_HCAL_det_PosZ - ci_HCAL_det_ThicknessZ) > cfg.SizeZ / 2) continue;
            //   ci_HCAL_det_PosZ=-ci_HCAL_GVol_SizeZ/2+hhlay*ci_HCAL_det_ThicknessZ+ci_HCAL_det_GapZ*hhlay;
            // ci_HCAL_det_PosZ= hhlay*ci_HCAL_det_ThicknessZ+ci_HCAL_det_GapZ*hhlay;
            auto name = fmt::format("ci_HCAL_det_Phys_{}", layerIndex);
            ci_HCAL_det_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, ci_HCAL_det_PosZ), name, ci_HCAL_det_Logic, Phys, false, layerIndex);
        }
    }

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ci_HCAL_Config ConstructionConfig;

private:

    // define here local variables and parameter of detectors

    //---------------ENDCAP-H HCAL det iron-------------------
    G4double ci_HCAL_det_RIn;
    G4double ci_HCAL_det_ROut;
    G4double ci_HCAL_det_ThicknessZ;
    G4double ci_HCAL_det_GapZ;
    G4double ci_HCAL_det_PosZ;
    size_t ci_HCAL_TotalLayers;
    G4VisAttributes *attr_ci_HCAL_det;
    G4Material *ci_HCAL_det_Material;
    G4Tubs *ci_HCAL_det_Solid;    //pointer to the solid HCAP_HCAL volume
    G4LogicalVolume *ci_HCAL_det_Logic;    //pointer to the logicalHCAP_HCAL  volume
    G4VPhysicalVolume *ci_HCAL_det_Phys;    //pointer to the physical EMCAL barrel volume


};

#endif //G4E_CI_HCAL_HH
