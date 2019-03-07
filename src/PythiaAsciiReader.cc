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
#include "G4ParticleTable.hh"
#include "PythiaAsciiReader.hh"
#include "PythiaAsciiReaderMessenger.hh"
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PythiaAsciiReader::PythiaAsciiReader()
  :  filename("xxx.dat"), verbose(0)
{

  messenger= new PythiaAsciiReaderMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PythiaAsciiReader::~PythiaAsciiReader()
{
  delete messenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PythiaAsciiReader::Initialize()
{
  //=====================  open pythia file =========================
  gif.close();
  gif.open(filename.c_str());
  if(!gif)
    {
      std::cerr << hd_msg << " Can't open input file " << filename.c_str() << ". Exiting. " << std::endl;
      exit(1);
    }

  gif.seekg(0);
  gformat = "LUND";
  hd_msg = "ERROR: Pythia:: ";
  GEN_VERBOSITY=0;
  //=================================================================
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PythiaAsciiReader* PythiaAsciiReader::GeneratePythiaEvent()
{


  
    printf("Read lund\n");
    if((gformat == "LUND" || gformat == "lund") && !gif.eof())     {
      //printf("=====> LUND 0 !!!!  \n");
      lundUserDefined.clear();                        
      gif >> nparticles ;
      for(unsigned i=0; i<9; i++)  {
	double tmp;
	if(i==3) {
	  gif >> beamPol;
	  if(beamPol>1)
	    beamPol = 1;
	} else {
	  gif >> tmp;                             
	  lundUserDefined.push_back(tmp);
	}
      }
      double dCone;
      gif >> dCone ;

      
      printf("=====> LUND 1  !!!! npart=%d \n",nparticles);
      N=0; pyEvt.clear();

      if (nparticles<0) { //--- generate particles in CONE ---

	nparticles=-nparticles;
	for(int kp=0; kp<nparticles; kp++)	{  
	  
	  //--- GEMC version of LUND ---
	  double px, py, pz, etot, mass;
	  int parent=0, daughter1=0, daughter2=0;
	  // i,icharge,flag,PID,K(I,4),K(I,5) P(I,1),P(I,2),P(I,3),P(I,4),P(I,5)  V(I,1)*0.1,V(I,2)*0.1,VZoffSet*0.1
	  int ip, icharge, itype, iPDG, K4, K5; //, pPDG=0;
	  gif >> ip >> icharge >> itype >> iPDG >> K4 >> K5 >> px >> py >> pz >> etot >> mass >> Vx >> Vy >> Vz;
	  printf("read: i=%3d Charge=%2d  Sts=%d  PDG=%5d  K4=%3d  K5=%3d    P=(%8.3f,%8.3f,%8.3f,%8.3f,%8.3f)    Vtx=(%f,%f,%f) \n"
		 ,ip,icharge,itype,iPDG,K4,K5,px,py,pz,etot,mass,Vx,Vy,Vz);
	  Vt=0;
	  daughter1=K4; daughter2=K5;
	  
	  printf("GeV=%f  mm=%f c_light=%f \n",GeV,mm,c_light);
	  
	  if(itype == 1)   {  // use only final stae particles , c_light=1 !!!
	    /*
	      K[1][N]=itype;   K[2][N]=iPDG;     K[4][N]=K4;      K[5][N]=K5;
	      P[0][N]=px*GeV;  P[1][N]=py*GeV;   P[2][N]=pz*GeV;  P[3][N]=etot*GeV;  P[4][N]=mass*GeV;
	      V[0][N]=Vx*mm;   V[1][N]=Vy*mm;    V[2][N]=Vz*mm;   V[3][N]=Vt*mm/c_light; 
	      N++;
	    */
	    //-- vector version --
	    PyTrack ptrk;
	    ptrk.K[0]=kp;  ptrk.K[1]=itype;  ptrk.K[2]=iPDG; ptrk.K[3]=parent; ptrk.K[4]=daughter1; ptrk.K[5]=daughter2;  
	    

	    double ptot = sqrt(px*px + py*py + pz*pz);

	    double Theta = acos(pz/ptot);
            
	    double dTheta = (0.5-G4UniformRand())*2. *  dCone; // --- flat
	    Theta+=dTheta;

 
	    //double rpz  = (0.5-G4UniformRand())*2.*0.05 * pz*GeV;
	    double rpz = ptot*cos(Theta);
	    
	    double Phi = atan2(py,px);
	    double dPhi = (0.5-G4UniformRand())*2. *  dCone; // --- flat

	    Phi += dPhi;
	    double rpt = ptot*sin(Theta);
	    double rpx=rpt*cos(Phi);
	    double rpy=rpt*sin(Phi);

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
	    G4LorentzVector p(rpx*GeV,rpy*GeV,rpz*GeV,etot*GeV);

	    G4LorentzVector v(Vx*mm,Vy*mm,Vz*mm,Vt*mm/c_light);
	    std::cout << "G4LorentzVector p= " << p;
	    ptrk.P=p;
	    ptrk.V=v;
	    pyEvt.push_back(ptrk);
	    N++;
	  }		
	  gif.clear();  gif.seekg(0);
	}
	
      } else { //-- read pythia record ---
	
	for(int kp=0; kp<nparticles; kp++)	{
	  
	  //--- GEMC version of LUND ---
	  double px, py, pz, etot, mass;
	  int parent=0, daughter1=0, daughter2=0;
	  // i,icharge,flag,PID,K(I,4),K(I,5) P(I,1),P(I,2),P(I,3),P(I,4),P(I,5)  V(I,1)*0.1,V(I,2)*0.1,VZoffSet*0.1
	  int ip, icharge, itype, iPDG, K4, K5; //, pPDG=0;
	  gif >> ip >> icharge >> itype >> iPDG >> K4 >> K5 >> px >> py >> pz >> etot >> mass >> Vx >> Vy >> Vz;
	  printf("read: i=%3d Charge=%2d  Sts=%d  PDG=%5d  K4=%3d  K5=%3d    P=(%8.3f,%8.3f,%8.3f,%8.3f,%8.3f)    Vtx=(%f,%f,%f) \n"
		 ,ip,icharge,itype,iPDG,K4,K5,px,py,pz,etot,mass,Vx,Vy,Vz);
	  Vt=0;
	  daughter1=K4; daughter2=K5;
	  
	  printf("GeV=%f  mm=%f c_light=%f \n",GeV,mm,c_light);
	  
	  if(itype == 1)   {  // use only final stae particles , c_light=1 !!!
	    /*
	      K[1][N]=itype;   K[2][N]=iPDG;     K[4][N]=K4;      K[5][N]=K5;
	      P[0][N]=px*GeV;  P[1][N]=py*GeV;   P[2][N]=pz*GeV;  P[3][N]=etot*GeV;  P[4][N]=mass*GeV;
	      V[0][N]=Vx*mm;   V[1][N]=Vy*mm;    V[2][N]=Vz*mm;   V[3][N]=Vt*mm/c_light; 
	      N++;
	    */
	    //-- vector version --
	    PyTrack ptrk;
	    ptrk.K[0]=kp;  ptrk.K[1]=itype;  ptrk.K[2]=iPDG; ptrk.K[3]=parent; ptrk.K[4]=daughter1; ptrk.K[5]=daughter2;  
	    G4LorentzVector p(px*GeV,py*GeV,pz*GeV,etot*GeV);
	    G4LorentzVector v(Vx*mm,Vy*mm,Vz*mm,Vt*mm/c_light);
	    std::cout << "G4LorentzVector p= " << p;
	    ptrk.P=p;
	    ptrk.V=v;
	    pyEvt.push_back(ptrk);
	    N++;
	  }		
	  //-------- end GEMC ------------
	  
	}  //-- pyhtia particles loop
      } //-- if NEG npart / RNDM
    } // end gformat
    
    return this;
    
}
