//
// Created by romanov on 1/24/20.
//

#include "JLeicActionInitialization.hh"
#include "JLeicTrackingAction.hh"
#include "JLeicSteppingAction.hh"
#include "JLeicRunAction.hh"
#include "JLeicEventAction.hh"

#include <G4MultiRunAction.hh>
#include <G4MultiEventAction.hh>
#include <G4MultiTrackingAction.hh>
#include <G4MultiSteppingAction.hh>
#include <generator/PrimaryGeneratorAction.hh>


JLeicActionInitialization::JLeicActionInitialization(g4e::RootOutputManager* rootOutput):
    mRootOuput(rootOutput)
{

}

void JLeicActionInitialization::Build() const
{


    // Primary Generator
    auto pgAction = new PrimaryGeneratorAction();
    SetUserAction(pgAction);

    // Event action
    auto jleicEventAction = new JLeicEventAction(mRootOuput->GetJLeicRootOutput(), mRootOuput->GetJLeicHistogramManager());
    SetUserAction(jleicEventAction);

    // Stepping action
    auto steppingAction = new JLeicSteppingAction(mRootOuput->GetJLeicHistogramManager());
    SetUserAction(steppingAction);

    // Tracking action
    auto trackingAction = new JLeicTrackingAction();
    SetUserAction(trackingAction);
}

void JLeicActionInitialization::BuildForMaster() const
{
    // RUN action
    auto runAction = new JLeicRunAction(mRootOuput->GetJLeicRootOutput(), mRootOuput->GetJLeicHistogramManager());
    SetUserAction(runAction);
}


