//
// Created by yulia on 6/12/19.
//

#ifndef G4E_CE_EMCAL_HH
#define G4E_CE_EMCAL_HH


#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include <spdlog/spdlog.h>
#include "JLeicDetectorConfig.hh"

struct ce_EMCAL_Config
{
    double RIn = 15 * cm;
    double ROut;
    double ROutshift = 5 * cm;
    double Thickness = 40 * cm;
    //double SizeZ;
    double PosZ;

    //............... EMCAL Crystals modules ......................
    double PWO_Thickness = 20. * cm;
    double PWO_Width = 2. * cm;
    double PWO_Gap = 0.01 * mm;
    double PWO_InnerR = 15. * cm;
    double PWO_OuterR = 82 * cm;
    double PWO_PosZ;

    //............... EMCAL Glass modules ......................
    double Glass_Thickness = 40. * cm;
    double Glass_Width = 4. * cm;
    double Glass_Gap = 0.01 * mm;
    double Glass_InnerR;
    double Glass_OuterR;
    double Glass_PosZ;
};


class ce_EMCAL_Design
{
public:
    inline void Construct(ce_EMCAL_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume)
    {
        printf("Begin cb_CTD volume \n");
        ConstructionConfig = cfg;

        //........................EMCAL Barrel detector----------------------------------------------
        printf("Begin ce_EMCAL  volume \n");

        Solid = new G4Tubs("ce_EMCAL_GVol_Solid", cfg.RIn, cfg.ROut, cfg.Thickness / 2., 0., 360 * deg);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "ce_EMCAL_GVol_Logic");
        G4VisAttributes *attr_ce_EMCAL_GVol = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
        attr_ce_EMCAL_GVol->SetLineWidth(1);
        attr_ce_EMCAL_GVol->SetForceSolid(false);
        Logic->SetVisAttributes(attr_ce_EMCAL_GVol);

        //   my_z= 0*cm;
        Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.PosZ), "ce_EMCAL_GVol_Phys", Logic, motherVolume, false, 0);


    }

    /// CE EMCAL module Crystals
    inline void ConstructCrystals()
    {
        static char abname[256];
        auto cfg = ConstructionConfig;

        cfg.PWO_PosZ = cfg.Thickness / 2 - cfg.PWO_Thickness / 2;

        ce_EMCAL_detPWO_Material = fMat->GetMaterial("PbWO4");
        ce_EMCAL_detPWO_Solid = new G4Box("ce_EMCAL_detPWO_Solid", cfg.PWO_Width * 0.5, cfg.PWO_Width * 0.5, cfg.PWO_Thickness * 0.5);
        ce_EMCAL_detPWO_Logic = new G4LogicalVolume(ce_EMCAL_detPWO_Solid, ce_EMCAL_detPWO_Material, "ce_EMCAL_detPWO_Logic");

        attr_ce_EMCAL_detPWO = new G4VisAttributes(G4Color(0.1, 1.0, 0.9, 1.));
        attr_ce_EMCAL_detPWO->SetLineWidth(1);
        attr_ce_EMCAL_detPWO->SetForceSolid(true);
        ce_EMCAL_detPWO_Logic->SetVisAttributes(attr_ce_EMCAL_detPWO);

        // Crystals
        double y_C = 0;
        double x_C;
        G4int k = -1;

        fmt::print("\nCE EMCAL PWO START\n");
        fmt::print("PWO_Thickness = {} cm;\n", cfg.PWO_Thickness / cm);
        fmt::print("PWO_Width     = {} cm;\n", cfg.PWO_Width / cm);
        fmt::print("PWO_Gap       = {} cm;\n", cfg.PWO_Gap / cm);
        fmt::print("PWO_InnerR    = {} cm;\n", cfg.PWO_InnerR / cm);
        fmt::print("PWO_OuterR    = {} cm;\n", cfg.PWO_OuterR / cm);
        fmt::print("PWO_PosZ      = {} cm;\n", cfg.PWO_PosZ / cm);

        //============  For sectors =====
        for (int j = 0; j < 50; j++) {
            y_C = j==0 ? cfg.PWO_Gap : y_C - cfg.PWO_Width + cfg.PWO_Gap;
            x_C = (cfg.PWO_Width + cfg.PWO_Gap) * 0.5;

            for (int i = 0; i < 50; i++) {
                double R = sqrt(x_C * x_C + y_C * y_C);

                //   printf("EMCALLL::k=%d  j=%d i =%d x=%f, y=%f  R=%f ce_EMCAL_detPWO_InnerR=%f \n ",k, j,i, x_C,y_C, R, ce_EMCAL_detPWO_InnerR);
                if (R < cfg.PWO_OuterR - cfg.PWO_Width + cfg.PWO_Gap && R > cfg.PWO_InnerR) {

                    double x,y;
                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.PWO_PosZ), abname, ce_EMCAL_detPWO_Logic, Phys, false, k);
                    fmt::print("({:<10}, {:<10}, {:<10}, {:<10.4f}, {:<10.4f}, '{}'),\n", k, i, j, x / cm, y / cm, abname);

                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=-x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.PWO_PosZ), abname, ce_EMCAL_detPWO_Logic, Phys, false, k);
                    fmt::print("({:<10}, {:<10}, {:<10}, {:<10.4f}, {:<10.4f}, '{}'),\n", k, i, j, x / cm, y / cm, abname);

                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.PWO_PosZ), abname, ce_EMCAL_detPWO_Logic, Phys, false, k);
                    fmt::print("({:<10}, {:<10}, {:<10}, {:<10.4f}, {:<10.4f}, '{}'),\n", k, i, j, x / cm, y / cm, abname);

                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=-x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.PWO_PosZ), abname, ce_EMCAL_detPWO_Logic, Phys, false, k);
                    fmt::print("({:<10}, {:<10}, {:<10}, {:<10.4f}, {:<10.4f}, '{}'),\n", k, i, j, x / cm, y / cm, abname);
                    //  printf("ce_EMCAL_detPWO::k=%d  j=%d i =%d x=%f, y=%f  R=%f ce_EMCAL_detPWO_InnerR=%f \n ",k, j,i, x_C,y_C, R, ce_EMCAL_detPWO_InnerR);

                }
                x_C += cfg.PWO_Width + cfg.PWO_Gap;
            }
        }
        fmt::print("CE EMCAL PWO END\n\n");
    }


    inline void ConstructGlass()
    {
        static char abname[256];
        auto cfg = ConstructionConfig;

        //------------------------------------------------------------------
        // Ecal module GLASS
        //-------------------------------------------------------------------
        cfg.Glass_OuterR = cfg.ROut - 1 * cm ;
        cfg.Glass_InnerR = cfg.PWO_OuterR + 3 * cm;
        cfg.Glass_PosZ = cfg.Thickness / 2. - cfg.Glass_Thickness / 2.;

        ce_EMCAL_detGLASS_Material = fMat->GetMaterial("DSBCe");
        //   ce_EMCAL_det_Material = fMat->GetMaterial("PbWO4");
        ce_EMCAL_detGLASS_Solid = new G4Box("ce_EMCAL_detGLASS_Solid", cfg.Glass_Width * 0.5, cfg.Glass_Width * 0.5, cfg.Glass_Thickness * 0.5);
        ce_EMCAL_detGLASS_Logic = new G4LogicalVolume(ce_EMCAL_detGLASS_Solid, ce_EMCAL_detGLASS_Material, "ce_EMCAL_detGLASS_Logic");

        attr_ce_EMCAL_detGLASS = new G4VisAttributes(G4Color(0.3, 0.4, 1., 0.5));
        attr_ce_EMCAL_detGLASS->SetLineWidth(1);
        attr_ce_EMCAL_detGLASS->SetForceSolid(true);
        ce_EMCAL_detGLASS_Logic->SetVisAttributes(attr_ce_EMCAL_detGLASS);

        // GLASS
        double y_C = 0;
        double x_C = 0.;
        int k = -1;

        fmt::print("\nCE EMCAL GLASS START\n");
        fmt::print("Glass_Thickness = {} cm;\n", cfg.Glass_Thickness / cm);
        fmt::print("Glass_Width     = {} cm;\n", cfg.Glass_Width / cm);
        fmt::print("Glass_Gap       = {} cm;\n", cfg.Glass_Gap / cm);
        fmt::print("Glass_InnerR    = {} cm;\n", cfg.Glass_InnerR / cm);
        fmt::print("Glass_OuterR    = {} cm;\n", cfg.Glass_OuterR / cm);
        fmt::print("Glass_PosZ      = {} cm;\n", cfg.Glass_PosZ / cm);

        //============  For sectors =====
        for (int j = 0; j < 50; j++) {
            if (j == 0) { y_C = cfg.Glass_Gap; }
            else { y_C -= cfg.Glass_Width + cfg.Glass_Gap; }
            x_C = (cfg.Glass_Width + cfg.Glass_Gap) * 0.5;

            for (int i = 0; i < 50; i++) {
                double R = sqrt(x_C * x_C + y_C * y_C);

                //   printf("EMCALLL::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, R0);

                if (R < cfg.Glass_OuterR - cfg.Glass_Width + cfg.Glass_Gap && R > cfg.Glass_InnerR) {
                    double x,y;
                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.Glass_PosZ), abname, ce_EMCAL_detGLASS_Logic, Phys, false, k);
                    fmt::print("{:<10} {:<10} {:<10} {:<10.4f} {:<10.4f} {}\n", k, i, j, x / cm, y / cm, abname);

                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=-x_C; y=y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.Glass_PosZ), abname, ce_EMCAL_detGLASS_Logic, Phys, false, k);
                    fmt::print("{:<10} {:<10} {:<10} {:<10.4f} {:<10.4f} {}\n", k, i, j, x / cm, y / cm, abname);

                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.Glass_PosZ), abname, ce_EMCAL_detGLASS_Logic, Phys, false, k);
                    fmt::print("{:<10} {:<10} {:<10} {:<10.4f} {:<10.4f} {}\n", k, i, j, x / cm, y / cm, abname);

                    k++;
                    sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                    x=-x_C; y=-y_C;
                    new G4PVPlacement(nullptr, G4ThreeVector(x, y, cfg.Glass_PosZ), abname, ce_EMCAL_detGLASS_Logic, Phys, false, k);
                    fmt::print("{:<10} {:<10} {:<10} {:<10.4f} {:<10.4f} {}\n", k, i, j, x / cm, y / cm, abname);
                }
                x_C += cfg.Glass_Width + cfg.Glass_Gap;

            }
        }

        fmt::print("CE EMCAL GLASS END\n\n");
    }

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical


    /// Parameters that was used in the moment of construction
    ce_EMCAL_Config ConstructionConfig;

    //--- crystals
    G4Material *ce_EMCAL_detPWO_Material;
    G4VisAttributes *attr_ce_EMCAL_detPWO;
    G4Box *ce_EMCAL_detPWO_Solid;
    G4LogicalVolume *ce_EMCAL_detPWO_Logic;
    //---glass
    G4Material *ce_EMCAL_detGLASS_Material;
    G4VisAttributes *attr_ce_EMCAL_detGLASS;
    G4Box *ce_EMCAL_detGLASS_Solid;
    G4LogicalVolume *ce_EMCAL_detGLASS_Logic;


private:
    g4e::Materials *fMat;
    double ce_EMCAL_det_RIn;
    double ce_EMCAL_det_ROut;
    double ce_EMCAL_det_SizeZ;
    G4Material *ce_EMCAL_det_Material;
    G4VisAttributes *attr_ce_EMCAL;
    // G4Tubs*            cb_EMCAL_det_Solid;    //pointer to the solid World
    G4Polycone *ce_EMCAL_det_Solid;    //pointer to the solid World
    G4LogicalVolume *ce_EMCAL_det_Logic;    //pointer to the logical World
    G4VPhysicalVolume *ce_EMCAL_det_Phys;    //pointer to the physical World
};

#endif //G4E_CE_EMCAL_HH
