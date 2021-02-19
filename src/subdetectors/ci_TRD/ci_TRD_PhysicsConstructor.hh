#ifndef JLeicXTRphysics_HH
#define JLeicXTRphysics_HH

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class G4ForwardXrayTR;
class JLeicStepCut;
class G4ProductionCuts;
class ci_TRD_Design;

class ci_TRD_PhysicsConstructor: public G4VPhysicsConstructor
{
public:

    ci_TRD_PhysicsConstructor(ci_TRD_Design* design);

    ~ci_TRD_PhysicsConstructor() = default;

    virtual void ConstructParticle() { };

    // Construct physics
    virtual void ConstructProcess();
 

private:

  // these methods Construct physics processes and register them


public:

  void SetCuts();

  void SetGammaCut(G4double);
  void SetElectronCut(G4double);

  void SetRegGammaCut(G4double    cut){fGammaCut    = cut;}
  void SetRegElectronCut(G4double cut){fElectronCut = cut;}
  void SetRegPositronCut(G4double cut){fPositronCut = cut;}

  void SetRadiatorCuts();
  void SetDetectorCuts();

  void SetMaxStep(G4double);
  void SetXTRModel(G4String model) { fXTRModel = model; }
  G4double cutForGamma;
  G4double cutForElectron, cutForPositron;

private:

  G4double MaxChargedStep;

  G4ForwardXrayTR*       fForwardXrayTR ;

  JLeicStepCut* theeminusStepCut ;
  JLeicStepCut* theeplusStepCut ;

  ci_TRD_Design* fTrd;

  G4ProductionCuts* fRadiatorCuts;
  G4ProductionCuts* fDetectorCuts;
  G4double fElectronCut, fGammaCut, fPositronCut;
  G4String fXTRModel;
};

#endif



