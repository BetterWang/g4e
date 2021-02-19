#ifndef BeamlineConstructionMessenger_h
#define BeamlineConstructionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "JLeicDetectorConfig.hh"

class BeamlineConstruction;


class BeamlineConstructionMessenger: public G4UImessenger
{
  public:

    explicit BeamlineConstructionMessenger(JLeicDetectorConfig &config);

    virtual ~BeamlineConstructionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    JLeicDetectorConfig &mBeamlineConstruction;
    G4UIdirectory*        mDirectory;
    G4UIcmdWithAString*   mBeamlineNameCmd;
};

#endif

