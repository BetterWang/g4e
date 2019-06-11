//
// Created by yulia on 6/6/19.
//

#ifndef G4E_JLeicDetectorParameters_HH
#define G4E_JLeicDetectorParameters_HH

#include "cb_Solenoid/cb_Solenoid.hh"

#include "cb_VTX/cb_VTX.hh"
#include "cb_CTD/cb_CTD.hh"
#include "cb_DIRC/cb_DIRC.hh"
#include "cb_HCAL/cb_HCAL.hh"


#include "ffe_CPOL/ffe_CPOL.hh"


struct JLeicDetectorConfig {

    struct WorldConfig {
        double ShiftVTX = 40. * cm;
        double SizeR = 10000. * cm;
        double SizeZ = 20000. * cm;
    } World;



    cb_Solenoid_Config cb_Solenoid;

    cb_VTX_Config  cb_VTX;
    cb_CTD_Config  cb_CTD;
    cb_DIRC_Config cb_DIRC;

    cb_HCAL_Config cb_HCAL;

    ffe_CPOL_Config ffe_CPOL;

    struct CentralBarrel_EMCAL {
        double GVol_RIn;
        double GVol_ROut;
        double GVol_ROutshift = 5*cm;
        double GVol_Thickness=40*cm;
        double GVol_SizeZ;
    } cb_EMCAL;

} ;

#endif //G4E_JLeicDetectorParameters_HH
