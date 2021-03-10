#include <fstream>

#include "PrimaryGeneratorAction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "PythiaAsciiReader.hh"
#include "HepMcAsciiGenerator.hh"
#include "BeagleGenerator.hh"
#include "StringHelpers.hh"

#include "ConeParticleGun.hh"
#include "ConeParticleGunEta.hh"

#include <spdlog/fmt/ostr.h>

PrimaryGeneratorAction::PrimaryGeneratorAction(): G4VUserPrimaryGeneratorAction()
{
    // default generator is particle gun.
    fConeParticleGunHWGenerator = new ConeParticleGunEta();
    fConeParticleGunGenerator = new ConeParticleGun();
    fParticleGunGenerator = new G4ParticleGun();
    fHepMcAsciiGenerator = new HepMcAsciiGenerator();
    fPythiaAsciiGenerator = new PythiaAsciiReader();
    fBeagleGenerator = new g4e::BeagleGenerator();

    gentypeMap["coneParticleGunEta"] = fConeParticleGunGenerator;
    gentypeMap["coneParticleGun"] = fConeParticleGunGenerator;
    gentypeMap["particleGun"] = fParticleGunGenerator;
    gentypeMap["hepmcAscii"] = fHepMcAsciiGenerator;
    gentypeMap["pythiaAscii"] = fPythiaAsciiGenerator;
    gentypeMap["beagle"] = fBeagleGenerator;

    messenger = new PrimaryGeneratorMessenger(this);
}

void PrimaryGeneratorAction::SelectGenerator(const G4String& name) {

    std::string realName;


    auto pos = gentypeMap.find(name);

    // Check we have such generator
    if (pos == gentypeMap.end()) {
        fmt::print(G4cerr, "PrimaryGeneratorAction::SelectGenerator no generator with name '{}'\n", name);
        return;
    }

    currentGenerator = pos->second;
    currentGeneratorName = name;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete messenger;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
    if (currentGenerator)
        currentGenerator->GeneratePrimaryVertex(anEvent);
    else
        G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                    "PrimaryGeneratorAction001", FatalException,
                    "generator is not instantiated.");
}

std::string  PrimaryGeneratorAction::DetermineGeneratorName(std::string fileName) 
{
    std::ifstream file(fileName);
    if (file.is_open()) {
        std::string line;

        // We want to read the 1st meaningful line and skip empty lines
        while (getline(file, line)) {             
            
            g4e::Trim(line);

            if(line.empty()) continue; // skip empty or spaces only lines

            if (line.find("BEAGLE EVENT FILE") != std::string::npos) {
                return "beagle";
            }

            if (line.find("HepMC") != std::string::npos) {
                return "hepmcAscii";
            }

            if (line.find("PYTHIA EVENT FILE") != std::string::npos) {
                return "pythiaAscii";
            }

            if ( g4e::Split(line).size() == 10 ) {
                return "pythiaAscii";
            }

            break;
        }
        file.close();
    }

    // If we are here, we where not able to determine file type
    return "unknown";
    
}
