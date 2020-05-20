#include "JLeicDetectorConstruction.hh"
#include "JLeicSteppingAction.hh"
#include <G4SteppingManager.hh>



JLeicSteppingAction::JLeicSteppingAction(JLeicHistogramManager *histo, g4e::RootFlatIO* rootEventOut): fHistoManager(histo), mFlatIo(rootEventOut)
{
}


JLeicSteppingAction::~JLeicSteppingAction()
{
}


void JLeicSteppingAction::UserSteppingAction(const G4Step *aStep)
{
}



