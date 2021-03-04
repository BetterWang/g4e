#ifndef ce_EMCAL_SD_HEADER
#define ce_EMCAL_SD_HEADER 1


#include "globals.hh"

#include "G4VSensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"

#include "root_output/RootOutputManager.hh"

#include "ce_EMCAL_Hit.hh"



class ce_EMCAL_SD : public G4VSensitiveDetector
{
public:

    ce_EMCAL_SD(G4String, g4e::RootOutputManager*);

    void Initialize(G4HCofThisEvent *) override;

    G4bool ProcessHits(G4Step *, G4TouchableHistory *) override;

    void EndOfEvent(G4HCofThisEvent *) override;

    void clear() override;

    void PrintAll() override;

    ce_EMCAL_Hit *FindExistingHit(string PID);

private:

    ce_EMCAL_HitsCollection *fHitsCollection;
    uint_fast64_t fHitsCount = 0;
    g4e::RootOutputManager* fRootManager = nullptr;
    static const uint_fast32_t fVerbose = 0;    // verbosity. 0=none, 1=some, 2=many, 3=all
};

#endif  // header lock guard
