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
/// \file eventgenerator/HepMC/HepMCEx01/src/ExN04PrimaryGeneratorAction.cc
/// \brief Implementation of the ExN04PrimaryGeneratorAction class
//
// $Id: ExN04PrimaryGeneratorAction.cc 77801 2013-11-28 13:33:20Z gcosmo $
//

#include "JLeicRunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "PythiaAsciiReader.hh"
#include "HepMCG4AsciiReader.hh"
#include "HepMCG4PythiaInterface.hh"
#include "BeagleInterface.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::PrimaryGeneratorAction(JLeicDetectorConstruction *DET, JLeicRunAction *RA)
        : G4VUserPrimaryGeneratorAction() {
    // default generator is particle gun.
    currentGenerator = fParticleGunGenerator = new G4ParticleGun();
    currentGeneratorName = "particleGun";
    fHepMcAsciiGenerator = new HepMCG4AsciiReader();
    fPythiaAsciiGenerator = new PythiaAsciiReader();
    fBeagleGenerator = new g4e::BeagleInterface();

    gentypeMap["particleGun"] = fParticleGunGenerator;
    gentypeMap["hepmcAscii"] = fHepMcAsciiGenerator;
    gentypeMap["pythiaAscii"] = fPythiaAsciiGenerator;
    gentypeMap["beagle"] = fBeagleGenerator;

    messenger = new PrimaryGeneratorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete messenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {

    printf("PrimaryGeneratorAction:: Next event \n");
    if (currentGenerator)
        currentGenerator->GeneratePrimaryVertex(anEvent);
    else
        G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                    "PrimaryGeneratorAction001", FatalException,
                    "generator is not instanciated.");
}

G4String PrimaryGeneratorAction::GetPrimaryName() {
    return "hepmc";
}

G4double PrimaryGeneratorAction::GetPrimaryEnergy() {
    return 555;
}
