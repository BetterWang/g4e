#include "JLeicZDCHit.hh"

// Using G4ThreadLocal external JLeicZDCHitAllocator as was suggested in
//
G4ThreadLocal G4Allocator<JLeicZDCHit>* JLeicZDCHitAllocator = nullptr;


JLeicZDCHit::JLeicZDCHit()
{
//    fEdepAbs = 0.;
//    fTrackLengthAbs = 0.;
//    fEdepGap = 0.;
//    fTrackLengthGap = 0.;
}

JLeicZDCHit::~JLeicZDCHit() = default;


/*
JLeicZDCHit::JLeicZDCHit(const JLeicZDCHit& right)
{
  EdepAbs = right.EdepAbs; TrackLengthAbs = right.TrackLengthAbs;
  EdepGap = right.EdepGap; TrackLengthGap = right.TrackLengthGap;
}
*/


//const JLeicZDCHit& JLeicZDCHit::operator=(const JLeicZDCHit& right)
//{
//    fEdepAbs = right.fEdepAbs;
//    fTrackLengthAbs = right.fTrackLengthAbs;
//
//    fEdepGap = right.fEdepGap;
//    fTrackLengthGap = right.fTrackLengthGap;
//    return *this;
//}

//void JLeicZDCHit::Print()
//{;}

