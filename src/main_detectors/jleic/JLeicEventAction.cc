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

#include <atomic>
#include "JLeicEventAction.hh"

#include "JLeicRunAction.hh"

#include "main_detectors/jleic/JLeicCalorHit.hh"
#include "main_detectors/jleic/JLeicVTXHit.hh"
#include "JLeicHistogramManager.hh"

#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include <G4Trajectory.hh>



JLeicEventAction::JLeicEventAction(g4e::RootFlatIO *rootOutput, JLeicHistogramManager *histos)
        : calorimeterCollID(-1),
          vertexCollID(-1),
          fVerbose(0),
          fPrintModulo(10),
          fHistos(histos),
          fMessenger(this, "/jleic/eventAction/")
{
    mRootEventsOut=rootOutput;

    //messenger for detectors and components
    fMessenger.DeclareProperty("verbose", fVerbose, "Sets verbosity. 0=nothing, 1=some, 2=all");
    fMessenger.DeclareProperty("printModulo", fPrintModulo);
}

void JLeicEventAction::BeginOfEventAction(const G4Event *evt) {
    G4int eventId = evt->GetEventID();

    mRootEventsOut->ClearForNewEvent();

    if (eventId % fPrintModulo == 0 && fVerbose > 0) {
        G4cout << "\n---> Begin of Event: " << eventId << G4endl;
    }

    if (fVerbose > 1)
        G4cout << "JLeicEventAction:: Event  " << eventId << " started." << G4endl;

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


    // Pretty prints generated particles and vertexes
    if (fVerbose > 1) {
        G4cout << "JLeicEventAction:: Event START " << evt->GetEventID() << G4endl;
        G4cout << "  |  GetNumberOfGrips          " << evt->GetNumberOfGrips() << G4endl;
        G4cout << "  |  GetNumberOfPrimaryVertex  " << evt->GetNumberOfPrimaryVertex() << G4endl;
        G4cout << "  +-+    " <<  G4endl;
        for(int vtxIndex=0; vtxIndex < evt->GetNumberOfPrimaryVertex(); vtxIndex++) {
            auto vtx = evt->GetPrimaryVertex(vtxIndex);
            fmt::print("   | VertexID: {}\n", vtxIndex);
            fmt::print("   | x: {:<10} y: {:<10} z: {:<10}\n", vtx->GetX0(), vtx->GetY0(), vtx->GetZ0());
            fmt::print("   | GetNumberOfParticle {}\n", vtx->GetNumberOfParticle());
            fmt::print("   +-+\n");
            for(int prtIndex=0; prtIndex < vtx->GetNumberOfParticle(); prtIndex++) {
                auto particle = vtx->GetPrimary(prtIndex);
                fmt::print("     | ID{:<10} trkId: {:<10}\n", prtIndex, particle->GetTrackID(), particle->GetTotalMomentum());
            }
        }
    }
}



void JLeicEventAction::EndOfEventAction(const G4Event *evt) {
    static std::atomic<std::uint64_t> totalEventCounter(0);
    totalEventCounter++;
    fmt::print("Events processed: {} \n", (uint64_t)totalEventCounter);

//    for(auto trajectory: *evt->GetTrajectoryContainer()->GetVector()) {
//        fmt::print("\ntrajectory start:\n");
//        for(int i=0; i < trajectory->GetPointEntries(); i++) {
//            auto point = trajectory->GetPoint(i);
//            auto pos = point->GetPosition();
//            fmt::print("   {:<10} {:<10} {:<10}\n", pos.x()/mm, pos.y()/mm, pos.z()/mm);
//        }
//    }

    G4HCofThisEvent *hitCollectionEvnt = evt->GetHCofThisEvent();

    JLeicCalorHitsCollection *hitCollectionCalo = nullptr;
    G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
    if(HCE) {
        hitCollectionCalo = (JLeicCalorHitsCollection *) (HCE->GetHC(calorimeterCollID));
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

        nstep = nstepCharged + nstepNeutral;
    }

    const G4int primeVtxCount = evt->GetNumberOfPrimaryVertex();
    size_t particleId = 0;  // prime particle ID unique for all prime vertexes


    // ROOOT OUTPUUUT
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

        // Add generated particles to root output
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

    mRootEventsOut->FillEvent((uint64_t)evt->GetEventID());


    // VERTEX HITS
    JLeicVTXHitsCollection *vertexHitsCollection = nullptr;
    if (hitCollectionEvnt) {
        vertexHitsCollection = (JLeicVTXHitsCollection *) (hitCollectionEvnt->GetHC(vertexCollID));
    }

    if (vertexHitsCollection) {
        int n_hit = vertexHitsCollection->entries();
        if (fVerbose >= 1)
            G4cout << "     " << n_hit
                   << " hits are stored in JLeicVTXHitsCollection." << G4endl;

        G4double totEAbs = 0, totLAbs = 0;
        for (int i = 0; i < n_hit; i++) {
            totEAbs += (*vertexHitsCollection)[i]->GetEdepAbs();
            totLAbs += (*vertexHitsCollection)[i]->GetTrakAbs();
        }

    
        if (fVerbose >= 1) {
            G4cout << "  VTX::  Absorber: total energy: " << std::setw(7) <<
                    G4BestUnit(totEAbs, "Energy")
                    << "       total track length: " << std::setw(7) <<
                    G4BestUnit(totLAbs, "Length")
                    << G4endl;
        }
    }

    // totalEventCounter++;
    //  G4cout << "Event processed: " << totalEventCounter << G4endl;

    if (fVerbose > 1) {
        G4cout << "JLeicEventAction:: Event END " << evt->GetEventID() << G4endl;
        G4cout << "  |  GetNumberOfGrips          " << evt->GetNumberOfGrips() << G4endl;
        G4cout << "  |  GetNumberOfPrimaryVertex  " << evt->GetNumberOfPrimaryVertex() << G4endl;
        G4cout << "  +-+    " <<  G4endl;

        std::vector<G4PrimaryParticle*> allPrimaries;
        for(int vtxIndex=0; vtxIndex < evt->GetNumberOfPrimaryVertex(); vtxIndex++) {
            auto vtx = evt->GetPrimaryVertex(vtxIndex);
            fmt::print("    | VertexID: {}\n", vtxIndex);
            fmt::print("    | x: {:<10} y: {:<10} z: {:<10}\n", vtx->GetX0(), vtx->GetY0(), vtx->GetZ0());
            fmt::print("    | GetNumberOfParticle {}\n", vtx->GetNumberOfParticle());
            fmt::print("    +-+\n");
            for(int prtIndex=0; prtIndex < vtx->GetNumberOfParticle(); prtIndex++) {
                auto particle = vtx->GetPrimary(prtIndex);
                fmt::print("      | ID{:<10} trkId: {:<10}\n", prtIndex, particle->GetTrackID());
                allPrimaries.push_back(particle);
            }
        }

        G4cout << "  |  PrimaryParticlesNumber  " << allPrimaries.size() << G4endl;

        for(auto particle: allPrimaries) {
            fmt::print("    | PID{:<10} trkId: {:<10} P:{:<10}\n",particle->GetParticleDefinition()->GetPDGEncoding(), particle->GetTrackID(), particle->GetTotalMomentum());
        }
    }
}
