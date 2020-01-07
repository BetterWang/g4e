//
// Created by yulia on 6/7/19.
//

#ifndef G4E_CB_EMCAL_HH
#define G4E_CB_EMCAL_HH


#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"

#include "JLeicDetectorConfig.hh"

struct cb_EMCAL_Config {
    double RIn;
    double ROut;
    double ROutshift = 5*cm;
    double Thickness=40*cm;
    double SizeZ;

};


class cb_EMCAL_Design {
public:
    inline void Construct(cb_EMCAL_Config cfg, cb_Solenoid_Config cfgsolenoid,G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin cb_CTD volume \n");

        ConstructionConfig = cfg;
        static char abname[256];

        //........................EMCAL Barrel detector----------------------------------------------
        printf("Begin cb_EMCAL  volume \n");

        // working

        //    G4double EMCALB_R_cone[4]={cb_EMCAL_GVol_RIn,  cb_EMCAL_GVol_ROut, cb_EMCAL_GVol_ROut, cb_EMCAL_GVol_RIn };
        //  G4double cb_EMCAL_GVol_ConeZ[4]={ -SizeZ/2, -SizeZ/2+60*cm, SizeZ/2-60*cm, SizeZ/2};


        double coneROut[4] = {cfg.ROut,           cfg.ROut,cfg.ROut, cfg.ROut};
        double coneRIn[4] =  {cfg.ROut - 1. * cm, cfg.RIn, cfg.RIn,  cfg.ROut - 1. * cm};
        double coneZ[4] = {-cfgsolenoid.SizeZ / 2, -cfgsolenoid.SizeZ / 2 + 30 * cm,
                           cfgsolenoid.SizeZ / 2 - 30 * cm,
                           cfgsolenoid.SizeZ / 2};

        Solid = new G4Polycone("cb_EMCAL_GVol_Solid", 0. * deg, 360. * deg, 4, coneZ, coneRIn,
                               coneROut);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "cb_EMCAL_GVol_Logic");
        Phys = new G4PVPlacement(0, G4ThreeVector(), "cb_EMCAL_GVol_Phys", Logic,
                                              motherVolume, false, 0);


        G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 1.));
        visAttr->SetLineWidth(1);
        visAttr->SetForceSolid(false);
        Logic->SetVisAttributes(visAttr);

    }


    inline void ConstructBars() {
        static char abname[256];
        auto cfg = ConstructionConfig;

    // cb_EMCAL_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);

    // Setting material
    cb_EMCAL_det_Material = fMat->GetMaterial("PbWO4");
    Logic-> SetMaterial(cb_EMCAL_det_Material);

    // Visualising it differently
    auto visualAttributes = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 1.));
    visualAttributes->SetLineWidth(1);
    visualAttributes->SetForceSolid(true);
    Logic-> SetVisAttributes(visualAttributes);
    //    fPhysicsEMCAL = new G4PVPlacement(0, G4ThreeVector(0,0,-40*cm), "EMCALbSolPhys",fLogicEMCAL,
    //                               cb_EMCAL_GVol_Phys, false,     0 );
}
    G4Polycone *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    cb_EMCAL_Config ConstructionConfig;

 private:

    g4e::Materials *fMat;
    G4double cb_EMCAL_det_RIn;
    G4double cb_EMCAL_det_ROut;
    G4double cb_EMCAL_det_SizeZ;
    G4Material *cb_EMCAL_det_Material;
    G4VisAttributes *attr_cb_EMCAL;
    // G4Tubs*            cb_EMCAL_det_Solid;    //pointer to the solid World
    G4Polycone *cb_EMCAL_det_Solid;    //pointer to the solid World
    G4LogicalVolume *cb_EMCAL_det_Logic;    //pointer to the logical World
    G4VPhysicalVolume *cb_EMCAL_det_Phys;    //pointer to the physical World

};

#endif //G4E_CB_EMCAL_HH
