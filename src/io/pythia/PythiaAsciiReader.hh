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
/// \file eventgenerator/HepMC/HepMCEx01/include/PythiaAsciiReader.hh
/// \brief Definition of the PythiaAsciiReader class
//
// $Id: PythiaAsciiReader.hh 73446 2013-08-27 11:32:59Z gcosmo $
//

#ifndef PYTHIA_ASCII_READER_H
#define PYTHIA_ASCII_READER_H

#include "PythialeInterface.hh"
#include <fstream>

class PythiaAsciiReaderMessenger;

class PyTrack {

public:
    int K[6];   //-- size [6] to keep fortran numbers 1..5;  [0] is not used;
    int Status; //-- particle status (1=final state)
    int PDG;    //-- particle id
    G4LorentzVector P;  //-- 4 momentum
    G4LorentzVector V;  //-- Vertex and time

    PyTrack() {

    };

    ~PyTrack() {};
};


class PythiaAsciiReader : public PythiaInterface {
protected:
    G4String filename;

    G4int verbose;
    PythiaAsciiReaderMessenger *messenger;

    virtual PythiaAsciiReader *GeneratePythiaEvent();

public:
    PythiaAsciiReader();

    ~PythiaAsciiReader();

    // set/get methods
    void SetFileName(G4String name);

    G4String GetFileName() const;

    void SetVerboseLevel(G4int i);

    G4int GetVerboseLevel() const;

    // methods...
    void Initialize();


    // Generators Input Files : Pythia
    std::ifstream gif;                    ///< Generator Input File
    G4String gformat;                ///< Generator Format. Supported: LUND.
    G4String gfilename;              ///< Input Filename

    //------------------------   LUND/BNL information Pythia -------------------------------
    // LUND format:
    // Header (Event Info):
    // These are the original LUND variables, however after # particles, and except beam polarization, these can be user defined.
    // 1               2                     3                    4               5                 6  7  8   9   10
    // # of Particles, # of Target Nucleons, # of Target Protons, Pol. of Target, Pol. of Electron, x, y, W2, Q2, nu
    //
    // Body (Particle Info):
    // 1       2      3     4            5       6         7    8    9    10   11    12        13        14
    // index, charge, type, particle id, parent, daughter, p_x, p_y, p_z, p_t, mass, x vertex, y vertex, z vertex
    // type is 1 for particles in the detector

    //   COMMON/PYJETS/N,NPAD,K(4000,5),P(4000,5),V(4000,5)
    //  K(I,1) thus gives information on whether or not a parton or particle has fragmented or decayed, 
    //  K(I,2) gives the particle code, 
    //  K(I,3) its origin, 
    //  K(I,4) and  K(I,5) the position of fragmentation/decay products

    // ----  pythia6.f95 ------
    //       write(asciilun, 32) N, trueX, trueQ2, trueW2, ebeampol,trueY,  sngl(py6f2), sngl(py6f1), sigobs, Photonflux
    //       write(asciilun,34) i,charge,flag,PID,K(I,4),K(I,5),
    // +               P(I,1),P(I,2),P(I,3),P(I,4),P(I,5),
    // +               V(I,1)*0.1,V(I,2)*0.1,VZoffSet*0.1
    //! Remove the unphysical partciels/quarks/diquark/gluons/...
    //! Comment out all beam particles
    //              flag = 0
    //               do ik = 1,203
    //                 IF(PID.eq.PhysPtls(ik).AND.K(I,4).lt.1.AND.i.gt.5) THEN
    //                    flag = 1
    //                    exit
    //                 ENDIF
    //              enddo
    //              VZoffSet = 0. ! offset -403.0 mm

    int nparticles;
    std::vector<double> lundUserDefined;   ///< user defined infos in the LUND header

    double beamPol;                  ///< Beam Polarization as from the LUND format, it
    double Vx, Vy, Vz, Vt;              ///< Randomized Beam Vertex coordinates

    G4String hd_msg;                    ///< Head Message Log
    //G4ParticleTable* particleTable;   ///< Geant4 Particle Table
    // Primary Beam
    //G4ParticleDefinition *Particle;   ///< Particle type
    double mom, dmom, Mom;       ///< beam momentum, delta momentum, randomized momentum
    double theta, dtheta, Theta;     ///< theta, delta theta, randomized theta
    double phi, dphi, Phi;       ///< phi, delta phi, randomized phi

    /*
    int N, K[6][4000];  //-- size [6] to keep fortran numbers 1..5;  [0] not used;
    double P[5][4000];  //-- particle 3momentum [GeV/c], energy [GeV], mass [GeV/c^2]
    double V[5][4000];  //-- vertex in [mm] , time og prod [mm/c] , life time [mm/c] (~ 3.33E-12 sec)
    */
    std::vector<PyTrack> pyEvt;
    int N;      // -- N particles

};

// ====================================================================
// inline functions
// ====================================================================

inline void PythiaAsciiReader::SetFileName(G4String name) {
    filename = name;
}

inline G4String PythiaAsciiReader::GetFileName() const {
    return filename;
}

inline void PythiaAsciiReader::SetVerboseLevel(G4int i) {
    verbose = i;
}

inline G4int PythiaAsciiReader::GetVerboseLevel() const {
    return verbose;
}

#endif
