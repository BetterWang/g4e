#include <G4MultiSteppingAction.hh>
#include <G4MultiTrackingAction.hh>
#include <G4MultiEventAction.hh>
#include <G4MultiRunAction.hh>
#include "MultiActionInitialization.hh"
#include <VolumeChangeSteppingAction.hh>

/**
 * A C++ standard library function shall not directly or indirectly modify objects (1.10) accessible by threads
 * other than the current thread unless the objects are accessed directly or indirectly via the function’s
 * non- const arguments, including this.
 */

void g4e::MultiActionInitialization::Build() const
{
    mBuildIsDone = true;
    // Add Generator action
    SetUserAction(mGeneratorAction);

    // Event actions
    if(!mEventActionGenerators.empty()) {
        auto eventActions = new G4MultiEventAction();

        // Using generators to generate event actions
        for(const auto& generator: mEventActionGenerators) {
            eventActions->push_back(G4UserEventActionUPtr(generator()));
        }
        SetUserAction(eventActions);
    }

    // Stepping actions
    if(!mSteppingActionGenerators.empty()) {
        auto multiSteppingAction = new G4MultiSteppingAction();

        // Using generators to generate event actions
        for(const auto& generator: mSteppingActionGenerators) {
            auto action = generator();
            auto volumeChangeAction = dynamic_cast<VolumeChangeSteppingAction*>(action);
            if(volumeChangeAction != nullptr) {
                std::lock_guard<std::mutex> lock(mBuildMutex);

                auto id = std::this_thread::get_id();
                mVolChangeActionsByThread[id]= volumeChangeAction;

            }

            multiSteppingAction->push_back(G4UserSteppingActionUPtr(action));
        }
        SetUserAction(multiSteppingAction);
    }

    // Tracking actions
    if(!mTrackingActionGenerators.empty()) {
        auto multiTrackingAction = new G4MultiTrackingAction();

        // Using generators to generate event actions
        for(const auto& generator: mTrackingActionGenerators) {
            multiTrackingAction->push_back(G4UserTrackingActionUPtr(generator()));
        }
        SetUserAction(multiTrackingAction);
    }
}

void g4e::MultiActionInitialization::BuildForMaster() const
{
    if(!mRunActionGenerators.empty()) {
        auto runActions = new G4MultiRunAction();

        // Using generators to generate event actions
        for(const auto& generator: mRunActionGenerators) {
            runActions->push_back(G4UserRunActionUPtr(generator()));
        }
        SetUserAction(runActions);
    }
}

G4VUserPrimaryGeneratorAction *g4e::MultiActionInitialization::GetGeneratorAction() const
{
    return mGeneratorAction;
}

void g4e::MultiActionInitialization::SetGeneratorAction(G4VUserPrimaryGeneratorAction *mGeneratorAction)
{
    MultiActionInitialization::mGeneratorAction = mGeneratorAction;
}

void g4e::MultiActionInitialization::AddEnterVolumeSteppingAction(G4VPhysicalVolume *vol, std::function<void(const G4Step *)> func)
{
    ThrowOnWrongInitializationOrder("AddEnterVolumeSteppingAction");
    VolumeChangeSteppingAction* steppingAction = mVolChangeActionsByThread[std::this_thread::get_id()];
    steppingAction->OnEnterVolumeAction(vol, std::move(func));
}

void g4e::MultiActionInitialization::AddExitVolumeSteppingAction(G4VPhysicalVolume *vol, std::function<void(const G4Step *)> func)
{
    ThrowOnWrongInitializationOrder("AddExitVolumeSteppingAction");
    VolumeChangeSteppingAction* steppingAction = mVolChangeActionsByThread[std::this_thread::get_id()];
    steppingAction->OnExitVolumeAction(vol, std::move(func));
}

void g4e::MultiActionInitialization::OnEnterVolumeWriteHit(G4VPhysicalVolume *vol)
{
    ThrowOnWrongInitializationOrder("OnEnterVolumeWriteHit");
    VolumeChangeSteppingAction* steppingAction = mVolChangeActionsByThread[std::this_thread::get_id()];
    steppingAction->OnEnterVolumeWriteHit(vol);
}

void g4e::MultiActionInitialization::OnExitVolumeWriteHit(G4VPhysicalVolume *vol)
{
    ThrowOnWrongInitializationOrder("OnExitVolumeWriteHit");
    VolumeChangeSteppingAction* steppingAction = mVolChangeActionsByThread[std::this_thread::get_id()];
    steppingAction->OnExitVolumeWriteHit(vol);
}
