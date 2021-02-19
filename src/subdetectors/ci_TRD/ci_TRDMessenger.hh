#ifndef ci_TRDMessenger_h
#define ci_TRDMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"



class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
class ci_TRD_Design;

class ci_TRDMessenger: public G4UImessenger
{
  public:
    ci_TRDMessenger(ci_TRD_Design* );
   ~ci_TRDMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    void SetNewValue(G4UIcommand*, G4int);
    
  private:
    ci_TRD_Design*          JLeicList;

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

    ci_TRD_Design*   fTrd;
    
    //--------- For TRD----------------
    G4UIdirectory*             TRDdetDir;

    G4UIcmdWithAnInteger*       ModelCmd;
    G4UIcmdWithAnInteger*       FoilNumCmd;

    G4UIcmdWithADoubleAndUnit* AbsThickCmd;
    G4UIcmdWithADoubleAndUnit* AbsRadCmd;
    G4UIcmdWithAString*        RadiatorMaterCmd;
    G4UIcmdWithADoubleAndUnit* RadiatorThickCmd;
    G4UIcmdWithADoubleAndUnit* GasGapThickCmd;
    G4UIcmdWithADoubleAndUnit* AbsZposCmd;


  // G4UIcmdWithADoubleAndUnit* ElectronCutCmd;
  // G4UIcmdWithADoubleAndUnit* PositronCutCmd;
  // G4UIcmdWithADoubleAndUnit* GammaCutCmd;


};

#endif

