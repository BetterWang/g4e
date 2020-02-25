#ifndef G4E_ACCELERATORMAGNETS_HH
#define G4E_ACCELERATORMAGNETS_HH

#include <string>
#include <G4Material.hh>
#include <G4FieldManager.hh>

#include "AcceleratorMagnets.hh"
#include "BeamLines.hh"

class AcceleratorMagnets
{

public:
    std::vector<QuadrupoleMagnet *> fMagnets;
    G4VPhysicalVolume *fMotherPhysVolume;
    int32_t fElectronBeamEnergy;
    int32_t fIonBeamEnergy;

    BeamLines fBeamLine;

    void SetMotherParams(G4VPhysicalVolume *physicalVolume)
    {
        // fMaterial = material;
        fMotherPhysVolume = physicalVolume;
    }

    void SetElectronBeamEnergy(int32_t electronBeamEnergy)
    {
        AcceleratorMagnets::fElectronBeamEnergy = electronBeamEnergy;
    }

    void SetIonBeamEnergy(int32_t ionBeamEnergy)
    {
        AcceleratorMagnets::fIonBeamEnergy = ionBeamEnergy;
    }


    AcceleratorMagnets(std::string fname, G4VPhysicalVolume *physicalVolume, G4Material *material, BeamLines beamLine)
    {
        using namespace spdlog;
        fMotherPhysVolume = physicalVolume;
        info("Initializing AcceleratorMagnets:: MotherVolume: '{}'\n", fMotherPhysVolume->GetName());

        ifstream sourcefile;                    // build a read-Stream
        sourcefile.open(fname, ios_base::in);  // open data
        fBeamLine = beamLine;

        if (!sourcefile) {                     // if it does not work
            cerr << "Can't open File with Lattice!\n";
        } else {
            debug("AcceleratorMagnets file opened {} \n", fname);
            for (std::string line; std::getline(sourcefile, line);) {

                std::istringstream in(line);      //make a stream for the line itself
                trace("AcceleratorMagnets:: read line = {} ", line);
                if (line[0] != 'e' && line[0] != 'i') {
                    trace(" skip line {}", line.c_str());
                    continue;
                }
                parse_line(line); //-- string--
            }
        }
    }

    //-----------------------------------------------------------
    void parse_line(std::string line)
    {
        using namespace spdlog;
        string name, type;
        double LengthZ, Rin, Rin2, Rout, DipoleFieldBx, DipoleFieldBy, QuadrupolFieldQnorm, QuadrupolFieldQskew;
        double SextupoleField, SolenoidField, Xcenter, Ycenter, Zcenter, MagTheta, MagPhi;


        std::istringstream in(line);
        in >> name >> type >> LengthZ >> Rin >> Rin2 >> Rout >> DipoleFieldBx >> DipoleFieldBy >> QuadrupolFieldQnorm >> QuadrupolFieldQskew >> SextupoleField >> SolenoidField
           >> Xcenter >> Ycenter >> Zcenter >> MagTheta >> MagPhi;

        trace("AcceleratorMagnets::parse_line: Rin={}, Rin2={} Rout ={} ", Rin, Rin2, Rout);
        if (fBeamLine == BeamLines::ERHIC) {
            // TODO WTF * 100?
            Rin = Rin * 100;
            Rin2 = Rin2 * 100;
            Rout = Rout * 100 / 2.;
        } // different usints for ERHIC and JLEIC designs
        trace("AcceleratorMagnets::parse_line: Rin={}, Rin2={} Rout ={} ", Rin, Rin2, Rout);
        QuadrupoleMagnet *qmag = new QuadrupoleMagnet(name, type, LengthZ, Rin, Rin2, Rout, DipoleFieldBx, DipoleFieldBy, QuadrupolFieldQnorm, QuadrupolFieldQskew, SextupoleField, SolenoidField, Xcenter,
                                                      Ycenter, Zcenter, MagTheta, MagPhi, fMotherPhysVolume, fBeamLine);

        fMagnets.push_back(qmag);
    }

};


#endif //G4E_ACCELERATORMAGNETS_HH
