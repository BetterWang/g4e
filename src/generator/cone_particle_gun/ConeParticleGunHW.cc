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

// ConeParticleGunHW

#include <cmath>

#include "ConeParticleGunHW.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryParticle.hh"
#include "ConeParticleGunMessengerHW.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "Randomize.hh"


ConeParticleGunHW::ConeParticleGunHW()
{
    SetInitialValues();
}

ConeParticleGunHW::ConeParticleGunHW(G4int numberofparticles)
{
    SetInitialValues();
    NumberOfParticlesToBeGenerated = numberofparticles;
}

ConeParticleGunHW::ConeParticleGunHW(G4ParticleDefinition *particleDef, G4int numberofparticles)
{
    SetInitialValues();
    NumberOfParticlesToBeGenerated = numberofparticles;
    SetParticleDefinition(particleDef);
}

void ConeParticleGunHW::SetInitialValues()
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
    theMessenger = new ConeParticleGunMessengerHW(this);
}

ConeParticleGunHW::~ConeParticleGunHW()
{
    delete theMessenger;
}

//ConeParticleGunHW::ConeParticleGunHW(const ConeParticleGunHW& /*right*/)
//:G4VPrimaryGenerator()
//{ G4Exception(
//  "ConeParticleGunHW::ConeParticleGunHW","Event0191",FatalException,
//  "ConeParticleGunHW : Copy constructor should not be used."); }
//
//const ConeParticleGunHW& ConeParticleGunHW::operator=(const ConeParticleGunHW& right)
//{ G4Exception(
//  "ConeParticleGunHW::operator=","Event0192",FatalException,
//  "ConeParticleGunHW : Equal operator should not be used.");
//  return right; }
//
//G4bool ConeParticleGunHW::operator==(const ConeParticleGunHW& /*right*/) const
//{ G4Exception(
//  "ConeParticleGunHW::operator==","Event0193",FatalException,
//  "ConeParticleGunHW : == operator should not be used.");
//  return true; }
//
//G4bool ConeParticleGunHW::operator!=(const ConeParticleGunHW& /*right*/) const
//{ G4Exception(
//  "ConeParticleGunHW::operator!=","Event0193",FatalException,
//  "ConeParticleGunHW : != operator should not be used.");
//  return false; }

void ConeParticleGunHW::SetParticleDefinition(G4ParticleDefinition *aParticleDefinition)
{
    if (!aParticleDefinition) {
        G4Exception("ConeParticleGunHW::SetParticleDefinition()", "Event0101", FatalException, "Null pointer is given.");
    }
    if (aParticleDefinition->IsShortLived()) {
        if (!(aParticleDefinition->GetDecayTable())) {
            G4ExceptionDescription ED;
            ED << "ConeParticleGunHW does not support shooting a short-lived particle without a valid decay table." << G4endl;
            ED << "ConeParticleGunHW::SetParticleDefinition for " << aParticleDefinition->GetParticleName() << " is ignored." << G4endl;
            G4Exception("ConeParticleGunHW::SetParticleDefinition()", "Event0102", JustWarning, ED);
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

void ConeParticleGunHW::SetParticleEnergy(G4double aKineticEnergy)
{
    particle_energy = aKineticEnergy;
    if (particle_momentum > 0.0) {
        if (particle_definition) {
            G4cout << "ConeParticleGunHW::" << particle_definition->GetParticleName() << G4endl;
        } else {
            G4cout << "ConeParticleGunHW::" << " " << G4endl;
        }
        G4cout << " was defined in terms of Momentum: " << particle_momentum / GeV << "GeV/c" << G4endl;
        G4cout << " is now defined in terms of KineticEnergy: " << particle_energy / GeV << "GeV" << G4endl;
        particle_momentum = 0.0;
    }
}

void ConeParticleGunHW::SetParticleMomentum(G4double aMomentum)
{
    if (particle_energy > 0.0) {
        if (particle_definition) {
            G4cout << "ConeParticleGunHW::" << particle_definition->GetParticleName() << G4endl;
        } else {
            G4cout << "ConeParticleGunHW::" << " " << G4endl;
        }
        G4cout << " was defined in terms of KineticEnergy: " << particle_energy / GeV << "GeV" << G4endl;
        G4cout << " is now defined in terms Momentum: " << aMomentum / GeV << "GeV/c" << G4endl;
    }
    if (!particle_definition) {
        G4cout << "Particle Definition not defined yet for ConeParticleGunHW" << G4endl;
        G4cout << "Zero Mass is assumed" << G4endl;
        particle_momentum = aMomentum;
        particle_energy = aMomentum;
    } else {
        G4double mass = particle_definition->GetPDGMass();
        particle_momentum = aMomentum;
        particle_energy = std::sqrt(particle_momentum * particle_momentum + mass * mass) - mass;
    }
}


/*Set mimimum momentum.
 *Very basic, but as long as it's used as intended... */
void ConeParticleGunHW::SetParticleMinMomentum(G4double aMomentum)
{
    particle_minMomentum = aMomentum;
}

/*Set maximum momentum*/
void ConeParticleGunHW::SetParticleMaxMomentum(G4double aMomentum)
{
    particle_maxMomentum = aMomentum;
}


/*Set mimimum eta*/
void ConeParticleGunHW::SetParticleMinEta(G4double aPseudorap)
{
    particle_minEta = aPseudorap;
}

/*Set maximum eta*/
void ConeParticleGunHW::SetParticleMaxEta(G4double aPseudorap)
{
    particle_maxEta = aPseudorap;
}


void ConeParticleGunHW::SetParticleMomentum(G4ParticleMomentum aMomentum)
{
    if (particle_energy > 0.0) {
        if (particle_definition) {
            G4cout << "ConeParticleGunHW::" << particle_definition->GetParticleName() << G4endl;
        } else {
            G4cout << "ConeParticleGunHW::" << " " << G4endl;
        }
        G4cout << " was defined in terms of KineticEnergy: " << particle_energy / GeV << "GeV" << G4endl;
        G4cout << " is now defined in terms Momentum: " << aMomentum.mag() / GeV << "GeV/c" << G4endl;
    }
    if (!particle_definition) {
        G4cout << "Particle Definition not defined yet for ConeParticleGunHW" << G4endl;
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

/* THIS IS THE KEY FUNCTION OF PARTICLE GENERATORS */
void ConeParticleGunHW::GeneratePrimaryVertex(G4Event *evt)
{
    if (!particle_definition) {
        G4ExceptionDescription ED;
        ED << "Particle definition is not defined." << G4endl;
        ED << "ConeParticleGunHW::SetParticleDefinition() has to be invoked beforehand." << G4endl;
        G4Exception("ConeParticleGunHW::GeneratePrimaryVertex()", "Event0109", FatalException, ED);
        return;
    }

    // create a new vertex
    // If standard deviation kept at 0: will just place it at the given position.
    G4PrimaryVertex *vertex = new G4PrimaryVertex(
            G4RandGauss::shoot(particle_position.x(), fPositionStdDev.x()),
            G4RandGauss::shoot(particle_position.y(), fPositionStdDev.y()),
            G4RandGauss::shoot(particle_position.z(), fPositionStdDev.z()),
            particle_time);



    // create new primaries and set them to the vertex
    G4double mass = particle_definition->GetPDGMass();
    for (G4int i = 0; i < NumberOfParticlesToBeGenerated; i++) {
		
		double phi = G4UniformRand()*2.0*M_PI; // --- flat in phi;
		
		//Theta will also give the direction, along with phi.
		//This is just for one particle.
		//Theta and phi define direction, momentum defines momentum.
		
		double eta = G4RandFlat::shoot(particle_minEta, particle_maxEta); //Gives flat distribution between the two values
		
		double theta = 2*atan(exp(-eta));
		
		//Direction from phi and theta
		double nx = sin(theta)*cos(phi);
		double ny = sin(theta)*sin(phi);
		double nz = cos(theta);
		//This always gives a unit vector
		
		G4ThreeVector randomisedDirection(nx, ny, nz);

		//std::cout << "ETA: " << eta << " THETA: " << theta*180.0/pi << " DIRECTION: " << randomisedDirection.unit() << std::endl;
	
		
		//Momentum
		double totalMomentum = G4RandFlat::shoot(particle_minMomentum, particle_maxMomentum);
		
		
        G4PrimaryParticle *particle = new G4PrimaryParticle(particle_definition);
        //particle->SetKineticEnergy(G4RandGauss::shoot(particle_energy, fEnergyStdDev));
        particle->SetMomentum(totalMomentum*nx, totalMomentum*ny, totalMomentum*nz);
        particle->SetMass(mass);
        //particle->SetMomentumDirection(randomisedDirection);
        particle->SetCharge(particle_charge);
        particle->SetPolarization(particle_polarization.x(), particle_polarization.y(), particle_polarization.z());
        vertex->SetPrimary(particle);
    }

    evt->AddPrimaryVertex(vertex);
}

void ConeParticleGunHW::SetParticlePositionStdDev(G4ThreeVector vector)
{
    fPositionStdDev = vector;


}

void ConeParticleGunHW::SetParticleEnergyStdDev(G4double d)
{
    fEnergyStdDev = d;

}

void ConeParticleGunHW::SetConeAngleStdDev(G4double d)
{
    fConeAngleStdDev = d;
}


