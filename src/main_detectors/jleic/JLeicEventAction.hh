#ifndef JLeicEventAction_h
#define JLeicEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "RootFlatIO.hh"
#include <G4GenericMessenger.hh>


class JLeicHistogramManager;


class JLeicEventAction : public G4UserEventAction
{
public:
    JLeicEventAction(g4e::RootFlatIO *);

    ~JLeicEventAction() = default;

public:
    void BeginOfEventAction(const G4Event *) override;

    void EndOfEventAction(const G4Event *) override;

    //----- EVENT STRUCTURE -----
    g4e::RootFlatIO *mRootEventsOut = nullptr;

private:
    G4int calorimeterCollID;
    G4int vertexCollID;
    G4int Ce_emcalCollID;
    G4int fVerbose;

    G4GenericMessenger fMessenger;
};

#endif

    
