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
#include "PythiaAsciiReaderMessenger.hh"
#include "PythiaAsciiReader.hh"


PythiaAsciiReaderMessenger::PythiaAsciiReaderMessenger(PythiaAsciiReader *pythiaAsciiReader): fReader(pythiaAsciiReader)
{
    fDirectory = new G4UIdirectory("/generator/pythiaAscii/");
    fDirectory->SetGuidance("Reading Pythia event from an Ascii file");

    fVerboseCmd = new G4UIcmdWithAnInteger("/generator/pythiaAscii/verbose", this);
    fVerboseCmd->SetGuidance("Set verbose level. 0=none, 1=info, 2=verbose");
    fVerboseCmd->SetParameterName("verboseLevel", false, false);
    fVerboseCmd->SetRange("verboseLevel>=0 && verboseLevel<=2");

    fOpenCmd = new G4UIcmdWithAString("/generator/pythiaAscii/open", this);
    fOpenCmd->SetGuidance("(re)open data file (Pythia Ascii format)");
    fOpenCmd->SetParameterName("input ascii file", true, true);
}


PythiaAsciiReaderMessenger::~PythiaAsciiReaderMessenger()
{
    delete fVerboseCmd;
    delete fOpenCmd;
    delete fDirectory;
}


void PythiaAsciiReaderMessenger::SetNewValue(G4UIcommand *command, G4String newValues)
{
    if (command == fVerboseCmd) {
        int level = fVerboseCmd->GetNewIntValue(newValues);
        fReader->SetVerboseLevel(level);
    } else if (command == fOpenCmd) {
        fReader->SetFileName(newValues);
        G4cout << "Pythia Ascii inputfile: "
               << fReader->GetFileName() << G4endl;
        fReader->Initialize();
    }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4String PythiaAsciiReaderMessenger::GetCurrentValue(G4UIcommand *command) {
    G4String cv;

    if (command == fVerboseCmd) {
        cv = fVerboseCmd->ConvertToString(fReader->GetVerboseLevel());
    } else if (command == fOpenCmd) {
        cv = fReader->GetFileName();
    }
    return cv;
}
