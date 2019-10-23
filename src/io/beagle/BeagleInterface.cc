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
/// \file eventgenerator/HepMC/HepMCEx01/src/BeagleInterface.cc
/// \brief Implementation of the BeagleInterface class
//
//

#include "BeagleReader.hh"
#include "BeagleInterface.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


BeagleInterface::BeagleInterface() {
    fReader = new BeagleReader();
    fMessenger = new BeagleInterfaceMessenger(this);
}


BeagleInterface::~BeagleInterface() {

}


G4bool BeagleInterface::CheckVertexInsideWorld(const G4ThreeVector &pos) const {
    G4Navigator *navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

    G4VPhysicalVolume *world = navigator->GetWorldVolume();
    G4VSolid *solid = world->GetLogicalVolume()->GetSolid();
    EInside checkResult = solid->Inside(pos);

    return checkResult == kInside;
}


void BeagleInterface::PyMC2G4(const BeagleReader *reader, G4Event *g4event) {
    printf("=======> ENTER PyMC2G4 N part=%d vec=%ld<===============\n", reader->N, reader->pyEvt.size());

    G4PrimaryVertex *g4vtx;
    int Np = reader->pyEvt.size(); // =reader->N;

    for (int ip = 0; ip < Np; ip++) {  // loop for particles ...

        // check world boundary
        //G4LorentzVector XVtxVect(reader->V[0][ip],reader->V[1][ip],reader->V[2][ip],reader->V[3][ip]);
        G4LorentzVector xvtx(reader->pyEvt.at(ip).V);

        if (!CheckVertexInsideWorld(xvtx.vect())) {
            printf("PyMC2G4: vtx outside world ip=%d\n", ip);
            continue;
        }

        //if (ip==0) {
        // create G4PrimaryVertex and associated G4PrimaryParticles
        g4vtx = new G4PrimaryVertex(xvtx.x(), xvtx.y(), xvtx.z(), xvtx.t());
        //}

        //G4int pdgcode= reader->K[2][ip];
        G4int pdgcode = reader->pyEvt.at(ip).K[2];
        G4LorentzVector p(reader->pyEvt.at(ip).P);
        G4PrimaryParticle *g4prim = new G4PrimaryParticle(pdgcode, p.x(), p.y(), p.z());

        printf("PyMC2G4:: PDG= %d vtx=(%f,%f,%f) mom=(%f,%f,%f) \n", pdgcode, xvtx.x() / mm, xvtx.y() / mm,
               xvtx.z() / mm, p.x() / GeV, p.y() / GeV, p.z() / GeV);

        g4vtx->SetPrimary(g4prim);
        g4event->AddPrimaryVertex(g4vtx);
    }
    //  g4event-> AddPrimaryVertex(g4vtx);

}


void BeagleInterface::GeneratePrimaryVertex(G4Event *anEvent) {
    // delete previous event object
    // delete hepmcEvent;

    // generate next event
    BeagleReader *pyEvent = GenerateBeagleEvent();

    if (!pyEvent) {
        G4cout << "BeagleInterface: no generated particles. Terminating the run..."<< G4endl;
        G4RunManager::GetRunManager()->AbortRun();
        return;
    }
    else {
        PyMC2G4(pyEvent, anEvent);
    }
}

void BeagleInterface::PyMC2G4(const BeagleEventData *beagleEvent, G4Event *g4event) {

}

void BeagleInterface::Open(const std::string &fileName)
{
    G4cout << "Beagle input file: '" << fileName << "'" << G4endl;
    

};
