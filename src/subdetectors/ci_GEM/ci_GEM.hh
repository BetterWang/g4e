//
// Created by yulia on 6/19/19.
//

#ifndef G4E_CI_GEM_HH
#define G4E_CI_GEM_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "main_detectors/DetectorConfig.hh"

struct ci_GEM_Config {
// define here Global volume parameters
    double RIn = 10 * cm;
    double ROut = 45 * cm + 50 * cm;
    double SizeZ = 30 * cm;
    double ShiftZ = 0 * cm;
    double PosZ = 0 * cm;
    double PosX =0*cm;
    int Nlayers=8;
};


class ci_GEM_Design {
public:
    inline void Construct(ci_GEM_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin ci_GEM volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors
        //====================================================================================
        //==                          GEM DETECTOR VOLUME  CAP-barrel- Hadron side          ==
        //====================================================================================

        Solid = new G4Tubs("ci_GEM_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);

        Logic = new G4LogicalVolume(Solid, worldMaterial, "ci_GEM_GVol_Logic");

        // ci_GEM_GVol_PosZ= SizeZ/2-abs(World_ShiftVTX)+ci_GEM_GVol_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed
         Phys = new G4PVPlacement(0, G4ThreeVector(cfg.PosX, 0, cfg.PosZ), "ci_GEM_GVol_Phys", Logic,
                                            motherVolume, false, 0);



    };

    inline void ConstructDetectors() {
        printf("Begin ci_GEM detector volumes \n");
        static char abname[256];
        auto cfg = ConstructionConfig;

        // construct here your detectors
        // ===================================================================================
//                           GEM tracker                                           ==
//===================================================================================
        printf("Begin ci_GEM_lay_\n");
        //ci_GEM_lay_Material = fMat->GetMaterial("Si");
        ci_GEM_lay_Material = G4Material::GetMaterial("Ar10CO2");  //----   !!!!! ----

        for (int lay = 0; lay < cfg.Nlayers; lay++) {

            ci_GEM_lay_RIn[lay] = cfg.RIn + 1 * cm + (double(lay) * 0.5) * cm;
            ci_GEM_lay_ROut[lay] = cfg.ROut - 25 * cm + (double(lay) * 2.) * cm;;

            //      ci_GEM_lay_PosZ[lay]=-ci_GEM_GVol_PosZ/2+(double(lay)*5.)*cm;
            ci_GEM_lay_PosZ[lay] = -cfg.SizeZ / 2 + 5 * cm + (double(lay) * 3.) * cm;
            ci_GEM_lay_SizeZ[lay] = 1 * cm;

            sprintf(abname, "ci_GEM_lay_Solid_%d", lay);
            lay_Solid[lay] = new G4Tubs(abname, ci_GEM_lay_RIn[lay], ci_GEM_lay_ROut[lay],
                                               ci_GEM_lay_SizeZ[lay] / 2., 0., 360 * deg);

            sprintf(abname, "ci_GEM_lay_Logic_%d", lay);
            lay_Logic[lay] = new G4LogicalVolume(lay_Solid[lay],
                                                        ci_GEM_lay_Material, abname);

            attr_ci_GEM_lay = new G4VisAttributes(G4Color(0.8, 0.4, 0.3, 0.8));
            attr_ci_GEM_lay->SetLineWidth(1);
            attr_ci_GEM_lay->SetForceSolid(true);
            lay_Logic[lay]->SetVisAttributes(attr_ci_GEM_lay);

            sprintf(abname, "ci_GEM_lay_Phys_%d", lay);
            lay_Phys[lay] = new G4PVPlacement(0, G4ThreeVector(0, 0, ci_GEM_lay_PosZ[lay]),
                                                     abname, lay_Logic[lay],
                                                     Phys, false, 0);



        }

        printf("END ci_GEM_lay_\n");

    };

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ci_GEM_Config  ConstructionConfig;
    G4Tubs *lay_Solid[20];    //pointer to the solid World
    G4LogicalVolume *lay_Logic[20];    //pointer to the logical World
    G4VPhysicalVolume *lay_Phys[20];    //pointer to the physical World

private:

    // define here local variables and parameter of detectors

    //-------------------ENDCAP-H GEM detector ------------------
    G4double ci_GEM_lay_RIn[20];
    G4double ci_GEM_lay_ROut[20];
    G4double ci_GEM_lay_SizeZ[20];
    G4double ci_GEM_lay_PosZ[20];
    G4Material *ci_GEM_lay_Material;
    G4VisAttributes *attr_ci_GEM_lay;
     //--------------------------------------------------------------


};


#endif //G4E_CI_GEM_HH
