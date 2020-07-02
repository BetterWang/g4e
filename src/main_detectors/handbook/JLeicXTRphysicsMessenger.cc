#include "JLeicXTRphysicsMessenger.hh"
#include "JLeicXTRphysics.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"


JLeicXTRphysicsMessenger::JLeicXTRphysicsMessenger(JLeicXTRphysics *List) : G4UImessenger(), JLeicList(List)
{
    cutGCmd = new G4UIcmdWithADoubleAndUnit("/calor/cutG", this);
    cutGCmd->SetGuidance("Set cut values by RANGE for Gamma.");
    cutGCmd->SetParameterName("range", true);
    cutGCmd->SetDefaultValue(1.);
    cutGCmd->SetDefaultUnit("mm");
    cutGCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    cutECmd = new G4UIcmdWithADoubleAndUnit("/calor/cutE", this);
    cutECmd->SetGuidance("Set cut values by RANGE for e- e+.");
    cutECmd->SetParameterName("range", true);
    cutECmd->SetDefaultValue(1.);
    cutECmd->SetDefaultUnit("mm");
    cutECmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    setMaxStepCmd = new G4UIcmdWithADoubleAndUnit("/step/setMaxStep", this);
    setMaxStepCmd->SetGuidance("Set max. step length in the detector");
    setMaxStepCmd->SetParameterName("mxStep", true);
    setMaxStepCmd->SetDefaultUnit("mm");

    ElectronCutCmd = new G4UIcmdWithADoubleAndUnit("/emphyslist/setElectronCut", this);
    ElectronCutCmd->SetGuidance("Set electron cut in mm for vertex region");
    ElectronCutCmd->SetParameterName("ElectronCut", false, false);
    ElectronCutCmd->SetDefaultUnit("mm");
    ElectronCutCmd->SetRange("ElectronCut>0.");
    ElectronCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    PositronCutCmd = new G4UIcmdWithADoubleAndUnit("/emphyslist/setPositronCut", this);
    PositronCutCmd->SetGuidance("Set positron cut in mm for vertex region");
    PositronCutCmd->SetParameterName("PositronCut", false, false);
    PositronCutCmd->SetDefaultUnit("mm");
    PositronCutCmd->SetRange("PositronCut>0.");
    PositronCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    GammaCutCmd = new G4UIcmdWithADoubleAndUnit("/emphyslist/setGammaCut", this);
    GammaCutCmd->SetGuidance("Set gamma cut in mm for vertex region");
    GammaCutCmd->SetParameterName("GammaCut", false, false);
    GammaCutCmd->SetDefaultUnit("mm");
    GammaCutCmd->SetRange("GammaCut>0.");
    GammaCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    RadiatorCutCmd = new G4UIcmdWithADoubleAndUnit("/emphyslist/setRadiatorCuts", this);
    RadiatorCutCmd->SetGuidance("Set radiator cut in mm for vertex region");
    RadiatorCutCmd->SetParameterName("RadiatorCuts", false, false);
    RadiatorCutCmd->SetDefaultUnit("mm");
    RadiatorCutCmd->SetRange("RadiatorCuts > 0.");
    RadiatorCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    DetectorCutCmd = new G4UIcmdWithADoubleAndUnit("/emphyslist/setDetectorCuts", this);
    DetectorCutCmd->SetGuidance("Set radiator cut in mm for vertex region");
    DetectorCutCmd->SetParameterName("DetectorCuts", false, false);
    DetectorCutCmd->SetDefaultUnit("mm");
    DetectorCutCmd->SetRange("DetectorCuts > 0.");
    DetectorCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    XTRModelCmd = new G4UIcmdWithAString("/emphyslist/setXTRModel", this);
    XTRModelCmd->SetGuidance("Set XTR model");
    XTRModelCmd->SetParameterName("XTRModel", false);
    XTRModelCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

}


JLeicXTRphysicsMessenger::~JLeicXTRphysicsMessenger()
{
    delete cutGCmd;
    delete cutECmd;

    delete setMaxStepCmd;

    delete ElectronCutCmd;
    delete PositronCutCmd;
    delete GammaCutCmd;
    delete XTRModelCmd;
}


void JLeicXTRphysicsMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{

    //printf("JLeicXTRphysicsMessenger::SetNewValue %p %p string= %s \n",command,GammaCutCmd,newValue.c_str());

    if (command == cutGCmd) { JLeicList->SetGammaCut(cutGCmd->GetNewDoubleValue(newValue)); }
    if (command == cutECmd) { JLeicList->SetElectronCut(eCmd->GetNewDoubleValue(newValue)); }
    if (command == setMaxStepCmd) { JLeicList->SetMaxStep(setMaxStepCmd->GetNewDoubleValue(newValue)); }

    if (command == ElectronCutCmd) {
        JLeicList->SetRegElectronCut(ElectronCutCmd->GetNewDoubleValue(newValue));
    }
    if (command == PositronCutCmd) {
        JLeicList->SetRegPositronCut(PositronCutCmd->GetNewDoubleValue(newValue));
    }
    if (command == GammaCutCmd) {

        JLeicList->SetRegGammaCut(GammaCutCmd->GetNewDoubleValue(newValue));
    }
    if (command == RadiatorCutCmd) {
        JLeicList->SetRadiatorCuts();
    }
    if (command == DetectorCutCmd) {
        JLeicList->SetDetectorCuts();
    }
    if (command == XTRModelCmd) {
        JLeicList->SetXTRModel(newValue);
    }
}
