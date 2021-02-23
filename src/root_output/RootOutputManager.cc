#include "RootOutputManager.hh"

#include <TFile.h>
#include <TTree.h>
#include <G4Step.hh>
#include <G4TouchableHistory.hh>
#include <G4PrimaryParticle.hh>
#include <G4VProcess.hh>
#include <G4SystemOfUnits.hh>
#include <HitTypes.h>
#include <JLeicTrackInformation.hh>

g4e::RootOutputManager::RootOutputManager(TFile *rootFile):
    mRootFile(rootFile),
    jleicRootOutput(new RootFlatIO()),
    fMessenger(this, "/rootOutput/")
{
    mFlatEventTree = new TTree("events", "Flattened root tree with event data");
    mFlatEventTree->SetDirectory(mRootFile);
    // We also create JLeic root output here , while g4e is in transition
    // TODO Move JLeic initialization to the appropriate phase
    jleicRootOutput->Initialize(mRootFile, mFlatEventTree);
    mSaveSecondaryLevel = 3;
    fMessenger.DeclareProperty("saveSecondaryLevel", mSaveSecondaryLevel, "-1 save all, 0 - save only generated particles, 1 - n level of secondaries to save");
}

void g4e::RootOutputManager::SaveStep(const G4Step * aStep, WriteStepPointChoices pointChoice, G4int copyIDx, G4int copyIDy)
{
    G4StepPoint * point = pointChoice == WriteStepPointChoices::PreStepPoint ? aStep->GetPreStepPoint(): aStep->GetPostStepPoint();

    auto touchable = point->GetTouchable();
    if(!touchable || !touchable->GetVolume()) return;

    std::string volumeName = touchable->GetVolume()->GetName();

    /* >oO Debug
    std::string preName = aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    std::string postName = aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();
    if(preName!=postName || volumeName!=preName)
        fmt::print(">>>> {:<30} {:<30} {:<30} {:<3} {:<3} {:<10}\n", volumeName, preName, postName,
                   aStep->IsFirstStepInVolume(), aStep->IsLastStepInVolume(), point->GetPosition().z());
    */


    // process    track
    int curTrackID = aStep->GetTrack()->GetTrackID();
    auto track = aStep->GetTrack();

    // particle
    //G4ParticleDefinition * aParticle = aTrack->GetDefinition();
    //G4DynamicParticle*     dParticle = aTrack->GetDynamicParticle();
    G4ThreeVector momentum = track->GetMomentum();
    G4int parentId = track->GetParentID();
    const G4ThreeVector& vertex = track->GetVertexPosition();
    const G4ThreeVector& vertexMom = track->GetVertexMomentumDirection();
    G4int pdg = track->GetDefinition()->GetPDGEncoding();

    auto process = track->GetCreatorProcess();

    // For generated particles process will be null, we set process_int == -1
    int process_int = -1;

    if(process) {
        process_int = (int)process->GetProcessType();
    }

    auto info = dynamic_cast<JLeicTrackInformation*>(track->GetUserInformation());
    int ancestryLevel = info ? info->GetAncestryLevel() : -1;



    if(mSaveSecondaryLevel>=0 && ancestryLevel > mSaveSecondaryLevel) {
        return;
    }

    HitTypes type = HitTypes::Sensitive;
    if(aStep->IsFirstStepInVolume()) type = HitTypes::VolumeEnter;
    if(aStep->IsLastStepInVolume()) type = HitTypes::VolumeLeave;

    //-- fill tracks --
    uint64_t trackIndex = jleicRootOutput->AddTrack(
            curTrackID,                /* int aTrackId    */
            parentId,                  /* int aParentId   */
            pdg,                       /* int aTrackPdg   */
            process_int,               /* creator proc id */
            ancestryLevel,             /* ancestry level  */
            vertex.x() / mm,   /* double aXVertex */
            vertex.y() / mm,   /* double aYVertex */
            vertex.z() / mm,   /* double aZVertex */
            vertexMom.x(),             /* double aXMom    */
            vertexMom.y(),             /* double aYMom    */
            vertexMom.z(),             /* double aZMom    */
            momentum.mag() / GeV /* double aMom     */
    );


    jleicRootOutput->AddHit(
                        trackIndex,                                         /* index of a track in tracks array */
                        aStep->GetTrack()->GetTrackID(),                    /* track id      */
                        aStep->GetTrack()->GetParentID(),                   /* parent Trk Id */
                        aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding(), /* PDG */
                        point->GetPosition().x() / mm,                   /* hit x         */
                        point->GetPosition().y() / mm,                   /* hit y         */
                        point->GetPosition().z() / mm,                   /* hit z         */
                        aStep->GetTotalEnergyDeposit() / GeV,            /* aELoss        */
                        copyIDx,                                         /* vol replic x  */
                        copyIDy,                                         /* vol replic y  */
                        volumeName,                                      /* Volume Name   */
                        (int)type
    );

//    auto primaryParticle = track->GetDynamicParticle()->GetPrimaryParticle();
//    if(primaryParticle) {
//        fmt::print("Primary aTrack.id={:<10} id={}\n", track->GetTrackID(), primaryParticle->GetTrackID());
//    }


}



