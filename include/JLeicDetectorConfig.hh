//
// Created by yulia on 6/6/19.
//

#ifndef G4E_JLeicDetectorParameters_HH
#define G4E_JLeicDetectorParameters_HH

#include "cb_CTD/cb_CTD.hh"
#include "cb_Solenoid/cb_Solenoid.hh"


struct JLeicDetectorConfig {

    struct WorldConfig {
        double ShiftVTX = 40. * cm;
        double SizeR = 10000. * cm;
        double SizeZ = 20000. * cm;
    } World;



    cb_Solenoid_Config cb_Solenoid;


    /// Central Barrel Vertex definition
    /// short name: cb_VTX

   struct CentralBarrel_Vertex {
        double GVol_RIn = 3.3 * cm;     /// Inner radius
        double GVol_ROut = 20 * cm;     /// Outer radius
        double GVol_SizeZ = 50 * cm;    /// Guess what

        double ladder_deltashi = -7. * deg;
    } cb_VTX;


    cb_CTD_Config cb_CTD;

    struct CentralBarrel_DIRC {

        G4double GVol_RIn;
        G4double GVol_ROut;
        G4double GVol_SizeZ;

    } cb_DIRC;

    struct CentralBarrel_EMCAL {
        double GVol_RIn;
        double GVol_ROut;
        double GVol_ROutshift = 5*cm;
        double GVol_Thickness=40*cm;
        double GVol_SizeZ;
    } cb_EMCAL;

} ;

#endif //G4E_JLeicDetectorParameters_HH
