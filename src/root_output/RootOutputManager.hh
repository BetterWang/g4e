#ifndef G4E_ROOTOUTPUTMANAGER_HH
#define G4E_ROOTOUTPUTMANAGER_HH

#include <G4GenericMessenger.hh>
#include "RootFlatIO.hh"
#include "JLeicHistogramManager.hh"



/** This class holds the main root file and various helper functions for it */

class TFile;
class TTree;
class G4Step;


namespace g4e {

    enum class WriteStepPointChoices{
        PreStepPoint,
        PostStepPoint
    };

    class RootOutputManager {
    public:
        explicit RootOutputManager(TFile*);

        TFile *GetMainRootFile() { return mRootFile; }

        g4e::RootFlatIO* GetJLeicRootOutput() {return jleicRootOutput.get(); }
        JLeicHistogramManager* GetJLeicHistogramManager() { return jleicHistos.get(); }

        ///
        /// \param usePreStepPoint - true PreStepPoint is used for Volume and Coords, false - PostStepPoint
        void SaveStep(const G4Step *, WriteStepPointChoices usePoint, G4int copyIDx=0, G4int copyIDy=0);

        void Write() {
            jleicRootOutput->Write();
        }

    private:
        TFile *mRootFile;
        TTree *mFlatEventTree;
        // We also create JLeic root output here , while g4e is in transition
        // TODO Move JLeic initialization to the appropriate phase
        std::unique_ptr<g4e::RootFlatIO> jleicRootOutput;
        std::unique_ptr<JLeicHistogramManager> jleicHistos;
        G4GenericMessenger fMessenger;
        G4int mSaveSecondaryLevel;
    };
}


#endif //G4E_ROOTOUTPUTMANAGER_HH
