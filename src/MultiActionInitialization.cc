#include "MultiActionInitialization.hh"
#include <G4MultiSteppingAction.hh>
#include <G4MultiTrackingAction.hh>
#include <G4MultiEventAction.hh>
#include <G4MultiRunAction.hh>


void g4e::MultiActionInitialization::Build() const
{
    // Add Generator action
    SetUserAction(mGeneratorAction);

    // Event actions
    if(!mEventActionGenerators.empty()) {
        auto eventActions = new G4MultiEventAction();

        // Using generators to generate event actions
        for(auto generator: mEventActionGenerators) {
            eventActions->push_back(G4UserEventActionUPtr(generator()));
        }
        SetUserAction(eventActions);
    }

    // Stepping actions
    if(!mSteppingActionGenerators.empty()) {
        auto multiSteppingAction = new G4MultiSteppingAction();

        // Using generators to generate event actions
        for(auto generator: mSteppingActionGenerators) {
            multiSteppingAction->push_back(G4UserSteppingActionUPtr(generator()));
        }
        SetUserAction(multiSteppingAction);
    }

    // Tracking actions
    if(!mTrackingActionGenerators.empty()) {
        auto multiTrackingAction = new G4MultiTrackingAction();

        // Using generators to generate event actions
        for(auto generator: mTrackingActionGenerators) {
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
        for(auto generator: mRunActionGenerators) {
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


