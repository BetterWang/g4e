#include "BeamlineConstructionMessenger.hh"


#include "JLeicDetectorConfig.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"


BeamlineConstructionMessenger::BeamlineConstructionMessenger(JLeicDetectorConfig &config) :
    G4UImessenger(),
    mBeamlineConstruction(config)
{
    mDirectory = new G4UIdirectory("/g4e/beamline/");
    mDirectory->SetGuidance("Detector control.");

    mBeamlineNameCmd = new G4UIcmdWithAString("/g4e/beamline/name", this);
    mBeamlineNameCmd->SetGuidance("Select beamline ip6 or ip8");
    mBeamlineNameCmd->SetParameterName("name", false);
    mBeamlineNameCmd->SetDefaultValue("ip6");

}

void BeamlineConstructionMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == mBeamlineNameCmd) {
        if(newValue == "erhic") newValue = "ip6";
        if(newValue == "eicIP2") newValue = "ip8";
        if(newValue == "jleic") newValue = "ip6";
        if(newValue != "ip6" && newValue != "ip8") {
            G4Exception("JLeicDetectorConstruction::Construct",
                        "InvalidSetup", FatalException,
                        "/g4e/beamline/name should be 'ip6' or 'ip8'");
        }

        mBeamlineConstruction.BeamlineName = newValue;
    }
}

BeamlineConstructionMessenger::~BeamlineConstructionMessenger() {
    delete mDirectory;
    delete mBeamlineNameCmd;
}

