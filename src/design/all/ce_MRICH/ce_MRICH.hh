//
// Created by yulia on 6/11/19.
//

#ifndef G4E_CE_MRICH_HH
#define G4E_CE_MRICH_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

struct ce_MRICH_Config {
// define here Global volume parameters
    double RIn= 12 * cm;
    double ROut= 82 * cm;
    double SizeZ= 15 * cm;
    double PosZ;
    double InnerR= 15. * cm;

    double mod_Thickness = 10. * cm;
    double mod_Width = 10. * cm;
    double mod_Gap = 1 * cm;


};


class ce_MRICH_Design {
public:
    inline void Construct(ce_MRICH_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin ce_MRICH volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors

        Solid = new G4Tubs("ce_MRICH_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "ce_MRICH_GVol_Logic");
        G4VisAttributes *visAttr= new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
        visAttr->SetLineWidth(1);
        visAttr->SetForceSolid(false);
        Logic->SetVisAttributes(visAttr);

        Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.PosZ), "ce_MRICH_GVol_Phys", Logic,
                                              motherVolume, false, 0);


    };

    inline void ConstructModules() {
        printf("Begin ce_MRICH detector volumes \n");

        // construct here your detectors

        static char abname[256];
        auto cfg = ConstructionConfig;
// ---need to replace material!!!
        ce_MRICH_bars_Material = G4Material::GetMaterial("quartz");
        ce_MRICH_mod_Solid = new G4Box("ce_MRICH_mod_Solid", cfg.mod_Width * 0.5, cfg.mod_Width * 0.5, cfg.mod_Thickness * 0.5);
        ce_MRICH_mod_Logic = new G4LogicalVolume(ce_MRICH_mod_Solid, ce_MRICH_bars_Material, "ce_MRICH_mod_Logic");

        attr_ce_MRICH_mod = new G4VisAttributes(G4Color(1, 0.6, 0.1, 0.8));
        attr_ce_MRICH_mod->SetLineWidth(1);
        attr_ce_MRICH_mod->SetForceSolid(true);
        ce_MRICH_mod_Logic->SetVisAttributes(attr_ce_MRICH_mod);

        G4double MRICH_R;
        G4double y_mrich = 0.;
        G4double x_mrich = 0;
        G4int kmrich = -1;

//============  For MRICH sectors =====
        for ( int j = 0; j < 10; j++) {
            y_mrich -= (cfg.mod_Width + cfg.mod_Gap);
            x_mrich = (cfg.mod_Width + cfg.mod_Gap) * 0.5;
            // printf("MRICH0:: x_mrich =%f,  y_mrich=%f\n", x_mrich, y_mrich);
            for (int i = 0; i < 10; i++) {
                double MRICH_R = sqrt(x_mrich * x_mrich + y_mrich * y_mrich);

                //       printf("MRICH1::kmrich=%d  j=%d i =%d x=%f, y=%f  MRICH_R=%f ce_MRICH_GVol_InnerR=%f \n ", kmrich, j, i, x_mrich,
                //       y_mrich, MRICH_R, ce_MRICH_GVol_InnerR);


                if (MRICH_R < (cfg.ROut - cfg.mod_Width/2 - cfg.mod_Gap/2) && MRICH_R > cfg.InnerR) {

                    kmrich++;
                    sprintf(abname, "ce_MRICH_mod_Phys_%d", kmrich);
                    new G4PVPlacement(0, G4ThreeVector(x_mrich, y_mrich, 0.), abname, ce_MRICH_mod_Logic,
                                      Phys, false, kmrich);
                    kmrich++;
                    sprintf(abname, "ce_MRICH_mod_Phys_%d", kmrich);
                    new G4PVPlacement(0, G4ThreeVector(-x_mrich, y_mrich, 0.), abname, ce_MRICH_mod_Logic,
                                      Phys, false, kmrich);

                    kmrich++;
                    sprintf(abname, "ce_MRICH_mod_Phys_%d", kmrich);
                    new G4PVPlacement(0, G4ThreeVector(x_mrich, -y_mrich, 0.), abname, ce_MRICH_mod_Logic,
                                      Phys, false, kmrich);

                    kmrich++;
                    sprintf(abname, "ce_MRICH_mod_Phys_%d", kmrich);
                    new G4PVPlacement(0, G4ThreeVector(-x_mrich, -y_mrich, 0.), abname, ce_MRICH_mod_Logic,
                                      Phys, false, kmrich);
                    printf("ce_MRICH::kmrich=%d  j=%d i =%d x=%f, y=%f  MRICH_R=%f ce_MRICH_GVol_InnerR=%f \n ", kmrich, j, i, x_mrich,
                           y_mrich, MRICH_R, cfg.InnerR);

                }
                x_mrich += (cfg.mod_Width + cfg.mod_Gap);

            }
        }
//----------------------------end MRICH --------------------------------------------



    };

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ce_MRICH_Config  ConstructionConfig;

private:

    G4Material *ce_MRICH_bars_Material;
    G4VisAttributes *attr_ce_MRICH_mod;
    G4Box *ce_MRICH_mod_Solid;
    G4LogicalVolume *ce_MRICH_mod_Logic;

    // define here local variables and parameter of detectors
};
#endif //G4E_CE_MRICH_HH
