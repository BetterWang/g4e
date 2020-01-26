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

#include "JLeicEventAction.hh"

#include "JLeicRunAction.hh"

#include "main_detectors/jleic/JLeicCalorHit.hh"
#include "main_detectors/jleic/JLeicVTXHit.hh"
#include "JLeicEventActionMessenger.hh"
#include "JLeicHistogramManager.hh"

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



JLeicEventAction::JLeicEventAction(g4e::RootFlatIO *rootOutput, JLeicHistogramManager *histos)
        : calorimeterCollID(-1),
          vertexCollID(-1),
          eventMessenger(0),
          fVerbose(0),
          printModulo(10000),
          fHistos(histos)
{
    eventMessenger = new JLeicEventActionMessenger(this);
    printf("JLeicEventAction:: Constructor \n");


    mRootEventsOut=rootOutput;
}



JLeicEventAction::~JLeicEventAction() {
    delete eventMessenger;
}



void JLeicEventAction::BeginOfEventAction(const G4Event *evt) {
    G4int eventId = evt->GetEventID();

    mRootEventsOut->ClearForNewEvent();

    if (eventId % printModulo == 0 && fVerbose > 0) {
        G4cout << "\n---> Begin of Event: " << eventId << G4endl;
    }

    if (fVerbose > 1)
        G4cout << "<<< Event  " << eventId << " started." << G4endl;

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

    if (eventId == 0) printf("----> Begin of Event: %d \n", eventId);


}



void JLeicEventAction::EndOfEventAction(const G4Event *evt) {
    G4HCofThisEvent *hitCollectionEvnt = evt->GetHCofThisEvent();
    auto vertex = evt->GetPrimaryVertex(0);
    if(vertex) {
        G4cout << "vertex particles: " << vertex->GetNumberOfParticle() << G4endl;
    }

    JLeicCalorHitsCollection *hitCollectionCalo = 0;
    if (hitCollectionEvnt) {
        hitCollectionCalo = (JLeicCalorHitsCollection *) (hitCollectionEvnt->GetHC(calorimeterCollID));
    }


    if (hitCollectionCalo) {
        int n_hit = hitCollectionCalo->entries();
        if (fVerbose >= 1)
            G4cout << "     " << n_hit
                   << " hits are stored in JLeicCalorHitsCollection." << G4endl;

        G4double totEAbs = 0, totLAbs = 0;
        for (int i = 0; i < n_hit; i++) {
            totEAbs += (*hitCollectionCalo)[i]->GetEdepAbs();
            totLAbs += (*hitCollectionCalo)[i]->GetTrakAbs();
        }

    
        if (fVerbose >= 1){
            G4cout
                    << " CAL::  Absorber: total energy: " << std::setw(7) <<
                    G4BestUnit(totEAbs, "Energy")
                    << "       total track length: " << std::setw(7) <<
                    G4BestUnit(totLAbs, "Length")
                    << G4endl;
        }




        const G4int primeVtxCount = evt->GetNumberOfPrimaryVertex();
        size_t particleId = 0;  // prime particle ID unique for all prime vertexes

        for (G4int primeVtxIndex = 0; primeVtxIndex < primeVtxCount; primeVtxIndex++) {
            auto primeVtx = evt->GetPrimaryVertex(primeVtxIndex);

            // Add primary vertex to root output
            mRootEventsOut->AddPrimaryVertex((size_t) primeVtxIndex,                    /* size_t aVtxIndex, */
                                             (size_t) primeVtx->GetNumberOfParticle(),  /* size_t aParticleCount, */
                                             primeVtx->GetX0(),                         /* double aX, */
                                             primeVtx->GetY0(),                         /* double aY, */
                                             primeVtx->GetZ0(),                         /* double aZ, */
                                             primeVtx->GetT0(),                         /* double aTime, */
                                             primeVtx->GetWeight());                    /* double aWeight */


            const G4int partCount = primeVtx->GetNumberOfParticle();
            for (G4int partIndex = 0; partIndex < partCount; partIndex++) {
                auto particle = primeVtx->GetPrimary(partIndex);
                mRootEventsOut->AddPrimaryParticle(particleId,                             /*size_t aId */
                                                   (size_t) primeVtxIndex,                  /*size_t aPrimeVtxId */
                                                   (size_t) particle->GetPDGcode(),         /*size_t aPDGCode */
                                                   (size_t) particle->GetTrackID(),         /*size_t aTrackId */
                                                   particle->GetCharge(),                  /*double aCharge */
                                                   particle->GetMomentumDirection().x(),   /*double aMomDirX */
                                                   particle->GetMomentumDirection().y(),   /*double aMomDirY */
                                                   particle->GetMomentumDirection().z(),   /*double aMomDirZ */
                                                   particle->GetTotalMomentum() / GeV,       /*double aTotalMomentum */
                                                   particle->GetTotalEnergy() / GeV,         /*double aTotalEnergy */
                                                   particle->GetProperTime() / ns,           /*double aProperTime */
                                                   particle->GetPolX(),                    /*double aPolX */
                                                   particle->GetPolY(),                    /*double aPolY */
                                                   particle->GetPolZ()                     /*double aPolZ */
                );

                particleId++;
            }
        }

        //mRootEventsOut.FillEvent((uint64_t)evt->GetEventID());


//        // count event, add deposits to the sum ...
//        runaction->CountEvent();
//        runaction->AddTrackLength(totLAbs);
//        runaction->AddnStepsCharged(nstepCharged);
//        runaction->AddnStepsNeutral(nstepNeutral);
//        if (fVerbose == 2)
//            G4cout << " Ncharged=" << Nch << "  ,   Nneutral=" << Nne << G4endl;
//        runaction->CountParticles(Nch, Nne);
//        runaction->AddEP(NE, NP);
//        // TODO        runaction->AddTrRef(Transmitted, Reflected);
//        runaction->AddEdeps(totEAbs);
//        //runaction->FillGamDE(GamDE) ;; // move to step action

        nstep = nstepCharged + nstepNeutral;
        // fHistos->FillEn(totEAbs);
        // fHistos->FillNbOfSteps(nstep);

	    mRootEventsOut->FillEvent((uint64_t)evt->GetEventID());

    }

    JLeicVTXHitsCollection *VCH = 0;
    if (hitCollectionEvnt)
        VCH = (JLeicVTXHitsCollection *) (hitCollectionEvnt->GetHC(vertexCollID));

    if (VCH) {
        int n_hit = VCH->entries();
        if (fVerbose >= 1)
            G4cout << "     " << n_hit
                   << " hits are stored in JLeicVTXHitsCollection." << G4endl;

        G4double totEAbs = 0, totLAbs = 0;
        for (int i = 0; i < n_hit; i++) {
            totEAbs += (*VCH)[i]->GetEdepAbs();
            totLAbs += (*VCH)[i]->GetTrakAbs();
        }

    
        if (fVerbose >= 1)
            G4cout
                    << "  VTX::  Absorber: total energy: " << std::setw(7) <<
                    G4BestUnit(totEAbs, "Energy")
                    << "       total track length: " << std::setw(7) <<
                    G4BestUnit(totLAbs, "Length")
                    << G4endl;

//        // count event, add deposits to the sum ...
//        runaction->CountEvent();
//        runaction->AddTrackLength(totLAbs);
//        runaction->AddnStepsCharged(nstepCharged);
//        runaction->AddnStepsNeutral(nstepNeutral);
//        if (fVerbose == 2)
//            G4cout << " Ncharged=" << Nch << "  ,   Nneutral=" << Nne << G4endl;
//        runaction->CountParticles(Nch, Nne);
//        runaction->AddEP(NE, NP);
//        //runaction->AddTrRef(Transmitted, Reflected);
//        runaction->AddEdeps(totEAbs);
//        //runaction->FillEn(totEAbs);
//        //runaction->FillGamDE(GamDE) ;; // move to step action
//
//        nstep = nstepCharged + nstepNeutral;
//        //runaction->FillNbOfSteps(nstep);
    }


    if (fVerbose > 0)
        G4cout << "<<< Event  " << evt->GetEventID() << " ended." << G4endl;


    //save rndm status
    /*
    if (runaction->GetRndmFreq() == 2) {
        CLHEP::HepRandom::saveEngineStatus("endOfEvent.rndm");
        G4int evtNb = evt->GetEventID();
        if (evtNb % printModulo == 0) {
            G4cout << "\n---> End of Event: " << evtNb << G4endl;
            CLHEP::HepRandom::showEngineStatus();
        }
    }*/
}



G4int JLeicEventAction::GetEventno() {
    G4int evno = fpEventManager->GetConstCurrentEvent()->GetEventID();
    return evno;
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


  


