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
/// \file electromagnetic/VertexEIC/include/JLeicPhysicsList.hh
/// \brief Definition of the JLeicPhysicsList class
//
//
//  JLeicPhysicsList.hh 2019-02-19  ---
//

#ifndef JLeicPhysicsList_h
#define JLeicPhysicsList_h 1

//#include "G4VUserPhysicsList.hh"
#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class G4ForwardXrayTR;

class JLeicStepCut;

class JLeicDetectorConstruction;

class JLeicPhysicsListMessenger;

class G4ProductionCuts;

class JLeicXTRphysics;

class JLeicPhysicsList : public G4VModularPhysicsList
{
public:
    JLeicPhysicsList(JLeicDetectorConstruction *);

    ~JLeicPhysicsList();

    // Construct particle and physics
    void ConstructParticle();
    //void ConstructProcess();

    void SetCuts();

private:

public:

private:

    G4double MaxChargedStep;

    G4ForwardXrayTR *fForwardXrayTR;

    JLeicStepCut *fEMinusStepCut;
    JLeicStepCut *fEPlusStepCut;

    G4double cutForGamma;
    G4double cutForElectron, cutForPositron;

    JLeicDetectorConstruction *pDet;
    JLeicXTRphysics *XTRphys;

    JLeicPhysicsListMessenger *physicsListMessenger;

    G4ProductionCuts *fRadiatorCuts;
    G4ProductionCuts *fDetectorCuts;
    G4double fElectronCut, fGammaCut, fPositronCut;
    G4String fXTRModel;
};

#endif



