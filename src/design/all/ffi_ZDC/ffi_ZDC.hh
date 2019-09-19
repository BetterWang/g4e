//
// Created by yulia on 6/25/19.
//

#ifndef G4E_FFI_ZDC_HH
#define G4E_FFI_ZDC_HH



#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

struct ffi_ZDC_Config {
// define here Global volume parameters
  double Width=70.*cm;
  double Thickness =150.*cm;
  double Zpos;
  double Xpos;
  double Angle=-0.05;
  G4RotationMatrix rot_matx;

};


class ffi_ZDC_Design {
public:
    inline void Construct(ffi_ZDC_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin ffi_ZDC volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors
        ffi_ZDC_det_Material = G4Material::GetMaterial("Iron");
        Solid = new G4Box("ffi_ZDC_GVol_Solid", cfg.Width*0.5 , cfg.Width *0.5, cfg.Thickness *0.5);
        Logic = new G4LogicalVolume(Solid, ffi_ZDC_det_Material, "ffi_ZDC_GVol_Logic");
        Phys = new G4PVPlacement(G4Transform3D(cfg.rot_matx,G4ThreeVector(cfg.Xpos,0,cfg.Zpos)), "ffi_ZDC_GVol_Phys", Logic, motherVolume, false, 0);

        // ffi_ZDC_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);
        G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.1, 0, 1., 1.));
        visAttr->SetLineWidth(1);
        visAttr->SetForceSolid(true);
        Logic->SetVisAttributes(visAttr);


    };

    inline void ConstructDetectors() {
        printf("Begin ffi_ZDC detector volumes \n");
        auto cfg = ConstructionConfig;
        // construct here your detectors

    };

    G4Box *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    ffi_ZDC_Config  ConstructionConfig;

private:

    // define here local variables and parameter of detectors
    G4Material *ffi_ZDC_det_Material;
};



#endif //G4E_FFI_ZDC_HH
