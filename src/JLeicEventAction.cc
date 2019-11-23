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
// $Id: JLeicEventAction.cc,v 1.7 2010-06-06 04:33:02 perl Exp $
// GEANT4 tag $Name: geant4-09-04-patch-01 $
//
// 




#include "JLeicEventAction.hh"

#include "JLeicRunAction.hh"

#include "design_main/jleic/JLeicCalorHit.hh"
#include "design_main/jleic/JLeicVTXHit.hh"
#include "JLeicEventActionMessenger.hh"

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include "Randomize.hh"



JLeicEventAction::JLeicEventAction(JLeicRunAction *JLeicRA)
        : calorimeterCollID(-1),
          vertexCollID(-1),
          eventMessenger(0),
          runaction(JLeicRA),
          verboselevel(0),
          printModulo(10000)
{
    eventMessenger = new JLeicEventActionMessenger(this);
    printf("JLeicEventAction:: Constructor \n");

    mHitsFile = runaction->mHitsFile;
    mRootEventsOut=&runaction->mRootEventsOut;
}



JLeicEventAction::~JLeicEventAction() {
    delete eventMessenger;
}



void JLeicEventAction::BeginOfEventAction(const G4Event *evt) {
    G4int evtNb = evt->GetEventID();

    mRootEventsOut->ClearForNewEvent();

    if (evtNb % printModulo == 0)
        G4cout << "\n---> Begin of Event: " << evtNb << G4endl;

    if (verboselevel > 1)
        G4cout << "<<< Event  " << evtNb << " started." << G4endl;

    if (calorimeterCollID == -1) {
        G4SDManager *SDman = G4SDManager::GetSDMpointer();
        calorimeterCollID = SDman->GetCollectionID("CalCollection");
    }
    if (vertexCollID == -1) {
        G4SDManager *SDman = G4SDManager::GetSDMpointer();
        vertexCollID = SDman->GetCollectionID("VTXCollection");
    }

    nstep = 0.;
    nstepCharged = 0.;
    nstepNeutral = 0.;
    Nch = 0.;
    GamDE = 0.;
    Nne = 0.;
    NE = 0.;
    NP = 0.;
    Transmitted = 0.;
    Reflected = 0.;

    if (evtNb == 0) printf("----> Begin of Event: %d \n", evtNb);


}



void JLeicEventAction::EndOfEventAction(const G4Event *evt) {
    G4HCofThisEvent *HCE = evt->GetHCofThisEvent();
    auto vertex = evt->GetPrimaryVertex(0);
    if(vertex) {
        G4cout << "vertex particles: " << vertex->GetNumberOfParticle() << G4endl;
    }

    JLeicCalorHitsCollection *CHC = 0;
    if (HCE)
        CHC = (JLeicCalorHitsCollection *) (HCE->GetHC(calorimeterCollID));

    if (CHC) {
        int n_hit = CHC->entries();
        if (verboselevel >= 1)
            G4cout << "     " << n_hit
                   << " hits are stored in JLeicCalorHitsCollection." << G4endl;

        G4double totEAbs = 0, totLAbs = 0;
        for (int i = 0; i < n_hit; i++) {
            totEAbs += (*CHC)[i]->GetEdepAbs();
            totLAbs += (*CHC)[i]->GetTrakAbs();
        }

    
        if (verboselevel >= 1)
            G4cout
                    << " CAL::  Absorber: total energy: " << std::setw(7) <<
                    G4BestUnit(totEAbs, "Energy")
                    << "       total track length: " << std::setw(7) <<
                    G4BestUnit(totLAbs, "Length")
                    << G4endl;

        // count event, add deposits to the sum ...
        runaction->CountEvent();
        runaction->AddTrackLength(totLAbs);
        runaction->AddnStepsCharged(nstepCharged);
        runaction->AddnStepsNeutral(nstepNeutral);
        if (verboselevel == 2)
            G4cout << " Ncharged=" << Nch << "  ,   Nneutral=" << Nne << G4endl;
        runaction->CountParticles(Nch, Nne);
        runaction->AddEP(NE, NP);
        runaction->AddTrRef(Transmitted, Reflected);
        runaction->AddEdeps(totEAbs);
        runaction->FillEn(totEAbs);
        //runaction->FillGamDE(GamDE) ;; // move to step action

        nstep = nstepCharged + nstepNeutral;
        runaction->FillNbOfSteps(nstep);

	mRootEventsOut->FillEvent((uint64_t)evt->GetEventID());

    }

    JLeicVTXHitsCollection *VCH = 0;
    if (HCE)
        VCH = (JLeicVTXHitsCollection *) (HCE->GetHC(vertexCollID));

    if (VCH) {
        int n_hit = VCH->entries();
        if (verboselevel >= 1)
            G4cout << "     " << n_hit
                   << " hits are stored in JLeicVTXHitsCollection." << G4endl;

        G4double totEAbs = 0, totLAbs = 0;
        for (int i = 0; i < n_hit; i++) {
            totEAbs += (*VCH)[i]->GetEdepAbs();
            totLAbs += (*VCH)[i]->GetTrakAbs();
        }

    
        if (verboselevel >= 1)
            G4cout
                    << "  VTX::  Absorber: total energy: " << std::setw(7) <<
                    G4BestUnit(totEAbs, "Energy")
                    << "       total track length: " << std::setw(7) <<
                    G4BestUnit(totLAbs, "Length")
                    << G4endl;

        // count event, add deposits to the sum ...
        runaction->CountEvent();
        runaction->AddTrackLength(totLAbs);
        runaction->AddnStepsCharged(nstepCharged);
        runaction->AddnStepsNeutral(nstepNeutral);
        if (verboselevel == 2)
            G4cout << " Ncharged=" << Nch << "  ,   Nneutral=" << Nne << G4endl;
        runaction->CountParticles(Nch, Nne);
        runaction->AddEP(NE, NP);
        runaction->AddTrRef(Transmitted, Reflected);
        runaction->AddEdeps(totEAbs);
        runaction->FillEn(totEAbs);
        //runaction->FillGamDE(GamDE) ;; // move to step action

        nstep = nstepCharged + nstepNeutral;
        runaction->FillNbOfSteps(nstep);
    }


    if (verboselevel > 0)
        G4cout << "<<< Event  " << evt->GetEventID() << " ended." << G4endl;


    //save rndm status
    if (runaction->GetRndmFreq() == 2) {
        CLHEP::HepRandom::saveEngineStatus("endOfEvent.rndm");
        G4int evtNb = evt->GetEventID();
        if (evtNb % printModulo == 0) {
            G4cout << "\n---> End of Event: " << evtNb << G4endl;
            CLHEP::HepRandom::showEngineStatus();
        }
    }
}



G4int JLeicEventAction::GetEventno() {
    G4int evno = fpEventManager->GetConstCurrentEvent()->GetEventID();
    return evno;
}



void JLeicEventAction::setEventVerbose(G4int level) {
    verboselevel = level;
}



void JLeicEventAction::CountStepsCharged() {
    nstepCharged += 1.;
}



void JLeicEventAction::CountStepsNeutral() {
    nstepNeutral += 1.;
}




void JLeicEventAction::AddCharged() {
    Nch += 1.;
}



void JLeicEventAction::AddNeutral() {
    Nne += 1.;
}


void JLeicEventAction::AddGammaDE(G4double de) {
    GamDE += de;
}



void JLeicEventAction::AddE() {
    NE += 1.;
}



void JLeicEventAction::AddP() {
    NP += 1.;
}



void JLeicEventAction::SetTr() {
    Transmitted = 1.;
}



void JLeicEventAction::SetRef() {
    Reflected = 1.;
}


  


