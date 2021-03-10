#ifndef JLeicSteppingAction_h
#define JLeicSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4ios.hh"

#include "globals.hh"

class ReferenceDetectorConstruction;
class ReferenceDetectorRunAction;
class ReferenceDetectorEventAction;



class ReferenceDetectorSteppingAction : public G4UserSteppingAction
{
  public:
    ReferenceDetectorSteppingAction(g4e::RootFlatIO*);
    ~ReferenceDetectorSteppingAction() override;

    void UserSteppingAction(const G4Step*) override;
   
  FILE* rc{};
  private:
    g4e::RootFlatIO*           mFlatIo;
};

#endif
