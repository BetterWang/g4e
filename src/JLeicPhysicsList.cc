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
#include "G4EmProcessOptions.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"
#include "G4PAIModel.hh"
#include "G4PAIPhotModel.hh"

#include "G4SynchrotronRadiation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

#include "G4Decay.hh"

#include "G4VXTRenergyLoss.hh"
#include "G4RegularXTRadiator.hh"
#include "G4TransparentRegXTRadiator.hh"
#include "G4GammaXTRadiator.hh"
#include "G4StrawTubeXTRadiator.hh"

#include "G4XTRGammaRadModel.hh"
#include "G4XTRRegularRadModel.hh"
#include "G4XTRTransparentRegRadModel.hh"
#include "JLeicXTRTransparentRegRadModel.hh"

#include "JLeicStepCut.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

/////////////////////////////////////////////////////////////
//
//

JLeicPhysicsList::JLeicPhysicsList(JLeicDetectorConstruction* p)
  :  G4VUserPhysicsList(),
     MaxChargedStep(DBL_MAX),
     //MaxChargedStep(100*um),
     theeminusStepCut(0),            theeplusStepCut(0),
     fRadiatorCuts(0),fDetectorCuts(0),fXTRModel("transpM")
{
  pDet = p;

  // world cuts

  defaultCutValue = 1.*mm;
  //defaultCutValue = 0.001*mm;  // 1 um
  cutForGamma     = defaultCutValue;
  cutForElectron  = defaultCutValue;
  cutForPositron  = defaultCutValue;

  // Region cuts

  fGammaCut    = defaultCutValue;
  fElectronCut = defaultCutValue;
  fPositronCut = defaultCutValue;

  SetVerboseLevel(1);
  physicsListMessenger = new JLeicPhysicsListMessenger(this);
}

JLeicPhysicsList::~JLeicPhysicsList()
{
  delete physicsListMessenger; 
}

///////////////////////////////////////////////////////////////////////////
//
//

void JLeicPhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  /*
  G4BosonConstructor  pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  //  G4IonConstructor pIonConstructor;
  //  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();
  */
  //---- old ---

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBarions();
}

////////////////////////////////////////////////////////////////////////////
//
//

void JLeicPhysicsList::ConstructBosons()
{
  // gamma
  G4Gamma::GammaDefinition();
}

void JLeicPhysicsList::ConstructLeptons()
{
  // leptons

  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();

  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
}

void JLeicPhysicsList::ConstructMesons()
{
 //  mesons

  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
}


void JLeicPhysicsList::ConstructBarions()
{
//  barions

  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();
}


///////////////////////////////////////////////////////////////////////
//
//

void JLeicPhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM();
  ConstructGeneral();
}

/////////////////////////////////////////////////////////////////////////////
//
//

void JLeicPhysicsList::ConstructEM()
{
  
  // G4cout<<"fMinElectronEnergy = "<<fMinElectronEnergy/keV<<" keV"<<G4endl;
  // G4cout<<"fMinGammaEnergy = "<<fMinGammaEnergy/keV<<" keV"<<G4endl;
  G4cout<<"XTR model = "<<fXTRModel<<G4endl;

  const G4RegionStore* theRegionStore = G4RegionStore::GetInstance();
  G4Region* gas = theRegionStore->GetRegion("XTRdEdxDetector");

  G4VXTRenergyLoss* processXTR = 0;

  if(fXTRModel == "gammaR" )          
  {      
    // G4GammaXTRadiator* 
    processXTR = new G4GammaXTRadiator(pDet->GetLogicalRadiator(),
				       100.,   //--  AlphaPlate 100
				       100.,   //--  AlphaGas   100
                                       pDet->GetFoilMaterial(),
                                       pDet->GetGasMaterial(),
                                       pDet->GetFoilThick(),
                                       pDet->GetGasThick(),
                                       pDet->GetFoilNumber(),
                                       "GammaXTRadiator");
  }
  else if(fXTRModel == "gammaM" ) 
  {
    // G4XTRGammaRadModel* 
    processXTR = new G4XTRGammaRadModel(pDet->GetLogicalRadiator(),
                                       100.,
                                       100.,
                                       pDet->GetFoilMaterial(),
                                       pDet->GetGasMaterial(),
                                       pDet->GetFoilThick(),
                                       pDet->GetGasThick(),
                                       pDet->GetFoilNumber(),
                                       "GammaXTRadiator");
  }
  else if(fXTRModel == "strawR" ) 
  {

    // G4StrawTubeXTRadiator* 
    processXTR = new G4StrawTubeXTRadiator(pDet->GetLogicalRadiator(),
                                         pDet->GetFoilMaterial(),
                                         pDet->GetGasMaterial(),
                                0.53,           // pDet->GetFoilThick(),
                                3.14159,           // pDet->GetGasThick(),
                                         pDet->GetAbsorberMaterial(),
                                         true,
                                         "strawXTRadiator");
  }
  else if(fXTRModel == "regR" ) 
  {      
    // G4RegularXTRadiator* 
    processXTR = new G4RegularXTRadiator(pDet->GetLogicalRadiator(),
                                         pDet->GetFoilMaterial(),
                                         pDet->GetGasMaterial(),
                                         pDet->GetFoilThick(),
                                         pDet->GetGasThick(),
                                         pDet->GetFoilNumber(),
                                         "RegularXTRadiator");            
  }
  else if(fXTRModel == "transpR" ) 
  {
    // G4TransparentRegXTRadiator* 
    processXTR = new G4TransparentRegXTRadiator(pDet->GetLogicalRadiator(),
                                         pDet->GetFoilMaterial(),
                                         pDet->GetGasMaterial(),
                                         pDet->GetFoilThick(),
                                         pDet->GetGasThick(),
                                         pDet->GetFoilNumber(),
                                         "RegularXTRadiator");
  }
  else if(fXTRModel == "regM" ) 
  {
    // G4XTRRegularRadModel* 
    processXTR = new G4XTRRegularRadModel(pDet->GetLogicalRadiator(),
                                         pDet->GetFoilMaterial(),
                                         pDet->GetGasMaterial(),
                                         pDet->GetFoilThick(),
                                         pDet->GetGasThick(),
                                         pDet->GetFoilNumber(),
                                         "RegularXTRadiator");
       
  }
  else if(fXTRModel == "transpM" ) 
  { 
    // G4XTRTransparentRegRadModel* 
    // processXTR = new G4XTRTransparentRegRadModel(pDet->GetLogicalRadiator(),
    processXTR = new JLeicXTRTransparentRegRadModel(pDet->GetLogicalRadiator(),
                                         pDet->GetFoilMaterial(),
                                         pDet->GetGasMaterial(),
                                         pDet->GetFoilThick(),
                                         pDet->GetGasThick(),
                                         pDet->GetFoilNumber(),
                                         "RegularXTRadiator");
  }     
  else
  {
    G4Exception("Invalid XTR model name", "InvalidSetup",
                 FatalException, "XTR model name is out of the name list");
  }     
  //  processXTR->SetCompton(true);
  processXTR->SetVerboseLevel(1);
  processXTR->SetAngleRadDistr(true);

  auto theParticleIterator=GetParticleIterator();
  theParticleIterator->reset();

  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma")
    {
      // Construct processes for gamma

      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
      pmanager->AddDiscreteProcess(new G4ComptonScattering);
      pmanager->AddDiscreteProcess(new G4GammaConversion);

    }
    else if (particleName == "e-")
    {
      // Construct processes for electron
      theeminusStepCut = new JLeicStepCut();
      theeminusStepCut->SetMaxStep(MaxChargedStep) ;
      //theeminusStepCut->SetMaxStep(100*um) ;
      G4eIonisation* eioni = new G4eIonisation();
      G4PAIModel*     pai = new G4PAIModel(particle,"PAIModel");
      eioni->AddEmModel(0,pai,pai,gas);

      pmanager->AddProcess(new G4eMultipleScattering,-1,1,1);
      //pmanager->AddProcess(new G4eMultipleScattering,-1,-1,-1);
      pmanager->AddProcess(eioni,-1,2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,-1,3,3);
      pmanager->AddDiscreteProcess(processXTR);
      pmanager->AddDiscreteProcess(new G4SynchrotronRadiation);
      pmanager->AddDiscreteProcess(theeminusStepCut);

    }
    else if (particleName == "e+")
    {
      // Construct processes for positron

      theeplusStepCut = new JLeicStepCut();
      theeplusStepCut->SetMaxStep(MaxChargedStep) ;
      G4eIonisation* eioni = new G4eIonisation();
      G4PAIModel*     pai = new G4PAIModel(particle,"PAIModel");
      eioni->AddEmModel(0,pai,pai,gas);

      pmanager->AddProcess(new G4eMultipleScattering,-1,1,1);
      //pmanager->AddProcess(new G4eMultipleScattering,-1,-1,-1);
      pmanager->AddProcess(eioni,-1,2,2);
      pmanager->AddProcess(new G4eBremsstrahlung,-1,3,3);
      pmanager->AddProcess(new G4eplusAnnihilation,0,-1,4);
      pmanager->AddDiscreteProcess(processXTR);
      pmanager->AddDiscreteProcess(new G4SynchrotronRadiation);
      pmanager->AddDiscreteProcess(theeplusStepCut);

    }
    else if( particleName == "mu+" ||
             particleName == "mu-"    )
    {
     // Construct processes for muon+

      JLeicStepCut* muonStepCut = new JLeicStepCut();
      muonStepCut->SetMaxStep(MaxChargedStep) ;

      G4MuIonisation* muioni = new G4MuIonisation() ;

      G4PAIModel*     pai = new G4PAIModel(particle,"PAIModel");
      muioni->AddEmModel(0,pai,pai,gas);

      pmanager->AddProcess(new G4MuMultipleScattering(),-1,1,1);
      pmanager->AddProcess(muioni,-1,2,2);
      pmanager->AddProcess(new G4MuBremsstrahlung(),-1,3,3);
      pmanager->AddProcess(new G4MuPairProduction(),-1,4,4);
      pmanager->AddProcess( muonStepCut,-1,-1,5);
      //pmanager->AddDiscreteProcess(processXTR);

    }
    else if (
                particleName == "proton"
               || particleName == "antiproton"
               || particleName == "pi+"
               || particleName == "pi-"
               || particleName == "kaon+"
               || particleName == "kaon-"
              )
    {
      JLeicStepCut* thehadronStepCut = new JLeicStepCut();
      thehadronStepCut->SetMaxStep(MaxChargedStep) ;

      G4hIonisation* thehIonisation = new G4hIonisation();
      G4PAIModel*     pai = new G4PAIModel(particle,"PAIModel");
      thehIonisation->AddEmModel(0,pai,pai,gas);

      pmanager->AddProcess(new G4hMultipleScattering,-1,1,1);
      pmanager->AddProcess(thehIonisation,-1,2,2);
      pmanager->AddProcess( thehadronStepCut,-1,-1,3);
      //pmanager->AddDiscreteProcess(processXTR);

    }
  }
  G4EmProcessOptions opt;
  opt.SetApplyCuts(true);
}

void JLeicPhysicsList::ConstructGeneral()
{
  // Add Decay Process

  G4Decay* theDecayProcess = new G4Decay();
  auto theParticleIterator=GetParticleIterator();
  theParticleIterator->reset();

  while( (*theParticleIterator)() )
  {
    G4ParticleDefinition* particle = theParticleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();

    if (theDecayProcess->IsApplicable(*particle)) 
    { 
      pmanager ->AddProcess(theDecayProcess);

      // set ordering for PostStepDoIt and AtRestDoIt

      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////

void JLeicPhysicsList::SetCuts()
{
  // set cut values for gamma at first and for e- second and next for e+,
  // because some processes for e+/e- need cut values for gamma
 
  SetCutValue(cutForGamma, "gamma", "DefaultRegionForTheWorld");
  SetCutValue(cutForElectron, "e-", "DefaultRegionForTheWorld");
  SetCutValue(cutForPositron, "e+", "DefaultRegionForTheWorld");

  if (verboseLevel > 0)
  {
    G4cout << "JLeicPhysicsList::SetCuts:";
    G4cout << "CutLength for e-, e+ and gamma is: " 
           << G4BestUnit(defaultCutValue,"Length") << G4endl;
  }
  G4Region* region;
  //* uncomment for FDC & depfet !!!!!!!!!!!!!!!!!!!!!!!!!!!
  if( !fRadiatorCuts ) SetRadiatorCuts();
  region = (G4RegionStore::GetInstance())->GetRegion("XTRradiator");
  region->SetProductionCuts(fRadiatorCuts);
  G4cout << "Radiator cuts are set" << G4endl;
  
  if( !fDetectorCuts ) SetDetectorCuts();
  region = (G4RegionStore::GetInstance())->GetRegion("XTRdEdxDetector");
  region->SetProductionCuts(fDetectorCuts);
  G4cout << "Detector cuts are set" << G4endl;
  
  if (verboseLevel > 1)     DumpCutValuesTable();
}

///////////////////////////////////////////////////////////////////////////

void JLeicPhysicsList::SetGammaCut(G4double val)
{
  cutForGamma = val;
}

///////////////////////////////////////////////////////////////////////////

void JLeicPhysicsList::SetElectronCut(G4double val)
{
  cutForElectron = val;
}

////////////////////////////////////////////////////////////////////////////

void JLeicPhysicsList::SetMaxStep(G4double step)
{
  MaxChargedStep = step ;
  G4cout << " MaxChargedStep=" << MaxChargedStep << G4endl;
  G4cout << G4endl;
}

/////////////////////////////////////////////////////

void JLeicPhysicsList::SetRadiatorCuts()
{
  if( !fRadiatorCuts ) fRadiatorCuts = new G4ProductionCuts();

  fRadiatorCuts->SetProductionCut(fGammaCut, idxG4GammaCut);
  fRadiatorCuts->SetProductionCut(fElectronCut, idxG4ElectronCut);
  fRadiatorCuts->SetProductionCut(fPositronCut, idxG4PositronCut);

  G4cout<<"Radiator gamma cut    = "<<fGammaCut/mm<<" mm"<<G4endl;
  G4cout<<"Radiator electron cut = "<<fElectronCut/mm<<" mm"<<G4endl;
  G4cout<<"Radiator positron cut = "<<fPositronCut/mm<<" mm"<<G4endl;
}

/////////////////////////////////////////////////////////////

void JLeicPhysicsList::SetDetectorCuts()
{
  if( !fDetectorCuts ) fDetectorCuts = new G4ProductionCuts();

  fDetectorCuts->SetProductionCut(fGammaCut, idxG4GammaCut);
  fDetectorCuts->SetProductionCut(fElectronCut, idxG4ElectronCut);
  fDetectorCuts->SetProductionCut(fPositronCut, idxG4PositronCut);

  G4cout<<"Detector gamma cut    = "<<fGammaCut/mm<<" mm"<<G4endl;
  G4cout<<"Detector electron cut = "<<fElectronCut/mm<<" mm"<<G4endl;
  G4cout<<"Detector positron cut = "<<fPositronCut/mm<<" mm"<<G4endl;

}
