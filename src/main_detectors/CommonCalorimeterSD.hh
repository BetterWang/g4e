#ifndef JLeicCalorimeterSD_h
#define JLeicCalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"

#include "ReferenceDetectorConstruction.hh"
#include "RootFlatIO.hh"

#include "CommonCalorimeterHit.hh"
#include "ReferenceDetectorRunAction.hh"
#include "G4RunManager.hh"

#include "root_output/RootFlatIO.hh"



class CommonCalorimeterSD : public G4VSensitiveDetector
{
public:

    CommonCalorimeterSD(G4String, g4e::RootOutputManager*);

    ~CommonCalorimeterSD() override;

    void Initialize(G4HCofThisEvent *) override;

    G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;

    void EndOfEvent(G4HCofThisEvent *) override;

    void clear() override;

    void PrintAll() override;

private:

    CommonCalorimeterHitsCollection *CalCollection;
    uint_fast64_t mHitsCount = 0;
    g4e::RootOutputManager* mRootEventsOut = nullptr;

    static const uint_fast32_t mVerbose = 0;    // verbosity. 0=none, 1=some, 2=many, 3=all
};

#endif

