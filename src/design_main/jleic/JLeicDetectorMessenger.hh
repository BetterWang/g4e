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
/// \file electromagnetic/VertexEIC/include/JLeicDetectorMessenger.hh
/// \brief Definition of the JLeicDetectorMessenger class
//
//
// $Id: JLeicDetectorMessenger.hh 66241 2012-12-13 18:34:42Z gunter $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef JLeicDetectorMessenger_h
#define JLeicDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class JLeicDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


class JLeicDetectorMessenger: public G4UImessenger
{
  public:
    JLeicDetectorMessenger(JLeicDetectorConstruction* );
   ~JLeicDetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    void SetNewValue(G4UIcommand*, G4int);
    
  private:
    JLeicDetectorConstruction*   JLeicDetector;
    
    //  ------- Initial beam energy settings -------
    G4UIdirectory*             DetDir;
    G4UIcmdWithAnInteger* EbeamECmd;
    G4UIcmdWithAnInteger* PbeamECmd;
    //--------- For TRD----------------
    G4UIdirectory*             TRDdetDir;

    G4UIcmdWithAnInteger*       ModelCmd;
    G4UIcmdWithAnInteger*       FoilNumCmd;
    G4UIcmdWithAString*        AbsMaterCmd;
    G4UIcmdWithAString*        DetectorSetUpCmd;
    G4UIcmdWithADoubleAndUnit* AbsThickCmd;
    G4UIcmdWithADoubleAndUnit* AbsRadCmd;
    G4UIcmdWithAString*        RadiatorMaterCmd;
    G4UIcmdWithADoubleAndUnit* RadiatorThickCmd;
    G4UIcmdWithADoubleAndUnit* GasGapThickCmd;
    G4UIcmdWithADoubleAndUnit* AbsZposCmd;

    G4UIcmdWithAString*        WorldMaterCmd;
    G4UIcmdWithADoubleAndUnit* WorldZCmd;
    G4UIcmdWithADoubleAndUnit* WorldRCmd;

    G4UIcmdWithADoubleAndUnit* MagFieldCmd;
    G4UIcmdWithoutParameter*   UpdateCmd;

  // G4UIcmdWithADoubleAndUnit* ElectronCutCmd;
  // G4UIcmdWithADoubleAndUnit* PositronCutCmd;
  // G4UIcmdWithADoubleAndUnit* GammaCutCmd;


};

#endif

