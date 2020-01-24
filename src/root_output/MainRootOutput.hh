#ifndef G4E_MAINROOTOUTPUT_HH
#define G4E_MAINROOTOUTPUT_HH

#include "JLeicRootOutput.hh"
#include "JLeicHistogramManager.hh"

/** This class holds the main root file and various helper functions for it */

class TFile;

namespace g4e {
    class MainRootOutput {
    public:
        explicit MainRootOutput(TFile*);

        TFile *GetMainRootFile() { return mRootFile; }

        g4e::JLeicRootOutput* GetJLeicRootOutput() {return jleicRootOutput.get(); }
        JLeicHistogramManager* GetJLeicHistogramManager() { return jleicHistos.get(); }

    private:
        TFile *mRootFile;
        // We also create JLeic root output here , while g4e is in transition
        // TODO Move JLeic initialization to the appropriate phase
        std::unique_ptr<g4e::JLeicRootOutput> jleicRootOutput;
        std::unique_ptr<JLeicHistogramManager> jleicHistos;
    };
}


#endif //G4E_MAINROOTOUTPUT_HH
