#ifndef JLeicCalorHit_h
#define JLeicCalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class JLeicCalorHit : public G4VHit
{
public:

    JLeicCalorHit();

    ~JLeicCalorHit() override;

    JLeicCalorHit(const JLeicCalorHit &);

    void *operator new(size_t);

    void operator delete(void *);

    const JLeicCalorHit &operator=(const JLeicCalorHit &);

    void Print();

public:

    void AddAbs(G4double de, G4double dl)
    {
        fEdepAbs += de;
        fTrackLengthAbs += dl;
    };

    void AddGap(G4double de, G4double dl)
    {
        fEdepGap += de;
        fTrackLengthGap += dl;
    };

    G4double GetEdepAbs() { return fEdepAbs; };

    G4double GetTrakAbs() { return fTrackLengthAbs; };

    G4double GetEdepGap() { return fEdepGap; };

    G4double GetTrakGap() { return fTrackLengthGap; };

private:

    G4double fEdepAbs, fTrackLengthAbs;
    G4double fEdepGap, fTrackLengthGap;
};


typedef G4THitsCollection<JLeicCalorHit> JLeicCalorHitsCollection;

extern G4ThreadLocal G4Allocator<JLeicCalorHit> *JLeicCalorHitAllocator;


inline void *JLeicCalorHit::operator new(size_t)
{
    void *aHit;
    if (!JLeicCalorHitAllocator) JLeicCalorHitAllocator = new G4Allocator<JLeicCalorHit>();
    aHit = (void *) JLeicCalorHitAllocator->MallocSingle();
    return aHit;
}


inline void JLeicCalorHit::operator delete(void *aHit)
{
    if (!JLeicCalorHitAllocator) JLeicCalorHitAllocator = new G4Allocator<JLeicCalorHit>();
    JLeicCalorHitAllocator->FreeSingle((JLeicCalorHit *) aHit);
}

#endif


