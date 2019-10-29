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
/// \file eventgenerator/HepMC/HepMCEx01/src/PythiaInterface.cc
/// \brief Implementation of the PythiaInterface class
//
// $Id: PythiaInterface.cc 77801 2013-11-28 13:33:20Z gcosmo $
//
#include "PythiaAsciiReader.hh"
#include "PythialeInterface.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PythiaInterface::PythiaInterface()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PythiaInterface::~PythiaInterface()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4bool PythiaInterface::CheckVertexInsideWorld
                         (const G4ThreeVector& pos) const
{
  G4Navigator* navigator= G4TransportationManager::GetTransportationManager()
                                                 -> GetNavigatorForTracking();

  G4VPhysicalVolume* world= navigator-> GetWorldVolume();
  G4VSolid* solid= world-> GetLogicalVolume()-> GetSolid();
  EInside qinside= solid-> Inside(pos);

  if( qinside != kInside) return false;
  else return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PythiaInterface::PyMC2G4(const PythiaAsciiReader* py,
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
PythiaAsciiReader* PythiaInterface::GeneratePythiaEvent()
{
  return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void PythiaInterface::GeneratePrimaryVertex(G4Event* anEvent)
{
  // delete previous event object
  // delete hepmcEvent;

  // generate next event
  PythiaAsciiReader* pyEvent = GeneratePythiaEvent();

  printf("=======> return from GeneratePythiaEvent <===============\n");


  if(! pyEvent) {
    G4cout << "PythiaInterface: no generated particles. run terminated..."
           << G4endl;
    G4RunManager::GetRunManager()-> AbortRun();
    return;
  }
  PyMC2G4(pyEvent, anEvent);
}
