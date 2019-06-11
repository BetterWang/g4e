//
// Created by yulia on 6/7/19.
//

#ifndef G4E_CB_CTD_HH
#define G4E_CB_CTD_HH


#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

/// Central Barrel Tracker definition
/// short name: cb_CTD
struct cb_CTD_Config {

    double RIn  = 21 * cm;
    double ROut = 80* cm;
    double SizeZCut=  60 * cm;
    double SizeZ;
    double SiLayerGap = 5.* cm;
    int    SiLayerCount = 15;
};


class cb_CTD_Design {
public:
    inline void Construct(cb_CTD_Config cfg, G4Material* worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin cb_CTD volume \n");

        ConstructionConfig=cfg;

        Solid = new G4Tubs("cb_CTD_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "cb_CTD_GVol_Logic");
        Phys = new G4PVPlacement(0, G4ThreeVector(), "cb_CTD_GVol_Phys", Logic, motherVolume, false, 0);

        // cb_CTD_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);
        G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.1, 0, 1., 0.1));
        visAttr->SetLineWidth(1);
        visAttr->SetForceSolid(false);
        Logic->SetVisAttributes(visAttr);
    }

    inline void ConstructLadders() {
        static char abname[256];
        auto cfg = ConstructionConfig;

        printf("Create CTD  Size Z =%f \n ",cfg.SizeZ);

        siMaterial =  G4Material::GetMaterial("Si");
        // ------- layers of Si in CTD
        for (int ia = 0; ia < cfg.SiLayerCount; ia++) {
            layerRIn[ia] = cfg.RIn + (cfg.SiLayerGap * ia);
            layerROut[ia] = cfg.RIn + (0.01 + cfg.SiLayerGap * ia);
            if(layerROut[ia] > cfg.ROut) continue;

            printf("cb_CTD_detSi %d  Rout=%f \n", ia, layerROut[ia]);
            sprintf(abname, "cb_CTD_detSi_Solid_lay_%d", ia);
            SiSolids[ia] = new G4Tubs(abname, layerRIn[ia], layerROut[ia], cfg.SizeZ / 2., 0., 360 * deg);

            sprintf(abname, "cb_CTD_detSi_Logic_lay_%d", ia);
            SiLogics[ia] = new G4LogicalVolume(SiSolids[ia], siMaterial, abname);

            auto layerVisAttr= new G4VisAttributes(G4Color(1.0 - 0.1 * ia, 1.0, 0.0 + 0.1 * ia, 1));
            layerVisAttr->SetLineWidth(1);
            layerVisAttr->SetForceSolid(true);
            SiLogics[ia]->SetVisAttributes(layerVisAttr);

            sprintf(abname, "cb_CTD_detSi_Phys_lay_%d", ia);
            cb_CTD_detSi_Phys[ia] = new G4PVPlacement(nullptr, G4ThreeVector(),
                                                      abname, SiLogics[ia],
                                                      Phys, false, 0);

            //----> Should be worked out          if (SiLogics[ia]) SiLogics[ia]->SetSensitiveDetector(fCalorimeterSD);
        }
    }

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    cb_CTD_Config ConstructionConfig;

private:
    G4Material *siMaterial;
    G4double layerRIn[100];
    G4double layerROut[100];
    G4Tubs *SiSolids[100];    //pointer to the solid World
    G4LogicalVolume *SiLogics[100];    //pointer to the logical World
    G4VPhysicalVolume *cb_CTD_detSi_Phys[100];    //pointer to the physical World
};

#endif //G4E_CB_CTD_HH
