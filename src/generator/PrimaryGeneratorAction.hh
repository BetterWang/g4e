/**
 * @file
 *
 * Defines PrimaryGeneratorAction which allows to select a primary generator
 * such as:
 *  - particle gun             // no comments
 *  - hepmc                    // Herwig, Pythia 8 and other HepMC ascii
 *  - lund                     // Pythia 6, and others sticking to lund format
 *  - beagle                   // Beagle
 *
 * The class provides @see - SelectGenerator(G4String name) method to select the generator
 * And is controlled by \sa PrimaryGeneratorMessenger commands
 *
 */


#ifndef PRIMARY_GENERATOR_ACTION_HEADER
#define PRIMARY_GENERATOR_ACTION_HEADER


#include <map>
#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;
class G4VPrimaryGenerator;
class PrimaryGeneratorMessenger;
class JLeicRunAction;
class JLeicDetectorConstruction;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction();

    ~PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event *anEvent);

    void SetGenerator(G4VPrimaryGenerator *gen);

    void SelectGenerator(const G4String& name);

    G4VPrimaryGenerator *GetGenerator() const;

    G4String GetGeneratorName() const;

    static G4String GetPrimaryName();

    static G4double GetPrimaryEnergy();

private:
    G4VPrimaryGenerator *fConeParticleGunGenerator;       // Particle gun
    G4VPrimaryGenerator *fParticleGunGenerator;       // Particle gun
    G4VPrimaryGenerator *fHepMcAsciiGenerator;        // Herwig, Pythia 8 and other HepMC ascii
    G4VPrimaryGenerator *fPythiaAsciiGenerator;       // Pythia 6 lund format
    G4VPrimaryGenerator *fBeagleGenerator;            // Beagle Shmeagle

    G4VPrimaryGenerator *currentGenerator;
    G4String currentGeneratorName;
    std::map<G4String, G4VPrimaryGenerator *> gentypeMap;

    PrimaryGeneratorMessenger *messenger;

};


inline void PrimaryGeneratorAction::SetGenerator(G4VPrimaryGenerator *gen) {
    currentGenerator = gen;
}

inline G4VPrimaryGenerator *PrimaryGeneratorAction::GetGenerator() const {
    return currentGenerator;
}

inline G4String PrimaryGeneratorAction::GetGeneratorName() const {
    return currentGeneratorName;
}

#endif
