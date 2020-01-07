//
// Created by yulia on 6/11/19.
//

#ifndef G4E_CB_HCAL_HH
#define G4E_CB_HCAL_HH


#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

struct cb_HCAL_Config {

    G4double RIn;
    G4double ROut;
    G4double Thickness = 100. * cm;

    G4double SizeZ;
    G4double ShiftZ;

};


class cb_HCAL_Design {
public:
    inline void Construct(cb_HCAL_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin cb_HCAL volume \n");

        ConstructionConfig = cfg;

        //cb_HCAL_det_Material = fMat->GetMaterial("StainlessSteel");
        Solid = new G4Tubs("cb_HCAL_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);

        Logic = new G4LogicalVolume(Solid, worldMaterial, "cb_HCAL_GVol_Logic");

  //      Phys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, cfg.ShiftZ - cfg.SizeZ / 2), "cb_HCAL_GVol_Phys",
   //                              Logic,motherVolume, false, 0);
        Phys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, cfg.ShiftZ), "cb_HCAL_GVol_Phys",
                                 Logic,motherVolume, false, 0);

        G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.3, 0, 3., 0.1));
        visAttr->SetLineWidth(1);
        visAttr->SetForceSolid(false);
        Logic->SetVisAttributes(visAttr);
        //  cb_HCAL_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);

    }


    inline void ConstructLayers() {
        static char abname[256];
        auto cfg = ConstructionConfig;

        printf("Create HCAL  Size Z =%f \n ", cfg.SizeZ);


        cb_HCAL_det_SizeZ = cfg.SizeZ;
        cb_HCAL_det_Thickness = 2 * cm;

        cb_HCAL_det_Material = G4Material::GetMaterial("Iron");

        int hlay = 0;
        int NLAY_HCALb = 25;
        for (hlay = 0; hlay < NLAY_HCALb; hlay++) {

            cb_HCAL_det_RIn = cfg.RIn + hlay * 2 * cb_HCAL_det_Thickness;
            cb_HCAL_det_ROut = cb_HCAL_det_RIn + cb_HCAL_det_Thickness;
            if (cb_HCAL_det_RIn > cfg.ROut || cb_HCAL_det_ROut > cfg.ROut) continue;

            sprintf(abname, "cb_HCAL_det_Solid_%d", hlay);
            cb_HCAL_det_Solid = new G4Tubs(abname, cb_HCAL_det_RIn, cb_HCAL_det_ROut, cb_HCAL_det_SizeZ / 2., 0., 360 * deg);
            sprintf(abname, "cb_HCAL_det_Logic_%d", hlay);
            cb_HCAL_det_Logic = new G4LogicalVolume(cb_HCAL_det_Solid, cb_HCAL_det_Material, abname);
            sprintf(abname, "cb_HCAL_det_Phys_%d", hlay);
            cb_HCAL_det_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), abname, cb_HCAL_det_Logic,
                                                 Phys, false, hlay);
            attr_cb_HCAL_det = new G4VisAttributes(G4Color(0.6, 0, 0.6, 1));
            attr_cb_HCAL_det->SetLineWidth(1);
            attr_cb_HCAL_det->SetForceSolid(true);
            cb_HCAL_det_Logic->SetVisAttributes(attr_cb_HCAL_det);
        }





    }
    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    cb_HCAL_Config ConstructionConfig;
private:

    G4double cb_HCAL_det_RIn;
    G4double cb_HCAL_det_ROut;
    G4double cb_HCAL_det_SizeZ;
    G4double cb_HCAL_det_Thickness;
    G4Material *cb_HCAL_det_Material;
    G4VisAttributes *attr_cb_HCAL_det;
    G4Tubs *cb_HCAL_det_Solid;      //pointer to the solid
    G4LogicalVolume *cb_HCAL_det_Logic;    //pointer to the logical
    G4VPhysicalVolume *cb_HCAL_det_Phys;  //pointer to the physical


};

#endif //G4E_CB_HCAL_HH
