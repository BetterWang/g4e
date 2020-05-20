#ifndef JLeicZDCHit_h
#define JLeicZDCHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class JLeicZDCHit : public G4VHit
{
public:

    JLeicZDCHit();

    ~JLeicZDCHit() override;

    JLeicZDCHit(const JLeicZDCHit &);

    void *operator new(size_t);

    void operator delete(void *);

//    const JLeicZDCHit &operator=(const JLeicZDCHit &);

//    void Print();

public:

private:
    double xpos, ypos, zpos;
    double eloss, stepl;
    int charge;
    int copy;
};


typedef G4THitsCollection<JLeicZDCHit> JLeicZDCHitsCollection;

extern G4ThreadLocal G4Allocator<JLeicZDCHit> *JLeicZDCHitAllocator;


inline void *JLeicZDCHit::operator new(size_t)
{
    void *aHit;
    if (!JLeicZDCHitAllocator) JLeicZDCHitAllocator = new G4Allocator<JLeicZDCHit>();
    aHit = (void *) JLeicZDCHitAllocator->MallocSingle();
    return aHit;
}


inline void JLeicZDCHit::operator delete(void *aHit)
{
    if (!JLeicZDCHitAllocator) JLeicZDCHitAllocator = new G4Allocator<JLeicZDCHit>();
    JLeicZDCHitAllocator->FreeSingle((JLeicZDCHit *) aHit);
}

#endif


