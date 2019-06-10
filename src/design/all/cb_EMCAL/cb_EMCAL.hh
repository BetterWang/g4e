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

    inline void Create(JLeicDetectorParameters& jleicParams,
            G4VPhysicalVolume *physicalVolume,
            G4LogicalVolume *logicalVolume
            ) {
        static char abname[256];

        auto p = jleicParams.cb_EMCAL;
        //........................EMCAL Barrel detector----------------------------------------------

        // Setting material
        cb_EMCAL_det_Material = fMat->GetMaterial("PbWO4");
        logicalVolume->SetMaterial(cb_EMCAL_det_Material);

        // Visualising it differently
        auto visualAttributes = new G4VisAttributes(G4Color(0.3, 0.5, 0.9, 1.));
        visualAttributes->SetLineWidth(1);
        visualAttributes->SetForceSolid(true);
        logicalVolume->SetVisAttributes(visualAttributes);
        //    fPhysicsEMCAL = new G4PVPlacement(0, G4ThreeVector(0,0,-40*cm), "EMCALbSolPhys",fLogicEMCAL,
        //                               cb_EMCAL_GVol_Phys, false,     0 );

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
