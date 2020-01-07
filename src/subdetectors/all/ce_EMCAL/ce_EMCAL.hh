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

#include "JLeicDetectorConfig.hh"

struct ce_EMCAL_Config {
    double RIn=12 * cm;;
    double ROut;
    double ROutshift = 5*cm;
    double Thickness=40*cm;
    //double SizeZ;
    double PosZ;
};


class ce_EMCAL_Design {
public:
    inline void Construct(ce_EMCAL_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin cb_CTD volume \n");

        ConstructionConfig = cfg;
        static char abname[256];

        //........................EMCAL Barrel detector----------------------------------------------
        printf("Begin ce_EMCAL  volume \n");

        Solid = new G4Tubs("ce_EMCAL_GVol_Solid", cfg.RIn, cfg.ROut, cfg.Thickness / 2., 0., 360 * deg);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "ce_EMCAL_GVol_Logic");
        G4VisAttributes *attr_ce_EMCAL_GVol = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
        attr_ce_EMCAL_GVol->SetLineWidth(1);
        attr_ce_EMCAL_GVol->SetForceSolid(false);
        Logic->SetVisAttributes(attr_ce_EMCAL_GVol);

        //   my_z= 0*cm;
        Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.PosZ), "ce_EMCAL_GVol_Phys", Logic,
                                               motherVolume, false, 0);




    }
        inline void ConstructCrystals() {
            static char abname[256];
            auto cfg = ConstructionConfig;

            //------------------------------------------------------------------
            // Ecal module Crystals
            //-------------------------------------------------------------------

            ce_EMCAL_detPWO_Thickness = 30. * cm;
            ce_EMCAL_detPWO_ROut= 82*cm ;
            ce_EMCAL_detPWO_Width = 2. * cm;
            ce_EMCAL_detPWO_Gap = 0.01 * mm;

            ce_EMCAL_detPWO_Material = fMat->GetMaterial("PbWO4");
            ce_EMCAL_detPWO_Solid = new G4Box("ce_EMCAL_detPWO_Solid", ce_EMCAL_detPWO_Width * 0.5, ce_EMCAL_detPWO_Width * 0.5, ce_EMCAL_detPWO_Thickness * 0.5);
            ce_EMCAL_detPWO_Logic = new G4LogicalVolume(ce_EMCAL_detPWO_Solid, ce_EMCAL_detPWO_Material, "ce_EMCAL_detPWO_Logic");

            attr_ce_EMCAL_detPWO = new G4VisAttributes(G4Color(0.1, 1.0, 0.9, 1.));
            attr_ce_EMCAL_detPWO->SetLineWidth(1);
            attr_ce_EMCAL_detPWO->SetForceSolid(true);
            ce_EMCAL_detPWO_Logic->SetVisAttributes(attr_ce_EMCAL_detPWO);

            // Crystals

            G4double x0 = 0 * cm;
            G4double y0 = 0 * cm;
            ce_EMCAL_detPWO_InnerR = 15. * cm;
            G4double y_C = 0;
            G4double x_C;
            ce_EMCAL_detPWO_PosZ =cfg.Thickness/2-ce_EMCAL_detPWO_Thickness/2;
            G4int k = -1;

//============  For sectors =====
            for (int j = 0; j < 50; j++) {
                if(j==0) { y_C = ce_EMCAL_detPWO_Gap; }
                else { y_C -= ce_EMCAL_detPWO_Width + ce_EMCAL_detPWO_Gap ; };


                x_C = (ce_EMCAL_detPWO_Width + ce_EMCAL_detPWO_Gap) * 0.5;

                for (int i = 0; i < 50; i++) {
                    double R = sqrt(x_C * x_C + y_C * y_C);

                    //   printf("EMCALLL::k=%d  j=%d i =%d x=%f, y=%f  R=%f ce_EMCAL_detPWO_InnerR=%f \n ",k, j,i, x_C,y_C, R, ce_EMCAL_detPWO_InnerR);


                    if (R < ce_EMCAL_detPWO_ROut - ce_EMCAL_detPWO_Width + ce_EMCAL_detPWO_Gap && R > ce_EMCAL_detPWO_InnerR) {

                        k++;
                        sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                        new G4PVPlacement(0, G4ThreeVector(x_C, y_C, ce_EMCAL_detPWO_PosZ), abname, ce_EMCAL_detPWO_Logic,
                                          Phys, false, k);
                        k++;
                        sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                        new G4PVPlacement(0, G4ThreeVector(-x_C, y_C, ce_EMCAL_detPWO_PosZ ), abname, ce_EMCAL_detPWO_Logic,
                                          Phys, false, k);

                        k++;
                        sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                        new G4PVPlacement(0, G4ThreeVector(x_C, -y_C,  ce_EMCAL_detPWO_PosZ), abname, ce_EMCAL_detPWO_Logic,
                                          Phys, false, k);

                        k++;
                        sprintf(abname, "ce_EMCAL_detPWO_Phys_%d", k);
                        new G4PVPlacement(0, G4ThreeVector(-x_C, -y_C,  ce_EMCAL_detPWO_PosZ), abname, ce_EMCAL_detPWO_Logic,
                                          Phys, false, k);
                        //  printf("ce_EMCAL_detPWO::k=%d  j=%d i =%d x=%f, y=%f  R=%f ce_EMCAL_detPWO_InnerR=%f \n ",k, j,i, x_C,y_C, R, ce_EMCAL_detPWO_InnerR);

                    }
                    x_C += ce_EMCAL_detPWO_Width + ce_EMCAL_detPWO_Gap;

                }
            }

        }


    inline void ConstructGlass() {
        static char abname[256];
        auto cfg = ConstructionConfig;

        //------------------------------------------------------------------
        // Ecal module GLASS
        //-------------------------------------------------------------------

        ce_EMCAL_detGLASS_Thickness = 40. * cm;
        ce_EMCAL_detGLASS_OuterR= cfg.ROut;

        ce_EMCAL_detGLASS_Width = 4. * cm;
        ce_EMCAL_detGLASS_Gap = 0.01 * mm;


        ce_EMCAL_detGLASS_Material= fMat->GetMaterial("DSBCe");
        //   ce_EMCAL_det_Material = fMat->GetMaterial("PbWO4");
        ce_EMCAL_detGLASS_Solid = new G4Box("ce_EMCAL_detGLASS_Solid", ce_EMCAL_detGLASS_Width * 0.5, ce_EMCAL_detGLASS_Width * 0.5, ce_EMCAL_detGLASS_Thickness * 0.5);
        ce_EMCAL_detGLASS_Logic = new G4LogicalVolume(ce_EMCAL_detGLASS_Solid, ce_EMCAL_detGLASS_Material, "ce_EMCAL_detGLASS_Logic");

        attr_ce_EMCAL_detGLASS = new G4VisAttributes(G4Color(0.3, 0.4, 1., 0.5));
        attr_ce_EMCAL_detGLASS->SetLineWidth(1);
        attr_ce_EMCAL_detGLASS->SetForceSolid(true);
        ce_EMCAL_detGLASS_Logic->SetVisAttributes(attr_ce_EMCAL_detGLASS);

        // GLASS

       double  x0 = 0 * cm;
       double  y0 = 0 * cm;
        ce_EMCAL_detGLASS_InnerR= ce_EMCAL_detPWO_ROut;
       double  y_C = 0;
        double x_C=0.;
        ce_EMCAL_detGLASS_PosZ =cfg.Thickness/2.-ce_EMCAL_detGLASS_Thickness/2.;
        int k = -1;

//============  For sectors =====
        for (int j = 0; j < 50; j++) {
            if(j==0) { y_C=ce_EMCAL_detGLASS_Gap;}
            else { y_C -= ce_EMCAL_detGLASS_Width + ce_EMCAL_detGLASS_Gap;}
            x_C = (ce_EMCAL_detGLASS_Width + ce_EMCAL_detGLASS_Gap) * 0.5;

            for ( int i = 0; i < 50; i++) {
                double R = sqrt(x_C * x_C + y_C * y_C);

                //   printf("EMCALLL::k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, R0);


                if (R <  cfg.ROut  - ce_EMCAL_detGLASS_Width + ce_EMCAL_detGLASS_Gap && R > ce_EMCAL_detGLASS_InnerR) {
                    // printf("ce_EMCAL_detGLASS::GLASS k=%d  j=%d i =%d x=%f, y=%f  R=%f R0=%f \n ",k, j,i, x_C,y_C, R, ce_EMCAL_detPWO_InnerR);


                    k++;
                    sprintf(abname, "ce_EMCAL_detGLASS_Phys_%d", k);
                    new G4PVPlacement(0, G4ThreeVector(x_C, y_C, ce_EMCAL_detGLASS_PosZ), abname, ce_EMCAL_detGLASS_Logic,
                                      Phys, false, k);
                    k++;
                    sprintf(abname, "ce_EMCAL_detGLASS_Phys_%d", k);
                    new G4PVPlacement(0, G4ThreeVector(-x_C, y_C, ce_EMCAL_detGLASS_PosZ ), abname, ce_EMCAL_detGLASS_Logic,
                                      Phys, false, k);

                    k++;
                    sprintf(abname, "ce_EMCAL_detGLASS_Phys_%d", k);
                    new G4PVPlacement(0, G4ThreeVector(x_C, -y_C,  ce_EMCAL_detGLASS_PosZ), abname, ce_EMCAL_detGLASS_Logic,
                                     Phys, false, k);

                    k++;
                    sprintf(abname, "ce_EMCAL_detGLASS_Phys_%d", k);
                    new G4PVPlacement(0, G4ThreeVector(-x_C, -y_C,  ce_EMCAL_detGLASS_PosZ), abname, ce_EMCAL_detGLASS_Logic,
                                      Phys, false, k);
                }
                x_C += ce_EMCAL_detGLASS_Width + ce_EMCAL_detGLASS_Gap;

            }
        }



    }

        G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical


    /// Parameters that was used in the moment of construction
    ce_EMCAL_Config ConstructionConfig;

private:
    g4e::Materials *fMat;
    G4double ce_EMCAL_det_RIn;
    G4double ce_EMCAL_det_ROut;
    G4double ce_EMCAL_det_SizeZ;
    G4Material *ce_EMCAL_det_Material;
    G4VisAttributes *attr_ce_EMCAL;
    // G4Tubs*            cb_EMCAL_det_Solid;    //pointer to the solid World
    G4Polycone *ce_EMCAL_det_Solid;    //pointer to the solid World
    G4LogicalVolume *ce_EMCAL_det_Logic;    //pointer to the logical World
    G4VPhysicalVolume *ce_EMCAL_det_Phys;    //pointer to the physical World


    //............... EMCAL Crystals modules ......................
    double ce_EMCAL_detPWO_Thickness;
    double ce_EMCAL_detPWO_Width;
    double ce_EMCAL_detPWO_ROut;
    G4double ce_EMCAL_detPWO_Gap ;
    G4double ce_EMCAL_detPWO_InnerR;
    G4double ce_EMCAL_detPWO_PosZ;

    G4Material *ce_EMCAL_detPWO_Material;
    G4VisAttributes *attr_ce_EMCAL_detPWO;
    G4Box *ce_EMCAL_detPWO_Solid;
    G4LogicalVolume *ce_EMCAL_detPWO_Logic;


    //............... EMCAL Glass modules ......................
    G4double ce_EMCAL_detGLASS_InnerR;
    G4double ce_EMCAL_detGLASS_Thickness;
    G4double ce_EMCAL_detGLASS_OuterR;
    G4double ce_EMCAL_detGLASS_PosZ;
    G4double ce_EMCAL_detGLASS_Width;
    double ce_EMCAL_detGLASS_Gap ;

    G4Material *ce_EMCAL_detGLASS_Material;
    G4VisAttributes *attr_ce_EMCAL_detGLASS;
    G4Box *ce_EMCAL_detGLASS_Solid;
    G4LogicalVolume *ce_EMCAL_detGLASS_Logic;



};

#endif //G4E_CE_EMCAL_HH
