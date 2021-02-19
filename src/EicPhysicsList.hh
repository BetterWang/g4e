#ifndef JLeicPhysicsList_h
#define JLeicPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4ForwardXrayTR;

class JLeicStepCut;

class JLeicDetectorConfig;

class G4ProductionCuts;



class EicPhysicsList : public G4VModularPhysicsList
{
public:
    explicit EicPhysicsList();

    // Construct particle and physics
    void ConstructParticle() override;

    //    void ConstructProcess() override ;

    void SetCuts() override;

private:

public:


private:

    G4double MaxChargedStep;

    G4ForwardXrayTR *fForwardXrayTR;

    JLeicStepCut *fEMinusStepCut;
    JLeicStepCut *fEPlusStepCut;

    G4double cutForGamma;
    G4double cutForElectron, cutForPositron;

    G4ProductionCuts *fRadiatorCuts;
    G4ProductionCuts *fDetectorCuts;
    G4double fElectronCut, fGammaCut, fPositronCut;
};

#endif



