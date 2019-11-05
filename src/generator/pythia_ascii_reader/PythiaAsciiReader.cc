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
/// \file eventgenerator/HepMC/HepMCEx01/src/PythiaAsciiReader.cc
/// \brief Implementation of the PythiaAsciiReader class
//
// $Id: PythiaAsciiReader.cc 77801 2013-11-28 13:33:20Z gcosmo $
//

#include "PythiaAsciiReader.hh"
#include "PythiaAsciiReaderMessenger.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"

#include <iostream>
#include <fstream>

//------ for Pythia ----
G4ThreeVector beam_dir;           ///< beam direction
G4ThreeVector beam_vrt;           ///< beam vertex
G4ThreeVector beam_pol;           ///< beam polarization vector
double TWINDOW;                    ///< Time Window
double TBUNCH;                     ///< Time Between Bunches
double GEN_VERBOSITY;
//----------------------


PythiaAsciiReader::PythiaAsciiReader()
        : filename("xxx.dat"), verbose(0) {
    messenger = new PythiaAsciiReaderMessenger(this);
}


PythiaAsciiReader::~PythiaAsciiReader() {
    gif.close();
    delete messenger;
}


void PythiaAsciiReader::Initialize() {
    //=====================  open pythia_ascii_reader file =========================
    gif.close();
    gif.open(filename.c_str());
    if (!gif) {
        std::cerr << hd_msg << " Can't open input file " << filename.c_str() << ". Exiting. " << std::endl;
        exit(1);
    }

    gif.seekg(0);
    gformat = "LUND";
    hd_msg = "ERROR: Pythia:: ";
    GEN_VERBOSITY = 0;
    //=================================================================
}


PythiaAsciiReader *PythiaAsciiReader::GeneratePythiaEvent() {
    printf("Read lund\n");
    if ((gformat == "LUND" || gformat == "lund") && !gif.eof()) {
        //printf("=====> LUND 0 !!!!  \n");
        lundUserDefined.clear();
        gif >> nparticles;
        for (unsigned i = 0; i < 9; i++) {
            double tmp;
            if (i == 3) {
                gif >> beamPol;
                if (beamPol > 1)
                    beamPol = 1;
            } else {
                gif >> tmp;
                lundUserDefined.push_back(tmp);
            }
        }
        // 11    0    1        2112        0        0       -1.332584       -0.299053       31.538593       31.582129        0.939570        0.000000        0.000000        0.000000
        //	printf("=====> LUND 1  !!!! npart=%d dCone=%f dmyMom=%f \n", nparticles, dCone, dmyMom);
        printf("=====> LUND 1  !!!! npart=%d \n", nparticles);
        N = 0;
        pyEvt.clear();

        if (nparticles < 0) { //--- generate particles in CONE ---

            double dCone;
            gif >> dCone;
            double dmyMom;
            gif >> dmyMom;


            nparticles = -nparticles;
            for (int kp = 0; kp < nparticles; kp++) {

                //--- GEMC version of LUND ---
                double px, py, pz, etot, mass;
                int parent = 0, daughter1 = 0, daughter2 = 0;
                // i,icharge,flag,PID,K(I,4),K(I,5) P(I,1),P(I,2),P(I,3),P(I,4),P(I,5)  V(I,1)*0.1,V(I,2)*0.1,VZoffSet*0.1
                int ip, icharge, itype, iPDG, K4, K5; //, pPDG=0;
                gif >> ip >> icharge >> itype >> iPDG >> K4 >> K5 >> px >> py >> pz >> etot >> mass >> Vx >> Vy >> Vz;
                printf("read: i=%3d Charge=%2d  Sts=%d  PDG=%5d  K4=%3d  K5=%3d    P=(%8.3f,%8.3f,%8.3f,%8.3f,%8.3f)    Vtx=(%f,%f,%f) \n",
                       ip, icharge, itype, iPDG, K4, K5, px, py, pz, etot, mass, Vx, Vy, Vz);
                Vt = 0;
                daughter1 = K4;
                daughter2 = K5;

                printf("GeV=%f  mm=%f c_light=%f \n", GeV, mm, c_light);

                if (itype == 1) {  // use only final stae particles , c_light=1 !!!
                    /*
                      K[1][N]=itype;   K[2][N]=iPDG;     K[4][N]=K4;      K[5][N]=K5;
                      P[0][N]=px*GeV;  P[1][N]=py*GeV;   P[2][N]=pz*GeV;  P[3][N]=etot*GeV;  P[4][N]=mass*GeV;
                      V[0][N]=Vx*mm;   V[1][N]=Vy*mm;    V[2][N]=Vz*mm;   V[3][N]=Vt*mm/c_light;
                      N++;
                    */
                    //-- vector version --
                    PyTrack ptrk;
                    ptrk.K[0] = kp;
                    ptrk.K[1] = itype;
                    ptrk.K[2] = iPDG;
                    ptrk.K[3] = parent;
                    ptrk.K[4] = daughter1;
                    ptrk.K[5] = daughter2;


                    double ptot = sqrt(px * px + py * py + pz * pz);

                    Theta = acos(pz / ptot);

                    //double dTheta = (0.5 - G4UniformRand()) * 2. * dCone; // --- flat
                    //  Theta += dTheta;
                    double dTheta = G4RandGauss::shoot(0, dCone);

                    //double rpz  = (0.5-G4UniformRand())*2.*0.05 * pz*GeV;
                    //double rpz = ptot * cos(Theta);

                    Phi = atan2(py, px);
                    double phi0 = G4UniformRand() * 2. * pi; // --- flat in phi

                    G4ThreeVector dir(std::sin(dTheta) * std::cos(phi0), std::sin(dTheta) * std::sin(phi0),
                                      std::cos(dTheta));
                    dir.rotateY(Theta);
                    dir.rotateZ(Phi);

                    ptot = G4RandGauss::shoot(ptot, dmyMom);
                    dir.setMag(ptot); //- new ptot Gauss !!!
                    //fDirectory.setMag(etot);  //--- use energi as momentum !!!! for beam protons !!! if beam is defined by momentum !!!!

                    //--- recalculate etot if PROTON !!!
                    double proton_mass = 0.938272;
                    //JF
                    etot = sqrt(ptot * ptot + proton_mass * proton_mass);
                    //  etot=sqrt(DMptot*DMptot+proton_mass*proton_mass);

                    /*
                            Phi += dPhi;
                            double rpt = ptot * sin(Theta);
                            double rpx = rpt * cos(Phi);
                            double rpy = rpt * sin(Phi);
                    */
                    double rpz = dir.z();
                    double rpx = dir.x();
                    double rpy = dir.y();

                    std::cout << "G4LorentzVector px= " << rpx << " py= " << rpy << " pz= " << rpz << std::endl;
                    /*
                    double rpt = sqrt(px*px + py*py);
                    //double rphi = (0.5-G4UniformRand())*2. * 3.1415 / 2.;
                    double rphi = (0.5-G4UniformRand())*2. * dCone;

                    double rpt = sqrt(px*px + py*py);
                    //double rphi = (0.5-G4UniformRand())*2. * 3.1415 / 2.;
                    double rphi = (0.5-G4UniformRand())*2. * dCone;

                    double rpx=rpt*cos(rphi);
                    double rpy=rpt*sin(rphi);
                    //double rpx  = (0.5-G4UniformRand())*2.*0.05 * px*GeV;
                    //double rpy  = (0.5-G4UniformRand())*2.*0.1 * py*GeV;
                    */

                    //G4LorentzVector p(px*GeV+rpx,py*GeV+rpy,pz*GeV+rpz,etot*GeV);

                    G4LorentzVector p(rpx * GeV, rpy * GeV, rpz * GeV, etot * GeV);
                    double dVx = G4RandGauss::shoot(0,Vx);
                    double dVy = G4RandGauss::shoot(0,Vy);
                    double dVz = G4RandGauss::shoot(0,Vz);
                    G4LorentzVector v(dVx * mm, dVy * mm, dVz * mm, Vt * mm / c_light);
                    std::cout << "G4LorentzVector p= " << p << std::endl;

                    ptrk.P = p;
                    ptrk.V = v;
                    pyEvt.push_back(ptrk);

                    N++;
                }
                gif.clear();
                gif.seekg(0);
            }

        } else { //-- read pythia_ascii_reader record ---

            for (int kp = 0; kp < nparticles; kp++) {

                //--- GEMC version of LUND ---
                double px, py, pz, etot, mass;
                int parent = 0, daughter1 = 0, daughter2 = 0;
                // i,icharge,flag,PID,K(I,4),K(I,5) P(I,1),P(I,2),P(I,3),P(I,4),P(I,5)  V(I,1)*0.1,V(I,2)*0.1,VZoffSet*0.1
                int ip, itype, iPDG, K4, K5; //, pPDG=0;
                double charge;
                //             gif >> ip >> icharge >> itype >> iPDG >> K4 >> K5 >> px >> py >> pz >> etot >> mass >> Vx >> Vy >> Vz;
                //             printf("read: i=%3d Charge=%2d  Sts=%d  PDG=%5d  K4=%3d  K5=%3d    P=(%8.3f,%8.3f,%8.3f,%8.3f,%8.3f)    Vtx=(%f,%f,%f) \n",
                //       ip, icharge, itype, iPDG, K4, K5, px, py, pz, etot, mass, Vx, Vy, Vz);
                gif >> ip >> charge >> itype >> iPDG >> K4 >> K5 >> px >> py >> pz >> etot >> mass >> Vx >> Vy >> Vz;
                printf("read LUND: i=%3d Charge=%f  Sts=%d  PDG=%5d  K4=%3d  K5=%3d    P=(%8.3f,%8.3f,%8.3f,%8.3f,%8.3f)    Vtx=(%f,%f,%f) \n",
                       ip, charge, itype, iPDG, K4, K5, px, py, pz, etot, mass, Vx, Vy, Vz);
                Vt = 0;
                daughter1 = K4;
                daughter2 = K5;

                printf("GeV=%f  mm=%f c_light=%f \n", GeV, mm, c_light);

                if (itype == 1) {  // use only final stae particles , c_light=1 !!!
                    /*
                      K[1][N]=itype;   K[2][N]=iPDG;     K[4][N]=K4;      K[5][N]=K5;
                      P[0][N]=px*GeV;  P[1][N]=py*GeV;   P[2][N]=pz*GeV;  P[3][N]=etot*GeV;  P[4][N]=mass*GeV;
                      V[0][N]=Vx*mm;   V[1][N]=Vy*mm;    V[2][N]=Vz*mm;   V[3][N]=Vt*mm/c_light;
                      N++;
                    */
                    //-- vector version --
                    PyTrack ptrk;
                    ptrk.K[0] = kp;
                    ptrk.K[1] = itype;
                    ptrk.K[2] = iPDG;
                    ptrk.K[3] = parent;
                    ptrk.K[4] = daughter1;
                    ptrk.K[5] = daughter2;
                    G4LorentzVector p(px * GeV, py * GeV, pz * GeV, etot * GeV);
                    G4LorentzVector v(Vx * mm, Vy * mm, Vz * mm, Vt * mm / c_light);
                    std::cout << "G4LorentzVector p= " << p;
                    ptrk.P = p;
                    ptrk.V = v;
                    pyEvt.push_back(ptrk);
                    N++;
                }
                //-------- end GEMC ------------

            }  //-- pyhtia particles loop
        } //-- if NEG npart / RNDM
    } // end gformat

    return this;
}



G4bool PythiaAsciiReader::CheckVertexInsideWorld(const G4ThreeVector& pos) const
{
    G4Navigator* navigator= G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

    G4VPhysicalVolume* world= navigator-> GetWorldVolume();
    G4VSolid* solid= world-> GetLogicalVolume()-> GetSolid();
    EInside qinside= solid-> Inside(pos);

    if( qinside != kInside) return false;
    else return true;
}


void PythiaAsciiReader::PyMC2G4(const PythiaAsciiReader* py,
                              G4Event* g4event)
{
    printf("=======> ENTER PyMC2G4 N part=%d vec=%ld<===============\n",py->N,py->pyEvt.size());

    G4PrimaryVertex* g4vtx;
    int Np = py->pyEvt.size(); // =py->N;

    for(int ip=0; ip<Np; ip++) {  // loop for particles ...

        // check world boundary
        //G4LorentzVector XVtxVect(py->V[0][ip],py->V[1][ip],py->V[2][ip],py->V[3][ip]);
        G4LorentzVector xvtx(py->pyEvt.at(ip).V);

        if (! CheckVertexInsideWorld(xvtx.vect())) { printf("PyMC2G4: vtx outside world ip=%d\n",ip); continue;}

        //if (ip==0) {
        // create G4PrimaryVertex and associated G4PrimaryParticles
        g4vtx = new G4PrimaryVertex(xvtx.x(), xvtx.y(), xvtx.z(), xvtx.t());
        //}

        //G4int pdgcode= py->K[2][ip];
        G4int pdgcode= py->pyEvt.at(ip).K[2];
        //G4LorentzVector p(py->P[0][ip],py->P[1][ip],py->P[2][ip],py->P[3][ip]);
        G4LorentzVector p(py->pyEvt.at(ip).P);
        G4PrimaryParticle* g4prim =  new G4PrimaryParticle(pdgcode, p.x(), p.y(), p.z());

        printf("PyMC2G4:: PDG= %d vtx=(%f,%f,%f) mom=(%f,%f,%f) \n",pdgcode, xvtx.x()/mm, xvtx.y()/mm, xvtx.z()/mm, p.x()/GeV, p.y()/GeV, p.z()/GeV);

        g4vtx-> SetPrimary(g4prim);
        g4event-> AddPrimaryVertex(g4vtx);
    }
    //  g4event-> AddPrimaryVertex(g4vtx);

}

void PythiaAsciiReader::GeneratePrimaryVertex(G4Event* anEvent) {
    // delete previous event object
    // delete hepmcEvent;

    // generate next event
    PythiaAsciiReader *pyEvent = GeneratePythiaEvent();

    printf("=======> return from GeneratePythiaEvent <===============\n");

    if (!pyEvent) {
        G4cout << "PythiaInterface: no generated particles. run terminated..." << G4endl;
        G4RunManager::GetRunManager()->AbortRun();
        return;
    }
    PyMC2G4(pyEvent, anEvent);
}