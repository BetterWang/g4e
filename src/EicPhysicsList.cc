#include "EicPhysicsList.hh"
#include "main_detectors/DetectorConfig.hh"

#include "G4ProcessManager.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include <iomanip>

#include "G4RegionStore.hh"

#include "main_detectors/ReferenceDetectorStepCut.hh"

#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4LFission.hh"
#include "G4NeutronHPCapture.hh"
#include "G4Ions.hh"

#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4DecayPhysics.hh"
#include "subdetectors/ci_TRD/ci_TRD_PhysicsConstructor.hh"


EicPhysicsList::EicPhysicsList() :
    G4VModularPhysicsList(),
    MaxChargedStep(DBL_MAX),
    fEMinusStepCut(nullptr),
    fEPlusStepCut(nullptr),
    fRadiatorCuts(nullptr),
    fDetectorCuts(nullptr)
{
    // world cuts
    SetVerboseLevel(0);

    defaultCutValue = 1.0 * mm;
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

    //--- hadron physics !!! OFF by default
    RegisterPhysics( new G4HadronPhysicsQGSP_BIC_HP());


    //--- electrons physics  !!
    RegisterPhysics( new G4EmStandardPhysics_option3() );
    // all physics processes having to do with electrons

    auto decayPhysics = new G4DecayPhysics();
    decayPhysics->SetVerboseLevel(0);
    RegisterPhysics(decayPhysics);

    // physics of unstable particles

    cutForGamma = defaultCutValue;
    cutForElectron = defaultCutValue;
    cutForPositron = defaultCutValue;

    // Region cuts

    fGammaCut = defaultCutValue;
    fElectronCut = defaultCutValue;
    fPositronCut = defaultCutValue;
}

///////////////////////////////////////////////////////////////////////
//
//
void EicPhysicsList::ConstructParticle()
{
    G4BosonConstructor::ConstructParticle();
    G4LeptonConstructor::ConstructParticle();
    G4MesonConstructor::ConstructParticle();
    G4BaryonConstructor::ConstructParticle();
    G4IonConstructor::ConstructParticle();
    G4ShortLivedConstructor::ConstructParticle();
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

void EicPhysicsList::SetCuts()
{
    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma

    SetCutsWithDefault();
    if (verboseLevel > 0) {
        G4cout << "JLeicPhysicsList::SetCuts:";
        G4cout << "CutLength for e-, e+ and gamma is: " << G4BestUnit(defaultCutValue, "Length") << G4endl;
    }
    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    //printf(" Set cuts for XTR processes: gamma=%f, e- = %f (%p) , e+ = %f, \n", cutForGamma, &cutForGamma, cutForElectron, cutForPositron);

    //SetCutValue(cutForGamma, "gamma", "DefaultRegionForTheWorld");
    //SetCutValue(cutForElectron, "e-", "DefaultRegionForTheWorld");
    //SetCutValue(cutForPositron, "e+", "DefaultRegionForTheWorld");

    // XTRphys->SetCuts();  // Doesn't affect anything

    if (verboseLevel > 1) DumpCutValuesTable();
}


