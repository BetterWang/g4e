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
//
//

// ConeParticleGun

#include <cmath>

#include "ConeParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryParticle.hh"
#include "ConeParticleGunMessenger.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "Randomize.hh"


ConeParticleGun::ConeParticleGun()
{
    SetInitialValues();
}

ConeParticleGun::ConeParticleGun(G4int numberofparticles)
{
    SetInitialValues();
    NumberOfParticlesToBeGenerated = numberofparticles;
}

ConeParticleGun::ConeParticleGun(G4ParticleDefinition *particleDef, G4int numberofparticles)
{
    SetInitialValues();
    NumberOfParticlesToBeGenerated = numberofparticles;
    SetParticleDefinition(particleDef);
}

void ConeParticleGun::SetInitialValues()
{
    NumberOfParticlesToBeGenerated = 1;
    particle_definition = nullptr;
    G4ThreeVector zero;
    particle_momentum_direction = (G4ParticleMomentum) zero;
    particle_energy = 0.0;
    particle_momentum = 0.0;
    particle_position = zero;
    particle_time = 0.0;
    particle_polarization = zero;
    particle_charge = 0.0;
    theMessenger = new ConeParticleGunMessenger(this);
}

ConeParticleGun::~ConeParticleGun()
{
    delete theMessenger;
}

//ConeParticleGun::ConeParticleGun(const ConeParticleGun& /*right*/)
//:G4VPrimaryGenerator()
//{ G4Exception(
//  "ConeParticleGun::ConeParticleGun","Event0191",FatalException,
//  "ConeParticleGun : Copy constructor should not be used."); }
//
//const ConeParticleGun& ConeParticleGun::operator=(const ConeParticleGun& right)
//{ G4Exception(
//  "ConeParticleGun::operator=","Event0192",FatalException,
//  "ConeParticleGun : Equal operator should not be used.");
//  return right; }
//
//G4bool ConeParticleGun::operator==(const ConeParticleGun& /*right*/) const
//{ G4Exception(
//  "ConeParticleGun::operator==","Event0193",FatalException,
//  "ConeParticleGun : == operator should not be used.");
//  return true; }
//
//G4bool ConeParticleGun::operator!=(const ConeParticleGun& /*right*/) const
//{ G4Exception(
//  "ConeParticleGun::operator!=","Event0193",FatalException,
//  "ConeParticleGun : != operator should not be used.");
//  return false; }

void ConeParticleGun::SetParticleDefinition(G4ParticleDefinition *aParticleDefinition)
{
    if (!aParticleDefinition) {
        G4Exception("ConeParticleGun::SetParticleDefinition()", "Event0101", FatalException, "Null pointer is given.");
    }
    if (aParticleDefinition->IsShortLived()) {
        if (!(aParticleDefinition->GetDecayTable())) {
            G4ExceptionDescription ED;
            ED << "ConeParticleGun does not support shooting a short-lived particle without a valid decay table." << G4endl;
            ED << "ConeParticleGun::SetParticleDefinition for " << aParticleDefinition->GetParticleName() << " is ignored." << G4endl;
            G4Exception("ConeParticleGun::SetParticleDefinition()", "Event0102", JustWarning, ED);
            return;
        }
    }
    particle_definition = aParticleDefinition;
    particle_charge = particle_definition->GetPDGCharge();
    if (particle_momentum > 0.0) {
        G4double mass = particle_definition->GetPDGMass();
        particle_energy = std::sqrt(particle_momentum * particle_momentum + mass * mass) - mass;
    }
}

void ConeParticleGun::SetParticleEnergy(G4double aKineticEnergy)
{
    particle_energy = aKineticEnergy;
    if (particle_momentum > 0.0) {
        if (particle_definition) {
            G4cout << "ConeParticleGun::" << particle_definition->GetParticleName() << G4endl;
        } else {
            G4cout << "ConeParticleGun::" << " " << G4endl;
        }
        G4cout << " was defined in terms of Momentum: " << particle_momentum / GeV << "GeV/c" << G4endl;
        G4cout << " is now defined in terms of KineticEnergy: " << particle_energy / GeV << "GeV" << G4endl;
        particle_momentum = 0.0;
    }
}

void ConeParticleGun::SetParticleMomentum(G4double aMomentum)
{
    if (particle_energy > 0.0) {
        if (particle_definition) {
            G4cout << "ConeParticleGun::" << particle_definition->GetParticleName() << G4endl;
        } else {
            G4cout << "ConeParticleGun::" << " " << G4endl;
        }
        G4cout << " was defined in terms of KineticEnergy: " << particle_energy / GeV << "GeV" << G4endl;
        G4cout << " is now defined in terms Momentum: " << aMomentum / GeV << "GeV/c" << G4endl;
    }
    if (!particle_definition) {
        G4cout << "Particle Definition not defined yet for ConeParticleGun" << G4endl;
        G4cout << "Zero Mass is assumed" << G4endl;
        particle_momentum = aMomentum;
        particle_energy = aMomentum;
    } else {
        G4double mass = particle_definition->GetPDGMass();
        particle_momentum = aMomentum;
        particle_energy = std::sqrt(particle_momentum * particle_momentum + mass * mass) - mass;
    }
}

void ConeParticleGun::SetParticleMomentum(G4ParticleMomentum aMomentum)
{
    if (particle_energy > 0.0) {
        if (particle_definition) {
            G4cout << "ConeParticleGun::" << particle_definition->GetParticleName() << G4endl;
        } else {
            G4cout << "ConeParticleGun::" << " " << G4endl;
        }
        G4cout << " was defined in terms of KineticEnergy: " << particle_energy / GeV << "GeV" << G4endl;
        G4cout << " is now defined in terms Momentum: " << aMomentum.mag() / GeV << "GeV/c" << G4endl;
    }
    if (!particle_definition) {
        G4cout << "Particle Definition not defined yet for ConeParticleGun" << G4endl;
        G4cout << "Zero Mass is assumed" << G4endl;
        particle_momentum_direction = aMomentum.unit();
        particle_momentum = aMomentum.mag();
        particle_energy = aMomentum.mag();
    } else {
        G4double mass = particle_definition->GetPDGMass();
        particle_momentum = aMomentum.mag();
        particle_momentum_direction = aMomentum.unit();
        particle_energy = std::sqrt(particle_momentum * particle_momentum + mass * mass) - mass;
    }
}

void ConeParticleGun::GeneratePrimaryVertex(G4Event *evt)
{
    if (!particle_definition) {
        G4ExceptionDescription ED;
        ED << "Particle definition is not defined." << G4endl;
        ED << "ConeParticleGun::SetParticleDefinition() has to be invoked beforehand." << G4endl;
        G4Exception("ConeParticleGun::GeneratePrimaryVertex()", "Event0109", FatalException, ED);
        return;
    }

    // create a new vertex
    G4PrimaryVertex *vertex = new G4PrimaryVertex(
            G4RandGauss::shoot(particle_position.x(), fPositionStdDev.x()),
            G4RandGauss::shoot(particle_position.y(), fPositionStdDev.y()),
            G4RandGauss::shoot(particle_position.z(), fPositionStdDev.z()),
            particle_time);



    // create new primaries and set them to the vertex
    G4double mass = particle_definition->GetPDGMass();
    for (G4int i = 0; i < NumberOfParticlesToBeGenerated; i++) {

        // smear direction
        double px = particle_momentum_direction.getX();
        double py = particle_momentum_direction.getY();
        double pz = particle_momentum_direction.getZ();
        double ptot = particle_momentum_direction.mag();


        double theta = acos(pz / ptot);
        double phi = atan2(py, px);

        double deltaTheta = G4RandGauss::shoot(fConeAngle / rad, fConeAngleStdDev / rad);
        double conePhi = G4UniformRand() * 2. * M_PI; // --- flat in phi
        G4ThreeVector smearedDirection(std::sin(deltaTheta) * std::cos(conePhi), std::sin(deltaTheta) * std::sin(conePhi), std::cos(deltaTheta));
        smearedDirection.rotateY(theta);
        smearedDirection.rotateZ(phi);

        G4PrimaryParticle *particle = new G4PrimaryParticle(particle_definition);
        particle->SetKineticEnergy(G4RandGauss::shoot(particle_energy, fEnergyStdDev));
        particle->SetMass(mass);
        particle->SetMomentumDirection(smearedDirection.unit());
        particle->SetCharge(particle_charge);
        particle->SetPolarization(particle_polarization.x(), particle_polarization.y(), particle_polarization.z());
        vertex->SetPrimary(particle);
    }

    evt->AddPrimaryVertex(vertex);
}

void ConeParticleGun::SetParticlePositionStdDev(G4ThreeVector vector)
{
    fPositionStdDev = vector;


}

void ConeParticleGun::SetParticleEnergyStdDev(G4double d)
{
    fEnergyStdDev = d;

}

void ConeParticleGun::SetConeAngleStdDev(G4double d)
{
    fConeAngleStdDev = d;
}

void ConeParticleGun::SetConeAngle(G4double d) {
    fConeAngle = d;
}


