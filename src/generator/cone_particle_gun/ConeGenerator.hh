#ifndef G4E_CONE_GENERATOR_HH
#define G4E_CONE_GENERATOR_HH

#include <G4LorentzVector.hh>
#include <G4VPrimaryGenerator.hh>
#include <fstream>
#include <vector>

class ConeGenerator : public G4VPrimaryGenerator {
protected:
    G4int fVerbose;

public:
    ConeGenerator() {
        fParticleTable = G4ParticleTable::GetParticleTable();
    }

    ~ConeGenerator();

    void SetVerboseLevel(G4int i);

    G4int GetVerboseLevel() const;

    // methods...
    void Initialize();


    // Generators Input Files : Pythia
    int fParticlesPerEvent;

    double Vx, Vy, Vz;    /// < Randomized Beam Vertex coordinates
    double Theta;         /// < theta, delta theta, randomized theta
    double Phi;           /// < phi, delta phi, randomized phi
    double dCone;         /// < Cone sigma
    double dmyMom;        /// < Momentum sigma

    G4bool CheckVertexInsideWorld(const G4ThreeVector &pos) const;

    void GeneratePrimaryVertex(G4Event *anEvent);

    G4ParticleTable *fParticleTable;


};

// ====================================================================
// inline functions
// ====================================================================

inline void ConeGenerator::SetVerboseLevel(G4int i) {
    fVerbose = i;
}

inline G4int ConeGenerator::GetVerboseLevel() const {
    return fVerbose;
}

#endif
