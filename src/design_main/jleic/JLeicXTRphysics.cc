#include "JLeicXTRphysics.hh"

#include <spdlog/spdlog.h>

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessTable.hh"

// Processes

#include "G4HadronElasticProcess.hh"
#include "G4ParticleHPElasticData.hh"
#include "G4ParticleHPThermalScatteringData.hh"
#include "G4ParticleHPElastic.hh"
#include "G4ParticleHPThermalScattering.hh"

#include "G4NeutronInelasticProcess.hh"
#include "G4ParticleHPInelasticData.hh"
#include "G4ParticleHPInelastic.hh"

#include "G4HadronCaptureProcess.hh"
#include "G4ParticleHPCaptureData.hh"
#include "G4ParticleHPCapture.hh"

#include "G4HadronFissionProcess.hh"
#include "G4ParticleHPFissionData.hh"
#include "G4ParticleHPFission.hh"

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
#include "G4EmProcessOptions.hh"
#include "G4ProductionCuts.hh"

#include "G4SystemOfUnits.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4VXTRenergyLoss.hh"
#include "G4RegularXTRadiator.hh"
#include "G4TransparentRegXTRadiator.hh"
#include "G4GammaXTRadiator.hh"
#include "G4StrawTubeXTRadiator.hh"

#include "G4XTRGammaRadModel.hh"
#include "G4XTRRegularRadModel.hh"
#include "G4XTRTransparentRegRadModel.hh"
#include "JLeicXTRTransparentRegRadModel.hh"
#include "JLeicDetectorConstruction.hh"
#include "JLeicStepCut.hh"
#include "JLeicPhysicsList.hh"

#include "JLeicXTRphysicsMessenger.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JLeicXTRphysics::JLeicXTRphysics(JLeicDetectorConstruction *p, JLeicPhysicsList *pl, const G4String &name) : G4VPhysicsConstructor(name), fJLeicXTRphysicsMessenger(0)
{
    pDet = p;
    pList = pl;
    SetXTRModel("none");
    fJLeicXTRphysicsMessenger = new JLeicXTRphysicsMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

JLeicXTRphysics::~JLeicXTRphysics()
{
    delete fJLeicXTRphysicsMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void JLeicXTRphysics::ConstructProcess()
{
    namespace log = spdlog;

    log::debug("XTR model = {}\n", fXTRModel);

    const G4RegionStore *theRegionStore = G4RegionStore::GetInstance();
    G4Region *gas = theRegionStore->GetRegion("XTRdEdxDetector");

    G4VXTRenergyLoss *processXTR = 0;

    if (fXTRModel == "gammaR" && pDet->GetFoilNumber() > 0) {
        // G4GammaXTRadiator*
        processXTR = new G4GammaXTRadiator(pDet->GetLogicalRadiator(), 100.,   //--  AlphaPlate 100
                                           100.,   //--  AlphaGas   100
                                           pDet->GetFoilMaterial(), pDet->GetGasMaterial(), pDet->GetFoilThick(), pDet->GetGasThick(), pDet->GetFoilNumber(), "GammaXTRadiator");
    } else if (fXTRModel == "gammaM" && pDet->GetFoilNumber() > 0) {
        // G4XTRGammaRadModel*
        processXTR = new G4XTRGammaRadModel(pDet->GetLogicalRadiator(), 100., 100., pDet->GetFoilMaterial(), pDet->GetGasMaterial(), pDet->GetFoilThick(), pDet->GetGasThick(),
                                            pDet->GetFoilNumber(), "GammaXTRadiator");
    } else if (fXTRModel == "strawR" && pDet->GetFoilNumber() > 0) {

        // G4StrawTubeXTRadiator*
        processXTR = new G4StrawTubeXTRadiator(pDet->GetLogicalRadiator(), pDet->GetFoilMaterial(), pDet->GetGasMaterial(), 0.53,           // pDet->GetFoilThick(),
                                               3.14159,           // pDet->GetGasThick(),
                                               pDet->GetAbsorberMaterial(), true, "strawXTRadiator");
    } else if (fXTRModel == "regR" && pDet->GetFoilNumber() > 0) {
        // G4RegularXTRadiator*
        processXTR = new G4RegularXTRadiator(pDet->GetLogicalRadiator(), pDet->GetFoilMaterial(), pDet->GetGasMaterial(), pDet->GetFoilThick(), pDet->GetGasThick(),
                                             pDet->GetFoilNumber(), "RegularXTRadiator");
    } else if (fXTRModel == "transpR" && pDet->GetFoilNumber() > 0) {
        // G4TransparentRegXTRadiator*
        processXTR = new G4TransparentRegXTRadiator(pDet->GetLogicalRadiator(), pDet->GetFoilMaterial(), pDet->GetGasMaterial(), pDet->GetFoilThick(), pDet->GetGasThick(),
                                                    pDet->GetFoilNumber(), "RegularXTRadiator");
    } else if (fXTRModel == "regM" && pDet->GetFoilNumber() > 0) {
        // G4XTRRegularRadModel*
        processXTR = new G4XTRRegularRadModel(pDet->GetLogicalRadiator(), pDet->GetFoilMaterial(), pDet->GetGasMaterial(), pDet->GetFoilThick(), pDet->GetGasThick(),
                                              pDet->GetFoilNumber(), "RegularXTRadiator");

    } else if (fXTRModel == "transpM" && pDet->GetFoilNumber() > 0) {
        processXTR = new JLeicXTRTransparentRegRadModel(pDet->GetLogicalRadiator(), pDet->GetFoilMaterial(), pDet->GetGasMaterial(), pDet->GetFoilThick(), pDet->GetGasThick(),
                                                        pDet->GetFoilNumber(), "RegularXTRadiator");
    } else {
        log::warn("Invalid XTR model name {}, or foil number = {} \n", fXTRModel, pDet->GetFoilNumber());
    }

    if (processXTR) processXTR->SetVerboseLevel(1);
    if (processXTR) processXTR->SetAngleRadDistr(true);

    auto theParticleIterator = GetParticleIterator();
    theParticleIterator->reset();

    while ((*theParticleIterator)()) {
        G4ParticleDefinition *particle = theParticleIterator->value();
        G4ProcessManager *pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();

        log::trace("JLeicXTRphysics::ConstructEM():: particle={} \n", particle->GetParticleName().c_str());

        if (particleName == "gamma") {
            // Construct processes for gamma

            pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
            pmanager->AddDiscreteProcess(new G4ComptonScattering);
            pmanager->AddDiscreteProcess(new G4GammaConversion);

        } else if (particleName == "e-") {
            // Construct processes for electron
            theeminusStepCut = new JLeicStepCut();
            theeminusStepCut->SetMaxStep(MaxChargedStep);
            //theeminusStepCut->SetMaxStep(100*um) ;
            G4eIonisation *eioni = new G4eIonisation();
            G4PAIModel *pai = new G4PAIModel(particle, "PAIModel");
            if (gas) eioni->AddEmModel(0, pai, pai, gas);

            pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
            //pmanager->AddProcess(new G4eMultipleScattering,-1,-1,-1);
            pmanager->AddProcess(eioni, -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung, -1, 3, 3);
            if (processXTR) pmanager->AddDiscreteProcess(processXTR);
            pmanager->AddDiscreteProcess(new G4SynchrotronRadiation);
            pmanager->AddDiscreteProcess(theeminusStepCut);

        } else if (particleName == "e+") {
            // Construct processes for positron

            theeplusStepCut = new JLeicStepCut();
            theeplusStepCut->SetMaxStep(MaxChargedStep);
            G4eIonisation *eioni = new G4eIonisation();
            G4PAIModel *pai = new G4PAIModel(particle, "PAIModel");
            if (gas) eioni->AddEmModel(0, pai, pai, gas);

            pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
            //pmanager->AddProcess(new G4eMultipleScattering,-1,-1,-1);
            pmanager->AddProcess(eioni, -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung, -1, 3, 3);
            pmanager->AddProcess(new G4eplusAnnihilation, 0, -1, 4);
            if (processXTR) pmanager->AddDiscreteProcess(processXTR);
            pmanager->AddDiscreteProcess(new G4SynchrotronRadiation);
            pmanager->AddDiscreteProcess(theeplusStepCut);

        } else if (particleName == "mu+" || particleName == "mu-") {
            // Construct processes for muon+

            JLeicStepCut *muonStepCut = new JLeicStepCut();
            muonStepCut->SetMaxStep(MaxChargedStep);

            G4MuIonisation *muioni = new G4MuIonisation();

            G4PAIModel *pai = new G4PAIModel(particle, "PAIModel");
            if (gas) muioni->AddEmModel(0, pai, pai, gas);

            pmanager->AddProcess(new G4MuMultipleScattering(), -1, 1, 1);
            pmanager->AddProcess(muioni, -1, 2, 2);
            pmanager->AddProcess(new G4MuBremsstrahlung(), -1, 3, 3);
            pmanager->AddProcess(new G4MuPairProduction(), -1, 4, 4);
            pmanager->AddProcess(muonStepCut, -1, -1, 5);
            //if (processXTR) pmanager->AddDiscreteProcess(processXTR);

        } else if (particleName == "proton" || particleName == "antiproton" || particleName == "pi+" || particleName == "pi-" || particleName == "kaon+" ||
                   particleName == "kaon-") {
            JLeicStepCut *thehadronStepCut = new JLeicStepCut();
            thehadronStepCut->SetMaxStep(MaxChargedStep);

            G4hIonisation *thehIonisation = new G4hIonisation();
            G4PAIModel *pai = new G4PAIModel(particle, "PAIModel");
            if (gas) thehIonisation->AddEmModel(0, pai, pai, gas);

            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(thehIonisation, -1, 2, 2);
            pmanager->AddProcess(thehadronStepCut, -1, -1, 3);
            //if (processXTR) pmanager->AddDiscreteProcess(processXTR);
        } //-- charged hadrons --
    }
    std::cout << " Exit ConstructEM " << std::endl;
    G4EmProcessOptions opt;
    opt.SetApplyCuts(true);


}

void JLeicXTRphysics::SetCuts()
{

    G4Region *region;
    //* uncomment for FDC & depfet !!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (!fRadiatorCuts) SetRadiatorCuts();
    region = (G4RegionStore::GetInstance())->GetRegion("XTRradiator");
    if (region) region->SetProductionCuts(fRadiatorCuts);
    G4cout << "Radiator cuts are set" << G4endl;

    if (!fDetectorCuts) SetDetectorCuts();
    region = (G4RegionStore::GetInstance())->GetRegion("XTRdEdxDetector");
    if (region) region->SetProductionCuts(fDetectorCuts);
    G4cout << "Detector cuts are set" << G4endl;

}

///////////////////////////////////////////////////////////////////////////

void JLeicXTRphysics::SetGammaCut(G4double val)
{
    printf("JLeicXTRphysics::SetGammaCut(%f) \n", val);
    cutForGamma = val;
}

///////////////////////////////////////////////////////////////////////////

void JLeicXTRphysics::SetElectronCut(G4double val)
{
    cutForElectron = val;
}

////////////////////////////////////////////////////////////////////////////

void JLeicXTRphysics::SetMaxStep(G4double step)
{
    MaxChargedStep = step;
    G4cout << " MaxChargedStep=" << MaxChargedStep << G4endl;
    G4cout << G4endl;
}

/////////////////////////////////////////////////////

void JLeicXTRphysics::SetRadiatorCuts()
{
    if (!fRadiatorCuts) fRadiatorCuts = new G4ProductionCuts();

    fRadiatorCuts->SetProductionCut(fGammaCut, idxG4GammaCut);
    fRadiatorCuts->SetProductionCut(fElectronCut, idxG4ElectronCut);
    fRadiatorCuts->SetProductionCut(fPositronCut, idxG4PositronCut);

    G4cout << "Radiator gamma cut    = " << fGammaCut / mm << " mm" << G4endl;
    G4cout << "Radiator electron cut = " << fElectronCut / mm << " mm" << G4endl;
    G4cout << "Radiator positron cut = " << fPositronCut / mm << " mm" << G4endl;
}

/////////////////////////////////////////////////////////////

void JLeicXTRphysics::SetDetectorCuts()
{
    if (!fDetectorCuts) fDetectorCuts = new G4ProductionCuts();

    fDetectorCuts->SetProductionCut(fGammaCut, idxG4GammaCut);
    fDetectorCuts->SetProductionCut(fElectronCut, idxG4ElectronCut);
    fDetectorCuts->SetProductionCut(fPositronCut, idxG4PositronCut);

    G4cout << "Detector gamma cut    = " << fGammaCut / mm << " mm" << G4endl;
    G4cout << "Detector electron cut = " << fElectronCut / mm << " mm" << G4endl;
    G4cout << "Detector positron cut = " << fPositronCut / mm << " mm" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
