#include "JLeicCalorHit.hh"

// Using G4ThreadLocal external JLeicCalorHitAllocator as was suggested in
//
G4ThreadLocal G4Allocator<JLeicCalorHit>* JLeicCalorHitAllocator = nullptr;


JLeicCalorHit::JLeicCalorHit()
{
    fEdepAbs = 0.;
    fTrackLengthAbs = 0.;
    fEdepGap = 0.;
    fTrackLengthGap = 0.;
}

JLeicCalorHit::~JLeicCalorHit() = default;


/*
JLeicCalorHit::JLeicCalorHit(const JLeicCalorHit& right)
{
  EdepAbs = right.EdepAbs; TrackLengthAbs = right.TrackLengthAbs;
  EdepGap = right.EdepGap; TrackLengthGap = right.TrackLengthGap;
}
*/


const JLeicCalorHit& JLeicCalorHit::operator=(const JLeicCalorHit& right)
{
    fEdepAbs = right.fEdepAbs;
    fTrackLengthAbs = right.fTrackLengthAbs;

    fEdepGap = right.fEdepGap;
    fTrackLengthGap = right.fTrackLengthGap;
  return *this;
}



void JLeicCalorHit::Print()
{;}



