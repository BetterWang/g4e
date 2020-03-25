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
//
//

#include "HepMcAsciiGenerator.hh"
#include "HepMcAsciiReaderMessenger.hh"

#include <iostream>

HepMcAsciiGenerator::HepMcAsciiGenerator(): filename(), verbose(0)
{
    asciiInput = new HepMC::IO_GenEvent(filename.c_str(), std::ios::in);
    messenger = new HepMcAsciiReaderMessenger(this);
}


HepMcAsciiGenerator::~HepMcAsciiGenerator()
{
    delete asciiInput;
    delete messenger;
}


void HepMcAsciiGenerator::Open()
{
    delete asciiInput;

    // Test that file exists. The {braces} to auto close infile
    {
        std::ifstream infile(filename);
        if(!infile.good()) {
            auto message = "HepMcAsciiGenerator:: Can't open input file. It doesn't exists or this proc have no rights/access";
            throw std::runtime_error(message);
        }
    }

    asciiInput = new HepMC::IO_GenEvent(filename.c_str(), std::ios::in);
}


HepMC::GenEvent *HepMcAsciiGenerator::GenerateHepMCEvent()
{
    const std::lock_guard<std::mutex> lock(readMutex);

    HepMC::GenEvent *evt = asciiInput->read_next_event();
    return evt;
}
