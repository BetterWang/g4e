//
// Created by yulia on 6/6/19.
//

#ifndef G4E_CB_VTX_HH
#define G4E_CB_VTX_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "JLeicDetectorConfig.hh"

typedef struct {
    double Dx;
    double Dy;
    double Dz;
    double Rin;
} cb_VTX_ladder_LayParam;

struct cb_VTX_Config {

    double RIn = 3.3 * cm;     /// Inner radius
    double ROut = 20 * cm;     /// Outer radius
    double SizeZ = 50 * cm;    /// Guess what
    double ShiftZ;
    double ladder_deltashi = -7. * deg;


};


class cb_VTX_Design {
public:
    inline void Construct(cb_VTX_Config cfg,G4Material* worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin cb_CTD volume \n");

        ConstructionConfig = cfg;

        printf("Begin cb_VERTEX volume \n");

        Solid = new G4Tubs("cb_VTX_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "cb_VTX_GVol_Logic");
        Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, cfg.ShiftZ), "cb_VTX_GVol_Phys", Logic,
                                            motherVolume, false, 0);

        // cb_VTX_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);
        G4VisAttributes *attr_cb_VTX = new G4VisAttributes(G4Color(0.1, 0, 1., 0.1));
        attr_cb_VTX->SetLineWidth(1);
        attr_cb_VTX->SetForceSolid(false);
        Logic->SetVisAttributes(attr_cb_VTX);





    }
    inline void ConstructLadders() {
        static char abname[256];
        auto cfg = ConstructionConfig;

        //--------------------------------------------------
        //----------vtx barrel ladder geometry--------------
        //--------------------------------------------------
        //===================================================================================

        G4RotationMatrix rm[10][40];
        // deltaphi1 = 0;

        double x, y, z;
        z = 0 * cm;
        //phi=26.*deg; x=0; y=0; z=fAbsorberZ;
        //phi=0.*deg; x=0; y=0; z=fAbsorberZ;

        int FDIV = 0;
        double dR;
        double myL;
        double phi=0;

        // Materials are defined in JLeicMaterials
        // TODO calling global stuff like that kills kittens, so think about it later!
        cb_VTX_ladder_Material = G4Material::GetMaterial("Si");

        std::vector <cb_VTX_ladder_LayParam> Lays;
        cb_VTX_ladder_LayParam Lay;
        // Lay 0
        Lay.Dx=0.050 * mm; Lay.Dy=2*cm; Lay.Dz=10*cm; Lay.Rin=3.5 * cm; Lays.push_back(Lay);
        // Lay 1
        Lay.Dx=0.050 * mm; Lay.Dy=2*cm; Lay.Dz=11*cm; Lay.Rin=4.5 * cm; Lays.push_back(Lay);
        // Lay 2
        Lay.Dx=0.050 * mm; Lay.Dy=4*cm; Lay.Dz=18*cm; Lay.Rin=6.5 * cm; Lays.push_back(Lay);
        // Lay 3
        Lay.Dx=0.050 * mm; Lay.Dy=4*cm; Lay.Dz=24*cm; Lay.Rin=10.5 * cm; Lays.push_back(Lay);
        // Lay 4
        Lay.Dx=0.050 * mm; Lay.Dy=4*cm; Lay.Dz=36*cm; Lay.Rin=13.5 * cm; Lays.push_back(Lay);
        // Lay 5
        Lay.Dx=0.050 * mm; Lay.Dy=4*cm; Lay.Dz=48*cm; Lay.Rin=15.5 * cm; Lays.push_back(Lay);

        if( Lays.size()>10) {printf("Nlayers in VERTEX >10 !!! \n"); exit(1); }

        for (uint lay = 0; lay < Lays.size(); lay++) {

            printf("cb_VTX_ladder:: Layer loop:: %d\n", lay);
            double cb_VTX_ladder_DZ = Lays[lay].Dz;
            double cb_VTX_ladder_DY = Lays[lay].Dy;
            double cb_VTX_ladder_Thickness = Lays[lay].Dx;
            dR =  Lays[lay].Rin;

            myL = 2*3.1415*dR;
            int NUM = myL/cb_VTX_ladder_DY;

            for(int i=0;i<2; i++){
                double LN = cb_VTX_ladder_DY * NUM;
                double LN1 = cb_VTX_ladder_DY * (NUM+1+i);
                printf("cb_VTX_ladder:: LN= Orig NUM=%d\n",NUM);
                if (LN/LN1>0.8) NUM=NUM+1;
                printf("cb_VTX_ladder:: LN=%f, LN1=%f  delenie=%f NUM=%d \n",LN,LN1,LN/LN1,NUM);
            }

            double cb_VTX_ladder_deltaphi = 2*3.1415926/NUM  ;


            sprintf(abname, "cb_VTX_ladder_Solid_%d", lay);
            cb_VTX_ladder_Solid[lay] = new G4Box(abname, cb_VTX_ladder_Thickness / 2.,   cb_VTX_ladder_DY / 2.,cb_VTX_ladder_DZ / 2.  );

            sprintf(abname, "cb_VTX_ladder_Logic_%d", lay);
            cb_VTX_ladder_Logic[lay] = new G4LogicalVolume(cb_VTX_ladder_Solid[lay], cb_VTX_ladder_Material,
                                                           abname);


            if (lay == 0 || lay == 1) { attr_cb_VTX_ladder = new G4VisAttributes(G4Color(0.0, 0.2, 0.8, 2.0)); }
            else if (lay == 2) { attr_cb_VTX_ladder = new G4VisAttributes(G4Color(0.0, 0.2, 0.8, 0.7)); }
            else {
                attr_cb_VTX_ladder = new G4VisAttributes(G4Color(0.0 + 0.1 * double(lay - 3), 1., 1. - 0.1 * double(lay - 3), 1.0));
            }
            attr_cb_VTX_ladder->SetForceSolid(true);
            cb_VTX_ladder_Logic[lay]->SetVisAttributes(attr_cb_VTX_ladder);

            if( NUM>40) {
                printf("cb_VTX_ladder:: Nladders in VERTEX >40 lay=%f !!! \n",lay);
                exit(1);
            }


            for (int ia = 0; ia < NUM; ia++) {
                //for (int ia=0;ia<1;ia++) {
                printf("cb_VTX_ladder:: lay=%d  NUM=%d, dR=%f cb_VTX_ladder_deltaphi=%f %f \n",lay, NUM,  dR, cb_VTX_ladder_deltaphi,cfg.ladder_deltashi);
                printf("cb_VTX_ladder:: Module  loop:: %d\n", ia);

                phi = (ia * (cb_VTX_ladder_deltaphi));
                x = - dR * cos(phi) ;
                y = - dR * sin(phi) ;
                rm[lay][ia].rotateZ(cb_VTX_ladder_deltaphi * ia);
                rm[lay][ia].rotateZ(cfg.ladder_deltashi);

                printf("cb_VTX_ladder::  %d %d x=%f  y=%f  \n", lay, ia, x, y);
                sprintf(abname, "cb_VTX_ladder_Phys_%d_%d", lay, ia);
                cb_VTX_ladder_Phys[lay] = new G4PVPlacement(G4Transform3D(rm[lay][ia], G4ThreeVector(x, y, z)),
                                                            abname, cb_VTX_ladder_Logic[lay],
                                                            Phys, false, 0.);
            }
            //-------------------------------------------------------------------------
            //                          VTX  slices and pixels
            //-------------------------------------------------------------------------
            G4Box *pxdBox_slice[10];
            G4Box *pxdBox_pixel[10];
            G4double PixelDX, PixelDY;
            if (lay < 2) {
                PixelDX = cb_VTX_ladder_DZ / 10.; //2000.*um;
                PixelDY = cb_VTX_ladder_DY / 50.; //2000.*um;
            } else {
                PixelDX = cb_VTX_ladder_DZ / 50.; //2000.*um;
                PixelDY = cb_VTX_ladder_DY / 10.; //2000.*um;

            }
            //G4double PixelDX=20.*um;
            //G4double PixelDY=20.*um;
            //G4double PixelDX=24.*um;
            //G4double PixelDY=24.*um;
            G4double PixelDZ = cb_VTX_ladder_Thickness; // 0.450*mm

            if (FDIV >= 1) {
                printf("cb_VTX_ladder_pxdSlice_:: construct slices %d \n", lay);

                sprintf(abname, "cb_VTX_ladder_pxdSlice_%d", lay);
                pxdBox_slice[lay] = new G4Box(abname,
                                              PixelDX / 2,                   //gD->GetPixelDX(),
                                              cb_VTX_ladder_DY / 2., // 10.*mm,  //gD->GetHalfMPXWaferDY(),
                                              cb_VTX_ladder_Thickness / 2.);    //gD->GetHalfMPXWaferDZ());

                pxdSlice_log[lay] = new G4LogicalVolume(pxdBox_slice[lay], cb_VTX_ladder_Material, abname, 0, 0, 0);

                G4VisAttributes *pixelVisAtt = new G4VisAttributes(G4Color(0, 1, 1, 1));
                pixelVisAtt->SetLineWidth(1);
                pixelVisAtt->SetForceWireframe(true);
                pxdSlice_log[lay]->SetVisAttributes(pixelVisAtt);


                // divide in slices
                sprintf(abname, "pxdSlice_%d", lay);
                G4PVDivision *sliceDiv = new G4PVDivision(abname,
                                                          pxdSlice_log[lay],
                                                          cb_VTX_ladder_Logic[lay],
                                                          kXAxis,
                                                          PixelDX,
                                                          0);
                printf("SetUpVertex16():: construct done\n");


                if (FDIV >= 2) {
                    printf("SetUpVertex16():: construct pixels \n");
                    if (lay < 2) { sprintf(abname, "pxdPixel"); }
                    else {
                        sprintf(abname, "svdPixel");
                    }

                    //sprintf(abname,"pxdPixel_%d",lay);
                    pxdBox_pixel[lay] = new G4Box(abname,
                                                  PixelDX / 2,
                                                  PixelDY / 2.,
                                                  PixelDZ / 2.);
                    pxdPixel_log[lay] = new G4LogicalVolume(pxdBox_pixel[lay], cb_VTX_ladder_Material, abname, 0, 0, 0);
                    pxdPixel_log[lay]->SetVisAttributes(pixelVisAtt);

                    // divide in pixels
                    G4PVDivision *pixelDiv = new G4PVDivision(abname,
                                                              pxdPixel_log[lay],
                                                              pxdSlice_log[lay],
                                                              kYAxis,
                                                              PixelDY,
                                                              0);
                } //-- end if pixel division
            } //-- end if slices division

            //	 };  // -- end loop over modules

        }; // --- end loop over layers
    }

    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    cb_VTX_Config ConstructionConfig;


private:
    JLeicMaterials *fMat;
    G4VisAttributes *attr_cb_VTX_ladder;
    G4Material *cb_VTX_ladder_Material;

    //--------------- VTX ladders geom-------
    G4Box *cb_VTX_ladder_Solid[10]; //pointer to the solid Absorber
    G4LogicalVolume *cb_VTX_ladder_Logic[10]; //pointer to the logical Absorber
    G4VPhysicalVolume *cb_VTX_ladder_Phys[10]; //pointer to the logical Absorber

    //-- slices and pixels

    G4LogicalVolume *pxdSlice_log[10]; //pointer to the logical slice
    G4LogicalVolume *pxdPixel_log[10]; //pointer to the logical pixel



};

#endif //G4E_CB_VTX_HH
