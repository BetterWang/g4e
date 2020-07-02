#ifndef JLeicXTRphysicsMessenger_h
#define JLeicXTRphysicsMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class JLeicXTRphysics;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAString;


class JLeicXTRphysicsMessenger: public G4UImessenger
{
  public:
    explicit JLeicXTRphysicsMessenger(JLeicXTRphysics* pp);
   ~JLeicXTRphysicsMessenger() override;
    
    void SetNewValue(G4UIcommand*, G4String) override;
    
  private:
    JLeicXTRphysics*          JLeicList;

    G4UIcmdWithADoubleAndUnit* setMaxStepCmd;

    G4UIcmdWithADoubleAndUnit* cutGCmd;
    G4UIcmdWithADoubleAndUnit* cutECmd;
    G4UIcmdWithADoubleAndUnit* eCmd;

    G4UIcmdWithADoubleAndUnit* ElectronCutCmd;
    G4UIcmdWithADoubleAndUnit* PositronCutCmd;
    G4UIcmdWithADoubleAndUnit* GammaCutCmd;

    G4UIcmdWithADoubleAndUnit* RadiatorCutCmd;
    G4UIcmdWithADoubleAndUnit* DetectorCutCmd;
    G4UIcmdWithAString*        XTRModelCmd;
};

#endif

