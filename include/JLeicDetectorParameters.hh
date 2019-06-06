//
// Created by yulia on 6/6/19.
//

#ifndef G4E_JLeicDetectorParameters_HH
#define G4E_JLeicDetectorParameters_HH

#include "G4SystemOfUnits.hh"

struct JLeicDetectorParameters {

    /// Central Barrel Vertex definition
    /// short name: cb_VTX
    struct CentralBarrel_Vertex {
        double GVol_RIn = 3.3 * cm;     /// Inner radius
        double GVol_ROut = 20 * cm;     /// Outer radius
        double GVol_SizeZ = 50 * cm;    /// Guess what

        double ladder_deltashi = -7. * deg;
    } cb_VTX;

};

#endif //G4E_JLeicDetectorParameters_HH
