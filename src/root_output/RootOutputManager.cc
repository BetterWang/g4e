#include "RootOutputManager.hh"

#include <TFile.h>
#include <TTree.h>

#include <G4Step.hh>
#include <G4TouchableHistory.hh>
#include <G4PrimaryParticle.hh>
#include <G4VProcess.hh>
#include <main_detectors/jleic/JLeicTrackInformation.hh>

g4e::RootOutputManager::RootOutputManager(TFile *rootFile):
    mRootFile(rootFile),
    jleicRootOutput(new RootFlatIO()),
    jleicHistos(new JLeicHistogramManager()),
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

    // process hit
    int mHitsCount=0;

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
    const G4double charge = track->GetDefinition()->GetPDGCharge();

    auto process = track->GetCreatorProcess();

    // For generated particles process will be null, we set process_int == -1
    int process_int = -1;

    if(process) {
        process_int = (int)process->GetProcessType();
    }

    auto info = dynamic_cast<JLeicTrackInformation*>(track->GetUserInformation());
    int ancestryLevel = info ? info->GetAncestryLevel() : -1;

    if(mSaveSecondaryLevel!=-1 && ancestryLevel > mSaveSecondaryLevel) {
        return;
    }

    //-- fill tracks --
    uint64_t trackIndex = jleicRootOutput->AddTrack(
            curTrackID,                /* int aTrackId    */
            parentId,                  /* int aParentId   */
            pdg,                       /* int aTrackPdg   */
            charge,                    /* double aTrackCharge */
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

    const G4TouchableHandle touchablepre = aStep->GetPreStepPoint()->GetTouchableHandle();
    G4ThreeVector worldPosition = aStep->GetPreStepPoint()->GetPosition();
    G4ThreeVector localPosition = touchablepre->GetHistory()->GetTopTransform().TransformPoint(worldPosition);
    G4double xinp = localPosition.x();
    G4double yinp = localPosition.y();
    G4double zinp = localPosition.z();

    G4ThreeVector worldPosition2 = aStep->GetPostStepPoint()->GetPosition();
    G4ThreeVector localPosition2 = touchablepre->GetHistory()->GetTopTransform().TransformPoint(worldPosition2);
    G4double xend = localPosition2.x();
    G4double yend = localPosition2.y();
    G4double zend = localPosition2.z();

    G4double xloc = (xinp + xend) / 2;
    G4double yloc = (yinp + yend) / 2;
    G4double zloc = (zinp + zend) / 2;

    jleicRootOutput->AddHit(
                        mHitsCount,                                         /* hit id        */
                        trackIndex,                                         /* index of a track in tracks array */
                        aStep->GetTrack()->GetTrackID(),                    /* track id      */
                        aStep->GetTrack()->GetParentID(),                   /* parent Trk Id */
                        aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding(), /* PDG */
                        point->GetPosition().x() / mm,                   /* hit x         */
                        point->GetPosition().y() / mm,                   /* hit y         */
                        point->GetPosition().z() / mm,                   /* hit z         */
                        xloc / mm,                         /* hit ox */
                        yloc / mm,                         /* hit oy */
                        zloc / mm,                         /* hit oz */
                        aStep->GetTotalEnergyDeposit() / GeV,         /* aELoss        */
                        copyIDx,                                            /* vol replic x  */
                        copyIDy,                                            /* vol replic y  */
                        volumeName                                          /* Volume Name   */
    );

//    auto primaryParticle = track->GetDynamicParticle()->GetPrimaryParticle();
//    if(primaryParticle) {
//        fmt::print("Primary aTrack.id={:<10} id={}\n", track->GetTrackID(), primaryParticle->GetTrackID());
//    }


}



