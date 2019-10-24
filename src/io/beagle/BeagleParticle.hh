//
// Created by romanov on 3/19/19.
//

#ifndef BEAGLE_PARTICLE_HH
#define BEAGLE_PARTICLE_HH

#include <cstdint>

    class BeagleParticle {
    public:
       int index;          // I:           I  line index, runs from 1 to nrTracks
       int ks_code;        // ISTHKK(I):   I  status code KS: KS=1 is the only stable final state particle code, Use NoBAM variable (below) to specify origin of particle
       int kf_code;        // IDHKK(I):    I  particle KF code (211: pion, 2112:n, ....). Code 80000 refers to a nucleus, specified in more detail by A=IDRES(I), Z=IDXRES(I)
       int mother_second;  // JMOHKK(2,I): I  line number of second mother particle
       int mother_first;   // JMOHKK(1,I): I  line number of first mother particle
       int daughter_first; // JDAHKK(1,I): I  normally the line number of the first daughter.
       int daughter_last;  // JDAHKK(2,I): I  normally the line number of the last daughter.
       double px;          // PHKK(1,I):   D  px of particle (GeV/c)
       double py;          // PHKK(2,I):   D  py of particle (GeV/c)
       double pz;          // PHKK(3,I):   D  pz of particle (GeV/c)
       double e;           // PHKK(4,I):   D  Energy of particle (GeV)
       double m;           // PHKK(5,I):   D  mass of particle (GeV/c^2)
       double vx;          // VHKK(1,I):   D  x vertex information (mm)
       double vy;          // VHKK(2,I):   D  y vertex information (mm)
       double vz;          // VHKK(3,I):   D  z vertex information (mm)
       int baryon_number;  // IDRES(I):    I  Baryon number, or A for a nucleus (IDHKK(I)=80000), fractional B set to 0.
       int charge;         // IDXRES(I):   I  Particle charge, (Z for a nucleus), 0 for fractional charge.
       int origin;         // NOBAM(I):    I  Flag describing the particle origin, particularly for final state particles.

    static std::unique_ptr<BeagleParticle> ParseParticle(std::vector<std::string>& tokens) {
        auto particle = new BeagleParticle();
        particle->index          = std::stoi(tokens[0]);   // I line index, runs from 1 to nrTracks
        particle->ks_code        = std::stoi(tokens[1]);   // I status code KS: KS=1 is the only stable final state particle code, Use NoBAM variable (below) to specify origin of particle
        particle->kf_code        = std::stoi(tokens[2]);   // I particle KF code (211: pion, 2112:n, ....). Code 80000 refers to a nucleus, specified in more detail by A=IDRES(I), Z=IDXRES(I)
        particle->mother_second  = std::stoi(tokens[3]);   // I line number of second mother particle
        particle->mother_first   = std::stoi(tokens[4]);   // I line number of first mother particle
        particle->daughter_first = std::stoi(tokens[5]);   // I normally the line number of the first daughter.
        particle->daughter_last  = std::stoi(tokens[6]);   // I normally the line number of the last daughter.
        particle->px             = std::stod(tokens[7]);   // D px of particle (GeV/c)
        particle->py             = std::stod(tokens[8]);   // D py of particle (GeV/c)
        particle->pz             = std::stod(tokens[9]);   // D pz of particle (GeV/c)
        particle->e              = std::stod(tokens[10]);  // D Energy of particle (GeV)
        particle->m              = std::stod(tokens[11]);  // D mass of particle (GeV/c^2)
        particle->vx             = std::stod(tokens[12]);  // D x vertex information (mm)
        particle->vy             = std::stod(tokens[13]);  // D y vertex information (mm)
        particle->vz             = std::stod(tokens[14]);  // D z vertex information (mm)
        particle->baryon_number  = std::stoi(tokens[15]);  // I Baryon number, or A for a nucleus (IDHKK(I)=80000), fractional B set to 0.
        particle->charge         = std::stoi(tokens[16]);  // I Particle charge, (Z for a nucleus), 0 for fractional charge.
        particle->origin         = std::stoi(tokens[17]);  // I Flag describing the particle origin, particularly for final state particles.

        return std::unique_ptr<BeagleParticle>(particle);
    }

    static std::vector<std::unique_ptr<BeagleParticle>>
    ParseParticles(std::vector<std::vector<std::string>>& multiline_values)
    {
        // Fill Original particles information
        std::vector<std::unique_ptr<BeagleParticle>> particles;

        for(auto particle_tokens: multiline_values) {
            particles.push_back(ParseParticle(particle_tokens));
        }

        return particles;
    };


#endif // header guard lock
