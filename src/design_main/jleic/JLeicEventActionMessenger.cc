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


#include "JLeicEventActionMessenger.hh"
#include "JLeicEventAction.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

#include "globals.hh"



JLeicEventActionMessenger::JLeicEventActionMessenger(JLeicEventAction *eventAction) :
    G4UImessenger(),
    fEventAction(eventAction)
{
    fVerboseCmd = new G4UIcmdWithAnInteger("/event/verbose", this);
    fVerboseCmd->SetGuidance("Set verbose level .");
    fVerboseCmd->SetParameterName("level", true);
    fVerboseCmd->SetDefaultValue(0);

    fPrintCmd = new G4UIcmdWithAnInteger("/event/printModulo", this);
    fPrintCmd->SetGuidance("Print events modulo n");
    fPrintCmd->SetParameterName("printModulo", false);
    fPrintCmd->SetDefaultValue(1000);
    fPrintCmd->SetRange("printModulo>0");
    fPrintCmd->AvailableForStates(G4State_Idle);
}



JLeicEventActionMessenger::~JLeicEventActionMessenger()
{
    delete fVerboseCmd;
    delete fPrintCmd;
}



void JLeicEventActionMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == fVerboseCmd) { fEventAction->SetVerbose(G4UIcmdWithAnInteger::GetNewIntValue(newValue)); }

    if (command == fPrintCmd) { fEventAction->SetPrintModulo(G4UIcmdWithAnInteger::GetNewIntValue(newValue)); }
}

G4String JLeicEventActionMessenger::GetCurrentValue(G4UIcommand *command)
{
    if (command == fVerboseCmd) { return G4UIcommand::ConvertToString(fEventAction->GetVerbose()); }

    if (command == fPrintCmd) {  return G4UIcommand::ConvertToString(fEventAction->GetPrintModulo()); }

    return G4UImessenger::GetCurrentValue(command);
}


