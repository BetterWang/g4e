//
// Created by yulia on 6/14/19.
//

#ifndef G4E_CI_EMCAL_HH
#define G4E_CI_EMCAL_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

struct ci_EMCAL_Config {
// define here Global volume parameters
   double RIn = 20 * cm;
   double ROut = 200 * cm;
   double ThicknessZ = 40 * cm;
   double PosZ=0.*cm;

   double det_Length = 40. * cm;
   double det_Width = 4. * cm;
   double det_Gap = 0.01 * mm;
   double  det_Rin1 = 20. * cm;
   double  det_Rin2 = 55. * cm;


};


class ci_EMCAL_Design {
public:
    inline void Construct(ci_EMCAL_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin ci_EMCAL volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors

        Solid = new G4Tubs("ci_EMCAL_GVol_Solid", cfg.RIn, cfg.ROut, cfg.ThicknessZ / 2., 0.,
                                         360 * deg);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "ci_EMCAL_GVol_Logic");
        G4VisAttributes *attr_ci_EMCAL_GVol = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
        attr_ci_EMCAL_GVol->SetLineWidth(1);
        attr_ci_EMCAL_GVol->SetForceSolid(false);
        Logic->SetVisAttributes(attr_ci_EMCAL_GVol);

        Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.PosZ), "ci_EMCAL_GVol_Phys", Logic,
                                              motherVolume, false, 0);


    };

    inline void ConstructDetectors() {
        printf("Begin ci_EMCAL detector volumes \n");

        // construct here your detectors

        //--------------------------------------------------------------------
        //                           Ecal modules
        //--------------------------------------------------------------------
       static char abname[256];
        auto cfg = ConstructionConfig;


        ci_EMCAL_det_Material = G4Material::GetMaterial("PbWO4");
        ci_EMCAL_det_Solid = new G4Box("ci_EMCAL_det_Solid", cfg.det_Width * 0.5, cfg.det_Width * 0.5, cfg.det_Length * 0.5);
        ci_EMCAL_det_Logic = new G4LogicalVolume(ci_EMCAL_det_Solid, ci_EMCAL_det_Material, "ci_EMCAL_det_Logic");

        attr_ci_EMCAL_det = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
        // G4VisAttributes* attr_ci_EMCAL_det= new G4VisAttributes(G4Color(0.1, 1.0, 0.9,0.5));
        attr_ci_EMCAL_det->SetLineWidth(1);
        attr_ci_EMCAL_det->SetForceSolid(true);
        ci_EMCAL_det_Logic->SetVisAttributes(attr_ci_EMCAL_det);

        // Crystals

        G4double y_Ch = 0;
        G4double x_Ch;
        G4int kh = -1;

//============  For sectors =====
        for (int j = 0; j < 50; j++) {
             if(j==0) { y_Ch = cfg.det_Gap ; }
             else { y_Ch -= cfg.det_Width + cfg.det_Gap;}
            x_Ch = (cfg.det_Width + cfg.det_Gap) * 0.5;

            for (int i = 0; i < 50; i++) {
                double R_H = sqrt(x_Ch * x_Ch + y_Ch * y_Ch);

                // printf("ci_EMCAL_det:: k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ", kh, j, i, x_Ch, y_Ch, R_H, ci_EMCAL_det_Rin1);

                //----------------------- Remove left side (small ring)----------------
                if (R_H < cfg.ROut - cfg.det_Width + cfg.det_Gap && R_H > cfg.det_Rin1) {
                    // printf("ci_EMCAL_det::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ", kh, j, i, x_Ch, y_Ch, R_H, ci_EMCAL_det_Rin1);


                    kh++;
                    sprintf(abname, "ci_EMCAL_det_Phys_%d", kh);

                    new G4PVPlacement(0, G4ThreeVector(x_Ch, y_Ch, 0.), abname, ci_EMCAL_det_Logic,
                                      Phys, false, kh);
                    kh++;
                    sprintf(abname, "ci_EMCAL_det_Phys_%d", kh);
                    new G4PVPlacement(0, G4ThreeVector(x_Ch, -y_Ch, 0.), abname, ci_EMCAL_det_Logic,
                                     Phys, false, kh);
                }

                //----------------------- Remove right side (large ring)----------------
                if (R_H < cfg.ROut - cfg.det_Width + cfg.det_Gap && R_H > cfg.det_Rin2) {
                    // printf("ci_EMCAL_det::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ", kh, j, i, x_Ch, y_Ch, R_H, ci_EMCAL_det_Rin2);

                    kh++;
                    sprintf(abname, "ci_EMCAL_det_Phys_%d", kh);
                    new G4PVPlacement(0, G4ThreeVector(-x_Ch, y_Ch, 0.), abname, ci_EMCAL_det_Logic,
                                      Phys, false, kh);
                    kh++;
                    sprintf(abname, "ci_EMCAL_detPhys_%d", kh);
                    new G4PVPlacement(0, G4ThreeVector(-x_Ch, -y_Ch, 0.), abname, ci_EMCAL_det_Logic,
                                      Phys, false, kh);
                }
                x_Ch += cfg.det_Width + cfg.det_Gap;

            }
        }



    };

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ci_EMCAL_Config  ConstructionConfig;

private:

    //  ------------ EMCAL modules -----

    G4Material *ci_EMCAL_det_Material ;
    G4VisAttributes *attr_ci_EMCAL_det;
    G4Box *ci_EMCAL_det_Solid ;
    G4LogicalVolume *ci_EMCAL_det_Logic ;

    // define here local variables and parameter of detectors
};
#endif //G4E_CI_EMCAL_HH
