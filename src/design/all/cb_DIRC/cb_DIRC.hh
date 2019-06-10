//
// Created by yulia on 6/7/19.
//

#ifndef G4E_CB_DIRC_HH
#define G4E_CB_DIRC_HH

#include <G4PVDivision.hh>
#include "G4RotationMatrix.hh"
#include "G4Material.hh"
#include "G4Color.hh"
#include "G4VisAttributes.hh"

#include "JLeicDetectorConfig.hh"

class CentralBarrelDIRC {
public:
    explicit CentralBarrelDIRC() {


    }

    inline void Create(JLeicDetectorConfig &jleicParams, G4VPhysicalVolume *physicalVolume) {
        static char abname[256];
        auto p = jleicParams.cb_DIRC;


    }

private:


};

#endif //G4E_CB_DIRC_HH
