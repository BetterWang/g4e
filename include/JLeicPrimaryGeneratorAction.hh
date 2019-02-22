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
/// \file eventgenerator/HepMC/HepMCEx01/include/ExN04PrimaryGeneratorAction.hh
/// \brief Definition of the ExN04PrimaryGeneratorAction class
//
// $Id: ExN04PrimaryGeneratorAction.hh 77801 2013-11-28 13:33:20Z gcosmo $
//

#ifndef JLeicPrimaryGeneratorAction_h
#define JLeicPrimaryGeneratorAction_h 1

#include <map>
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4VPrimaryGenerator;
class JLeicPrimaryGeneratorMessenger;
class JLeicRunAction;
class JLeicDetectorConstruction;

class JLeicPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  JLeicPrimaryGeneratorAction(JLeicDetectorConstruction*,JLeicRunAction*);
  ~JLeicPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* anEvent);

  void SetGenerator(G4VPrimaryGenerator* gen);
  void SetGenerator(G4String genname);

  G4VPrimaryGenerator* GetGenerator() const;
  G4String GetGeneratorName() const;
  static G4String GetPrimaryName();
  static G4double GetPrimaryEnergy();


private:
  G4VPrimaryGenerator* particleGun;
  G4VPrimaryGenerator* hepmcAscii;
  G4VPrimaryGenerator* pythiaAscii;
  G4VPrimaryGenerator* pythiaGen;

  G4VPrimaryGenerator* currentGenerator;
  G4String currentGeneratorName;
  std::map<G4String, G4VPrimaryGenerator*> gentypeMap;

  JLeicPrimaryGeneratorMessenger* messenger;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
inline void JLeicPrimaryGeneratorAction::SetGenerator(G4VPrimaryGenerator* gen)
{
  currentGenerator = gen;
}

inline void JLeicPrimaryGeneratorAction::SetGenerator(G4String genname)
{
  std::map<G4String, G4VPrimaryGenerator*>::iterator pos =
                                            gentypeMap.find(genname);
  if ( pos != gentypeMap.end() ) {
    currentGenerator = pos->second;
    currentGeneratorName = genname;
  }
}

inline G4VPrimaryGenerator* JLeicPrimaryGeneratorAction::GetGenerator() const
{
  return currentGenerator;
}

inline G4String JLeicPrimaryGeneratorAction::GetGeneratorName() const
{
  return currentGeneratorName;
}

#endif
