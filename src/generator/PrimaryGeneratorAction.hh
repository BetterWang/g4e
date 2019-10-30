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
 * @file
 *
 * Defines PrimaryGeneratorAction which allows to select a primary generator
 * such as:
 *  - particle gun             // no comments
 *  - hepmc                    // Herwig, Pythia 8 and other HepMC ascii
 *  - lund                     // Pythia 6, and others sticking to lund format
 *  - beagle                   // Beagle
 *
 * The class provides @see - SelectGenerator(G4String name) method to select the generator
 * And is controlled by \sa PrimaryGeneratorMessenger commands
 *
 */


#ifndef PRIMARY_GENERATOR_ACTION_HEADER
#define PRIMARY_GENERATOR_ACTION_HEADER

#include <map>
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4VPrimaryGenerator;
class PrimaryGeneratorMessenger;
class JLeicRunAction;
class JLeicDetectorConstruction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  PrimaryGeneratorAction();
  ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event* anEvent);

  void SetGenerator(G4VPrimaryGenerator* gen);
  void SelectGenerator(G4String genname);

  G4VPrimaryGenerator* GetGenerator() const;
  G4String GetGeneratorName() const;
  static G4String GetPrimaryName();
  static G4double GetPrimaryEnergy();


private:
  G4VPrimaryGenerator* fParticleGunGenerator;       // Particle gun
  G4VPrimaryGenerator* fHepMcAsciiGenerator;        // Herwig, Pythia 8 and other HepMC ascii
  G4VPrimaryGenerator* fPythiaAsciiGenerator;       // Pythia 6 lund format
  G4VPrimaryGenerator* fBeagleGenerator;            // Beagle Shmeagle

  G4VPrimaryGenerator* currentGenerator;
  G4String currentGeneratorName;
  std::map<G4String, G4VPrimaryGenerator*> gentypeMap;

  PrimaryGeneratorMessenger* messenger;

};


inline void PrimaryGeneratorAction::SetGenerator(G4VPrimaryGenerator* gen)
{
  currentGenerator = gen;
}

inline void PrimaryGeneratorAction::SelectGenerator(G4String genname)
{
  auto pos = gentypeMap.find(genname);
  if ( pos != gentypeMap.end() ) {
    currentGenerator = pos->second;
    currentGeneratorName = genname;
  }
}

inline G4VPrimaryGenerator* PrimaryGeneratorAction::GetGenerator() const
{
  return currentGenerator;
}

inline G4String PrimaryGeneratorAction::GetGeneratorName() const
{
  return currentGeneratorName;
}

#endif
