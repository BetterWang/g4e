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
/// \file electromagnetic/VertexEIC/src/JLeicPhysicsList.cc
/// \brief Implementation of the JLeicPhysicsList class
//
//
// $Id: JLeicPhysicsList.cc  2019-02-19  --- JF---- $
//

#include "JLeicPhysicsList.hh"
#include "JLeicDetectorConstruction.hh"
#include "JLeicPhysicsListMessenger.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleTypes.hh"
#include "G4Material.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include <iomanip>

#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "G4ProductionCuts.hh"

//#include "HadronElasticPhysicsHP.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4Decay.hh"


#include "JLeicStepCut.hh"

#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

//--- hadron inelastic ---

#include "G4ProtonInelasticProcess.hh"
#include "G4BinaryCascade.hh"
#include "G4ProtonInelasticCrossSection.hh"
#include "G4NeutronInelasticProcess.hh"
#include "G4BinaryCascade.hh"
#include "G4NeutronInelasticCrossSection.hh"
#include "G4HadronFissionProcess.hh"
#include "G4LFission.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4NeutronHPCapture.hh"

#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4DecayPhysics.hh"
#include "JLeicXTRphysics.hh"

/////////////////////////////////////////////////////////////
//
//

JLeicPhysicsList::JLeicPhysicsList(JLeicDetectorConstruction* p)
  :  G4VModularPhysicsList(),
     MaxChargedStep(DBL_MAX),
     //MaxChargedStep(100*um),
     theeminusStepCut(0),            theeplusStepCut(0),
  fRadiatorCuts(0),fDetectorCuts(0),fXTRModel("transpM")
{
  pDet = p;

  // world cuts

  SetVerboseLevel(1);

  defaultCutValue = 1.0*mm; 
  //defaultCutValue = 0.001*mm;  // 1 um


  /*
    G4EmStandardPhysics
    G4Decay
    G4EmExtraPhysics
    -- Eg: gamma-nuclear
    G4HadronElasticPhysics
    G4HadronInelasticPhysics
    G4StoppingPhysics
    -- For particle at rest
    G4IonPhysics
  */

  //RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP()); 
  // all physics processes having to do with protons
  RegisterPhysics( new G4EmStandardPhysics_option4() );
  // all physics processes having to do with electrons
  RegisterPhysics( new G4DecayPhysics() );
  // physics of unstable particles

  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;

  // Region cuts

  fGammaCut    = defaultCutValue;
  fElectronCut = defaultCutValue;
  fPositronCut = defaultCutValue;

  printf(" default:: Set cuts for XTR processes: gamma=%f (%p) , e- = %f, e+ = %f, \n",cutForGamma,&cutForGamma,cutForElectron,cutForPositron);

  // Transition Radiation Physics
  XTRphys = new JLeicXTRphysics(pDet,this,"XTRmodel");
  RegisterPhysics(XTRphys);

  SetVerboseLevel(1);
  physicsListMessenger = new JLeicPhysicsListMessenger(this);
  printf(" end constr:: :: Set cuts for XTR processes: gamma=%f, e- = %f, e+ = %f, \n",cutForGamma,cutForElectron,cutForPositron);
}

JLeicPhysicsList::~JLeicPhysicsList()
{
  delete physicsListMessenger; 
}

///////////////////////////////////////////////////////////////////////
//
//
void JLeicPhysicsList::ConstructParticle()
{
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();  
}

///////////////////////////////////////////////////////////////////////
//
//
/*
void JLeicPhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM();
  //ConstructHAD();
  //ConstructGeneral();
}
*/
/////////////////////////////////////////////////////////////////////////////
//
//

void JLeicPhysicsList::SetCuts()
{
  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma

  SetCutsWithDefault();
  if (verboseLevel > 0)
  {
    G4cout << "JLeicPhysicsList::SetCuts:";
    G4cout << "CutLength for e-, e+ and gamma is: " 
           << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }
   // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
  printf(" Set cuts for XTR processes: gamma=%f, e- = %f (%p) , e+ = %f, \n",cutForGamma,&cutForGamma,cutForElectron,cutForPositron);

  
  SetCutValue(cutForGamma, "gamma", "DefaultRegionForTheWorld");
  SetCutValue(cutForElectron, "e-", "DefaultRegionForTheWorld");
  SetCutValue(cutForPositron, "e+", "DefaultRegionForTheWorld");
  XTRphys->SetCuts();
  

  //if (verboseLevel > 1)     
    DumpCutValuesTable();
}

