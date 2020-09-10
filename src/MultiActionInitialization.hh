#ifndef G4E_INITIALIZATIONACTION_HH
#define G4E_INITIALIZATIONACTION_HH

#include <memory>
#include <functional>
#include <thread>
#include <mutex>

#include <G4VUserActionInitialization.hh>
#include <G4UserEventAction.hh>
#include <atomic>
#include <G4ExceptionSeverity.hh>
#include <G4Exception.hh>
#include <spdlog/spdlog.h>

class G4VPhysicalVolume;
class G4Step;


namespace g4e{

    class VolumeChangeSteppingAction;

    class MultiActionInitialization: public G4VUserActionInitialization
    {
    public:
        MultiActionInitialization() = default;


        void Build() const override ;
        // Virtual method to be implemented by the user to instantiate user action
        // class objects.

        /** Virtual method to be implemented by the user to instantiate user run action
        * class object to be used by G4MTRunManager. This method is not invoked in
        * the sequential mode. The user should not use this method to instantiate
        * user action classes rather than user run action.
        */
        void BuildForMaster() const override;

        void AddUserActionGenerator(const std::function<G4UserSteppingAction*()>& generator) {
            mSteppingActionGenerators.push_back(generator);
        }


        void AddUserActionGenerator(const std::function<G4UserEventAction*()>& generator) {
            mEventActionGenerators.push_back(generator);
        }

        void AddUserActionGenerator(const std::function<G4UserTrackingAction*()>& generator) {
            mTrackingActionGenerators.push_back(generator);
        }

        void AddUserActionGenerator(const std::function<G4UserRunAction*()>& generator) {
            mRunActionGenerators.push_back(generator);
        }

        void AddEnterVolumeSteppingAction(G4VPhysicalVolume* vol, std::function<void(const G4Step *)> action);

        void AddExitVolumeSteppingAction(G4VPhysicalVolume* vol, std::function<void(const G4Step *)> action);

        void OnEnterVolumeWriteHit(G4VPhysicalVolume* vol);
        void OnExitVolumeWriteHit(G4VPhysicalVolume* vol);


    private:
        void ThrowOnWrongInitializationOrder(const char* funcName) {
            /**
             * It is assumed that AddXXXVolumeSteppingAction and OnXXXVolumeWriteHit are called from ConstructSDandField method of DetectorConstruction
             * In this case initialization order is the next:
             *    MultiActionInitialization::Build           is called for each thread
             *    DetectorConstruction::Construct            from master thread
             *    DetectorConstruction::ConstructSDandField  from each worker thread
             *      |
             *      +--> AddXXXVolumeSteppingAction, OnXXXVolumeWriteHit ...
             */

            // The order is correct?
            if(mBuildIsDone) return;

            // The order is wrong!
            auto origin = fmt::format("MultiActionInitialization::{}(...)", funcName);
            auto description = fmt::format("Wrong inialization order! {} is called before UserInitializationAction::Build", funcName);

            G4Exception(origin.c_str(), "g4e", FatalException, description.c_str());
        }

        std::vector<std::function<G4UserSteppingAction*()>> mSteppingActionGenerators;
        std::vector<std::function<G4UserTrackingAction*()>> mTrackingActionGenerators;
        std::vector<std::function<G4UserEventAction*()>> mEventActionGenerators;
        std::vector<std::function<G4UserRunAction*()>> mRunActionGenerators;

        mutable std::unordered_map<std::thread::id, g4e::VolumeChangeSteppingAction*> mVolChangeActionsByThread;

        mutable std::mutex mBuildMutex;  // This lock is used for Build funcion for thread workers
        mutable std::atomic_bool mBuildIsDone{false};  // Flag that indicates, that Build function was called at least once

        G4VUserPrimaryGeneratorAction *mGeneratorAction;
    public:
        G4VUserPrimaryGeneratorAction *GetGeneratorAction() const;

        void SetGeneratorAction(G4VUserPrimaryGeneratorAction *mGeneratorAction);
    };
}

#endif //G4E_INITIALIZATIONACTION_HH
