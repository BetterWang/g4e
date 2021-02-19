#include "JLeicDetectorMessenger.hh"

#include "JLeicDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"


JLeicDetectorMessenger::JLeicDetectorMessenger(JLeicDetectorConstruction *JLeicDet) : G4UImessenger(), JLeicDetector(JLeicDet)
{
    DetDir = new G4UIdirectory("/eic/");
    DetDir->SetGuidance("Detector control.");

    EbeamECmd = new G4UIcmdWithAnInteger("/eic/refdet/eBeam", this);
    EbeamECmd->SetGuidance("Electron beam energy settings");
    EbeamECmd->SetParameterName("EbeamE", false, false);
    EbeamECmd->SetDefaultValue(10);
    EbeamECmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    PbeamECmd = new G4UIcmdWithAnInteger("/eic/refdet/pBeam", this);
    PbeamECmd->SetGuidance("Ion/proton beam energy settings");
    PbeamECmd->SetParameterName("PbeamE", false, false);
    PbeamECmd->SetDefaultValue(10);
    PbeamECmd->AvailableForStates(G4State_PreInit, G4State_Idle);


    IbeamACmd = new G4UIcmdWithAnInteger("/eic/refdet/iBeamA", this);
    IbeamACmd->SetGuidance("Ion species A ");
    IbeamACmd->SetParameterName("iBeamA", false, false);
    IbeamACmd->SetDefaultValue(1);
    IbeamACmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    IbeamZCmd = new G4UIcmdWithAnInteger("/eic/refdet/iBeamZ", this);
    IbeamZCmd->SetGuidance("Ion species Z ");
    IbeamZCmd->SetParameterName("iBeamZ", false, false);
    IbeamZCmd->SetDefaultValue(1);
    IbeamZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    CheckOverlapsCmd = new G4UIcommand("/eic/refdet/checkOverlaps", this);
    CheckOverlapsCmd->SetGuidance("Checks volumes overlap. Must be initialized");    
    // CheckOverlapsCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

///////////////////////////////////////////////////////////////////////////////

JLeicDetectorMessenger::~JLeicDetectorMessenger()
{
    delete EbeamECmd;
    delete PbeamECmd;
    delete IbeamACmd;
    delete IbeamZCmd;

    delete DetDir;
}


void JLeicDetectorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == EbeamECmd) {
        JLeicDetector->GetConfigRef().ElectronBeamEnergy = G4UIcmdWithAnInteger::GetNewIntValue(newValue);
    }
    if (command == PbeamECmd) {
        JLeicDetector->GetConfigRef().IonBeamEnergy = G4UIcmdWithAnInteger::GetNewIntValue(newValue);
    }
    if (command == IbeamZCmd) {
        JLeicDetector->GetConfigRef().IonBeamZ = G4UIcmdWithAnInteger::GetNewIntValue(newValue);
    }
    if (command == IbeamACmd) {
        JLeicDetector->GetConfigRef().IonBeamA = G4UIcmdWithAnInteger::GetNewIntValue(newValue);
    }
    if (command == CheckOverlapsCmd) {
        JLeicDetector->CheckVolumeOverlap();
    }
}
