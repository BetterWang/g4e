//
// Created by yulia on 6/14/19.
//

#ifndef G4E_CI_TRD_HH
#define G4E_CI_TRD_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"


struct ci_TRD_Config {
// define here Global volume parameters
    double RIn = 20 * cm;
    double ROut = 200 * cm;
    double ThicknessZ = 30 * cm;
    double PosZ;
    G4double fRadZ;
    //----------------------------
    double fGasGap = 0.600 * mm;    // for ZEUS  300-publication
    double fRadThick = NAN;
    int fFoilNumber = NAN;
    //----------------------------
    double det_RIn = 50 * cm;
    double det_ROut = 100 * cm;
    double det_ThicknessZ = 2.5 * cm;
    double det_PosZ;
    G4double fDetThickness;
    G4double fDetLength;

    double fAbsorberThickness = 0.050 * mm;
    double fAbsorberRadius = 100. * mm;
    double fAbsorberZ = 136. * cm;
    double fDetGap = 0.01 * mm;
    int fModuleNumber = 1;
    G4Material *fRadiatorMat;        // pointer to the mixed TR radiator material
    G4Material *det_Material;

    G4double fRadThickness = 0.020 * mm;    // 16 um // ZEUS NIMA 323 (1992) 135-139, D=20um, dens.= 0.1 g/cm3


};


class ci_TRD_Design {
public:
    inline void Construct(ci_TRD_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin ci_TRD volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors
        Solid = new G4Tubs("ci_TRD_GVol_Solid", cfg.RIn, cfg.ROut, cfg.ThicknessZ / 2., 0.,
                           360 * deg);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "ci_TRD_GVol_Logic");
        G4VisAttributes *attr_ci_TRD_GVol = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 0.9));
        attr_ci_TRD_GVol->SetLineWidth(1);
        attr_ci_TRD_GVol->SetForceSolid(false);
        Logic->SetVisAttributes(attr_ci_TRD_GVol);

        Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.PosZ), "H_CAP_TRD_Physics", Logic,
                                 motherVolume, false, 0);
    };

    //------------------------------------------------------------------
    inline void ConstructDetectors() {
        auto &cfg = ConstructionConfig;
        ConstructRadiator(cfg); // needs to be constructed always!
        ConstructAbsorber(cfg);
        printf("fFoilNumber2=%d \n", cfg.fFoilNumber);

    };

    //------------------------------------------------------------------
    inline void ConstructAbsorber(ci_TRD_Config& cfg) {
        static char abname[256];

        cfg.det_PosZ = cfg.fRadZ + cfg.fRadThick / 2 + cfg.det_ThicknessZ / 2.;
        cfg.det_Material = fMat->GetMaterial("Xe20CO2");
        ci_TRD_det_Solid = new G4Tubs("ci_TRD_det_Solid", cfg.det_RIn, cfg.det_ROut, cfg.det_ThicknessZ / 2., 0.,
                                      360 * deg);
        ci_TRD_det_Logic = new G4LogicalVolume(ci_TRD_det_Solid, cfg.det_Material, "ci_TRD_det_Logic");
        attr_ci_TRD_det = new G4VisAttributes(G4Color(0.8, 0.4, 0.3, 0.8));
        attr_ci_TRD_det->SetLineWidth(1);
        attr_ci_TRD_det->SetForceSolid(true);
        ci_TRD_det_Logic->SetVisAttributes(attr_ci_TRD_det);

        ci_TRD_det_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.det_PosZ), "ci_TRD_det_Physics",
                                            ci_TRD_det_Logic,
                                            Phys, false, 0);


    };

    inline void ConstructRadiator(ci_TRD_Config& cfg) {
        static char abname[256];
        // construct here your detectors
        //=========================================================================
        //                   TR radiator envelope
        //=========================================================================

        if (fRegGasDet != 0) delete fRegGasDet;
        if (fRegGasDet == 0) fRegGasDet = new G4Region("XTRdEdxDetector");

        printf("Radiator\n");
        // Preparation of mixed radiator material

        G4Material *Mylar = fMat->GetMaterial("Mylar");
        G4Material *Air = fMat->GetMaterial("Air");
        G4Material *Al = fMat->GetMaterial("Al");
        G4Material *CH2 = fMat->GetMaterial("CH2");
        G4Material *He = fMat->GetMaterial("He");


        G4cout << "G4Material* CH2   = " << CH2 << G4endl;
        G4cout << "G4Material* Mylar = " << Mylar << G4endl;
        G4double foilDensity = 0.91 * g / cm3;  // CH2 1.39*g/cm3; // Mylar //  0.534*g/cm3; //Li
        G4double gasDensity = 1.2928 * mg / cm3; // Air // 1.977*mg/cm3; // CO2 0.178*mg/cm3; // He
        G4double totDensity = foilDensity * foilGasRatio + gasDensity * (1.0 - foilGasRatio);
        G4cout << "Rad totDensity = " << totDensity / (g / cm3) << " g/cm3 " << G4endl;
        G4cout << "use for Rad totDensity = " << totDensity / (g / cm3) << " g/cm3 " << G4endl;


        G4double fractionFoil = foilDensity * foilGasRatio / totDensity;
        G4double fractionGas = gasDensity * (1.0 - foilGasRatio) / totDensity;
        G4Material *radiatorMat0 = new G4Material("radiatorMat0", totDensity, 2);
        radiatorMat0->AddMaterial(CH2, fractionFoil);
        radiatorMat0->AddMaterial(Air, fractionGas);
        G4double NewDensity = 0.083 * (g / cm3);
        G4Material *radiatorMat = new G4Material("radiatorMat", NewDensity, 1);
        radiatorMat->AddMaterial(radiatorMat0, 1.);
        G4cout << "new  Rad with totDensity = " << NewDensity / (g / cm3) << " g/cm3 " << G4endl;

        G4double XTR_density = radiatorMat->GetDensity();
        G4cout << "Read back Rad totDensity = " << XTR_density / (g / cm3) << " g/cm3 " << G4endl;
        // default materials of the detector and TR radiator
        cfg.fRadiatorMat = radiatorMat;
        fFoilMat = CH2; // Kapton; // Mylar ; // Li ; // CH2 ;
        fGasMat = Air; // CO2; // He; //
//--------------------------material -------------------------------


        cfg.fRadThick = 10. * cm - cfg.fGasGap + cfg.fDetGap;

        cfg.fFoilNumber = cfg.fRadThick / (cfg.fRadThickness + cfg.fGasGap);

        printf("fFoilNumber1=%d \n", cfg.fFoilNumber);
        cfg.fRadZ = -cfg.ThicknessZ / 2 + cfg.fRadThick / 2 + 2 * cm;

        foilGasRatio = cfg.fRadThickness / (cfg.fRadThickness + cfg.fGasGap);

        fSolidRadiator = new G4Tubs("ci_TRD_Radiator_Solid", 50 * cm, 100 * cm, 0.5 * cfg.fRadThick, 0., 360 * deg);
        fLogicRadiator = new G4LogicalVolume(fSolidRadiator, cfg.fRadiatorMat,
                                             "ci_TRD_Radiator_Logic");

        attr_ci_TRD_rad = new G4VisAttributes(G4Color(0.8, 0.7, 0.6, 0.8));
        attr_ci_TRD_rad->SetLineWidth(1);
        attr_ci_TRD_rad->SetForceSolid(true);
        fLogicRadiator->SetVisAttributes(attr_ci_TRD_rad);

        fPhysicsRadiator = new G4PVPlacement(0,
                                             G4ThreeVector(0, 0, cfg.fRadZ),
                                             "ci_TRD_Radiator_Phys", fLogicRadiator,
                                             Phys, false, 0);

        delete fRadRegion;
        if (fRadRegion == nullptr) fRadRegion = new G4Region("XTRradiator");
        fRadRegion->AddRootLogicalVolume(fLogicRadiator);

        printf("Radiator done \n");
        //-----------------------------------------------------------------

    };
    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ci_TRD_Config ConstructionConfig;


    G4LogicalVolume *fLogicRadiator;
    G4double foilGasRatio;
    G4Material *fFoilMat;            // pointer to the TR foil radiator material
    G4Region *fRadRegion;
    G4Material *fGasMat;             // pointer to the TR gas radiator material
    G4Region *fRegGasDet;


private:
    JLeicMaterials *fMat;
    // define here local variables and parameter of detectors
//-------------rad---------------

    //------------- det -----------

    G4VisAttributes *attr_ci_TRD_det;
    G4VisAttributes *attr_ci_TRD_rad;
    G4Tubs *ci_TRD_det_Solid;
    G4LogicalVolume *ci_TRD_det_Logic;
    G4PVPlacement *ci_TRD_det_Phys;

    G4Tubs *fSolidRadiator;
    G4VPhysicalVolume *fPhysicsRadiator;


};

#endif //G4E_CI_TRD_HH
