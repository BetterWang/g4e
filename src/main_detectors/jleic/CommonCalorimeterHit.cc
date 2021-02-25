#include "CommonCalorimeterHit.hh"

// Using G4ThreadLocal external JLeicCalorHitAllocator as was suggested in
//
G4ThreadLocal G4Allocator<CommonCalorimeterHit>* JLeicCalorHitAllocator = nullptr;


CommonCalorimeterHit::CommonCalorimeterHit()
{
    fEdepAbs = 0.;
    fTrackLengthAbs = 0.;
    fEdepGap = 0.;
    fTrackLengthGap = 0.;
}

CommonCalorimeterHit::~CommonCalorimeterHit() = default;




