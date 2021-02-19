#ifndef ir_Beampipe_Config_HH
#define ir_Beampipe_Config_HH

#include <G4PVDivision.hh>
#include <G4RotationMatrix.hh>
#include <G4Material.hh>
#include <G4Color.hh>
#include <G4VisAttributes.hh>
#include <G4SystemOfUnits.hh>
#include <G4Tubs.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Polycone.hh>

struct ir_Beampipe_Config
{
    // define here Global volume parameters
    double ShiftVTX;
    double Rin = 3.1 * cm;
    double Rout = 3.2 * cm;
    //  double Thickness =150.*cm;
    double SizeZ = 1.485 * m;
    double ShiftZ;
    double Zpos;
    double Xpos;
    double Angle = -0.0265;
    G4RotationMatrix rot_matx;

    //-------------Forward Cone ---------------
    G4double f_Rin1 = 3.1 * cm;
    G4double f_Rin2 = 11. * cm;
    G4double f_Rout1 = 3.2 * cm;
    G4double f_Rout2 = 11.2 * cm;
    G4double f_SizeZ = 364.4 * cm;
    double f_ShiftZ = 0.5 * cm;
    double f_Zpos = 0;
    // double f_Xpos=-4*cm;
    double f_Xpos = 0 * cm;
    double f_Ypos = 0.;
    // double f_Angle=-0.0265;      // JLAB half-angle
    double f_Angle = 0.011 * rad;   // BNL half angle
    // double f_Angle=;
};

class ir_Beampipe_Design
{
public:


    //----------------BEAMPIPE elements ----------------------------------------------

    // ------------inner Be beampipe ------------

    G4Material *BeamPipeMaterialBe;
    G4Material *BeamPipeVacuum;
    G4Tubs *SolidBeamBe;                // pointer to the solid World
    G4LogicalVolume *LogicBeamBe;       // pointer to the logical World
    G4VPhysicalVolume *PhysicsBeamBe;   // pointer to the physical World
    G4Tubs *SolidBeamBe1;               // pointer to the solid World
    G4LogicalVolume *LogicBeamBe1;      // pointer to the logical World
    G4VPhysicalVolume *PhysicsBeamBe1;  // pointer to the physical World

    //-----------------------------------------------
    // ------------ForwardCone beampipe ------------

    G4Material *f_BeamPipeMaterial;

    //------------Volume -----------
    G4Polycone *f_SolidBeamCone;
    G4LogicalVolume *f_LogicBeamCone;
    G4VPhysicalVolume *f_PhysicsBeamCone;

    G4Polycone *f_SolidBeamConeAl;
    G4LogicalVolume *f_LogicBeamConeAl;
    G4VPhysicalVolume *f_PhysicsBeamConeAl;

    ir_Beampipe_Config ConstructionConfig;


    inline void ConstructCentral(ir_Beampipe_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume)
    {
        ConstructionConfig = cfg;
        /*--------------------------------------*/;
        /*-------Volume for Inner Berilium------*/;
        BeamPipeVacuum = G4Material::GetMaterial("G4_Galactic");
        SolidBeamBe = new G4Tubs("ir_Beampipe_Central_Solid", 0, cfg.Rout, cfg.SizeZ / 2., 0., 360 * deg);
        LogicBeamBe = new G4LogicalVolume(SolidBeamBe, BeamPipeVacuum, "ir_Beampipe_Central_Logic");

        G4RotationMatrix bpr_h;
        //      bpr_h.rotateY((-0.05 * 180 / 3.1415) * deg);
        bpr_h.rotateY((0.) * deg);

        PhysicsBeamBe = new G4PVPlacement(G4Transform3D(bpr_h, G4ThreeVector(0, 0, cfg.Zpos)), "ir_Beampipe_Central_Phys", LogicBeamBe, motherVolume, false, 0);

        G4VisAttributes *vb1;
        vb1 = new G4VisAttributes(G4Color(1., 1., 0., 1.));
        vb1->SetForceSolid(true);
        LogicBeamBe->SetVisAttributes(vb1);


        //-------Wall------

        BeamPipeMaterialBe = G4Material::GetMaterial("Be");
        //  fBeamPipeMaterialBe = fMat->GetMaterial("G4_Galactic");
        SolidBeamBe1 = new G4Tubs("ir_Beampipe_Central_Be_Solid", cfg.Rin, cfg.Rout - 0.01 * cm, cfg.SizeZ / 2., 0., 360 * deg);
        LogicBeamBe1 = new G4LogicalVolume(SolidBeamBe1, BeamPipeMaterialBe, "ir_Beampipe_Central_Be_Logic");

        PhysicsBeamBe1 = new G4PVPlacement(0, G4ThreeVector(), "ir_Beampipe_Central_Be_Phys", LogicBeamBe1, PhysicsBeamBe, false, 0);

    }


    inline void ConstructForwardCone(ir_Beampipe_Config cfg, G4Material *worldMaterial, G4VPhysicalVolume *motherVolume)
    {
        ConstructionConfig = cfg;

        f_BeamPipeMaterial = G4Material::GetMaterial("G4_Galactic");

        G4RotationMatrix bpr1, bpr2, bpr;

        // double xPos=-2*cm;
        // fBeamSizeZCone=100*cm;
        // double fBeamSizeZCone2=155*cm;
        // fBeamSizeRin1Cone=3.115*cm;
        // fBeamSizeRin2Cone=fBeamSizeRin1Cone +0.025*(fBeamSizeZCone);

        // G4double R_cone[4]={ 0.*cm, 3.5*cm , 15.*cm, 0.*cm};
        // G4double Z_cone[4]={ 0.*cm,  0.*cm,  (SizeZ/2),  (SizeZ/2)};
        G4double R_cone[4] = {0. * cm, cfg.f_Rout1, cfg.f_Rout2, 0. * cm};
        G4double Z_cone[4] = {cfg.SizeZ / 2. + cfg.f_ShiftZ, cfg.SizeZ / 2. + cfg.f_ShiftZ, cfg.SizeZ / 2. + (cfg.f_SizeZ) + cfg.f_ShiftZ,
                              cfg.SizeZ / 2. + (cfg.f_SizeZ) + cfg.f_ShiftZ};
        // World_ShiftVTX
        // Z1=  fBeamSizeZ_Be

        // printf("zPos=%f,fBeamSizeRin2Cone=%f fBeamSizeRout2Cone=%f fBeamSizeZCone+zPos=%f\n",zPos,
        // fBeamSizeRin2Cone,fBeamSizeRout2Cone,fBeamSizeZCone+zPos);
        // cfg.f_Xpos=-cfg.f_Rout1*2.;
        f_SolidBeamCone = new G4Polycone("ir_Beampipe_f_Cone_Solid", 0., 360., 4, R_cone, Z_cone);

        f_LogicBeamCone = new G4LogicalVolume(f_SolidBeamCone, f_BeamPipeMaterial, "ir_Beampipe_f_Cone_Logic");

        // fPhysicsBeamCone = new G4PVPlacement(0, G4ThreeVector(0,0,-30*cm), "BeampipeCone",fLogicBeamCone, Phys, false, 0);
        bpr1.rotateY(cfg.f_Angle);
        f_PhysicsBeamCone = new G4PVPlacement(G4Transform3D(bpr1, G4ThreeVector(cfg.f_Xpos, cfg.f_Ypos, cfg.f_Zpos)), "ir_Beampipe_f_Cone_Phys", f_LogicBeamCone, motherVolume,
                                              false, 0);

        G4VisAttributes *vb1;
        vb1 = new G4VisAttributes(G4Color(0.4, 0.4, 0.5, 1.));
        vb1->SetForceSolid(true);
        f_LogicBeamCone->SetVisAttributes(vb1);

        /*
        double zPos2=(fBeamSizeZCone2/2)+ 25*cm;
        fSolidBeamCone2 = new G4Cons("VTX1pipeCone2",  fBeamSizeRin1Cone, fBeamSizeRout1Cone, fBeamSizeRin2Cone,fBeamSizeRout2Cone,fBeamSizeZCone2/2.,0.,360*deg);
        fLogicBeamCone2 = new G4LogicalVolume(fSolidBeamCone2,  fBeamPipeConeMaterial,  "Beam2Cone");

        brm.rotateY((180.-0.026*180/3.1415)*deg);
        xPos= 2*cm;
        fPhysicsBeamCone2 = new G4PVPlacement(G4Transform3D(brm, G4ThreeVector(xPos,0,-zPos2)), "Beam2Cone",fLogicBeamCone2, Phys, false,  0 );
        vb1->SetForceSolid(true); fLogicBeamCone2->SetVisAttributes(vb1);fLogicBeamCone->SetVisAttributes(vb1);
        */
    };

};

#endif //G4E_IR_BEAMPIPE_HH
