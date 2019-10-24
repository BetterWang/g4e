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
/// \file eventgenerator/HepMC/HepMCEx01/src/BeagleAsciiReader.cc
/// \brief Implementation of the BeagleAsciiReader class
//
// $Id: BeagleReader.cc 77801 2013-11-28 13:33:20Z gcosmo $
//
#include <iostream>

#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "BeagleReader.hh"
#include "BeagleInterfaceMessenger.hh"
#include "BeagleEventData.hh"

#include <StringHelpers.hh>
#include <TextEventFileReader.hh>
#include <spdlog/fmt/bundled/format.h>


void BeagleReader::Open(const std::string& fileName) {
    using namespace g4e;

    // unique_ptr reset first creates a new object and only then deletes an old one
    // We want to first close the underlying file, then open a new one.
    // So we put this reset first
    fLundReader.reset(nullptr);

    fLundReader = std::unique_ptr<g4e::TextEventFileReader>(new TextEventFileReader(
            fileName,

            [](uint64_t line_num, const std::string& line) {
                // skip first 6 lines of beagle event and lines that starts with '====='
                if(line_num<6 || StartsWith(line, "=")) return TextEventLineDecisions::Skip;

                // Tokenize and process all other lines
                return TextEventLineDecisions::Tokenize;
            },

            [](uint64_t, const std::vector<std::string>& tokens) {
                // Particle record has 18 columns, event header more than 50. So:
                if(tokens.size() > 18)  return TextEventLineDecisions::Event;   // It is event
                return TextEventLineDecisions::Particle;                        // Otherwise it is a particle record
            }
    ));
}


std::unique_ptr<BeagleEventData> BeagleReader::ReadNextEvent() {
    uint line_count = 0;
    // Read file lines until full event is read
    while(!fLundReader->IsNewEventReady())
    {
        // Read next line. false = EOF
        if(!fLundReader->ParseNextLine()) {

            // We hit the end of the file, but...
            // In the end of file we may have the last event compiled
            // We will process the last event and the next time hit the throw below
            if(fLundReader->IsNewEventReady()) break;
            return nullptr;
        }

        // Check if there is no new event for too long
        if (line_count++ > 1000) {
            auto message = fmt::format("ERROR. {} lines where read from file and no new/end of event found. "
                                       "This error often means, that this is not BeAGLE file");
            throw std::runtime_error(message);
        }
    }

    auto event = new BeagleEventData();
    event->text_event = fLundReader->GetEvent();
    event->Parse();
    event->particles = BeagleParticle::ParseParticles(event->text_event->particle_values);
    return event;
}
