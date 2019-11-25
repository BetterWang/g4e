#ifndef G4E_MULTICASTSTEPPINGACTION_HH
#define G4E_MULTICASTSTEPPINGACTION_HH

#include <vector>
#include <algorithm>

#include <G4UserSteppingAction.hh>


class MulticastSteppingAction : public G4UserSteppingAction
{
public:
    MulticastSteppingAction() = default;

    ~MulticastSteppingAction() override = default;


    void UserSteppingAction(const G4Step *step) override;


    void AddUserAction(G4UserSteppingAction * action);
    std::vector<G4UserSteppingAction*>& GetActions() { return fActions; }

private:
    std::vector<G4UserSteppingAction*> fActions;
};

void MulticastSteppingAction::AddUserAction(G4UserSteppingAction *action) {

    // Do we already have a pointer to this UserAction?
    if(std::find(fActions.begin(), fActions.end(), action) != fActions.end()) {
        return;
    }

    fActions.push_back(action);
}


void MulticastSteppingAction::UserSteppingAction(const G4Step *step)
{
    for(auto action: fActions) {
        action->UserSteppingAction(step);      // Call BeginOfEventAction for each action
    }
}


#endif //G4E_MULTICASTSTEPPINGACTION_HH
