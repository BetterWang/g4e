#include "JLeicTrackInformation.hh"
#include "G4ios.hh"

G4ThreadLocal G4Allocator<JLeicTrackInformation> * aTrackInformationAllocator = nullptr;

void* JLeicTrackInformation::operator new(size_t)
{
    if(!aTrackInformationAllocator)
        aTrackInformationAllocator = new G4Allocator<JLeicTrackInformation>;
    return (void*)aTrackInformationAllocator->MallocSingle();
}

void JLeicTrackInformation::operator delete(void *aTrackInfo)
{
    aTrackInformationAllocator->FreeSingle((JLeicTrackInformation*)aTrackInfo);
}


