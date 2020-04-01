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
/// \file eventgenerator/HepMC/HepMCEx01/src/HepMCG4Interface.cc
/// \brief Implementation of the HepMCG4Interface class
//
// $Id: HepMcGeneratorInterface.cc 77801 2013-11-28 13:33:20Z gcosmo $
//

#include "HepMcGeneratorInterface.hh"

#include "G4RunManager.hh"
#include "G4LorentzVector.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


HepMcGeneratorInterface::HepMcGeneratorInterface()
        : hepmcEvent(nullptr),
        fVerbose(false)
{
}


HepMcGeneratorInterface::~HepMcGeneratorInterface() {
    delete hepmcEvent;
}


G4bool HepMcGeneratorInterface::CheckVertexInsideWorld(const G4ThreeVector &pos) const {
    G4Navigator *navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

    G4VPhysicalVolume *world = navigator->GetWorldVolume();
    G4VSolid *solid = world->GetLogicalVolume()->GetSolid();
    EInside testResult = solid->Inside(pos);

    return testResult == kInside;
}


void HepMcGeneratorInterface::HepMC2G4(const HepMC::GenEvent *hepmcevt, G4Event *g4event, bool verbose)
{
    if(verbose) {
        std::cout<<"HepMC2G4: BEGIN EVENT"<<std::endl;
    }

    for (auto vitr = hepmcevt->vertices_begin(); vitr != hepmcevt->vertices_end(); ++vitr) { // loop for vertex ...

        auto vertex = (*vitr);

        // real vertex?
        bool isRealVertex = false;
        for (auto pitr = vertex->particles_begin(HepMC::children); pitr != vertex->particles_end(HepMC::children); ++pitr) {
            if (!(*pitr)->end_vertex() && (*pitr)->status() == 1) {
                isRealVertex = true;
                break;
            }
        }
        if (!isRealVertex) continue;

        // check world boundary
        HepMC::FourVector pos = (*vitr)->position();
        G4LorentzVector xvtx(pos.x(), pos.y(), pos.z(), pos.t());
        if (!CheckVertexInsideWorld(xvtx.vect() * mm)) continue;

        // create G4PrimaryVertex and associated G4PrimaryParticles
        auto *g4vtx = new G4PrimaryVertex(xvtx.x() * mm, xvtx.y() * mm, xvtx.z() * mm, xvtx.t() * mm / c_light);

        for (auto vpitr = (*vitr)->particles_begin(HepMC::children); vpitr != (*vitr)->particles_end(HepMC::children); ++vpitr) {

            if ((*vpitr)->status() != 1) continue;

            G4int pdgcode = (*vpitr)->pdg_id();



            pos = (*vpitr)->momentum();
            G4LorentzVector p(pos.px(), pos.py(), pos.pz(), pos.e());
            auto *g4prim = new G4PrimaryParticle(pdgcode, p.x() * GeV, p.y() * GeV, -p.z() * GeV); //- fsv for HERWIG6 only : pz = - pz !!!

            g4vtx->SetPrimary(g4prim);

            if(verbose) {
                std::cout<<"HepMC2G4: Particle: "<<pdgcode<<std::endl;
            }
        }


        g4event->AddPrimaryVertex(g4vtx);
    }

    if(verbose) {
        std::cout<<"HepMC2G4: END EVENT"<<std::endl;
    }
}

void HepMcGeneratorInterface::GeneratePrimaryVertex(G4Event *anEvent) {
    // delete previous event object
    delete hepmcEvent;

    // generate next event
    hepmcEvent = GenerateHepMCEvent();
    if (!hepmcEvent) {
        if(fVerbose) std::cout << "HepMCInterface: no generated particles. run terminated..." << std::endl;
        G4RunManager::GetRunManager()->AbortRun();
        return;
    }
    HepMC2G4(hepmcEvent, anEvent, fVerbose);
}
