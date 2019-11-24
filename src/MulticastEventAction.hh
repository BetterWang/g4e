#ifndef _MULTICAST_EVENT_ACTION_HEADER_
#define _MULTICAST_EVENT_ACTION_HEADER_

#include <vector>

#include "G4UserEventAction.hh"
#include "globals.hh"




class MulticastEventAction : public G4UserEventAction
{
public:
    MulticastEventAction();

    ~MulticastEventAction();

public:
    void BeginOfEventAction(const G4Event *) override;

    void EndOfEventAction(const G4Event *) override;


    void SetVerbose(G4int level) { fVerbose = level; }    /// 0 = nothing, 1 = some, 2 = debug
    G4int GetVerbose() { return fVerbose; }               /// 0 = nothing, 1 = some, 2 = debug

    void SetPrintModulo(G4int val) { printModulo = val; }
    G4int GetPrintModulo() { return printModulo; }

    void AddUserAction(G4UserEventAction * action);
    std::vector<G4UserEventAction*>& GetActions() { return fActions; }

private:

    G4int fVerbose;
    G4int printModulo;

    std::vector<G4UserEventAction*> fActions;
};

void MulticastEventAction::AddUserAction(G4UserEventAction *action) {

    // Do we already have a pointer to this UserAction?
    if(std::find(fActions.begin(), fActions.end(), action) != fActions.end()) {
        return;
    }

    fActions.push_back(action);
}


void MulticastEventAction::BeginOfEventAction(const G4Event *event)
{
    for(auto action: fActions) {
        action->BeginOfEventAction(event);      // Call BeginOfEventAction for each action
    }
}


void MulticastEventAction::EndOfEventAction(const G4Event *event)
{
    for(auto action: fActions) {
        action->EndOfEventAction(event);       // Call BeginOfEventAction for each action
    }
}

#endif

    
