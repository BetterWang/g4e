#include "ReferenceDetectorConstruction.hh"
#include "ReferenceDetectorSteppingAction.hh"
#include <G4SteppingManager.hh>



ReferenceDetectorSteppingAction::ReferenceDetectorSteppingAction(g4e::RootFlatIO* rootEventOut): mFlatIo(rootEventOut)
{
}


ReferenceDetectorSteppingAction::~ReferenceDetectorSteppingAction()
{
}


void ReferenceDetectorSteppingAction::UserSteppingAction(const G4Step *aStep)
{

}



