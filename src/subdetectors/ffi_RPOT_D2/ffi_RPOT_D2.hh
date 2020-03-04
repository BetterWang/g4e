//
// Created by yulia on 9/13/19.
//

#ifndef G4E_FFI_RPOT_D2_HH
#define G4E_FFI_RPOT_D2_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

struct ffi_RPOT_D2_Config {
// define here Global volume parameters
    double RIn = 0 * cm;
    double ROut = 10 * cm;
    double SizeZ = 30 * cm;
    double ShiftZ = 0 * cm;
    double PosZ = 0 * cm;
    double PosX=0*cm;
    double Angle=-0.05;
    G4RotationMatrix rot_matx;

    int Nlayers=4;

    double    lay_RIn;
    double    lay_ROut;
    double  lay_SizeZ;
    double lay_PosZ;
};


class ffi_RPOT_D2_Design {
public:
    inline void Construct(ffi_RPOT_D2_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin ffi_RPOT_D2 volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors
        //====================================================================================
        //==                        Roman Pot DETECTOR VOLUME Far-Forward  Hadron side          ==
        //====================================================================================
        ffi_RPOT_D2_det_Material = G4Material::GetMaterial("Ar10CO2");
        Solid = new G4Tubs("ffi_RPOT_D2_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);

        Logic = new G4LogicalVolume(Solid, worldMaterial, "ffi_RPOT_D2_GVol_Logic");

        // ffi_RPOT_D2_GVol_PosZ= SizeZ/2-abs(World_ShiftVTX)+ffi_RPOT_D2_GVol_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed
        Phys = new G4PVPlacement(G4Transform3D(cfg.rot_matx,G4ThreeVector(cfg.PosX,0,cfg.PosZ)), "ffi_RPOT_D2_GVol_Phys", Logic,
                                 motherVolume, false, 0);

        G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.1, 0, 1., 0.1));
        visAttr->SetLineWidth(1);
        visAttr->SetForceSolid(true);
        Logic->SetVisAttributes(visAttr);


    };

    inline void ConstructDetectors() {
        printf("Begin ffi_RPOT_D2 detector volumes \n");
        static char abname[256];
        auto cfg = ConstructionConfig;

        // construct here your detectors
        // ===================================================================================
        //                           Silicon layers  tracker                                           ==
        //===================================================================================

        printf("Begin ffi_RPOT_D2_lay_\n");
        //ffi_RPOT_D2_lay_Material = fMat->GetMaterial("Si");
      //  ffi_RPOT_D2_lay_Material = G4Material::GetMaterial("Ar10CO2");  //----   !!!!! ----
        lay_Material = G4Material::GetMaterial("Si");  //----   !!!!! ----
            cfg.lay_RIn = cfg.RIn + 0.5 * cm ;
            cfg.lay_ROut = cfg.ROut -1.* cm;
            cfg.lay_SizeZ = 0.01 * cm;

        for (int lay = 0; lay < cfg.Nlayers; lay++) {


            //      ffi_RPOT_D2_lay_PosZ[lay]=-ffi_RPOT_D2_GVol_PosZ/2+(double(lay)*5.)*cm;
            cfg.lay_PosZ = -cfg.SizeZ / 2 + 5 * cm + (double(lay) * 3.) * cm;


            sprintf(abname, "ffi_RPOT_D2_lay_Solid_%d", lay);
            lay_Solid[lay] = new G4Tubs(abname, cfg.lay_RIn, cfg.lay_ROut,
                                        cfg.lay_SizeZ / 2., 0., 360 * deg);

            sprintf(abname, "ffi_RPOT_D2_lay_Logic_%d", lay);
            lay_Logic[lay] = new G4LogicalVolume(lay_Solid[lay],
                                                 lay_Material, abname);

            attr_ffi_RPOT_D2_lay = new G4VisAttributes(G4Color(0.8, 0.4, 0.3, 0.8));
            attr_ffi_RPOT_D2_lay->SetLineWidth(1);
            attr_ffi_RPOT_D2_lay->SetForceSolid(true);
            lay_Logic[lay]->SetVisAttributes(attr_ffi_RPOT_D2_lay);

            sprintf(abname, "ffi_RPOT_D2_lay_Phys_%d", lay);
            lay_Phys[lay] = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.lay_PosZ),
                                              abname, lay_Logic[lay],
                                              Phys, false, 0);
        }

        printf("END ffi_RPOT_D2_lay_\n");

    };


    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ffi_RPOT_D2_Config  ConstructionConfig;
     G4Material *lay_Material;
    G4Tubs *lay_Solid[20];    //pointer to the solid World
    G4LogicalVolume *lay_Logic[20];    //pointer to the logical World
    G4VPhysicalVolume *lay_Phys[20];    //pointer to the physical World

private:

    // define here local variables and parameter of detectors

    //--------
     G4Material *ffi_RPOT_D2_det_Material;
    G4VisAttributes *attr_ffi_RPOT_D2_lay;
    //--------------------------------------------------------------


};


#endif //G4E_FFI_RPOT_D2_HH
