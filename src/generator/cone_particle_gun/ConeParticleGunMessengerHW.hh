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
//
//
// 

#ifndef ConeParticleGunMessengerHW_h
#define ConeParticleGunMessengerHW_h 1

class ConeParticleGunHW;
class G4ParticleTable;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWith3Vector;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithAnInteger;

#include "G4UImessenger.hh"
#include "globals.hh"

// class description:
//
//  This is a concrete class of G4UImessenger which handles commands for
// ConeParticleGun.
//

class ConeParticleGunMessengerHW: public G4UImessenger
{
  public:
    ConeParticleGunMessengerHW(ConeParticleGunHW * fPtclGun);
    ~ConeParticleGunMessengerHW();
    
  public:
    void SetNewValue(G4UIcommand * command,G4String newValues);
    G4String GetCurrentValue(G4UIcommand * command);

  private:
    void IonCommand(G4String newValues);

  private:
    ConeParticleGunHW * fParticleGun;
    G4ParticleTable * particleTable;

  private: //commands
    G4UIdirectory *             gunDirectory;
    G4UIcmdWithoutParameter *   listCmd;
    G4UIcmdWithAString *        particleCmd;
    G4UIcmdWith3Vector *        directionCmd;
    G4UIcmdWithADoubleAndUnit * energyCmd;
    G4UIcmdWithADoubleAndUnit * momAmpCmd;
    G4UIcmdWith3VectorAndUnit * momCmd;
    G4UIcmdWith3VectorAndUnit * positionCmd;
    G4UIcmdWithADoubleAndUnit * timeCmd;
    G4UIcmdWith3Vector *        polCmd;
    G4UIcmdWithAnInteger *      numberCmd;
    G4UIcmdWithADoubleAndUnit * coneAngleStdDevCmd;
    G4UIcmdWith3VectorAndUnit * positionStdDevCmd;
    G4UIcmdWithADoubleAndUnit * energyStdDevCmd;
    G4UIcommand *               ionCmd;
    
    G4UIcmdWithADouble * minEtaCmd;
    G4UIcmdWithADouble * maxEtaCmd;
    
    G4UIcmdWithADoubleAndUnit * minMomCmd;
    G4UIcmdWithADoubleAndUnit * maxMomCmd;


  private: // for ion shooting
    G4bool   fShootIon; 
    G4int    fAtomicNumber;
    G4int    fAtomicMass;
    G4int    fIonCharge;
    G4double fIonExciteEnergy;
    char     fIonFloatingLevelBase;
    G4int    fIonEnergyLevel;
};

#endif

