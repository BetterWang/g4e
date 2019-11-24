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
/// \file electromagnetic/VertexEIC/include/JLeicPhysicsListMessenger.hh
/// \brief Definition of the JLeicPhysicsListMessenger class
//
//
// $Id: JLeicPhysicsListMessenger.hh 73033 2013-08-15 09:24:45Z gcosmo $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef JLeicPhysicsListMessenger_h
#define JLeicPhysicsListMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class JLeicPhysicsList;

class G4UIcmdWithoutParameter;

class G4UIcmdWithADouble;

class G4UIcmdWithADoubleAndUnit;

class G4UIcmdWithAString;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class JLeicPhysicsListMessenger : public G4UImessenger
{
public:
    JLeicPhysicsListMessenger(JLeicPhysicsList *);

    ~JLeicPhysicsListMessenger();

    void SetNewValue(G4UIcommand *, G4String);

private:
    JLeicPhysicsList *JLeicList;

    G4UIcmdWithADoubleAndUnit *setMaxStepCmd;

    G4UIcmdWithADoubleAndUnit *cutGCmd;
    G4UIcmdWithADoubleAndUnit *cutECmd;
    G4UIcmdWithADoubleAndUnit *eCmd;

    G4UIcmdWithADoubleAndUnit *ElectronCutCmd;
    G4UIcmdWithADoubleAndUnit *PositronCutCmd;
    G4UIcmdWithADoubleAndUnit *GammaCutCmd;

    G4UIcmdWithADoubleAndUnit *RadiatorCutCmd;
    G4UIcmdWithADoubleAndUnit *DetectorCutCmd;
    G4UIcmdWithAString *XTRModelCmd;
};

#endif

