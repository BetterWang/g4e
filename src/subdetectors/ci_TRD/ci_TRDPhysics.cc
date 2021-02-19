#include "ci_TRDPhysics.hh"

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

#include "G4Decay.hh"

#include "G4XTRGammaRadModel.hh"
#include "G4XTRRegularRadModel.hh"
#include "G4XTRTransparentRegRadModel.hh"
#include "JLeicXTRTransparentRegRadModel.hh"
#include "JLeicStepCut.hh"
#include "ci_TRD_Config.hh"
#include "ci_TRDPhysics.hh"
#include "ci_TRD_Design.hh"


ci_TRDPhysics::ci_TRDPhysics(ci_TRD_Design *p) :
    G4VPhysicsConstructor("ci_TRD_Physics")
{
    fTrd = p;
    SetXTRModel("none");
    SetVerboseLevel(0);
}




void ci_TRDPhysics::ConstructProcess()
{
    namespace log = spdlog;

    log::debug("XTR model = {}\n", fXTRModel);

    const G4RegionStore *theRegionStore = G4RegionStore::GetInstance();
    G4Region *gas = theRegionStore->GetRegion("XTRdEdxDetector");

    G4VXTRenergyLoss *processXTR = nullptr;

    auto foilNumber = fTrd->ConstructionConfig.fFoilNumber;
    auto radiator = fTrd->fLogicRadiator;
    auto absorberMaterial = fTrd->ConstructionConfig.det_Material;
    auto foilMaterial = fTrd->fFoilMat;
    auto gasMaterial = fTrd->fGasMat;
    auto foilThickness = fTrd->ConstructionConfig.fRadThickness;
    auto gasThickness = fTrd->ConstructionConfig.fGasGap;

    if (fXTRModel == "gammaR" && foilNumber > 0) {
        // G4GammaXTRadiator*
        processXTR = new G4GammaXTRadiator(radiator, 100.,   //--  AlphaPlate 100
                                           100.,   //--  AlphaGas   100
                                           foilMaterial, gasMaterial, foilThickness, gasThickness, foilNumber, "GammaXTRadiator");
    } else if (fXTRModel == "gammaM" && foilNumber > 0) {
        // G4XTRGammaRadModel*
        processXTR = new G4XTRGammaRadModel(radiator, 100., 100., foilMaterial, gasMaterial, foilThickness, gasThickness,
                                            foilNumber, "GammaXTRadiator");
    } else if (fXTRModel == "strawR" && foilNumber > 0) {

        // G4StrawTubeXTRadiator*
        processXTR = new G4StrawTubeXTRadiator(radiator, foilMaterial, gasMaterial, 0.53,           // foilThickness,
                                               3.14159,           // gasThickness,
                                               absorberMaterial, true, "strawXTRadiator");
    } else if (fXTRModel == "regR" && foilNumber > 0) {
        // G4RegularXTRadiator*
        processXTR = new G4RegularXTRadiator(radiator, foilMaterial, gasMaterial, foilThickness, gasThickness,
                                             foilNumber, "RegularXTRadiator");
    } else if (fXTRModel == "transpR" && foilNumber > 0) {
        // G4TransparentRegXTRadiator*
        processXTR = new G4TransparentRegXTRadiator(radiator, foilMaterial, gasMaterial, foilThickness, gasThickness,
                                                    foilNumber, "RegularXTRadiator");
    } else if (fXTRModel == "regM" && foilNumber > 0) {
        // G4XTRRegularRadModel*
        processXTR = new G4XTRRegularRadModel(radiator, foilMaterial, gasMaterial, foilThickness, gasThickness,
                                              foilNumber, "RegularXTRadiator");

    } else if (fXTRModel == "transpM" && foilNumber > 0) {
        processXTR = new JLeicXTRTransparentRegRadModel(radiator, foilMaterial, gasMaterial, foilThickness, gasThickness,
                                                        foilNumber, "RegularXTRadiator");
    } else {
        log::warn("Invalid XTR model name {}, or foil number = {} \n", fXTRModel, foilNumber);
    }

    if (processXTR) processXTR->SetVerboseLevel(0);
    if (processXTR) processXTR->SetAngleRadDistr(true);
 

    auto theParticleIterator = GetParticleIterator();
    theParticleIterator->reset();

    while ((*theParticleIterator)()) {
        G4ParticleDefinition *particle = theParticleIterator->value();
        G4ProcessManager *pmanager = particle->GetProcessManager();
        pmanager->SetVerboseLevel(0);
        G4String particleName = particle->GetParticleName();

        log::trace("ci_TRDPhysics::ConstructEM():: particle={} \n", particle->GetParticleName().c_str());
        //printf("ci_TRDPhysics::ConstructEM():: particle=%s \n", particle->GetParticleName().c_str());

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
            eioni->SetVerboseLevel(0);
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
            eioni->SetVerboseLevel(0);
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

    G4EmProcessOptions opt;
    opt.SetApplyCuts(true);


}

void ci_TRDPhysics::SetCuts()
{
    G4Region *region;
    //* uncomment for FDC & depfet !!!!!!!!!!!!!!!!!!!!!!!!!!!
    if (!fRadiatorCuts) SetRadiatorCuts();
    region = (G4RegionStore::GetInstance())->GetRegion("XTRradiator");
    if (region) region->SetProductionCuts(fRadiatorCuts);

    if (!fDetectorCuts) SetDetectorCuts();
    region = (G4RegionStore::GetInstance())->GetRegion("XTRdEdxDetector");
    if (region) region->SetProductionCuts(fDetectorCuts);
}

void ci_TRDPhysics::SetGammaCut(G4double val)
{
    cutForGamma = val;
}

void ci_TRDPhysics::SetElectronCut(G4double val)
{
    cutForElectron = val;
}

void ci_TRDPhysics::SetMaxStep(G4double step)
{
    MaxChargedStep = step;
}

void ci_TRDPhysics::SetRadiatorCuts()
{
    if (!fRadiatorCuts) fRadiatorCuts = new G4ProductionCuts();

    fRadiatorCuts->SetProductionCut(fGammaCut, idxG4GammaCut);
    fRadiatorCuts->SetProductionCut(fElectronCut, idxG4ElectronCut);
    fRadiatorCuts->SetProductionCut(fPositronCut, idxG4PositronCut);
}

void ci_TRDPhysics::SetDetectorCuts()
{
    if (!fDetectorCuts) fDetectorCuts = new G4ProductionCuts();

    fDetectorCuts->SetProductionCut(fGammaCut, idxG4GammaCut);
    fDetectorCuts->SetProductionCut(fElectronCut, idxG4ElectronCut);
    fDetectorCuts->SetProductionCut(fPositronCut, idxG4PositronCut);
}
