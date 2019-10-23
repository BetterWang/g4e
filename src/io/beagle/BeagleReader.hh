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
/// \file eventgenerator/HepMC/HepMCEx01/include/BeagleAsciiReader.hh
/// \brief Definition of the BeagleReader class
///
/// Beagle stands for
/// The full specification is located at:
/// https://wiki.bnl.gov/eic/index.php/BeAGLE#Output_Data_Format
//
// $Id: BeagleReader.hh 73446 2013-08-27 11:32:59Z gcosmo $
//

#ifndef BEAGLE_ASCII_READER_H
#define BEAGLE_ASCII_READER_H

#include "BeagleInterface.hh"
#include "BeagleEventData.hh"
#include <TextEventFileReader.hh>
#include <fstream>

class BeagleInterfaceMessenger;

class BeagleReader{
public:
    BeagleReader();
    virtual ~BeagleReader();

    // methods...
    void Open(const std::string& fileName);   /// Opens the file with

    g4e::BeagleEventData* ReadNextEvent();    /// Tries to read the next event. nullptr=no more events, throws on io errors

private:

    // Generators Input Files : Beagle
    std::unique_ptr<g4e::TextEventFileReader> fLundReader;
};


#endif
