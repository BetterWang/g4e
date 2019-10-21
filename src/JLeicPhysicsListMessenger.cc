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
/// \file electromagnetic/VertexEIC/src/JLeicPhysicsListMessenger.cc
/// \brief Implementation of the JLeicPhysicsListMessenger class
//
//
// $Id: JLeicPhysicsListMessenger.cc 67268 2013-02-13 11:38:40Z ihrivnac $
//
// 
//
///////////////////////////////////////////////////////////////////////



#include "JLeicPhysicsListMessenger.hh"

#include "JLeicPhysicsList.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"


//////////////////////////////////////////////////////////////////////////////
//
//

JLeicPhysicsListMessenger::JLeicPhysicsListMessenger(JLeicPhysicsList* List)
:G4UImessenger(),JLeicList(List)
{
  /*
  cutGCmd = new G4UIcmdWithADoubleAndUnit("/calor/cutG",this);
  cutGCmd->SetGuidance("Set cut values by RANGE for Gamma.");
  cutGCmd->SetParameterName("range",true);
  cutGCmd->SetDefaultValue(1.);
  cutGCmd->SetDefaultUnit("mm");
  cutGCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  cutECmd = new G4UIcmdWithADoubleAndUnit("/calor/cutE",this);
  cutECmd->SetGuidance("Set cut values by RANGE for e- e+.");
  cutECmd->SetParameterName("range",true);
  cutECmd->SetDefaultValue(1.);
  cutECmd->SetDefaultUnit("mm");
  cutECmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  setMaxStepCmd = new G4UIcmdWithADoubleAndUnit("/step/setMaxStep",this);
  setMaxStepCmd->SetGuidance("Set max. step length in the detector");
  setMaxStepCmd->SetParameterName("mxStep",true);
  setMaxStepCmd->SetDefaultUnit("mm");
  */


}

/////////////////////////////////////////////////////////////////////////////

JLeicPhysicsListMessenger::~JLeicPhysicsListMessenger()
{
  /*
  delete cutGCmd;
  delete cutECmd;

  delete setMaxStepCmd;

  delete ElectronCutCmd;
  delete PositronCutCmd;
  delete GammaCutCmd;
  delete XTRModelCmd;
  */
}

////////////////////////////////////////////////////////////////////////////////

void JLeicPhysicsListMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  printf("JLeicPhysicsListMessenger::SetNewValue string= %s \n",newValue.c_str());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

