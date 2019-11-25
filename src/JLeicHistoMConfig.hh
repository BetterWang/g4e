/**///
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
/// \file electromagnetic/VertexEIC/include/JLeicRunMessenger.hh
/// \brief Definition of the JLeicRunMessenger class
//

//
// $Id: JLeicRunMessenger.hh 66241 2012-12-13 18:34:42Z gunter $
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef JLeicRunMessenger_h
#define JLeicRunMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4ios.hh"

class G4UIdirectory;

class G4UIcmdWithAnInteger;

class G4UIcmdWithADoubleAndUnit;

class G4UIcmdWithADouble;

class G4UIcmdWithAString;

class JLeicHistoMConfig : public G4UImessenger
{
public:

    JLeicHistoMConfig();

    ~JLeicHistoMConfig();

    void SetNewValue(G4UIcommand *, G4String) override;

private:


    G4UIdirectory *RndmDir;
    G4UIcmdWithAnInteger *RndmSaveCmd;
    G4UIcmdWithAString *RndmReadCmd;



};

#endif

