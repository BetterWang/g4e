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
//


#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIdirectory.hh"
#include "G4UIparameter.hh"
#include "PrimaryGeneratorMessenger.hh"
#include "PrimaryGeneratorAction.hh"


PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction *genaction) : fGeneratorAction(genaction) {

    fDirectory = new G4UIdirectory("/generator/");
    fDirectory->SetGuidance("Control commands for primary generator");

    fSelectCmd = new G4UIcmdWithAString("/generator/select", this);
    fSelectCmd->SetGuidance("Select generator type");
    fSelectCmd->SetParameterName("generator_type", false, false);
    fSelectCmd->SetCandidates("particleGun hepmcAscii pythiaAscii beagle");
    fSelectCmd->SetDefaultValue("particleGun");
}


PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger() {
    delete fSelectCmd;
    delete fDirectory;
}


void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand *command, G4String newValues) {
    // generator/select
    if (command == fSelectCmd) {
        fGeneratorAction->SelectGenerator(newValues);
        G4cout << "PrimaryGeneratorMessenger:: current generator type: " << fGeneratorAction->GetGeneratorName() << G4endl;
    }
}


G4String PrimaryGeneratorMessenger::GetCurrentValue(G4UIcommand *command) {
    G4String cv, st;
    if (command == fSelectCmd) {
        cv = fGeneratorAction->GetGeneratorName();
    }

    return cv;
}
