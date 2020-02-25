//
// Created by yulia on 2/17/20.
//

#ifndef G4E_FFE_LOWQ2_HH
#define G4E_FFE_LOWQ2_HH



#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

struct ffe_LOWQ2_Config
{
// define here Global volume parameters
    double SizeX = 1. * m;
    double SizeY = 1 * m;
  // old   double SizeZ = 30 * m;
    double SizeZ = 2 * m;
    double PosX = 2 * m;
    double PosY = 2 * m;
    double PosZ = 2 * m;
    //-----------------------
    double lay_ROut=30* cm;
    double lay_RIn=0* cm;
    int Nlayers =1;
    double lay_SizeZ = 0.5*cm;
    double lay_Zshift=10.*cm;
    double lay_Xshift=-20.*cm;
    //------------------------
    double BPC_Thickness = 40*cm;
    double BPC_ROut = 70 * cm;
    double BPC_Width = 5. * cm;
    double BPC_Gap = 0.01 * mm;

};

class ffe_LOWQ2_Design
{
public:

    inline void Construct(ffe_LOWQ2_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume)
    {
        printf("Begin ffe_LOWQ2 volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors

        //Global volume ---- ( check that your world is smaller then this volume
        Solid = new G4Box("ffe_LOWQ2_GVol_Solid", cfg.SizeX / 2., cfg.SizeY / 2., cfg.SizeZ / 2.);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "ffe_LOWQ2_GVol_Logic");
        //top_l->SetVisAttributes( G4VisAttributes::GetInvisible() );
        Phys = new G4PVPlacement(nullptr, G4ThreeVector(cfg.PosX, cfg.PosY, cfg.PosZ), "ffe_LOWQ2_GVol_Phys", Logic, motherVolume, false, 0);
        // Visual attributes
        auto *VisAttr = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
        VisAttr->SetLineWidth(1);
        VisAttr->SetForceSolid(true);
        Logic->SetVisAttributes(VisAttr);

    };

    inline void ConstructDetectors(){

          Construct_TRK_Detectors();
        Construct_BPC_Detectors();

    };

   inline void Construct_BPC_Detectors()
   {
       printf("Begin ffe_LOWQ2 CAL detector volumes \n");
       auto cfg = ConstructionConfig;
        static char abname[256];
       // construct here your detectors
          //------------------------------------------------------------------
          // Ecal module Crystals
          //-------------------------------------------------------------------


          BPC_Material = G4Material::GetMaterial("PbWO4");
          BPC_Solid = new G4Box("ffe_BPC_Solid", cfg.BPC_Width * 0.5,
                                            cfg.BPC_Width * 0.5, cfg.BPC_Thickness * 0.5);
          BPC_Logic = new G4LogicalVolume(BPC_Solid, BPC_Material,
                                                      "ffe_BPC_Logic");

          G4VisAttributes *attr_BPC = new G4VisAttributes(G4Color(0.1, 1.0, 0.9, 1.));
          attr_BPC->SetLineWidth(1);
          attr_BPC->SetForceSolid(true);
          BPC_Logic->SetVisAttributes(attr_BPC);

          // Crystals

          G4double x0 = 0 * cm;
          G4double y0 = 0 * cm;
          double BPC_InnerR = 0. * cm;
          G4double y_C = 0;
          G4double x_C;
          double BPC_PosZ = -cfg.SizeZ / 2 + cfg.BPC_Thickness / 2;
          G4int k = -1;

//============  For sectors =====
          for (int j = 0; j < 50; j++) {
              if(j==0) {y_C=cfg.BPC_Gap;}
                  else { y_C -= cfg.BPC_Width + cfg.BPC_Gap;}
              x_C = (cfg.BPC_Width + cfg.BPC_Gap) * 0.5;

              for (int i = 0; i < 50; i++) {
                  double R = sqrt(x_C * x_C + y_C * y_C);

                  //   printf("EMCALLL::k=%d  j=%d i =%d x=%f, y=%f  R=%f BPC_InnerR=%f \n ",k, j,i, x_C,y_C, R, BPC_InnerR);


                  if (abs(x_C) < (cfg.SizeX-20) /2. &&  abs(y_C)< cfg.SizeY/2.)
                  {

                      k++;
                      sprintf(abname, "ffe_BPC_Phys_%d", k);
                      new G4PVPlacement(0, G4ThreeVector(x_C, y_C, BPC_PosZ), abname, BPC_Logic,
                                        Phys, false, k);
                      k++;
                      sprintf(abname, "ffe_BPC_Phys_%d", k);
                      new G4PVPlacement(0, G4ThreeVector(-x_C, y_C, BPC_PosZ), abname,
                                        BPC_Logic,
                                        Phys, false, k);

                      k++;
                      sprintf(abname, "ffe_BPC_Phys_%d", k);
                      new G4PVPlacement(0, G4ThreeVector(x_C, -y_C, BPC_PosZ), abname,
                                        BPC_Logic,
                                        Phys, false, k);

                      k++;
                      sprintf(abname, "ffe_BPC_Phys_%d", k);
                      new G4PVPlacement(0, G4ThreeVector(-x_C, -y_C, BPC_PosZ), abname,
                                        BPC_Logic,
                                        Phys, false, k);
                      //  printf("BPC::k=%d  j=%d i =%d x=%f, y=%f  R=%f BPC_InnerR=%f \n ",k, j,i, x_C,y_C, R, BPC_InnerR);

                  }
                  x_C += cfg.BPC_Width + cfg.BPC_Gap;

              }
          }


   };


    inline void Construct_TRK_Detectors()
    {
        printf("Begin ffe_LOWQ2 detector volumes \n");
        auto cfg = ConstructionConfig;
        // construct here your detectors
        static char abname[256];
        // ---------------------------------------------------------------------------
        //                     Low-Q2 tracking
        // ---------------------------------------------------------------------------
       // cfg.lay_RIn = 10 * cm;
      //  cfg.lay_ROut = cfg.SizeX - 5 * cm;
     //   cfg.lay_SizeZ = 1 * cm;

         ffe_LOWQ2_lay_Material = G4Material::GetMaterial("Ar10CO2");  //----   !!!!! ----
        //ffe_LOWQ2_lay_Material =G4Material::GetMaterial("G4_Galactic");
        lay_Solid = new G4Tubs("ffe_LOWQ2_lay_Solid", cfg.lay_RIn, cfg.lay_ROut,
                               cfg.lay_SizeZ / 2., 0., 360 * deg);
        sprintf(abname, "ffe_LOWQ2_lay_Logic");
        lay_Logic = new G4LogicalVolume(lay_Solid, ffe_LOWQ2_lay_Material, abname);



        int ffsi_counter = 0;
        for (int lay = 0; lay < cfg.Nlayers; lay++) {
            double Z = -cfg.SizeZ / 2 + (lay + 1) * cfg.lay_SizeZ / 2 + (lay + 1) * cfg.lay_Zshift * cm;
            sprintf(abname, "ffe_LOWQ2_lay_Phys_%d", lay);
            lay_Phys [lay]= new G4PVPlacement(0, G4ThreeVector(cfg.lay_Xshift, 0, Z),
                                         abname, lay_Logic,
                                         Phys, false, ffsi_counter);
            ffsi_counter++;
            G4VisAttributes *attr_lay = new G4VisAttributes(G4Color(0.8, 0.4 + 0.1 * lay, 0.3, 1.));
            attr_lay->SetLineWidth(1);
            attr_lay->SetForceSolid(true);
            lay_Logic->SetVisAttributes(attr_lay);
        }

    };

    G4Box *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ffe_LOWQ2_Config  ConstructionConfig;
    G4Tubs *lay_Solid;    //pointer to the solid  FARFORWD
    G4LogicalVolume *lay_Logic;    //pointer to the logical FARFORWD
    G4VPhysicalVolume *lay_Phys[20];    //pointer to the physical FARFORWD

    G4Material *ffe_LOWQ2_lay_Material;
    //-------------------------------------------------
    G4Material *BPC_Material;
    G4Box *BPC_Solid;
    G4LogicalVolume *BPC_Logic;    //pointer to the logical
    G4VPhysicalVolume *BPC_Phys;  //pointer to the physical


private:

    // define here local variables and parameter of detectors
};




#endif //G4E_FFE_LOWQ2_HH
