#ifndef JLeicCalorHit_h
#define JLeicCalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class JLeicCalorimeterHit : public G4VHit
{
public:

    JLeicCalorimeterHit();

    ~JLeicCalorimeterHit() override;

    JLeicCalorimeterHit(const JLeicCalorimeterHit &);

    void *operator new(size_t);

    void operator delete(void *);

    const JLeicCalorimeterHit &operator=(const JLeicCalorimeterHit &);

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


typedef G4THitsCollection<JLeicCalorimeterHit> JLeicCalorHitsCollection;

extern G4ThreadLocal G4Allocator<JLeicCalorimeterHit> *JLeicCalorHitAllocator;


inline void *JLeicCalorimeterHit::operator new(size_t)
{
    void *aHit;
    if (!JLeicCalorHitAllocator) JLeicCalorHitAllocator = new G4Allocator<JLeicCalorimeterHit>();
    aHit = (void *) JLeicCalorHitAllocator->MallocSingle();
    return aHit;
}


inline void JLeicCalorimeterHit::operator delete(void *aHit)
{
    if (!JLeicCalorHitAllocator) JLeicCalorHitAllocator = new G4Allocator<JLeicCalorimeterHit>();
    JLeicCalorHitAllocator->FreeSingle((JLeicCalorimeterHit *) aHit);
}

#endif


