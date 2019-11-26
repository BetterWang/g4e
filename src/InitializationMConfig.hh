//
// Created by romanov on 11/25/19.
//

#ifndef G4E_INITIALIZATIONCONFIGMESSENGER_HH
#define G4E_INITIALIZATIONCONFIGMESSENGER_HH

#include <G4UImessenger.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithAString.hh>

#include "LogLevels.hh"


class InitializationMConfig : public G4UImessenger {
public:

    InitializationMConfig();

    void SetNewValue(G4UIcommand *command, G4String newValue) override;
    G4String GetCurrentValue(G4UIcommand *command) override;

    LogLevels GetLogLevel() {return fLogLevel;}

private:

    LogLevels fLogLevel = LogLevels::INFO;     /// General|Global log level
    G4UIcmdWithAString *fLogLevelCmd;                /// LogLevel corresponding UI command
    G4UIdirectory* fDirectory;                       /// Ui commands directory 'g4e'
};


InitializationMConfig::InitializationMConfig()
{
    fDirectory = new G4UIdirectory("/g4/");
    fDirectory->SetGuidance("Control commands for general config");

    fLogLevelCmd = new G4UIcmdWithAString("/g4e/logLevel", this);
    fLogLevelCmd->SetGuidance("Sets general log level");
    fLogLevelCmd->SetParameterName("logLevel", false, false);
    fLogLevelCmd->SetCandidates("off fatal error warn info debug trace");
    fLogLevelCmd->SetDefaultValue("info");
}

void InitializationMConfig::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == fLogLevelCmd) {
        fGlobalLogLevel = LogLevels(newValue);
    }
}

G4String InitializationMConfig::GetCurrentValue(G4UIcommand *command)
{
    if (command == fLogLevelCmd) {
        return fGlobalLogLevel.ToString();
    }

    return G4UImessenger::GetCurrentValue(command);
}


#endif //G4E_INITIALIZATIONCONFIGMESSENGER_HH
