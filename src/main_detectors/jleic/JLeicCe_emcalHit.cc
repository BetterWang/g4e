#include "JLeicCe_emcalHit.hh"

// Using G4ThreadLocal external JLeicCe_emcalHitAllocator as was suggested in
//
G4ThreadLocal G4Allocator<JLeicCe_emcalHit>* JLeicCe_emcalHitAllocator = nullptr;


JLeicCe_emcalHit::JLeicCe_emcalHit()
{



   fEdepAbs = 0.;
    fTrackLengthAbs = 0.;
    fEdepGap = 0.;
    fTrackLengthGap = 0.;
}

JLeicCe_emcalHit::~JLeicCe_emcalHit() = default;

//JLeicCe_emcalHit::~JLeicCe_emcalHit(){;}
/*
JLeicCe_emcalHit::JLeicCe_emcalHit(const JLeicCe_emcalHit& right)
{
  EdepAbs = right.EdepAbs; TrackLengthAbs = right.TrackLengthAbs;
  EdepGap = right.EdepGap; TrackLengthGap = right.TrackLengthGap;
}
*/


const JLeicCe_emcalHit& JLeicCe_emcalHit::operator=(const JLeicCe_emcalHit& right)
{
    fEdepAbs = right.fEdepAbs;
    fTrackLengthAbs = right.fTrackLengthAbs;

    fEdepGap = right.fEdepGap;
    fTrackLengthGap = right.fTrackLengthGap;
  return *this;
}



void JLeicCe_emcalHit::Print()
{;}




