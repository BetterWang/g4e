#ifndef JLeicSteppingAction_h
#define JLeicSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4ios.hh"

#include "globals.hh"

class JLeicDetectorConstruction;
class JLeicRunAction;
class JLeicEventAction;
class JLeicHistogramManager;


class JLeicSteppingAction : public G4UserSteppingAction
{
  public:
    JLeicSteppingAction(JLeicHistogramManager *histo);
    ~JLeicSteppingAction() override;

    void UserSteppingAction(const G4Step*) override;
   
  FILE* rc{};
  private:

    JLeicHistogramManager*     fHistoManager;
};

#endif
