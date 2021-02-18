#ifndef BeamlineConstructionMessenger_h
#define BeamlineConstructionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

class BeamlineConstruction;


class BeamlineConstructionMessenger: public G4UImessenger
{
  public:
    explicit BeamlineConstructionMessenger(BeamlineConstruction* );
    virtual ~BeamlineConstructionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    BeamlineConstruction* mBeamlineConstruction;
    G4UIdirectory*        mDirectory;
    G4UIcmdWithAString*   mBeamlineNameCmd;
};

#endif

