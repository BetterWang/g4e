//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "BeagleInterfaceMessenger.hh"
#include "BeagleReader.hh"


BeagleInterfaceMessenger::BeagleInterfaceMessenger(BeagleInterface *interface)
        : fBeagleInterface(interface)
{
    fDirectory = new G4UIdirectory("/generator/beagle/");
    fDirectory->SetGuidance("Reading Beagle event from an Ascii file");

    fVerbose = new G4UIcmdWithAnInteger("/generator/beagle/verbose", this);
    fVerbose->SetGuidance("Set verbose level");
    fVerbose->SetParameterName("verboseLevel", false, false);
    fVerbose->SetRange("verboseLevel>=0 && verboseLevel<=1");

    fOpenCmd = new G4UIcmdWithAString("/generator/generator/open", this);
    fOpenCmd->SetGuidance("(re)open data file (Beagle Ascii format)");
    fOpenCmd->SetParameterName("input ascii file", true, true);
}


BeagleInterfaceMessenger::~BeagleInterfaceMessenger() {
    delete fVerbose;
    delete fOpenCmd;
    delete fDirectory;
}


void BeagleInterfaceMessenger::SetNewValue(G4UIcommand *command, G4String newValues) {
    // verbosity level
    if (command == fVerbose) {
        int level = fVerbose->GetNewIntValue(newValues);
        fBeagleInterface->SetVerboseLevel(level);
    }

    // open command
    else if (command == fOpenCmd) {
        fBeagleInterface->Open(newValues);
    }
}


G4String BeagleInterfaceMessenger::GetCurrentValue(G4UIcommand *command) {
    G4String value;
    if (command == fVerbose) {
        value = fVerbose->ConvertToString(fBeagleInterface->GetVerboseLevel());
    } else if (command == fOpenCmd) {
        value = fBeagleInterface->GetFileName();
    }
    return value;
}
