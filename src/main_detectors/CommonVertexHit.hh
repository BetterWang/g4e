#ifndef JLeicVTXHit_h
#define JLeicVTXHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class CommonVertexHit : public G4VHit
{
public:

  CommonVertexHit();
  ~CommonVertexHit();
  CommonVertexHit(const CommonVertexHit&);

  void* operator new(size_t);
  void  operator delete(void*);

  const CommonVertexHit& operator=(const CommonVertexHit&);

  void Print();
      
public:
  
  void AddAbs(G4double de, G4double dl) { fEdepAbs += de; fTrackLengthAbs += dl;};
  void AddGap(G4double de, G4double dl) { fEdepGap += de; fTrackLengthGap += dl;};
  
  G4double GetEdepAbs()     { return fEdepAbs; };
  G4double GetTrakAbs()     { return fTrackLengthAbs; };
  G4double GetEdepGap()     { return fEdepGap; };
  G4double GetTrakGap()     { return fTrackLengthGap; };
     
private:

  G4double fEdepAbs;
  G4double fTrackLengthAbs;
  G4double fEdepGap;
  G4double fTrackLengthGap;
};



typedef G4THitsCollection<CommonVertexHit> JLeicVTXHitsCollection;

extern G4Allocator<CommonVertexHit> JLeicVTXHitAllocator;



inline void* CommonVertexHit::operator new(size_t)
{
  void* aHit;
  aHit = (void*) JLeicVTXHitAllocator.MallocSingle();
  return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

inline void CommonVertexHit::operator delete(void* aHit)
{
  JLeicVTXHitAllocator.FreeSingle((CommonVertexHit*) aHit);
}

#endif


