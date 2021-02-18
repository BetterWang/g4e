#include "BeamlineConstructionMessenger.hh"


#include "BeamlineConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"


BeamlineConstructionMessenger::BeamlineConstructionMessenger(BeamlineConstruction *beamlineConstruction) :
    G4UImessenger(),
    mBeamlineConstruction(beamlineConstruction)
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
        mBeamlineConstruction->GetConfigRef().BeamlineName = newValue;
    }
}

BeamlineConstructionMessenger::~BeamlineConstructionMessenger() {
    delete mDirectory;
    delete mBeamlineNameCmd;
}

