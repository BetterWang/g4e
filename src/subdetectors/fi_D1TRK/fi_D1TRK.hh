//
// Created by yulia on 6/25/19.
//

#ifndef G4E_FI_D1TRK_HH
#define G4E_FI_D1TRK_HH



#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

struct fi_D1TRK_Config {
// define here Global volume parameters
    double RIn = 0 * cm;
    double ROut;
    double SizeZ = 30 * cm;
    double Zpos;
    //---------tracking
    int Nlayers =5;
    double PhiStart;
    double PhiTot;

};


class fi_D1TRK_Design {
public:
    inline void ConstructA(fi_D1TRK_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin fi_D1TRK volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors

        Solid = new G4Tubs("fi_D1TRK_GVol_Solid",cfg.RIn, cfg.ROut,
                                       cfg.SizeZ / 2., 0., 360 * deg);

        Logic = new G4LogicalVolume(Solid, worldMaterial, "fi_D1TRK_GVol_Logic");

        // ci_GEM_GVol_PosZ= SizeZ/2-abs(World_ShiftVTX)+ci_GEM_GVol_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed
        Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.Zpos), "fi_D1TRK_GVol_Phys", Logic,
                                             motherVolume, false, 0);

        //  G4VisAttributes* vgemff= new G4VisAttributes(G4Color(0.8,0.4,0.3,0.8));
        G4VisAttributes *attr_fi_D1_GVol = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
        attr_fi_D1_GVol->SetLineWidth(1);
        attr_fi_D1_GVol->SetForceSolid(false);
        Logic->SetVisAttributes(attr_fi_D1_GVol);


    };

    inline void ConstructDetectorsA() {
        printf("Begin fi_D1TRK detector volumes \n");
        static char abname[256];
        auto cfg = ConstructionConfig;

        // construct here your detectors

        // ---------------------------------------------------------------------------
        //                     D1 tracking  all
        // ---------------------------------------------------------------------------
        fi_D1_lay_RIn = 3.4 * cm;
        fi_D1_lay_ROut = cfg.ROut - 5 * cm;
        fi_D1_lay_SizeZ = 1 * cm;

        fi_D1_lay_Material = G4Material::GetMaterial("Ar10CO2");  //----   !!!!! ----
     //   fi_D1_lay_Material =G4Material::GetMaterial("G4_Galactic");
         int ffsi_counter = 0;
        for (int lay = 0; lay < cfg.Nlayers; lay++) {
            sprintf(abname, "fi_D1_TRK_lay_Solid_%d", lay);
        f1_D1_lay_Solid[lay] = new G4Tubs(abname, fi_D1_lay_RIn, fi_D1_lay_ROut,
                                      fi_D1_lay_SizeZ / 2., cfg.PhiStart, cfg.PhiTot);

 // for JLEIC        f1_D1_lay_Solid = new G4Tubs("f1_D1_lay_Solid", fi_D1_lay_RIn, fi_D1_lay_ROut,
 //                                    fi_D1_lay_SizeZ / 2., 170., 330 * deg);
          sprintf(abname, "fi_D1_TRK_lay_Logic_%d", lay);

         f1_D1_Lay_Logic[lay] = new G4LogicalVolume(f1_D1_lay_Solid[lay], fi_D1_lay_Material, abname);



           double Z = -cfg.SizeZ / 2 + (lay + 1) * fi_D1_lay_SizeZ / 2 + (lay + 1) * 5 * cm;
           sprintf(abname, "fi_D1_TRK_lay_Phys_%d", lay);
        f1_D1_lay_Phys[lay] = new G4PVPlacement(0, G4ThreeVector(0, 0, Z),
                                                abname, f1_D1_Lay_Logic[lay],
                                                Phys, false, ffsi_counter);
            ffsi_counter++;
            attr_fi_D1_lay = new G4VisAttributes(G4Color(0.8, 0.4 + 0.1 * lay, 0.3, 1.));
            attr_fi_D1_lay->SetLineWidth(1);
            attr_fi_D1_lay->SetForceSolid(true);
            f1_D1_Lay_Logic[lay]->SetVisAttributes(attr_fi_D1_lay);
        }

    };

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical
    G4Tubs *f1_D1_lay_Solid[20];    //pointer to the solid  FARFORWD
    G4LogicalVolume *f1_D1_Lay_Logic[20];    //pointer to the logical FARFORWD
    G4VPhysicalVolume *f1_D1_lay_Phys[20];    //pointer to the physical FARFORWD

    /// Parameters that was used in the moment of construction
    fi_D1TRK_Config  ConstructionConfig;

private:

    // define here local variables and parameter of detectors
   G4double fi_D1_lay_RIn;
    G4double fi_D1_lay_ROut;
    G4double fi_D1_lay_SizeZ;
    G4Material *fi_D1_lay_Material;
    G4VisAttributes* attr_fi_D1_lay;


};



#endif //G4E_FI_D1TRK_HH
