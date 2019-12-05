//
// Created by yulia on 11/19/19.
//

#ifndef G4E_IR_BEAMPIPE_HH
#define G4E_IR_BEAMPIPE_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"

#include "JLeicDetectorConfig.hh"

struct ir_Beampipe_Config {
// define here Global volume parameters

  double Rin=3.114 *cm;
  double Rout=3.115 *cm;
  double Thickness =150.*cm;
  double  Size_Z=1600 * cm;
  double Zpos;
  double Xpos;
  double Angle=-0.0265;
  G4RotationMatrix rot_matx;


};
class ir_Beampipe_Design {
public:


    //----------------BEAMPIPE elements ----------------------------------------------
    G4Material *fBeampipeVacuum;

    // ------------inner Be beampipe ------------

    G4Material *fBeamPipeMaterialBe;

    //------------Volume -----------
    G4Tubs *fSolidBeamBe;    //pointer to the solid World
    G4LogicalVolume *fLogicBeamBe;    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsBeamBe;    //pointer to the physical World

    //------------Wall -----------
    G4Tubs *fSolidBeamBe1;    //pointer to the solid World
    G4LogicalVolume *fLogicBeamBe1;    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsBeamBe1;    //pointer to the physical World
    //------------Vacuum-----------
    G4Tubs *fSolidBeamBe2;    //pointer to the solid World
    G4LogicalVolume *fLogicBeamBe2;    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsBeamBe2;    //pointer to the physical World

    //-----------------------------------------------
    // ------------HAdron beampipe ------------
    G4double fBeamSizeRinH;
    G4double fBeamSizeRoutH;
    G4double fBeamSizeZ_H;
    G4Material *fBeamPipeMaterialH;

    //------------Volume -----------
    G4Tubs *fSolidBeamH;    //pointer to the solid World
    G4LogicalVolume *fLogicBeamH;    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsBeamH;    //pointer to the physical World

    // ------------Cone inner  beampipe ------------
    G4double fBeamSizeRin;
    G4double fBeamSizeRout;
    G4double fBeamSizeZ;
    G4Material *fBeamPipeMaterial;


    G4double fBeamSizeRin1Cone;
    G4double fBeamSizeRin2Cone;
    G4double fBeamSizeRout1Cone;
    G4double fBeamSizeRout2Cone;
    G4double fBeamSizeZCone;
    G4Material *fBeamPipeConeMaterial;

    G4Polycone *fSolidBeamCone;    //pointer to the solid World
    G4LogicalVolume *fLogicBeamCone;    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsBeamCone;    //pointer to the physical World


    G4Cons *fSolidBeamCone2;    //pointer to the solid World
    G4LogicalVolume *fLogicBeamCone2;    //pointer to the logical World
    G4VPhysicalVolume *fPhysicsBeamCone2;    //pointer to the physical World
    ir_Beampipe_Config  ConstructionConfig;


    inline void Construct(ir_Beampipe_Config cfg, double ShiftVTX, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume) {
         ConstructionConfig = cfg;
        /*--------------------------------------*/;
        /*-------Volume for Inner Berilium------*/;
        fBeampipeVacuum = G4Material::GetMaterial("G4_Galactic");
        fSolidBeamBe = new G4Tubs("BeampipeBe", 0, cfg.Rout, cfg.Size_Z/ 2., 0., 360 * deg);
        fLogicBeamBe = new G4LogicalVolume(fSolidBeamBe, fBeampipeVacuum, "BeampipeBe");

        G4RotationMatrix bpr_h;
        bpr_h.rotateY((-0.05 * 180 / 3.1415) * deg);

        fPhysicsBeamBe = new G4PVPlacement(G4Transform3D(bpr_h, G4ThreeVector(0, 0, ShiftVTX)), "BeampipeBe1-b", fLogicBeamBe, motherVolume, false, 0);

        G4VisAttributes *vb1;
        vb1 = new G4VisAttributes(G4Color(0.4, 0.4, 0.5, 1.));
        vb1->SetForceSolid(true);
        fLogicBeamBe->SetVisAttributes(vb1);


        //-------Wall------

        /*
        //  fBeamPipeMaterialBe = fMat->GetMaterial("Be");
        fBeamPipeMaterialBe = fMat->GetMaterial("G4_Galactic");
       fSolidBeamBe1 = new G4Tubs("BeampipeBe_wall",  fBeamSizeRinBe,fBeamSizeRoutBe,fBeamSizeZ_Be/2.,0.,360*deg);
       fLogicBeamBe1 = new G4LogicalVolume(fSolidBeamBe1,  fBeamPipeMaterialBe,  "BeampipeBe_wall");

       fPhysicsBeamBe1 = new G4PVPlacement(0, G4ThreeVector(), "BeampipeBe_wall",fLogicBeamBe1,
                                        fPhysicsBeamBe, false,  0 );


      //-------Vacuum------

        fBeampipeVacuum  = fMat->GetMaterial("G4_Galactic");
        fSolidBeamBe2 = new G4Tubs("BeampipeBe_vac", 0.0,fBeamSizeRinBe,fBeamSizeZ_Be/2.,0.,360*deg);
       fLogicBeamBe2 = new G4LogicalVolume(fSolidBeamBe2,  fBeampipeVacuum,  "BeampipeBe_vac");

       fPhysicsBeamBe2 = new G4PVPlacement(0, G4ThreeVector(), "BeampipeBe_vac",fLogicBeamBe2,
                                       fPhysicsBeamBe, false,  0 );


       //-------End -Volume for Inner Berilium-------
       //--------------------------------------------

       //--------------------------------------
       //-------Beampipe Hadrons---------------
       //--------------------------------------
      fBeamSizeRoutH=3.3*cm; fBeamSizeZ_H=3200.*cm;
       fSolidBeamH = new G4Tubs("BeampipeH", 0,fBeamSizeRoutH,fBeamSizeZ_H/2.,0.,360*deg);
       fLogicBeamH = new G4LogicalVolume(fSolidBeamH, fBeamPipeMaterialBe,  "BeampipeH");
       G4RotationMatrix  bpr_h;
       //   bpr_h.rotateY(-2.5*deg);
       bpr_h.rotateY((-0.05*180/3.1415)*deg);
       // bpr_h.rotateY(-2.5*deg);
       fPhysicsBeamH = new G4PVPlacement(G4Transform3D(bpr_h, G4ThreeVector(0,0,-World_ShiftVTX)), "BeampipeH",fLogicBeamH,
                                       Phys, false,  0 );

        vb1= new G4VisAttributes(G4Color(0.5, 0., 0.3,1.));
        vb1->SetForceSolid(true);  fLogicBeamH->SetVisAttributes(vb1);


        //--------------------------------------
       //--------------------cone-------------------------------
       //--------------------------------------

      fBeamPipeConeMaterial = fMat->GetMaterial("G4_Galactic");
      G4RotationMatrix bpr1,bpr2,bpr;
      double xPos=-2*cm;


       fBeamSizeZCone=100*cm;
       double fBeamSizeZCone2=155*cm;
       fBeamSizeRin1Cone=3.115*cm;
       fBeamSizeRin2Cone=fBeamSizeRin1Cone +0.025*(fBeamSizeZCone);

       G4double R_cone[4]={ 0.*cm, 3.5*cm , 15.*cm, 0.*cm};
       G4double Z_cone[4]={ 0.*cm,  0.*cm,  (SizeZ/2),  (SizeZ/2)};
       //World_ShiftVTX
       // Z1=  fBeamSizeZ_Be

       //  printf("zPos=%f,fBeamSizeRin2Cone=%f fBeamSizeRout2Cone=%f fBeamSizeZCone+zPos=%f\n",zPos,
       //       fBeamSizeRin2Cone,fBeamSizeRout2Cone,fBeamSizeZCone+zPos);

       fSolidBeamCone = new G4Polycone("BeampipeCone", 0., 360., 4, R_cone , Z_cone );

        fLogicBeamCone = new G4LogicalVolume(fSolidBeamCone,  fBeamPipeConeMaterial,  "BeampipeCone");

        //  fPhysicsBeamCone = new G4PVPlacement(0, G4ThreeVector(0,0,-30*cm), "BeampipeCone",fLogicBeamCone,
        //                           Phys, false,    0 );



        bpr1.rotateY(-2.5*deg);
       fPhysicsBeamCone = new G4PVPlacement(G4Transform3D(bpr1, G4ThreeVector(0,0,0*cm)), "BeampipeCone",fLogicBeamCone,
                                   Phys, false,    0 );

       vb1= new G4VisAttributes(G4Color(1.0, 1.0, 0.1,1));

       */;

        /*


         double zPos2=(fBeamSizeZCone2/2)+ 25*cm;

        fSolidBeamCone2 = new G4Cons("VTX1pipeCone2",  fBeamSizeRin1Cone, fBeamSizeRout1Cone,
                                  fBeamSizeRin2Cone,fBeamSizeRout2Cone,fBeamSizeZCone2/2.,0.,360*deg);

        fLogicBeamCone2 = new G4LogicalVolume(fSolidBeamCone2,  fBeamPipeConeMaterial,  "Beam2Cone");


        brm.rotateY((180.-0.026*180/3.1415)*deg);
        xPos= 2*cm;
         fPhysicsBeamCone2 = new G4PVPlacement(G4Transform3D(brm, G4ThreeVector(xPos,0,-zPos2)), "Beam2Cone",fLogicBeamCone2,
                                       Phys, false,  0 );


         vb1->SetForceSolid(true); fLogicBeamCone2->SetVisAttributes(vb1);fLogicBeamCone->SetVisAttributes(vb1);


        */




    };

};
#endif //G4E_IR_BEAMPIPE_HH
