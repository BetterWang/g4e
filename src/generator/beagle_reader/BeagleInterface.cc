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

#include <cmath>

#include "BeagleReader.hh"
#include "BeagleInterface.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"


#include <spdlog/fmt/fmt.h>


    g4e::BeagleInterface::BeagleInterface()
    {
        fReader = new BeagleReader();
        fMessenger = new BeagleInterfaceMessenger(this);
        fVerbose=1;
    }

    g4e::BeagleInterface::~BeagleInterface()
    {

    }

    G4bool g4e::BeagleInterface::CheckVertexInsideWorld(const G4ThreeVector &pos) const {
        G4Navigator *navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();

        G4VPhysicalVolume *world = navigator->GetWorldVolume();
        G4VSolid *solid = world->GetLogicalVolume()->GetSolid();
        EInside checkResult = solid->Inside(pos);

        return checkResult == kInside;
    }

    void g4e::BeagleInterface::Open(const std::string &fileName) {
        fFileName = fileName;
        G4cout << "Beagle input file: '" << fileName << "'" << G4endl;
    }

    void g4e::BeagleInterface::GeneratePrimaryVertex(G4Event *event)
    {
        using namespace fmt;

        auto beagleEvent = fReader->ReadNextEvent();

        // TODO at this point we set primary vertex
        auto zeroVertex = new G4PrimaryVertex(0,0,0,0);

        for(const auto& beagleParticle: beagleEvent->particles) {

            int pdg = beagleParticle->kf_code;
            auto *geantParticle = new G4PrimaryParticle(pdg,
                                    beagleParticle->px * GeV,
                                    beagleParticle->py * GeV,
                                    beagleParticle->pz * GeV);

            double vx = beagleParticle->vx;
            double vy = beagleParticle->vy;
            double vz = beagleParticle->vz;

            // Hope 1e-8 mm is enough to say it is zero
            if( std::abs(vx) > 1e-8 ||
                std::abs(vy) > 1e-8 ||
                std::abs(vz) > 1e-8 )
            {
                // The vertex is not zero. Create it

                // Check volume
                if (!CheckVertexInsideWorld(G4ThreeVector(vx, vy, vz))) {
                    if(fVerbose > 1) {
                        fmt::print("g4e::BeagleInterface::GeneratePrimaryVertex: vtx outside world ({},{},{})\n", vx, vy, vz);
                    }

                    continue;
                }

                // Create vertex
                auto vertex = new G4PrimaryVertex(beagleParticle->vx, beagleParticle->vy, beagleParticle->vz,0);
                vertex->SetPrimary(geantParticle);
                event->AddPrimaryVertex(vertex);
            } else {
                // Use 0,0,0 vertex
                zeroVertex->SetPrimary(geantParticle);
                // zeroVertex is added in the end;
            }
        }

        // Add zero vertex if it has some particles
        if (zeroVertex->GetNumberOfParticle()) {
            event->AddPrimaryVertex(zeroVertex);
        }
    }



