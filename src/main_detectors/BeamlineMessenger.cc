#include "BeamlineMessenger.hh"


#include "DetectorConfig.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include <G4GenericMessenger.hh>

BeamlineMessenger::BeamlineMessenger(DetectorConfig &config) :
    G4UImessenger(),
    mBeamlineConstruction(config)
{
    mDirectory = new G4UIdirectory("/eic/beamline/");
    mDirectory->SetGuidance("Detector control.");

    mBeamlineNameCmd = new G4UIcmdWithAString("/eic/beamline/name", this);
    mBeamlineNameCmd->SetGuidance("Select beamline ip6 or ip8");
    mBeamlineNameCmd->SetParameterName("name", false);
    mBeamlineNameCmd->SetDefaultValue("ip6");
}

void BeamlineMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == mBeamlineNameCmd) {
        if(newValue == "erhic") newValue = "ip6";       // This is for backward compatibility
        if(newValue == "eicIP2") newValue = "ip8";      // This is for backward compatibility
        if(newValue == "jleic") newValue = "ip6";       // This is for backward compatibility
        if(newValue != "ip6" && newValue != "ip8") {
            G4Exception("BeamlineMessenger::SetNewValue",
                        "InvalidSetup", FatalException,
                        "/g4e/beamline/name should be 'ip6' or 'ip8'");
        }

        mBeamlineConstruction.BeamlineName = newValue;
    }
}

BeamlineMessenger::~BeamlineMessenger() {
    delete mDirectory;
    delete mBeamlineNameCmd;
}

