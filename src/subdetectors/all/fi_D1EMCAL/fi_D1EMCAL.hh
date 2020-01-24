//
// Created by yulia on 6/21/19.
//

#ifndef G4E_FI_D1EMCAL_HH
#define G4E_FI_D1EMCAL_HH


#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

struct fi_D1EMCAL_Config {
// define here Global volume parameters

    double SizeZ = 40. * cm;
    double Width = 4. * cm;
    double Gap = 0.01 * mm;
    double Angle=0.05;
    double Shift= -35.*cm;
    G4RotationMatrix rot_matx;
    double Zpos=0.*cm;
    double ROut=10.*cm;

};


class fi_D1EMCAL_Design {
public:
    inline void Construct(fi_D1EMCAL_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin fi_D1EMCAL volume \n");
        static char abname[256];
        ConstructionConfig = cfg;
        // create  a global volume for your detectors
        // ce_EMCAL_det_Material= fMat->GetMaterial("DSBCe");
        G4Material *fi_D1EMCAL_Material = G4Material::GetMaterial("PbWO4");
        Solid= new G4Box("fi_D1_EMCAL_Solid", cfg.Width * 0.5, cfg.Width * 0.5, cfg.SizeZ / 2.);
        Logic = new G4LogicalVolume(Solid,fi_D1EMCAL_Material, "fi_D1_EMCAL_Logic");

        G4VisAttributes *attr_fi_D1EMCAL = new G4VisAttributes(G4Color(0.1, 1.0, 0.9, 1.));
        attr_fi_D1EMCAL->SetLineWidth(1);
        attr_fi_D1EMCAL->SetForceSolid(true);
        Logic->SetVisAttributes(attr_fi_D1EMCAL);


        // Crystals

        double x0 =0*cm;
        double y0 = 0 * cm;
        double R0 = 7. * cm;
         double y_C = 0;
        double x_C= 0*cm;
        int k = -1;

        //    z = fi_D1B_GVol_SizeZ / 2 - fi_D1EMCAL_SizeZ / 2;
 //============  For sectors =====
        for (int j = 0; j < 50; j++) {
            y_C -= cfg.Width + cfg.Gap;
            x_C = (cfg.Width + cfg.Gap) * 0.5 ;

            for (int i = 0; i < 50; i++) {
                float R = sqrt(x_C * x_C + y_C * y_C);

                // printf("found D1b= EMCAL D1::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, R0);


                if (R < cfg.ROut - cfg.Width + cfg.Gap && R > R0) {
                    //  printf("found D1b= EMCAL D1::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, R0);


                    k++;
                    sprintf(abname, "fi_D1EMCAL_Phys_%d", k);

                    //               new G4PVPlacement(0, G4ThreeVector(x_C, y_C, z), abname, fi_D1EMCAL_Logic,
                    //                fi_D1B_GVol_Phys, false, k);
                    new G4PVPlacement(G4Transform3D(cfg.rot_matx, G4ThreeVector(x_C+cfg.Shift, y_C, cfg.Zpos)), abname, Logic,
                                     motherVolume, false, k);

                    k++;
                    sprintf(abname, "fi_D1EMCAL_Phys_%d", k);
                    new G4PVPlacement(G4Transform3D(cfg.rot_matx, G4ThreeVector(-x_C+cfg.Shift, y_C, cfg.Zpos)), abname, Logic,
                                      motherVolume, false, k);

                    k++;
                    sprintf(abname, "fi_D1EMCAL_Phys_%d", k);
                    new G4PVPlacement(G4Transform3D(cfg.rot_matx, G4ThreeVector(x_C+cfg.Shift, -y_C,cfg.Zpos )), abname, Logic,
                                     motherVolume, false, k);

                    k++;
                    sprintf(abname, "fi_D1EMCAL_Phys_%d", k);
                    new G4PVPlacement(G4Transform3D(cfg.rot_matx, G4ThreeVector(-x_C+cfg.Shift, -y_C, cfg.Zpos)), abname, Logic,
                                      motherVolume, false, k);
                }
                x_C += cfg.Width + cfg.Gap;

            }
        }


    };

    inline void ConstructDetectors() {
        printf("Begin fi_D1EMCAL detector volumes \n");

        // construct here your detectors

    };

    G4Box *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    fi_D1EMCAL_Config  ConstructionConfig;

private:

    // define here local variables and parameter of detectors
};




#endif //G4E_FI_D1EMCAL_HH
