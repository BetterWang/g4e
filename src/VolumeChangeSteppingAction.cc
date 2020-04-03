#include "VolumeChangeSteppingAction.hh"

#include <G4Step.hh>


g4e::VolumeChangeSteppingAction::VolumeChangeSteppingAction(g4e::RootOutputManager *rootOutputManager):
    mRootOutputManager(rootOutputManager)
{

}

void g4e::VolumeChangeSteppingAction::UserSteppingAction(const G4Step *aStep)
{
    // Entering volume logic
    if(aStep->IsFirstStepInVolume()) {
        auto volume = aStep->GetPostStepPoint()->GetPhysicalVolume();
        if(mEnterActions.count(volume)) {
            mEnterActions[volume](aStep);
        }

        // We need to write a hit for this volume
        if(mEnterHitWriteVolumes.count(volume)){
            mRootOutputManager->SaveStep(aStep, WriteStepPointChoices::PostStepPoint);
        }
    }

    if(aStep->IsLastStepInVolume())
    {
        auto volume = aStep->GetPostStepPoint()->GetPhysicalVolume();
        if(mExitActions.count(volume)) {
            mExitActions[volume](aStep);
        }

        // We need to write a hit for this volume
        if(mExitHitWriteVolumes.count(volume)){
            mRootOutputManager->SaveStep(aStep, WriteStepPointChoices::PreStepPoint);
        }
    }
}