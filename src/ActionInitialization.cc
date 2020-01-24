#include "ActionInitialization.hh"

void g4e::ActionInitialization::Build() const
{
    for(auto& init: mActionInits) {
        init->Build();
    }

    //    std::unique_ptr<G4MultiRunAction> mRunAction;
//    std::unique_ptr<G4MultiEventAction> mEventAction;
//    std::unique_ptr<G4MultiTrackingAction> mTrackingAction;
//    std::unique_ptr<G4MultiSteppingAction> mSteppingAction;
    //UserArguments mArguments;
}

void g4e::ActionInitialization::BuildForMaster() const
{
    for(auto& init: mActionInits) {
        init->BuildForMaster();
    }
}


