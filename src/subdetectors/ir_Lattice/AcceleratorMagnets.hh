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
    int32_t fIonBeamA,fIonBeamZ;
    double scaleFactor;
    BeamLines fBeamLine;

    void SetMotherParams(G4VPhysicalVolume *physicalVolume)
    {
        // fMaterial = material;
        fMotherPhysVolume = physicalVolume;
    }



    AcceleratorMagnets(std::string fname, G4VPhysicalVolume *physicalVolume, G4Material *material, BeamLines beamLine, int beamID, int beamEnergy, int ionBeamA=0, int ionBeamZ=0)
    {
        using namespace spdlog;
        fMotherPhysVolume = physicalVolume;
        info("Initializing AcceleratorMagnets:: MotherVolume: '{}'\n", fMotherPhysVolume->GetName());
        scaleFactor=1.;
        ifstream sourcefile;                    // build a read-Stream
        sourcefile.open(fname, ios_base::in);  // open data
        fBeamLine = beamLine;
        fIonBeamA = ionBeamA;
        fIonBeamZ = ionBeamZ;

        if(fIonBeamA>1 || fIonBeamZ >1 ) {
            trace(" |- Ion Beam Z or A >1 !!!! ");
        }

        // Set scaling if this is ion beamline part
        if(beamID==1 && beamEnergy != 275 ) {
            scaleFactor = beamEnergy / 275. ;
        }

        // Set scaling if this is electron beamline
        if(beamID==0 && beamEnergy != 10 ) {
            scaleFactor = beamEnergy / 10. ;
        }

        fmt::print(" |- Scaling for {} beamline. Beam energy: {}  Scaling: {} \n", beamID == 1 ? "ion": "electron", beamEnergy, scaleFactor);

        if (!sourcefile) {
            // if it does not work
            error("ERROR Can't open File with Lattice!\n");
        } else {
            info(" |- Opened AcceleratorMagnets file {} \n", fname);

            for (std::string line; std::getline(sourcefile, line);) {

                std::istringstream in(line);      //make a stream for the line itself
                trace(" |- AcceleratorMagnets:: line = {} ", line);

                if (line[0] != 'e' && line[0] != 'i') {
                    trace(" |- skip line {}", line.c_str());
                    continue;
                }
                parse_line(line, scaleFactor); //-- string--
            }
        }
    }

    //-----------------------------------------------------------
    void parse_line(std::string line, double scaleFactor)
    {
        using namespace spdlog;
        string name, type;
        double LengthZ, Rin, Rin2, Rout, DipoleFieldBx, DipoleFieldBy, QuadrupolFieldQnorm, QuadrupolFieldQskew;
        double SextupoleField, SolenoidField, Xcenter, Ycenter, Zcenter, MagTheta, MagPhi;

        fmt::print("parse_line \n");
        std::istringstream in(line);
        in >> name >> type >> LengthZ >> Rin >> Rin2 >> Rout >> DipoleFieldBx >> DipoleFieldBy >> QuadrupolFieldQnorm >> QuadrupolFieldQskew >> SextupoleField >> SolenoidField
           >> Xcenter >> Ycenter >> Zcenter >> MagTheta >> MagPhi;
        trace("AcceleratorMagnets::parse_line: Rin={}, Rin2={} Rout ={} ", Rin, Rin2, Rout);
        fmt::print("AcceleratorMagnets::parse_line: Rin={}, Rin2={} Rout ={} \n", Rin, Rin2, Rout);

        if (fBeamLine == BeamLines::IP6) {
            // TODO WTF * 100?
            // conversion from m to cm
            Rin = Rin * 100;
            Rin2 = Rin2 * 100;
            Rout = Rout * 100 / 2.; // Dout in the file is a diameter
        } else if(fBeamLine == BeamLines::IP8) {
            Rin = Rin * 100;
            Rin2 = Rin2 * 100;
            Rout = Rout * 100 ; //Dout  in the file is a radius
        }// different usints for ERHIC and JLEIC designs
        trace("AcceleratorMagnets::parse_line: Rin={}, Rin2={} Rout ={} ", Rin, Rin2, Rout);
        QuadrupoleMagnet *qmag = new QuadrupoleMagnet(name, type, LengthZ, Rin, Rin2, Rout, DipoleFieldBx, DipoleFieldBy, QuadrupolFieldQnorm, QuadrupolFieldQskew, SextupoleField, SolenoidField, Xcenter,
                                                      Ycenter, Zcenter, MagTheta, MagPhi, fMotherPhysVolume, fBeamLine, scaleFactor);

        fMagnets.push_back(qmag);
    }

};


#endif //G4E_ACCELERATORMAGNETS_HH
