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
// $Id: JLeicTrackingAction.cc 67268 2013-02-13 11:38:40Z ihrivnac $
//
/// \file electromagnetic/VertexEIC/src/JLeicTrackingAction.cc
/// \brief Implementation of the JLeicTrackingAction class
//
#include <external/spdlog/include/spdlog/spdlog.h>
#include "JLeicTrackingAction.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"

#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "JLeicTrackInformation.hh"

bool hasWrite = false;
JLeicTrackingAction::JLeicTrackingAction() : G4UserTrackingAction() {}

void JLeicTrackingAction::PreUserTrackingAction(const G4Track *aTrack)
{
    JLeicTrackInformation * info;

    if(aTrack->GetParentID() == 0) {
        info = new JLeicTrackInformation();
        info->SetLevel(0);
        aTrack->SetUserInformation(info);
    } else {
        info = (JLeicTrackInformation*) aTrack->GetUserInformation();
    }


//    if(aTrack->GetParentID() == 0) {
//
//    }
//    mTrackLevelsById[aTrack->GetTrackID()] = level;
//    if(level > 1) {
//        level3++;
//    }
//    total ++;
//
//    if(level > 1) {
//        const_cast<G4Track *>(aTrack)->SetTrackStatus(fKillTrackAndSecondaries);
//    }



//    if(aTrack && aTrack->GetTrackID() < 40) {
//        auto part = aTrack->GetDynamicParticle();
//        spdlog::info("PreTracking  id: {:<7} parent: {:<7}  P: {:<12} GeV DynPrtP: {:<12}", aTrack->GetTrackID(), aTrack->GetParentID(), aTrack->GetMomentum().mag() / GeV, part->GetTotalMomentum()/GeV);
//    }
//
//if (aTrack->IsGoodForTracking() ) {


//
//        G4cout << "[JLeicTrackingAction::PreUserTrackingAction]" << G4endl;
//        G4cout << "  Track ID:          " << aTrack->GetTrackID() << G4endl;
//        G4cout << "  Parent ID:         " << aTrack->GetParentID() << G4endl;
//        G4cout << "  particle:          " << aTrack->GetDynamicParticle()->GetDefinition()->GetParticleName() << " : "<< aTrack->GetDynamicParticle()->GetDefinition()->GetPDGEncoding()<< G4endl;
//        G4cout << "  level:             " << info->GetLevel() << G4endl;
//
//        G4cout << "  level>3:             " << level3 << G4endl;
//        G4cout << "  total:             " << total << G4endl;


//        G4cout << "  Good 4 tracking:   " << aTrack->IsGoodForTracking() << G4endl;
//
//        if(aTrack->GetDynamicParticle()->GetPrimaryParticle()) {
//            G4cout << "  primary:   " << aTrack->GetDynamicParticle()->GetPrimaryParticle()->GetPDGcode() << G4endl;
//        }
//
//        if(aTrack->GetCreatorProcess()) {
//            G4cout << "  created by:        " << aTrack->GetCreatorProcess()->GetProcessName() << G4endl;
//            aTrack->GetCreatorProcess()->DumpInfo();
//        } else {
//            G4cout << "  created by:        NullPtr" << G4endl;
//        }
//
//        G4cout << "  kin. energy (GeV): " << aTrack->GetKineticEnergy() / GeV << G4endl;
//        G4cout << "  tot. energy (GeV): " << aTrack->GetTotalEnergy() / GeV << G4endl;
//
//        if(aTrack->GetVolume()) {
//            G4cout << "  volume:            " << aTrack->GetVolume()->GetName() << G4endl;
//        } else {
//            G4cout << "  volume:            NullPtr" << G4endl;
//        }
//
////        aTrack->SetGoodForTrackingFlag()
//
//        G4cout << "  global time:       " << aTrack->GetGlobalTime() << G4endl;
//
////        G4cout << " Killing event..." << G4endl;
//        //if (aTrack->GetTrackID() != 1)
//          //  const_cast<G4Track *>(aTrack)->SetTrackStatus(fKillTrackAndSecondaries);
//    }
}

void JLeicTrackingAction::PostUserTrackingAction(const G4Track *aTrack)
{
    auto info = (JLeicTrackInformation*) aTrack->GetUserInformation();

    // From the tracking manager we can retrieve the secondary track vector,
    // which is a container class for tracks:
    auto secTracks = fpTrackingManager->GimmeSecondaries();
    for(auto secTrack: *secTracks) {
        auto secInfo = new JLeicTrackInformation();
        secInfo->SetLevel(info->GetLevel()+1);
        secTrack->SetUserInformation(secInfo);
    }



    /*
    if(aTrack && aTrack->GetTrackID() < 20) {
        auto part = aTrack->GetDynamicParticle();
        spdlog::info("PostTracking  id: {:<7} parent: {:<7}  P: {:<12} GeV DynPrtP: {:<12}", aTrack->GetTrackID(), aTrack->GetParentID(), aTrack->GetMomentum().mag() / GeV, part->GetTotalMomentum()/GeV);
    }*/

////    if (aTrack->GetParentID() == 1 && aTrack->GetKineticEnergy() > 100. * GeV) {
    //G4cout << "[JLeicTrackingAction::PostUserTrackingAction]" << G4endl;
//    G4cout << "  Track ID:          " << aTrack->GetTrackID() << G4endl;
//    G4cout << "  particle:          " << aTrack->GetDynamicParticle()->GetDefinition()->GetParticleName() << G4endl;
//    G4cout << "  Parent ID:         " << aTrack->GetParentID() << G4endl;
    ///G4cout << "  secTracks:             " << secTracks->size() << G4endl;
//    if(aTrack->GetCreatorProcess()) {
//        G4cout << "  created by:        " << aTrack->GetCreatorProcess()->GetProcessName() << G4endl;
//    } else {
//        G4cout << "  created by:        NullPtr" << G4endl;
//    }
//
//    G4cout << "  kin. energy (TeV): " << aTrack->GetKineticEnergy() / TeV << G4endl;
//
//    if(aTrack->GetVolume()) {
//        G4cout << "  volume:            " << aTrack->GetVolume()->GetName() << G4endl;
//    } else {
//        G4cout << "  volume:            NullPtr" << G4endl;
//    }
//
//    G4cout << "  global time:       " << aTrack->GetGlobalTime() << G4endl;

//        G4cout << " Killing event..." << G4endl;
    //if (aTrack->GetTrackID() != 1)
    //  const_cast<G4Track *>(aTrack)->SetTrackStatus(fKillTrackAndSecondaries);
//    }
}
