#ifndef G4E_ROOTOUTPUTMANAGER_HH
#define G4E_ROOTOUTPUTMANAGER_HH

#include "RootFlatIO.hh"
#include "JLeicHistogramManager.hh"

/** This class holds the main root file and various helper functions for it */

class TFile;

namespace g4e {
    class RootOutputManager {
    public:
        explicit RootOutputManager(TFile*);

        TFile *GetMainRootFile() { return mRootFile; }

        g4e::RootFlatIO* GetJLeicRootOutput() {return jleicRootOutput.get(); }
        JLeicHistogramManager* GetJLeicHistogramManager() { return jleicHistos.get(); }

    private:
        TFile *mRootFile;
        // We also create JLeic root output here , while g4e is in transition
        // TODO Move JLeic initialization to the appropriate phase
        std::unique_ptr<g4e::RootFlatIO> jleicRootOutput;
        std::unique_ptr<JLeicHistogramManager> jleicHistos;
    };
}


#endif //G4E_ROOTOUTPUTMANAGER_HH
