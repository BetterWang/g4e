#include "ce_EMCAL_Hit.hh"

// Using G4ThreadLocal external JLeicCe_emcalHitAllocator as was suggested in
//
G4ThreadLocal G4Allocator<ce_EMCAL_Hit>* JLeicCe_emcalHitAllocator = nullptr;


ce_EMCAL_Hit::ce_EMCAL_Hit()
{



   fEdepAbs = 0.;
    fTrackLengthAbs = 0.;
    fEdepGap = 0.;
    fTrackLengthGap = 0.;
}

ce_EMCAL_Hit::~ce_EMCAL_Hit() = default;

//JLeicCe_emcalHit::~JLeicCe_emcalHit(){;}
/*
JLeicCe_emcalHit::JLeicCe_emcalHit(const JLeicCe_emcalHit& right)
{
  EdepAbs = right.EdepAbs; TrackLengthAbs = right.TrackLengthAbs;
  EdepGap = right.EdepGap; TrackLengthGap = right.TrackLengthGap;
}
*/


const ce_EMCAL_Hit& ce_EMCAL_Hit::operator=(const ce_EMCAL_Hit& right)
{
    fEdepAbs = right.fEdepAbs;
    fTrackLengthAbs = right.fTrackLengthAbs;

    fEdepGap = right.fEdepGap;
    fTrackLengthGap = right.fTrackLengthGap;
  return *this;
}



void ce_EMCAL_Hit::Print()
{;}




