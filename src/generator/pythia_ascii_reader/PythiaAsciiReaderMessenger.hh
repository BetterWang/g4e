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

/**
 * This messenger defines the next:
 *     /generator/pythiaAscii/          # G4UIDirectory - Reading Pythia event from an Ascii file
 *     /generator/pythiaAscii/verbose   # Set verbose level. 0=none, 1=info, 2=verbose
 *     /generator/pythiaAscii/open      # (re)open data file (Pythia Ascii format)
 */


#ifndef PYTHIA_ASCII_READER_MESSENGER_H
#define PYTHIA_ASCII_READER_MESSENGER_H

#include "G4UImessenger.hh"

class PythiaAsciiReader;
class G4UIdirectory;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;

class PythiaAsciiReaderMessenger : public G4UImessenger
{
public:
    explicit PythiaAsciiReaderMessenger(PythiaAsciiReader *pythiaAsciiReader);

    ~PythiaAsciiReaderMessenger() override;

    void SetNewValue(G4UIcommand *command, G4String newValues) override;

    G4String GetCurrentValue(G4UIcommand *command) override;

private:
    PythiaAsciiReader *fReader;
    G4UIdirectory *fDirectory;
    G4UIcmdWithAnInteger *fVerboseCmd;
    G4UIcmdWithAString *fOpenCmd;
};

#endif
