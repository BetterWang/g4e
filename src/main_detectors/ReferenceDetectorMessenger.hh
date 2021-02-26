#ifndef JLeicDetectorMessenger_h
#define JLeicDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "ReferenceDetectorConstruction.hh"

class ReferenceDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


class ReferenceDetectorMessenger: public G4UImessenger
{
  public:
    ReferenceDetectorMessenger(ReferenceDetectorConstruction* );
   ~ReferenceDetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    void SetNewValue(G4UIcommand*, G4int);
    
  private:
    ReferenceDetectorConstruction*   JLeicDetector;
    
    //  ------- Initial beam energy settings -------
    G4UIdirectory*             DetDir;
    G4UIcmdWithAnInteger* EbeamECmd;
    G4UIcmdWithAnInteger* PbeamECmd;
    G4UIcmdWithAnInteger* IbeamZCmd;
    G4UIcmdWithAnInteger* IbeamACmd;

    G4UIcommand* CheckOverlapsCmd;
};

#endif

