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
    std::string volumeName = touchable->GetVolume()->GetName();

    // process hit
    int mHitsCount=0;

    // process    track
    int curTrackID = aStep->GetTrack()->GetTrackID();
    auto track = aStep->GetTrack();

    // particle
    //G4ParticleDefinition * aParticle = aTrack->GetDefinition();
    //G4DynamicParticle*     dParticle = aTrack->GetDynamicParticle();
    G4ThreeVector momentum = track->GetMomentum();
    const G4ThreeVector& momentumDir = track->GetMomentumDirection();
    G4int parentId = track->GetParentID();
    const G4ThreeVector& position = track->GetPosition();
    const G4ThreeVector& vertex = track->GetVertexPosition();
    const G4ThreeVector& vertexMom = track->GetVertexMomentumDirection();
    G4int PDG = track->GetDefinition()->GetPDGEncoding();
    const double charge = track->GetDefinition()->GetPDGCharge();

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
            /* hit id        */ mHitsCount,
            /* track id      */ aStep->GetTrack()->GetTrackID(),
            /* parent Trk Id */ aStep->GetTrack()->GetParentID(),
            /* hit x         */ point->GetPosition().x() / mm,
            /* hit y         */ point->GetPosition().y() / mm,
            /* hit z         */ point->GetPosition().z() / mm,
            /* hit ox        */ (xinp + xend) / 2.,
            /* hit oy        */ (yinp + yend) / 2.,
            /* hit oz        */ (zinp + zend) / 2.,
            /* aELoss        */ aStep->GetTotalEnergyDeposit() / GeV,
            /* vol replic x  */  copyIDx,
            /* vol replic y  */  copyIDy,
            /* Volume Name   */  volumeName
    );


//    auto primaryParticle = track->GetDynamicParticle()->GetPrimaryParticle();
//    if(primaryParticle) {
//        fmt::print("Primary aTrack.id={:<10} id={}\n", track->GetTrackID(), primaryParticle->GetTrackID());
//    }

    //-- fill tracks --
    jleicRootOutput->AddTrack(curTrackID,                   /* int aTrackId */
                      parentId,                             /* int aParentId */
                      PDG,                                  /* int aTrackPdg */
                      process_int,                           /* creator proc id */
                      charge,                                /* pdg charge */
                      ancestryLevel,                        /* ancestry level */
                      vertex.x() / mm,              /* double aXVertex */
                      vertex.y() / mm,              /* double aYVertex */
                      vertex.z() / mm,              /* double aZVertex */
                      vertexMom.x(),                        /* double aXMom    */
                      vertexMom.y(),                        /* double aYMom  */
                      vertexMom.z(),                        /* double aZMom */
                      momentum.mag() / GeV            /* double aMom  */
    );

}



