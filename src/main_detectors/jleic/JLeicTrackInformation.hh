#ifndef JLeicTrackInformation_h
#define JLeicTrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class JLeicTrackInformation : public G4VUserTrackInformation
{
public:

    inline G4int GetLevel() const { return fLevel; }
    inline void  SetLevel(G4int level) { fLevel = level; }

    void *operator new(size_t);
    void operator delete(void *aTrackInfo);

private:
  G4int                 fLevel;
};

#endif

