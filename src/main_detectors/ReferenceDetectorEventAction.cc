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
#include "ReferenceDetectorEventAction.hh"

#include "ReferenceDetectorRunAction.hh"

#include "CommonCalorimeterHit.hh"
#include "CommonVertexHit.hh"

#include "subdetectors/ce_EMCAL/JLeicCe_emcalDigiHit.hh"
#include "subdetectors/ce_EMCAL/JLeicCe_emcalHit.hh"


#include "G4Event.hh"
#include "G4PrimaryVertex.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4UnitsTable.hh"
#include <G4Trajectory.hh>
#include <G4SystemOfUnits.hh>

ReferenceDetectorEventAction::ReferenceDetectorEventAction(g4e::RootFlatIO *rootOutput) :
    calorimeterCollID(-1),
    vertexCollID(-1),
    Ce_emcalCollID(-1),
    mRootEventsOut(rootOutput),
    fVerbose(1),
    fMessenger(this, "/eic/refdet/eventAction/")
{
    fMessenger.DeclareProperty("verbose", fVerbose, "Verbosity. 0-none, 1-some, 2 many");
}

void ReferenceDetectorEventAction::BeginOfEventAction(const G4Event *evt)
{
    G4int eventId = evt->GetEventID();

    mRootEventsOut->ClearForNewEvent();

    if (calorimeterCollID == -1) {
        G4SDManager *SDman = G4SDManager::GetSDMpointer();
        calorimeterCollID = SDman->GetCollectionID("CalCollection");
    }
    if (vertexCollID == -1) {
        G4SDManager *SDman = G4SDManager::GetSDMpointer();
        vertexCollID = SDman->GetCollectionID("VTXCollection");
    }

    if (Ce_emcalCollID == -1) {
        G4SDManager *SDman = G4SDManager::GetSDMpointer();
        Ce_emcalCollID = SDman->GetCollectionID("Ce_emcalCollection");
    }


    if (eventId == 0) printf("----> Begin of Event: %d \n", eventId);


    // Pretty prints generated particles and vertexes
    if (fVerbose > 1) {
        G4cout << "JLeicEventAction:: Event START " << evt->GetEventID() << G4endl;
        G4cout << "  |  GetNumberOfGrips          " << evt->GetNumberOfGrips() << G4endl;
        G4cout << "  |  GetNumberOfPrimaryVertex  " << evt->GetNumberOfPrimaryVertex() << G4endl;
        G4cout << "  +-+    " << G4endl;
        for (int vtxIndex = 0; vtxIndex < evt->GetNumberOfPrimaryVertex(); vtxIndex++) {
            auto vtx = evt->GetPrimaryVertex(vtxIndex);
            fmt::print("   | VertexID: {}\n", vtxIndex);
            fmt::print("   | x: {:<10} y: {:<10} z: {:<10}\n", vtx->GetX0(), vtx->GetY0(), vtx->GetZ0());
            fmt::print("   | GetNumberOfParticle {}\n", vtx->GetNumberOfParticle());
            fmt::print("   +-+\n");
            for (int prtIndex = 0; prtIndex < vtx->GetNumberOfParticle(); prtIndex++) {
                auto particle = vtx->GetPrimary(prtIndex);
                fmt::print("     | ID{:<10} trkId: {:<10}\n", prtIndex, particle->GetTrackID(), particle->GetTotalMomentum());
            }
        }
    }
}


void ReferenceDetectorEventAction::EndOfEventAction(const G4Event *evt)
{
    static std::atomic<std::uint64_t> totalEventCounter(0);
    totalEventCounter++;
    fmt::print("Events processed: {} \n", (uint64_t) totalEventCounter);

    if(fVerbose > 3) {
        for(auto trajectory: *evt->GetTrajectoryContainer()->GetVector()) {
            fmt::print("\ntrajectory start:\n");
            for(int i=0; i < trajectory->GetPointEntries(); i++) {
                auto point = trajectory->GetPoint(i);
                auto pos = point->GetPosition();
                fmt::print("   {:<10} {:<10} {:<10}\n", pos.x()/mm, pos.y()/mm, pos.z()/mm);
            }
        }
    }

    G4HCofThisEvent *hitCollection = evt->GetHCofThisEvent();

    CommonCalorimeterHitsCollection *hitCollectionCalo = nullptr;
    JLeicCe_emcalHitsCollection *hitCollectionCe_emcal = nullptr;


    if (hitCollection && calorimeterCollID!=-1) {
        hitCollectionCalo = (CommonCalorimeterHitsCollection *) (hitCollection->GetHC(calorimeterCollID));
    }

    if(hitCollection && Ce_emcalCollID!=-1
    ) {
        hitCollectionCe_emcal = (JLeicCe_emcalHitsCollection *) (hitCollection->GetHC(Ce_emcalCollID));
    }


    if (hitCollectionCe_emcal) {
        JLeicCe_emcalHit *aHit;
        JLeicCe_emcalDigiHit *dHit;
        int nhitC = hitCollectionCe_emcal->GetSize();

        for (int i = 0; i < nhitC; i++) {

            aHit = (*hitCollectionCe_emcal)[i];
            JLeicCe_emcalDigiHit *dHit2 = dHit->JLeicCe_emcalDigi(aHit);
            string name_det = dHit2->GetDetName();
            double Etot_crs = dHit2->GetEdep();
            int Npe = dHit2->GetNpe();
            double ADC_crs = dHit2->GetADC();
            double TDCL_crs = dHit2->GetTDC();
            double Xxcrs = aHit->GetX_crs();
            double Yycrs = aHit->GetY_crs();
            double Zzcrs = aHit->GetZ_crs();

            if (Etot_crs > 0) mRootEventsOut->FillCe_EMCAL({name_det, aHit->GetCopyNumber(), Etot_crs, Npe, ADC_crs, TDCL_crs, Xxcrs, Yycrs, Zzcrs});
        }
    }


    if (hitCollectionCalo) {
        int n_hit = hitCollectionCalo->entries();

        G4double totEAbs = 0, totLAbs = 0;
        for (int i = 0; i < n_hit; i++) {
            totEAbs += (*hitCollectionCalo)[i]->GetEdepAbs();
            totLAbs += (*hitCollectionCalo)[i]->GetTrakAbs();
        }
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

    mRootEventsOut->FillEvent(evt);


    // VERTEX HITS
    JLeicVTXHitsCollection *vertexHitsCollection = nullptr;
    if (hitCollection && vertexCollID != -1) {
        vertexHitsCollection = (JLeicVTXHitsCollection *) (hitCollection->GetHC(vertexCollID));
    }

    if (vertexHitsCollection) {
        int n_hit = vertexHitsCollection->entries();
        if (fVerbose >= 1)
            G4cout << "     " << n_hit << " hits are stored in JLeicVTXHitsCollection." << G4endl;

        G4double totEAbs = 0, totLAbs = 0;
        for (int i = 0; i < n_hit; i++) {
            totEAbs += (*vertexHitsCollection)[i]->GetEdepAbs();
            totLAbs += (*vertexHitsCollection)[i]->GetTrakAbs();
        }


        if (fVerbose >= 1) {
            G4cout << "  VTX::  Absorber: total energy: " << std::setw(7) << G4BestUnit(totEAbs, "Energy") << "       total track length: " << std::setw(7)
                   << G4BestUnit(totLAbs, "Length") << G4endl;
        }
    }

    // totalEventCounter++;
    //  G4cout << "Event processed: " << totalEventCounter << G4endl;

    if (fVerbose > 1) {
        G4cout << "JLeicEventAction:: Event END " << evt->GetEventID() << G4endl;
        G4cout << "  |  GetNumberOfGrips          " << evt->GetNumberOfGrips() << G4endl;
        G4cout << "  |  GetNumberOfPrimaryVertex  " << evt->GetNumberOfPrimaryVertex() << G4endl;
        G4cout << "  +-+    " << G4endl;

        std::vector<G4PrimaryParticle *> allPrimaries;
        for (int vtxIndex = 0; vtxIndex < evt->GetNumberOfPrimaryVertex(); vtxIndex++) {
            auto vtx = evt->GetPrimaryVertex(vtxIndex);
            fmt::print("    | VertexID: {}\n", vtxIndex);
            fmt::print("    | x: {:<10} y: {:<10} z: {:<10}\n", vtx->GetX0(), vtx->GetY0(), vtx->GetZ0());
            fmt::print("    | GetNumberOfParticle {}\n", vtx->GetNumberOfParticle());
            fmt::print("    +-+\n");
            for (int prtIndex = 0; prtIndex < vtx->GetNumberOfParticle(); prtIndex++) {
                auto particle = vtx->GetPrimary(prtIndex);
                fmt::print("      | ID{:<10} trkId: {:<10}\n", prtIndex, particle->GetTrackID());
                allPrimaries.push_back(particle);
            }
        }

        G4cout << "  |  PrimaryParticlesNumber  " << allPrimaries.size() << G4endl;

        for (auto particle: allPrimaries) {
            fmt::print("    | PID{:<10} trkId: {:<10} P:{:<10}\n", particle->GetParticleDefinition()->GetPDGEncoding(), particle->GetTrackID(), particle->GetTotalMomentum());
        }
    }
}
