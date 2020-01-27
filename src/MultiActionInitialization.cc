#include "MultiActionInitialization.hh"
#include <G4MultiEventAction.hh>
#include <G4MultiRunAction.hh>

void g4e::MultiActionInitialization::Build() const
{
    for(auto& init: mActionInits) {
        init->Build();
    }

    if(!mEventActionGenerators.empty()) {
        auto eventActions = new G4MultiEventAction();

        // Using generators to generate event actions
        for(auto generator: mEventActionGenerators) {
            eventActions->push_back(G4UserEventActionUPtr(generator()));
        }
        SetUserAction(eventActions);
    }






    //    std::unique_ptr<G4MultiRunAction> mRunAction;
//    std::unique_ptr<G4MultiEventAction> mEventAction;
//    std::unique_ptr<G4MultiTrackingAction> mTrackingAction;
//    std::unique_ptr<G4MultiSteppingAction> mSteppingAction;
    //UserArguments mArguments;
}

void g4e::MultiActionInitialization::BuildForMaster() const
{
    for(auto& init: mActionInits) {
        init->BuildForMaster();
    }

    if(!mRunActionGenerators.empty()) {
        auto runActions = new G4MultiRunAction();

        // Using generators to generate event actions
        for(auto generator: mRunActionGenerators) {
            runActions->push_back(G4UserRunActionUPtr(generator()));
        }
        SetUserAction(runActions);
    }
}


