#include "RootOutputManager.hh"

#include <TFile.h>
#include <TTree.h>

#include <G4Step.hh>
#include <G4TouchableHistory.hh>

g4e::RootOutputManager::RootOutputManager(TFile *rootFile):
    mRootFile(rootFile),
    jleicRootOutput(new RootFlatIO()),
    jleicHistos(new JLeicHistogramManager())
{
    mFlatEventTree = new TTree("events", "Flattened root tree with event data");
    mFlatEventTree->SetDirectory(mRootFile);
    // We also create JLeic root output here , while g4e is in transition
    // TODO Move JLeic initialization to the appropriate phase
    jleicRootOutput->Initialize(mRootFile, mFlatEventTree);
}

void g4e::RootOutputManager::SaveStep(const G4Step * aStep, WriteStepPointChoices pointChoice, G4int copyIDx, G4int copyIDy)
{
    G4StepPoint * point = pointChoice == WriteStepPointChoices::PreStepPoint ? aStep->GetPreStepPoint(): aStep->GetPostStepPoint();

    auto touchable = point->GetTouchable();
    std::string volumeName = touchable->GetVolume()->GetName();

    // process hit
    int mHitsCount=0;

    // process    track
    G4Track *aTrack = aStep->GetTrack();
    int curTrackID = aStep->GetTrack()->GetTrackID();
    auto track = aStep->GetTrack();

    // particle
    //G4ParticleDefinition * aParticle = aTrack->GetDefinition();
    //G4DynamicParticle*     dParticle = aTrack->GetDynamicParticle();
    G4ThreeVector momentum = aTrack->GetMomentum();
    G4ThreeVector momentumDir = aTrack->GetMomentumDirection();
    G4int parentId = aTrack->GetParentID();
    G4ThreeVector position = aTrack->GetPosition();
    G4ThreeVector vertex = aTrack->GetVertexPosition();
    G4ThreeVector vertexMom = aTrack->GetVertexMomentumDirection();
    G4int PDG = aTrack->GetDefinition()->GetPDGEncoding();

    // Save only good for tracking tracks
    // TODO make it an variable
    if(!aTrack->IsGoodForTracking()) {
        return;
    }

    jleicRootOutput->AddHit(
            /* hit id        */ mHitsCount,
            /* track id      */ aStep->GetTrack()->GetTrackID(),
            /* parent Trk Id */ aStep->GetTrack()->GetParentID(),
            /* hit x         */ point->GetPosition().x() / mm,
            /* hit y         */ point->GetPosition().y() / mm,
            /* hit z         */ point->GetPosition().z() / mm,
            /* aELoss        */ aStep->GetTotalEnergyDeposit() / GeV,
            /* vol replic x  */  copyIDx,
            /* vol replic y  */  copyIDy,
            /* Volume Name   */  volumeName
    );


    //-- fill tracks --
    jleicRootOutput->AddTrack(curTrackID,                           /* int aTrackId,*/
                      parentId,                             /* int aParentId,*/
                      PDG,                                  /* int aTrackPdg,*/
                      vertex.x() / mm,              /* double aXVertex,*/
                      vertex.y() / mm,              /* double aYVertex,*/
                      vertex.z() / mm,              /* double aZVertex,*/
                      vertexMom.x(),                        /* double aXMom, */
                      vertexMom.y(),                        /* double aYMom,*/
                      vertexMom.z(),                        /* double aZMom,*/
                      momentum.mag() / GeV            /* double aMom*/
    );

}



