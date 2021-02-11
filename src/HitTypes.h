#ifndef G4E_HITTYPES_H
#define G4E_HITTYPES_H

namespace g4e {
    enum class HitTypes{
        Sensitive = 0,      // Hit is saved from a sensitive detector
        VolumeEnter = 1,    // Hit is saved because it is the first step in volume
        VolumeLeave = 2     // Hit is saved because it is the last step in volume
    };
}

#endif //G4E_HITTYPES_H
