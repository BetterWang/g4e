//
// Created by yulia on 6/7/19.
//

#ifndef G4E_CB_CTD_HH
#define G4E_CB_CTD_HH


#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"

#include "main_detectors/DetectorConfig.hh"

/// Central Barrel Tracker definition
/// short name: cb_CTD
struct cb_CTD_Config {

    double RIn  = 21 * cm;
    double ROut = 80* cm;
    double SizeZCut=  120 * cm;
 //   double SizeZCut=  60 * cm;
    double SizeZ;
    double SiLayerGap = 5.* cm;
    int    SiLayerCount = 15;
};


class cb_CTD_Design {
public:
    inline void Construct(cb_CTD_Config cfg, G4Material* worldMaterial, G4VPhysicalVolume *motherVolume) {
        printf("Begin cb_CTD volume \n");

        ConstructionConfig=cfg;

        Solid = new G4Tubs("cb_CTD_GVol_Solid", cfg.RIn, cfg.ROut, cfg.SizeZ / 2., 0., 360 * deg);
        Logic = new G4LogicalVolume(Solid, worldMaterial, "cb_CTD_GVol_Logic");
        Phys = new G4PVPlacement(0, G4ThreeVector(), "cb_CTD_GVol_Phys", Logic, motherVolume, false, 0);

        // cb_CTD_GVol_Logic->SetVisAttributes(G4VisAttributes::Invisible);
        G4VisAttributes *visAttr = new G4VisAttributes(G4Color(0.1, 0, 1., 0.1));
        visAttr->SetLineWidth(1);
        visAttr->SetForceSolid(false);
        Logic->SetVisAttributes(visAttr);
    }
//--------------------------Construct Silicon layers --------------------------
    inline void ConstructLadders() {
        using namespace spdlog;
        static char abname[256];
        auto cfg = ConstructionConfig;

        debug("Create CTD  Size Z ={} ",cfg.SizeZ);

        siMaterial =  G4Material::GetMaterial("Si");
        // ------- layers of Si in CTD
        for (int ia = 0; ia < cfg.SiLayerCount; ia++) {
            layerRIn[ia] = cfg.RIn + (cfg.SiLayerGap * ia);
            layerROut[ia] = cfg.RIn + (0.01 + cfg.SiLayerGap * ia);
            trace("cb_CTD_detSi:: Number of layers ={} ",cfg.SiLayerCount);
            if(layerROut[ia] > cfg.ROut) {
                info("cb_CTD_detSi:: Radius of the layer={} is too high \n",ia);
                cfg.SiLayerCount =ia+1;
                ConstructionConfig.SiLayerCount=ia+1;
                trace("cb_CTD_detSi:: Number of layers ={} ",cfg.SiLayerCount);
                continue; }

            trace("cb_CTD_detSi {}  Rout={} \n", ia, layerROut[ia]);
            sprintf(abname, "cb_CTD_detSi_Solid_lay_%d", ia);
            SiSolids[ia] = new G4Tubs(abname, layerRIn[ia], layerROut[ia], cfg.SizeZ / 2., 0., 360 * deg);

            sprintf(abname, "cb_CTD_detSi_Logic_lay_%d", ia);
            SiLogics[ia] = new G4LogicalVolume(SiSolids[ia], siMaterial, abname);

            auto layerVisAttr= new G4VisAttributes(G4Color(1.0 - 0.1 * ia, 1.0, 0.0 + 0.1 * ia, 1));
            layerVisAttr->SetLineWidth(1);
            layerVisAttr->SetForceSolid(true);
            SiLogics[ia]->SetVisAttributes(layerVisAttr);

            sprintf(abname, "cb_CTD_detSi_Phys_lay_%d", ia);
            cb_CTD_detSi_Phys[ia] = new G4PVPlacement(nullptr, G4ThreeVector(),
                                                      abname, SiLogics[ia],
                                                      Phys, false, 0);

            //----> Should be worked out          if (SiLogics[ia]) SiLogics[ia]->SetSensitiveDetector(fCalorimeterSD);
        }
    }
//------------------Straw options -----------------------------------------
    inline void ConstructStraws() {
         using namespace spdlog;
     info("begin STRAW volume ");
          static char abname[256];
          auto cfg = ConstructionConfig;

 int NUMFs[100];
 G4double Stheta,Sphi,phi_s, Sx, Sy, Sz;;
 G4RotationMatrix rm1s;
 G4double  RxF_Straw[100],RyF_Straw[100],RzF_Straw[100];
 G4double CTD_Str_in=0.3, CTD_Str_out=0.31;
 G4VisAttributes* vstr2[100];

 cb_CTD_detStraw_SizeZ=200*cm;
 cb_CTD_detStraw_Layers=20;

 Straw_Material= G4Material::GetMaterial("G4_Galactic");

 Straw_RIn=CTD_Str_in*cm;
 Straw_ROut=CTD_Str_out*cm;
 Straw_SizeZ=cfg.SizeZ;
 sprintf(abname,"cb_CTD_det_Straw_Solid");
 Straw_Solid = new  G4Tubs(abname,0., Straw_ROut,Straw_SizeZ/2.,0.,360*deg);

 sprintf(abname,"cb_CTD_det_Straw_Logic");
 Straw_Logic = new G4LogicalVolume(Straw_Solid,
                                                   Straw_Material, abname);
 G4VisAttributes* vstr= new G4VisAttributes(G4Color(0.5,0.5,0.5,1.));
 vstr->SetLineWidth(1); vstr->SetForceSolid(true);
 Straw_Logic->SetVisAttributes(vstr);

 //-----Walls
 Straw_Wall_Material=G4Material::GetMaterial("Mylar");
 sprintf(abname,"cb_CTD_det_Straw_Wall_Solid");
 Straw_Wall_Solid = new  G4Tubs(abname,0., Straw_ROut,Straw_SizeZ/2.,0.,360*deg);

 sprintf(abname,"cb_CTD_det_Straw_Wall_Logic");
 Straw_Wall_Logic = new G4LogicalVolume(Straw_Wall_Solid,
                                                   Straw_Wall_Material, abname);
sprintf(abname,"cb_CTD_det_Straw_Wall_Phys");
 Straw_Wall_Phys = new G4PVPlacement(0, G4ThreeVector(0,0,0),
                                                  Straw_Wall_Logic, abname,
                                                   Straw_Logic, false,   0 );

 //-----Gas
 Straw_Gas_Material=G4Material::GetMaterial("Ar10CO2");
 sprintf(abname,"cb_CTD_det_Straw_Gas_Solid");
 Straw_Gas_Solid = new  G4Tubs(abname,0., Straw_RIn,Straw_SizeZ/2.,0.,360*deg);

 sprintf(abname,"cb_CTD_det_Straw_Gas_Logic");
 Straw_Gas_Logic = new G4LogicalVolume(Straw_Gas_Solid,
                                                   Straw_Gas_Material, abname);

 G4VisAttributes* vstr1= new G4VisAttributes(G4Color(1.,0.5,0.,1.));
 vstr1->SetLineWidth(1); vstr1->SetForceSolid(true);
 Straw_Gas_Logic->SetVisAttributes(vstr1);

 sprintf(abname,"Phys_CTD_Straw_layer_gas");
 Straw_Gas_Phys = new G4PVPlacement(0, G4ThreeVector(0,0,0),
                                                  Straw_Gas_Logic, abname,
                                                  Straw_Logic, false,   0 );




 //   if (Straw_Gas_Logic)  Straw_Gas_Logic->SetSensitiveDetector(fCalorimeterSD);


       // ------- layers of Straw in CTD
    int counter=0;
    for (int lay=0;lay<cb_CTD_detStraw_Layers;lay++) {

        //      RxF_Straw[lay]=fCTDSizeRin;
        //     printf("Straw X=%f\n",   RxF_Straw[lay]);
       RxF_Straw[lay]=cfg.RIn+30.+(Straw_ROut*2*lay)+lay*20.; // position of first layer
        RyF_Straw[lay]=RxF_Straw[lay]+Straw_ROut/2;
        RzF_Straw[lay]=0*cm;

        // CTD_Straw_Rout[lay]=fCTDSizeRin+(0.01+fCTD_Straw_steps*lay)*cm;
        //      if(lay==0) NUMFs[lay]=213;
        //      if(lay==1) NUMFs[lay]=213;
        NUMFs[lay]=213;
        int j=2*3.1415*RxF_Straw[lay]/2/Straw_ROut+1;
        printf("NLAY=%d %d %f\n",lay, j ,2*3.1415*RxF_Straw[lay]/2/Straw_ROut);
        NUMFs[lay]=j;


        Sphi= (Straw_ROut*2/RxF_Straw[lay]);
        Stheta=0*deg;

        for (int ia=0;ia<NUMFs[lay];ia++) {

          phi_s=(ia*(Sphi));
          Sx=- RxF_Straw[lay]*cos(phi_s);
          Sy=- RyF_Straw[lay]*sin(phi_s);
          Sz= 2*RzF_Straw[lay];
           rm1s.rotateX(Stheta);
           rm1s.rotateZ(0+(Sphi*(ia+1)));

           printf("Straw X=%f (%f,%f,%f \n",    RxF_Straw[lay],Sx,Sy,Sz);

           //       G4VisAttributes* vstr= new G4VisAttributes(G4Color(1,0,1,0.2));

          //sprintf(abname,"Barrel_CTD_Straw_layer_%d_str_%d",lay,ia);
          sprintf(abname,"Barrel_CTD_Straw");
          Straw_Phys = new G4PVPlacement(G4Transform3D(rm1s,G4ThreeVector(Sx,Sy,Sz)),
                                                       abname,
                                                       Straw_Logic,
                                                       Phys,
                                                       false,
                                                       counter);
          counter++;
          /* ------------------------------------------------
            G4PVPlacement(  G4RotationMatrix*       pRot,
                            const G4ThreeVector&    tlate,
                            G4LogicalVolume*        pCurrentLogical,
                            const G4String&         pName,
                            G4LogicalVolume*   pMotherLogical,
                            G4bool             pMany,
                            G4int              pCopyNo,
                            G4bool             pSurfChk=false );
            -------------------------------------------------
            physiHoleSF = new G4PVPlacement(
                         rm3,
                         G4ThreeVector(x,y,z),
                         logicHoleSF,
                         "HoleSF",
                         logicZnScomp,
                         false,
                         counter);
            counter++;
            ------------------------------------------------
          */

        }
        vstr2[lay]= new G4VisAttributes(G4Color(1.0-0.1*lay, 1.0, 0.0+0.1*lay,1));
        vstr2[lay]->SetLineWidth(1); vstr2[lay]->SetForceSolid(true);
        Straw_Logic->SetVisAttributes(vstr2[lay]);
       }


    info("STRAW end  number=%d\n",counter );


    }
        G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical

    /// Parameters that was used in the moment of construction
    cb_CTD_Config ConstructionConfig;

// -----Silicone layers option----------------
    G4Tubs *SiSolids[100];    //pointer to the solid World
    G4LogicalVolume *SiLogics[100];    //pointer to the logical World
    G4VPhysicalVolume *cb_CTD_detSi_Phys[100];    //pointer to the physical World

 //---- Straw tracker options ----------------
   // G4VisAttributes* Straws_Gas;
    G4Material *Straw_Gas_Material;
    G4Tubs *Straw_Gas_Solid;    //pointer to the solid World
    G4LogicalVolume *Straw_Gas_Logic;    //pointer to the logical World
    G4VPhysicalVolume *Straw_Gas_Phys;    //pointer to the physical World

private:
    G4Material *siMaterial;
    G4double layerRIn[100];
    G4double layerROut[100];

//-------------straw---------------
 //-------------------CTD_Si barrel detector ------------------
     //-------------------CTD_Straw barrel detector ------------------
    G4double cb_CTD_detStraw_RIn;
    G4double cb_CTD_detStraw_ROut;
    G4double cb_CTD_detStraw_SizeZ;
    G4double cb_CTD_detStraw_Steps;
    G4Material *Straw_Material;
    G4int cb_CTD_detStraw_Layers;
    G4double Straw_RIn;
    G4double Straw_ROut;
    G4double Straw_SizeZ;
    G4VisAttributes* attr_cb_CTD_Straws;
    G4Tubs *Straw_Solid;    //pointer to the solid World
    G4LogicalVolume *Straw_Logic;    //pointer to the logical World
    G4VPhysicalVolume *Straw_Phys;    //pointer to the physical World

    G4Material *Straw_Wall_Material;
    G4Tubs *Straw_Wall_Solid;    //pointer to the solid World
    G4LogicalVolume *Straw_Wall_Logic;    //pointer to the logical World
    G4VPhysicalVolume *Straw_Wall_Phys;    //pointer to the physical World



};

#endif //G4E_CB_CTD_HH
