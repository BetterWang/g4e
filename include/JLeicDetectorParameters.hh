//
// Created by yulia on 6/6/19.
//

#ifndef G4E_JLeicDetectorParameters_HH
#define G4E_JLeicDetectorParameters_HH

#include "G4SystemOfUnits.hh"

struct JLeicDetectorParameters {

     double World_ShiftVTX = 40. * cm;

    double Solenoid_SizeZ = 400. * cm;
    double Solenoid_ROut = 144. * cm;
    double Solenoid_RIn = 0.0 * cm;


    /// Central Barrel Vertex definition
    /// short name: cb_VTX

   struct CentralBarrel_Vertex {
        double GVol_RIn = 3.3 * cm;     /// Inner radius
        double GVol_ROut = 20 * cm;     /// Outer radius
        double GVol_SizeZ = 50 * cm;    /// Guess what

        double ladder_deltashi = -7. * deg;
    } cb_VTX;


    /// Central Barrel Tracker definition
    /// short name: cb_CTD
    struct CentralBarrel_Tracker {

        double GVol_RIn  = 21 * cm;
        double GVol_ROut = 80* cm;
      //  double GVol_SizeZ= Solenoid_SizeZ - 60 * cm;
        double GVol_SizeZcut=  60 * cm;
        double GVol_SizeZ;
        double detSi_Steps = 5.;
        int    detSi_Layers = 15;

    } cb_CTD;

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
