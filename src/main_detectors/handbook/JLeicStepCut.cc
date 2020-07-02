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
/// \file electromagnetic/VertexEIC/src/JLeicStepCut.cc
/// \brief Implementation of the JLeicStepCut class
//
//
// $Id: JLeicStepCut.cc 66241 2012-12-13 18:34:42Z gunter $
//

#include "JLeicStepCut.hh"

#include "G4Step.hh"
#include "G4UserLimits.hh"
#include "G4VParticleChange.hh"
#include "G4EnergyLossTables.hh"

JLeicStepCut::JLeicStepCut(const G4String &aName) : G4VDiscreteProcess(aName), MaxChargedStep(DBL_MAX)
{
    if (verboseLevel > 0) {
        G4cout << GetProcessName() << " is created " << G4endl;
    }
}

JLeicStepCut::~JLeicStepCut()
{
}

JLeicStepCut::JLeicStepCut(JLeicStepCut &right) : G4VDiscreteProcess(right) {}

void JLeicStepCut::SetMaxStep(G4double step)
{
    MaxChargedStep = step;
}


