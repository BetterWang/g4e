//
// Created by yulia on 6/14/19.
//

#ifndef G4E_CI_TRD_CONFIG_HH
#define G4E_CI_TRD_CONFIG_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"


struct ci_TRD_Config
{
// define here Global volume parameters
    double RIn = 20 * cm;
    double ROut = 200 * cm;
    double ThicknessZ = 30 * cm;
    double PosZ;
    G4double fRadZ;
    //----------------------------
    double fGasGap = 0.600 * mm;    // for ZEUS  300-publication
    double fRadThick = NAN;
    int fFoilNumber = 0;
    //----------------------------
    double det_RIn = 20 * cm;
    double det_ROut = 100 * cm;
    double det_ThicknessZ = 2.5 * cm;
    double det_PosZ;
    G4double fDetThickness;
    G4double fDetLength;

    double fAbsorberThickness = 0.050 * mm;
    double fAbsorberRadius = 100. * mm;
    double fAbsorberZ = 136. * cm;
    double fDetGap = 0.01 * mm;
    int fModuleNumber = 1;
    G4Material *fRadiatorMat;        // pointer to the mixed TR radiator material
    G4Material *det_Material;

    G4double fRadThickness = 0.020 * mm;    // 16 um // ZEUS NIMA 323 (1992) 135-139, D=20um, dens.= 0.1 g/cm3
};



#endif //G4E_CI_TRD_CONFIG_HH
