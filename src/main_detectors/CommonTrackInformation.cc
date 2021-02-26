#include "CommonTrackInformation.hh"
#include "G4ios.hh"

G4ThreadLocal G4Allocator<CommonTrackInformation> * aTrackInformationAllocator = nullptr;

void* CommonTrackInformation::operator new(size_t)
{
    if(!aTrackInformationAllocator)
        aTrackInformationAllocator = new G4Allocator<CommonTrackInformation>;
    return (void*)aTrackInformationAllocator->MallocSingle();
}

void CommonTrackInformation::operator delete(void *aTrackInfo)
{
    aTrackInformationAllocator->FreeSingle((CommonTrackInformation*)aTrackInfo);
}


