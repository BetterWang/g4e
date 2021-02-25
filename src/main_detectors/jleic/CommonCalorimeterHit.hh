#ifndef JLeicCalorHit_h
#define JLeicCalorHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class CommonCalorimeterHit : public G4VHit
{
public:

    CommonCalorimeterHit();

    ~CommonCalorimeterHit() override;

    CommonCalorimeterHit(const CommonCalorimeterHit &);

    void *operator new(size_t);

    void operator delete(void *);


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


typedef G4THitsCollection<CommonCalorimeterHit> JLeicCalorHitsCollection;

extern G4ThreadLocal G4Allocator<CommonCalorimeterHit> *JLeicCalorHitAllocator;


inline void *CommonCalorimeterHit::operator new(size_t)
{
    void *aHit;
    if (!JLeicCalorHitAllocator) JLeicCalorHitAllocator = new G4Allocator<CommonCalorimeterHit>();
    aHit = (void *) JLeicCalorHitAllocator->MallocSingle();
    return aHit;
}


inline void CommonCalorimeterHit::operator delete(void *aHit)
{
    if (!JLeicCalorHitAllocator) JLeicCalorHitAllocator = new G4Allocator<CommonCalorimeterHit>();
    JLeicCalorHitAllocator->FreeSingle((CommonCalorimeterHit *) aHit);
}

#endif


