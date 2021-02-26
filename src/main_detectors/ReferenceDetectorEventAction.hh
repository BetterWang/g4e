#ifndef JLeicEventAction_h
#define JLeicEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "RootFlatIO.hh"
#include <G4GenericMessenger.hh>


class JLeicHistogramManager;


class ReferenceDetectorEventAction : public G4UserEventAction
{
public:
    ReferenceDetectorEventAction(g4e::RootFlatIO *);

    ~ReferenceDetectorEventAction() = default;

public:
    void BeginOfEventAction(const G4Event *) override;

    void EndOfEventAction(const G4Event *) override;

    //----- EVENT STRUCTURE -----
    g4e::RootFlatIO *mRootEventsOut = nullptr;

private:
    G4int calorimeterCollID;
    G4int vertexCollID;
    G4int Ce_emcalCollID;
    G4int fVerbose = 0;

    G4GenericMessenger fMessenger;
};

#endif

    
