#include "CommonVertexHit.hh"

G4Allocator<CommonVertexHit> JLeicVTXHitAllocator;

CommonVertexHit::CommonVertexHit()
{
    fEdepAbs = 0.; fTrackLengthAbs = 0.;
    fEdepGap = 0.; fTrackLengthGap = 0.;
}

CommonVertexHit::~CommonVertexHit()
{

}


void CommonVertexHit::Print()
{
    
}


