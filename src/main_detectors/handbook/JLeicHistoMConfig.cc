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
/// \file electromagnetic/VertexEIC/src/JLeicRunMessenger.cc
/// \brief Implementation of the JLeicRunMessenger class
//
//
// $Id: JLeicRunMessenger.cc 67268 2013-02-13 11:38:40Z ihrivnac $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "JLeicHistoMConfig.hh"

#include "JLeicRunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4ios.hh"
#include "globals.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

JLeicHistoMConfig::JLeicHistoMConfig(): G4UImessenger()
{
    RndmDir = new G4UIdirectory("/rndm/");
    RndmDir->SetGuidance("Rndm status control.");

    RndmSaveCmd = new G4UIcmdWithAnInteger("/rndm/save", this);
    RndmSaveCmd->SetGuidance("set frequency to save rndm status on external files.");
    RndmSaveCmd->SetGuidance("freq = 0 not saved");
    RndmSaveCmd->SetGuidance("freq > 0 saved on: beginOfRun.rndm");
    RndmSaveCmd->SetGuidance("freq = 1 saved on:   endOfRun.rndm");
    RndmSaveCmd->SetGuidance("freq = 2 saved on: endOfEvent.rndm");
    RndmSaveCmd->SetParameterName("frequency", false);
    RndmSaveCmd->SetRange("frequency>=0 && frequency<=2");
    RndmSaveCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    RndmReadCmd = new G4UIcmdWithAString("/rndm/read", this);
    RndmReadCmd->SetGuidance("get rndm status from an external file.");
    RndmReadCmd->SetParameterName("fileName", true);
    RndmReadCmd->SetDefaultValue("beginOfRun.rndm");
    RndmReadCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

JLeicHistoMConfig::~JLeicHistoMConfig()
{


    delete RndmSaveCmd;
    delete RndmReadCmd;
    delete RndmDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void JLeicHistoMConfig::SetNewValue(G4UIcommand *command, G4String newValues)
{

    if (command == RndmReadCmd) {
        G4cout << "\n---> rndm status restored from file: " << newValues << G4endl;
        CLHEP::HepRandom::restoreEngineStatus(newValues);
        CLHEP::HepRandom::showEngineStatus();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

   
