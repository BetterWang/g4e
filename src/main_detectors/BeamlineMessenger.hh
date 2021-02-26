#ifndef BeamlineConstructionMessenger_h
#define BeamlineConstructionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "DetectorConfig.hh"

class SingleSubdetectorConstruction;


class BeamlineMessenger: public G4UImessenger
{
  public:

    explicit BeamlineMessenger(DetectorConfig &config);

    virtual ~BeamlineMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    DetectorConfig &mBeamlineConstruction;
    G4UIdirectory*        mDirectory;
    G4UIcmdWithAString*   mBeamlineNameCmd;
};

#endif

