//
// Created by yulia on 2/19/20.
//

#ifndef G4E_CB_SIDISCS_HH
#define G4E_CB_SIDISCS_HH



#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "main_detectors/DetectorConfig.hh"

struct cb_SiDISCS_Config {
// define here Global volume parameters
    double RIn = 0 * cm;
    double ROut =20. *cm;
    double SizeZ = 30. * cm;
    double Zpos =50. *cm;
    int Nlayers =5;

};


class cb_SiDISCS_Design {
public:
    inline void Construct(cb_SiDISCS_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin cb_SiDISCS volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors
       // double Zpos =150. *cm;
         printf("cb_SiDISCS :: Zpos=%f Rin %f %f  \n ",cfg.Zpos,cfg.RIn, cfg.ROut);
        Solid = new G4Tubs("cb_SiDISCS_GVol_Solid",cfg.RIn, cfg.ROut,
                                       cfg.SizeZ / 2., 0., 360 * deg);

        Logic = new G4LogicalVolume(Solid, worldMaterial, "cb_SiDISCS_GVol_Logic");

        // ci_GEM_GVol_PosZ= SizeZ/2-abs(World_ShiftVTX)+ci_GEM_GVol_SizeZ-5*cm;   // --- need to find out why this 5 cm are needed
        Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.Zpos), "cb_SiDISCS_GVol_Phys", Logic,
                                             motherVolume, false, 0);

      //    G4VisAttributes* attr_GVol = new G4VisAttributes(G4Color(1,0.,0.,1));
        G4VisAttributes *attr_GVol = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
        attr_GVol->SetLineWidth(1);
        attr_GVol->SetForceSolid(false);
        Logic->SetVisAttributes(attr_GVol);

    };

    inline void ConstructDetectors() {
        printf("Begin cb_SiDISCS detector volumes \n");
        static char abname[256];
        auto cfg = ConstructionConfig;

        // construct here your detectors

        // ---------------------------------------------------------------------------
        //                     D1 tracking  all
        // ---------------------------------------------------------------------------
        cb_SiDISCS_lay_RIn = 5 * cm;
        cb_SiDISCS_lay_ROut = cfg.ROut - 1 * cm;

        cb_SiDISCS_lay_Material = G4Material::GetMaterial("Ar10CO2");  //----   !!!!! ----
     //   cb_SiDISCS_lay_Material =G4Material::GetMaterial("G4_Galactic");
        cb_SiDISCS_lay_Solid = new G4Tubs("cb_SiDISCS_lay_Solid", cb_SiDISCS_lay_RIn, cb_SiDISCS_lay_ROut,
                                      cb_SiDISCS_lay_SizeZ / 2., 170., 330 * deg);
        cb_SiDISCS_Lay_Logic = new G4LogicalVolume(cb_SiDISCS_lay_Solid, cb_SiDISCS_lay_Material, "cb_SiDISCS_lay_Logic");



        int ffsi_counter = 0;
        for (int fflay = 0; fflay < cfg.Nlayers; fflay++) {
            double Z = -cfg.SizeZ / 2 + (fflay + 1) * cb_SiDISCS_lay_SizeZ / 2 + (fflay + 1) * 5 * cm;
            cb_SiDISCS_lay_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, Z),
                                                "cb_SiDISCS_lay_Phys", cb_SiDISCS_Lay_Logic,
                                                Phys, false, ffsi_counter);
            ffsi_counter++;
            attr_cb_SiDISCS_lay = new G4VisAttributes(G4Color(0.8, 0.4 + 0.1 * fflay, 0.3, 1.));
            attr_cb_SiDISCS_lay->SetLineWidth(1);
            attr_cb_SiDISCS_lay->SetForceSolid(true);
            cb_SiDISCS_Lay_Logic->SetVisAttributes(attr_cb_SiDISCS_lay);
        }

    };

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical
    G4Tubs *cb_SiDISCS_lay_Solid;    //pointer to the solid  FARFORWD
    G4LogicalVolume *cb_SiDISCS_Lay_Logic;    //pointer to the logical FARFORWD
    G4VPhysicalVolume *cb_SiDISCS_lay_Phys;    //pointer to the physical FARFORWD

    /// Parameters that was used in the moment of construction
    cb_SiDISCS_Config  ConstructionConfig;

private:

    // define here local variables and parameter of detectors
   G4double cb_SiDISCS_lay_RIn;
    G4double cb_SiDISCS_lay_ROut;
    G4double cb_SiDISCS_lay_SizeZ;
    G4Material *cb_SiDISCS_lay_Material;
    G4VisAttributes* attr_cb_SiDISCS_lay;


};




#endif //G4E_CB_SIDISCS_HH
