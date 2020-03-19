//
// Created by yulia on 6/7/19.
//

#ifndef G4E_CB_DIRC_HH
#define G4E_CB_DIRC_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "JLeicDetectorConfig.hh"

struct cb_DIRC_Config {

    G4double RIn;   // Set outside
    G4double ROut;  // Set in general construction
    G4double SizeZ; // Set in general construction
};

class cb_DIRC_Design {
public:
    inline void Construct(cb_DIRC_Config cfg, G4Material* worldMaterial, G4VPhysicalVolume *motherVolume) {


        ConstructionConfig=cfg;


       Solid = new G4Tubs("cb_DIRC_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
       Logic = new G4LogicalVolume(Solid, worldMaterial, "cb_DIRC_GVol_Logic");
       Phys = new G4PVPlacement(nullptr, G4ThreeVector(), "DIRC", Logic, motherVolume, false, 0);

        // cb_DIRC_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);
        G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.1, 0, 1., 0.1));
        visAttr->SetLineWidth(1);
        visAttr->SetForceSolid(false);
        Logic->SetVisAttributes(visAttr);

    }

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical
    G4UniformMagField *MagneticField;      //pointer to the magnetic field

    /// Parameters that was used in the moment of construction
    cb_DIRC_Config ConstructionConfig;

    inline void ConstructBars() {

        static char abname[256];
        auto cfg = ConstructionConfig;

        G4RotationMatrix rm_dirc[40];
        printf("CB_DIRC:: \n");
        cb_DIRC_bars_DZ = cfg.SizeZ;
        cb_DIRC_bars_DY = 42. * cm;
        cb_DIRC_bars_DX = 1.7 * cm;
        //   dR =  cb_DIRC_GVol_RIn+3.*cm;
        double dR = 83.65 * cm;

        double myL = 2 * 3.1415 * dR;
        int NUM = myL / cb_DIRC_bars_DY;

        /*     for(int i=0;i<2; i++){
      double LN = cb_DIRC_bars_DY * NUM;
      double LN1 = cb_DIRC_bars_DY * (NUM+1+i);
      printf("cb_DIRC_bars:: LN= Orig NUM=%d\n",NUM);
      if (LN/LN1>0.8) NUM=NUM+1;
      printf("cb_DIRC_bars:: LN=%f, LN1=%f  delenie=%f NUM=%d \n",LN,LN1,LN/LN1,NUM);
        }
        */
        cb_DIRC_bars_deltaphi = 2 * 3.1415926 / NUM;

        cb_DIRC_bars_Material = G4Material::GetMaterial("quartz");
        sprintf(abname, "cb_DIRC_bars_Solid");
        cb_DIRC_bars_Solid = new G4Box(abname, cb_DIRC_bars_DX / 2., cb_DIRC_bars_DY / 2., cb_DIRC_bars_DZ / 2.);

        sprintf(abname, "cb_DIRC_bars_Logic");
        cb_DIRC_bars_Logic = new G4LogicalVolume(cb_DIRC_bars_Solid, cb_DIRC_bars_Material, abname);


        attr_cb_DIRC_bars = new G4VisAttributes(G4Color(0., 1., 0., 1.0));
        attr_cb_DIRC_bars->SetForceSolid(true);
        cb_DIRC_bars_Logic->SetVisAttributes(attr_cb_DIRC_bars);

        for (int ia = 0; ia < NUM; ia++) {
            // for (int ia = 0; ia < 1; ia++) {
            //for (int ia=0;ia<1;ia++) {
            printf("cb_DIRC_bars:: la =%d NUM=%d, dR=%f cb_DIRC_bars_deltaphi=%f  \n", ia, NUM, dR,
                   cb_DIRC_bars_deltaphi);

            double phi = (ia * (cb_DIRC_bars_deltaphi));
            double x = -dR * cos(phi);
            double y = -dR * sin(phi);
            rm_dirc[ia].rotateZ(cb_DIRC_bars_deltaphi * ia);
            //	rm[lay][ia].rotateZ(cb_DIRC_bars_deltashi);

            printf("cb_DIRC_bars::   %d x=%f  y=%f  \n", ia, x, y);
            sprintf(abname, "cb_DIRC_bars_Phys_%d", ia);
            cb_DIRC_bars_Phys = new G4PVPlacement(G4Transform3D(rm_dirc[ia], G4ThreeVector(x, y, 0)),
                                                  abname, cb_DIRC_bars_Logic,
                    //	      Phys, false, 0.);
                                                  Phys, false, 0.);
        }


    }



    G4Material *cb_DIRC_bars_Material;
    G4VisAttributes* attr_cb_DIRC_bars;
    // G4Tubs*            cb_DIRC_bars_Solid;    //pointer to the solid World
    G4Box *cb_DIRC_bars_Solid;    //pointer to the solid World
    G4LogicalVolume *cb_DIRC_bars_Logic;    //pointer to the logical World
    G4VPhysicalVolume *cb_DIRC_bars_Phys;    //pointer to the physical World

private:

    //--------------DIRC bars detector----------------------

    G4double cb_DIRC_bars_RIn;
    G4double cb_DIRC_bars_ROut;
    G4double cb_DIRC_bars_DZ;
    G4double cb_DIRC_bars_DY;
    G4double cb_DIRC_bars_DX;
    G4double cb_DIRC_bars_deltaphi;

     //*************************************************************

};

#endif //G4E_CB_DIRC_HH
