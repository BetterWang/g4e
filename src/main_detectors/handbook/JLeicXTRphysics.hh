#ifndef JLeicXTRphysics_HH
#define JLeicXTRphysics_HH

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4ForwardXrayTR;
class JLeicStepCut;
class JLeicDetectorConstruction;
class JLeicXTRphysicsMessenger;
class G4ProductionCuts;
class EicPhysicsList;

class JLeicXTRphysics: public G4VPhysicsConstructor
{
public:
  
  JLeicXTRphysics(JLeicDetectorConstruction* p, EicPhysicsList* pl, const G4String& name);

  ~JLeicXTRphysics() override;

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

  JLeicDetectorConstruction* pDet;
  EicPhysicsList* pList;

  JLeicXTRphysicsMessenger* fJLeicXTRphysicsMessenger;

  G4ProductionCuts* fRadiatorCuts;
  G4ProductionCuts* fDetectorCuts;
  G4double fElectronCut, fGammaCut, fPositronCut;
  G4String fXTRModel;
};

#endif



