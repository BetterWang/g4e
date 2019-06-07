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

#include "JLeicDetectorParameters.hh"
class CentralBarrelEMCAL {
public:

    explicit CentralBarrelEMCAL()
    {


    }

    inline void Create(JLeicDetectorParameters& jleicParams, G4VPhysicalVolume *physicalVolume) {
        static char abname[256];

        auto p = jleicParams.cb_EMCAL;

        //........................EMCAL Barrel detector----------------------------------------------

        cb_EMCAL_det_Material = fMat->GetMaterial("PbWO4");
        // fSolidEMCAL = new G4Tubs("EMCALbSol",  cb_EMCAL_GVol_RIn,cb_EMCAL_GVol_ROut,cb_EMCAL_GVol_SizeZ/2.,0.,360*deg);
        cb_EMCAL_det_Solid = new G4Polycone("cb_EMCAL_det_Solid", 0. * deg, 360. * deg, 4, p.GVol_ConeZ, p.GVol_ConeRIn,
                                            p.GVol_ConeROut);

        cb_EMCAL_det_Logic = new G4LogicalVolume(cb_EMCAL_det_Solid, cb_EMCAL_det_Material, "cb_EMCAL_det_Logic");

        // G4VisAttributes* attr_cb_EMCAL= new G4VisAttributes(G4Color(0.3,0.5,0.9,0.9));
        //  G4VisAttributes* attr_cb_EMCAL= new G4VisAttributes(G4Color(0.7,0.7,0.7,1.));
        attr_cb_EMCAL->SetLineWidth(1);
        attr_cb_EMCAL->SetForceSolid(true);
        cb_EMCAL_det_Logic->SetVisAttributes(attr_cb_EMCAL);

        //    fPhysicsEMCAL = new G4PVPlacement(0, G4ThreeVector(0,0,-40*cm), "EMCALbSolPhys",fLogicEMCAL,
        //                               cb_EMCAL_GVol_Phys, false,     0 );

        cb_EMCAL_det_Phys = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "cb_EMCAL_det_Phys", cb_EMCAL_det_Logic,
                                              physicalVolume, false, 0);

//...............................detector----------------------------------------------

        //--------------EMCAL barrel detector----------------------


    }

    inline void CreateLadders() {

    }
private:

    JLeicMaterials *fMat;
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
