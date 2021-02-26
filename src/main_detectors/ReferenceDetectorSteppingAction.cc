#include "ReferenceDetectorConstruction.hh"
#include "ReferenceDetectorSteppingAction.hh"
#include <G4SteppingManager.hh>



ReferenceDetectorSteppingAction::ReferenceDetectorSteppingAction(JLeicHistogramManager *histo, g4e::RootFlatIO* rootEventOut): fHistoManager(histo), mFlatIo(rootEventOut)
{
}


ReferenceDetectorSteppingAction::~ReferenceDetectorSteppingAction()
{
}


void ReferenceDetectorSteppingAction::UserSteppingAction(const G4Step *aStep)
{

}



