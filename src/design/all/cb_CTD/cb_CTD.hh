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

#include "JLeicDetectorParameters.hh"


class CentralBarrelTracker {
public:
    explicit CentralBarrelTracker()
    {


    }

    inline void Create(JLeicDetectorParameters& jleicParams, G4VPhysicalVolume *physicalVolume) {
        static char abname[256];
        auto p = jleicParams.cb_CTD;

        double cb_CTD_detSi_SizeZ = p.GVol_SizeZ;
        printf("Create CTD  Size Z =%f \n ",cb_CTD_detSi_SizeZ);
        p.detSi_Steps = 5.;
        p.detSi_Layers = 15;
        cb_CTD_detSi_Material =  G4Material::GetMaterial("Si");
        // ------- layers of Si in CTD
        for (int ia = 0; ia < cb_CTD_detSi_Layers; ia++) {
            cb_CTD_detSi_lay_Rin[ia] = p.GVol_RIn + (cb_CTD_detSi_Steps * ia) * cm;
            cb_CTD_detSi_lay_Rout[ia] = p.GVol_RIn + (0.01 + cb_CTD_detSi_Steps * ia) * cm;
            if(   cb_CTD_detSi_lay_Rout[ia] >  p.GVol_ROut ) continue;
            printf("cb_CTD_detSi %d  Rout=%f \n", ia, cb_CTD_detSi_lay_Rout[ia]);

            sprintf(abname, "cb_CTD_detSi_Solid_lay_%d", ia);
            cb_CTD_detSi_Solid[ia] = new G4Tubs(abname, cb_CTD_detSi_lay_Rin[ia], cb_CTD_detSi_lay_Rout[ia], cb_CTD_detSi_SizeZ / 2., 0.,
                                                360 * deg);

            sprintf(abname, "cb_CTD_detSi_Logic_lay_%d", ia);
            cb_CTD_detSi_Logic[ia] = new G4LogicalVolume(cb_CTD_detSi_Solid[ia],
                                                         cb_CTD_detSi_Material, abname);

            //   attr_cb_CTD_det    = new G4VisAttributes(G4Color(1,0,1,0.2));
            attr_cb_CTD_det= new G4VisAttributes(G4Color(1.0 - 0.1 * ia, 1.0, 0.0 + 0.1 * ia, 1));
            attr_cb_CTD_det->SetLineWidth(1);
            attr_cb_CTD_det->SetForceSolid(true);
            cb_CTD_detSi_Logic[ia]->SetVisAttributes(attr_cb_CTD_det);

            sprintf(abname, "cb_CTD_detSi_Phys_lay_%d", ia);
            cb_CTD_detSi_Phys[ia] = new G4PVPlacement(0, G4ThreeVector(),
                                                      abname, cb_CTD_detSi_Logic[ia],
                                                      physicalVolume, false, 0.);

            //----> Should be worked out          if (cb_CTD_detSi_Logic[ia]) cb_CTD_detSi_Logic[ia]->SetSensitiveDetector(fCalorimeterSD);
        }
    }
private:
    JLeicMaterials *fMat;
    G4int cb_CTD_detSi_Layers;
    G4Material *cb_CTD_detSi_Material;
    G4VisAttributes *attr_cb_CTD_det;
    G4double cb_CTD_detSi_lay_Rin[100];
    G4double cb_CTD_detSi_lay_Rout[100];
    G4Tubs *cb_CTD_detSi_Solid[100];    //pointer to the solid World
    G4LogicalVolume *cb_CTD_detSi_Logic[100];    //pointer to the logical World
    G4VPhysicalVolume *cb_CTD_detSi_Phys[100];    //pointer to the physical World

    G4double cb_CTD_detSi_RIn;
    G4double cb_CTD_detSi_ROut;
    G4double cb_CTD_detSi_SizeZ;
    G4double cb_CTD_detSi_Steps;

};

#endif //G4E_CB_CTD_HH
