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

#include "main_detectors/DetectorConfig.hh"
//#include "../../ZDC/006.FoCalGeant4/source/include/constants.hh"
#include "Alice_ZDC_Geometry.hh"

struct ffi_ZDC_Config {
// define here Global volume parameters
  double Width=60.*cm;//60.*cm;
  double Thickness = 250.*cm;//250.*cm;
  double Zpos;
  double Xpos;
  double Angle=-0.0265;
  double EMCAL_Thickness=30*cm;
  G4RotationMatrix rot_matx;
  int NtowersX=20;
  int NtowersY=20;

  bool bAliceAbsorber = true;
  bool bZDCB0 = false;
};


class ffi_ZDC_Design {
public:
    inline void Construct(ffi_ZDC_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin ffi_ZDC volume \n");

        ConstructionConfig = cfg;
        // create  a global volume for your detectors
    //    ffi_ZDC_det_Material = G4Material::GetMaterial("Iron");
        Solid = new G4Box("ffi_ZDC_GVol_Solid", cfg.Width*0.5 , cfg.Width *0.5, cfg.Thickness *0.5);

        Logic = new G4LogicalVolume(Solid, worldMaterial, "ffi_ZDC_GVol_Logic");
        Phys = new G4PVPlacement(G4Transform3D(cfg.rot_matx,G4ThreeVector(cfg.Xpos,0,cfg.Zpos)), "ffi_ZDC_GVol_Phys", Logic, motherVolume, false, 0);

        // ffi_ZDC_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);

        G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.1, 0, 1., 0.1));
        visAttr->SetLineWidth(1);
        visAttr->SetForceSolid(true);
        Logic->SetVisAttributes(visAttr);

    }


      inline void ConstructALICE() {
        Geometry* alice = new Geometry();
        alice->ConstructZDC(Phys);
      }

      inline void ConstructALICEPrototype(ffi_ZDC_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
          std::cout << " --> Construct ALICE ZDC Prototype Width = " << cfg.Width << " Thickness = " << cfg.Thickness << " rotation = " << cfg.rot_matx << std::endl;
          Solid = new G4Box("ffi_ZDC_GVol_Solid", cfg.Width*0.5 , cfg.Width *0.5, cfg.Thickness *0.5);
          Logic = new G4LogicalVolume(Solid, worldMaterial, "ffi_ZDC_GVol_Logic");
          G4VisAttributes *visAttr = new G4VisAttributes(false);
          Logic->SetVisAttributes(visAttr);
          Phys = new G4PVPlacement(G4Transform3D(cfg.rot_matx,G4ThreeVector(cfg.Xpos,0,cfg.Zpos)), "ffi_ZDC_GVol_Phys", Logic, motherVolume, false, 0);

          Geometry* alice = new Geometry(cfg.bAliceAbsorber);

          alice->ConstructZDCPrototype(Phys);
          ffi_ZDC_HCAL_Logic = alice->GetScoringVol_PAD();
          ffi_ZDC_SCI_Logic  = alice->GetScoringVol_SCI();
      }

      inline void ConstructB0(G4double Rout, G4double length, G4double Zpos, G4VPhysicalVolume* motherVolume ) {
        std::cout << " --> Construct ZDC B0 unit, radius = " << Rout / cm - 2<< " length = " << length / cm << std::endl;

        SolidB0 = new G4Tubs("ffi_ZDC_GVol_SolidB0", 0., Rout - 2*cm, length, -130 * deg, 275 * deg);
        G4NistManager* material_Man = G4NistManager::Instance();  //NistManager: start element destruction
        G4Material* material_PbWO4 = material_Man->FindOrBuildMaterial("G4_PbWO4");
        LogicB0 = new G4LogicalVolume(SolidB0, material_PbWO4, "ffi_ZDC_GVol_LogicB0");
        PhysB0 = new G4PVPlacement(0, G4ThreeVector(0, 0, Zpos), "ffi_ZDC_GVol_PhysB0_", LogicB0, motherVolume, false, 0);

        G4VisAttributes *attr = new G4VisAttributes(G4Color(0.8, 0.2, 0.2, 0.9));
        attr->SetLineWidth(1);
        attr->SetForceSolid(true);
        LogicB0->SetVisAttributes(attr);

      }

      inline void ConstructTowels(int Type) {
          printf("Begin ffi_ZDC detector Tower volumes \n");
          auto cfg = ConstructionConfig;
          // construct here your detectors
          static char abname[256];

          //------------------------------------------------------------------
          // Ecal module Crystals
          //-------------------------------------------------------------------

          ffi_ZDC_HCAL_Thickness = cfg.EMCAL_Thickness;
          ffi_ZDC_HCAL_ROut = 60 * cm;
          ffi_ZDC_HCAL_Width = 5. * cm;
          ffi_ZDC_HCAL_Gap = 5 * mm;

	  if(Type==0) {    ffi_ZDC_HCAL_Material = fMat->GetMaterial("PbWO4");}
          else { ffi_ZDC_HCAL_Material = fMat->GetMaterial("IronAll");}

          ffi_ZDC_HCAL_Solid = new G4Box("ffi_ZDC_HCAL_Solid_Tower", ffi_ZDC_HCAL_Width * 0.5,
                                            ffi_ZDC_HCAL_Width * 0.5, ffi_ZDC_HCAL_Thickness * 0.5);
          ffi_ZDC_HCAL_Logic = new G4LogicalVolume(ffi_ZDC_HCAL_Solid, ffi_ZDC_HCAL_Material,
                                                      "ffi_ZDC_HCAL_Logic_Tower");

          attr_ffi_ZDC_HCAL = new G4VisAttributes(G4Color(0.1, 1.0, 0.9, 1.));
          attr_ffi_ZDC_HCAL->SetLineWidth(1);
          attr_ffi_ZDC_HCAL->SetForceSolid(true);
          ffi_ZDC_HCAL_Logic->SetVisAttributes(attr_ffi_ZDC_HCAL);

          // Crystals

          ffi_ZDC_HCAL_InnerR = 0. * cm;
          G4double y_C = 0;
          G4double x_C;
          ffi_ZDC_HCAL_PosZ = -cfg.Thickness / 2 + ffi_ZDC_HCAL_Thickness / 2 +2*mm;
          G4int k = -1;

//============  For sectors =====
          for (int j = 0; j < cfg.NtowersX; j++) {
              if(j==0) {y_C=cfg.Width/2.-ffi_ZDC_HCAL_Width/2. - ffi_ZDC_HCAL_Gap;}
	      else { y_C -= (ffi_ZDC_HCAL_Width + ffi_ZDC_HCAL_Gap);}

              if( abs(y_C+ffi_ZDC_HCAL_Width/2.) > cfg.Width/2) continue;


              x_C = cfg.Width/2.-(ffi_ZDC_HCAL_Width + ffi_ZDC_HCAL_Gap) * 0.5;

              for (int i = 0; i <  cfg.NtowersY; i++) {

		if (i > 0)  x_C  -= (ffi_ZDC_HCAL_Width + ffi_ZDC_HCAL_Gap);

                 if (abs(x_C +ffi_ZDC_HCAL_Width/2.  ) >  cfg.Width/2.) continue;

                       printf("EMCALLL::k=%d  j=%d i =%d x=%f, y=%f   \n ",k, j,i, x_C,y_C );

                      k++;
                      sprintf(abname, "ffi_ZDC_HCAL_Phys_%d", k);
                      new G4PVPlacement(0, G4ThreeVector(x_C, y_C, ffi_ZDC_HCAL_PosZ), abname, ffi_ZDC_HCAL_Logic,
                                        Phys, false, k);
		      //  printf("ffi_ZDC_HCAL::k=%d  j=%d i =%d x=%f, y=%f  R=%f ffi_ZDC_HCAL_InnerR=%f \n ",k, j,i, x_C,y_C, R, ffi_ZDC_HCAL_InnerR);

	      }

	  }

      };


    G4Box *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    // B0 unit
    G4Tubs              *SolidB0;
    G4LogicalVolume     *LogicB0;
    G4VPhysicalVolume   *PhysB0;

    /// Parameters that was used in the moment of construction
    ffi_ZDC_Config  ConstructionConfig;
    G4Box *ffi_ZDC_HCAL_Solid;
    G4LogicalVolume *ffi_ZDC_HCAL_Logic;
    G4LogicalVolume *ffi_ZDC_SCI_Logic;


private:
g4e::Materials *fMat;
    // define here local variables and parameter of detectors
    G4Material *ffi_ZDC_det_Material;
      //............... EMCAL Crystals modules ......................
    double ffi_ZDC_HCAL_Thickness;
    double ffi_ZDC_HCAL_Width;
    double ffi_ZDC_HCAL_ROut;
    G4double ffi_ZDC_HCAL_Gap ;
    G4double ffi_ZDC_HCAL_InnerR;
    G4double ffi_ZDC_HCAL_PosZ;

    G4Material *ffi_ZDC_HCAL_Material;
    G4VisAttributes *attr_ffi_ZDC_HCAL;


};



#endif //G4E_FFI_ZDC_HH
