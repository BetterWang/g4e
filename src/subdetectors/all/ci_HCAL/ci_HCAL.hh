//
// Created by yulia on 6/14/19.
//

#ifndef G4E_CI_HCAL_HH
#define G4E_CI_HCAL_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

struct ci_HCAL_Config {
// define here Global volume parameters
    double RIn=0*cm ;
    double ROut=300*cm;
    double SizeZ=195*cm;
    double ShiftZ=5*cm;
    double PosZ=0.*cm;
    double det_RIn= 80*cm ;
};


class ci_HCAL_Design {
public:
    inline void Construct(ci_HCAL_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin ci_HCAL volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors
//cb_HCAL_det_Material = fMat->GetMaterial("StainlessSteel");
        //   ci_HCAL_GVol_Solid = new G4Polycone("HCAP_HCAL_vol_sol",0.*deg,360.*deg,2,fHCAL_HCAP_Zcone, ci_HCAL_GVol_RIn, ci_HCAL_GVol_ROut);
        Solid  = new G4Tubs("ci_HCAL_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ/2.,0.,360*deg);
        Logic = new G4LogicalVolume(Solid,  worldMaterial,  "ci_HCAL_GVol_Logic");
        //  ci_HCAL_GVol_Phys = new G4PVPlacement(0, G4ThreeVector(0,0,0), "HCAP_HCAL_vol_phys",ci_HCAL_GVol_Logic,
        //					  World_Phys, false,	0 );

        Phys = new G4PVPlacement(0, G4ThreeVector(0,0,cfg.PosZ), "ci_HCAL_GVol_Phys",Logic,
                                              motherVolume, false,	0 );

        G4VisAttributes *attr_ci_HCAL_GVol= new G4VisAttributes(G4Color(0.3,0,3.,0.1));
        attr_ci_HCAL_GVol->SetLineWidth(1); attr_ci_HCAL_GVol->SetForceSolid(false);
        Logic->SetVisAttributes(attr_ci_HCAL_GVol);
        //  cb_HCAL_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);



    };

    inline void ConstructDetectors(ci_HCAL_Config cfg) {
        using namespace spdlog;
        debug("Begin ci_HCAL detector volumes ");

        // construct here your detectors

        static char abname[256];
       // auto cfg = ConstructionConfig;


        ci_HCAL_det_ROut=cfg.ROut -1*cm;
        ci_HCAL_det_ThicknessZ=2*cm;
        ci_HCAL_det_GapZ=2*cm;
        ci_HCAL_Nlay=25;
        ci_HCAL_det_Material= G4Material::GetMaterial("Iron");
        trace("ci_HCAL_det::1 {} {} , {}",cfg.det_RIn, ci_HCAL_det_ROut, ci_HCAL_det_ThicknessZ);

        sprintf(abname,"ci_HCAL_det_Solid");
        ci_HCAL_det_Solid  = new G4Tubs(abname, cfg.det_RIn, ci_HCAL_det_ROut, ci_HCAL_det_ThicknessZ/2.,0.,360*deg);

        sprintf(abname,"ci_HCAL_det_Logic");
        ci_HCAL_det_Logic = new G4LogicalVolume(ci_HCAL_det_Solid, ci_HCAL_det_Material,abname);

        attr_ci_HCAL_det= new G4VisAttributes(G4Color(0.6,0,0.6,1));
        attr_ci_HCAL_det->SetLineWidth(1); attr_ci_HCAL_det->SetForceSolid(true);
        ci_HCAL_det_Logic->SetVisAttributes(attr_ci_HCAL_det);

        int hhlay=0;

        for (hhlay=0;hhlay<ci_HCAL_Nlay;hhlay++){
            ci_HCAL_det_PosZ= -cfg.SizeZ/2   +(hhlay+1)*ci_HCAL_det_ThicknessZ +(hhlay+1)*5*cm;
            trace("ci_HCAL_det::2 {} {} ,{}", ci_HCAL_det_PosZ,ci_HCAL_det_PosZ- ci_HCAL_det_ThicknessZ,cfg.SizeZ);

            if( (ci_HCAL_det_PosZ- ci_HCAL_det_ThicknessZ) > cfg.SizeZ/2) continue;
            //   ci_HCAL_det_PosZ=-ci_HCAL_GVol_SizeZ/2+hhlay*ci_HCAL_det_ThicknessZ+ci_HCAL_det_GapZ*hhlay;
            // ci_HCAL_det_PosZ= hhlay*ci_HCAL_det_ThicknessZ+ci_HCAL_det_GapZ*hhlay;
            sprintf(abname,"ci_HCAL_det_Phys_%d",hhlay);
            ci_HCAL_det_Phys = new G4PVPlacement(0, G4ThreeVector(0,0, ci_HCAL_det_PosZ), abname,ci_HCAL_det_Logic,
                                                 Phys,  false, hhlay);
        }



    };

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ci_HCAL_Config  ConstructionConfig;

private:

    // define here local variables and parameter of detectors

    //---------------ENDCAP-H HCAL det iron-------------------

    G4double ci_HCAL_det_ROut;
    G4double ci_HCAL_det_ThicknessZ;
    G4double ci_HCAL_det_GapZ;
    G4double ci_HCAL_det_PosZ;
    int ci_HCAL_Nlay;
    G4VisAttributes* attr_ci_HCAL_det;
    G4Material *ci_HCAL_det_Material;
    G4Tubs *ci_HCAL_det_Solid;    //pointer to the solid HCAP_HCAL volume
    G4LogicalVolume *ci_HCAL_det_Logic;    //pointer to the logicalHCAP_HCAL  volume
    G4VPhysicalVolume *ci_HCAL_det_Phys;    //pointer to the physical EMCAL barrel volume


};

#endif //G4E_CI_HCAL_HH
