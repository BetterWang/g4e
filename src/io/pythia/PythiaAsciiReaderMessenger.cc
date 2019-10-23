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
#include "PythiaAsciiReader.hh"


PythiaAsciiReaderMessenger::PythiaAsciiReaderMessenger
        (PythiaAsciiReader *agen)
        : gen(agen) {
    dir = new G4UIdirectory("/generator/pythiaAscii/");
    dir->SetGuidance("Reading Pythia event from an Ascii file");

    verbose = new G4UIcmdWithAnInteger("/generator/pythiaAscii/verbose", this);
    verbose->SetGuidance("Set verbose level");
    verbose->SetParameterName("verboseLevel", false, false);
    verbose->SetRange("verboseLevel>=0 && verboseLevel<=1");

    open = new G4UIcmdWithAString("/generator/pythiaAscii/open", this);
    open->SetGuidance("(re)open data file (Pythia Ascii format)");
    open->SetParameterName("input ascii file", true, true);
}


PythiaAsciiReaderMessenger::~PythiaAsciiReaderMessenger() {
    delete verbose;
    delete open;
    delete dir;
}


void PythiaAsciiReaderMessenger::SetNewValue(G4UIcommand *command,
                                             G4String newValues) {
    if (command == verbose) {
        int level = verbose->GetNewIntValue(newValues);
        gen->SetVerboseLevel(level);
    } else if (command == open) {
        gen->SetFileName(newValues);
        G4cout << "Pythia Ascii inputfile: "
               << gen->GetFileName() << G4endl;
        gen->Initialize();
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String PythiaAsciiReaderMessenger::GetCurrentValue(G4UIcommand *command) {
    G4String cv;

    if (command == verbose) {
        cv = verbose->ConvertToString(gen->GetVerboseLevel());
    } else if (command == open) {
        cv = gen->GetFileName();
    }
    return cv;
}
