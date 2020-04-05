#ifndef G4E_VOLUMECHANGESTEPPINGACTION_HH
#define G4E_VOLUMECHANGESTEPPINGACTION_HH

#include <functional>

#include <G4UserSteppingAction.hh>
#include <root_output/RootOutputManager.hh>
#include <utility>

class G4Step;
class G4VPhysicalVolume;
namespace g4e {
    class VolumeChangeSteppingAction: public G4UserSteppingAction
    {
    public:
        explicit  VolumeChangeSteppingAction(g4e::RootOutputManager*);

        void OnEnterVolumeAction(G4VPhysicalVolume* vol, std::function<void(const G4Step *)> action) {mEnterActions[vol] = std::move(action);}
        void OnExitVolumeAction(G4VPhysicalVolume* vol, std::function<void(const G4Step *)> action) {mExitActions[vol] = std::move(action);}

        void OnEnterVolumeWriteHit(G4VPhysicalVolume* vol) { mEnterHitWriteVolumes.insert(vol); }
        void OnExitVolumeWriteHit(G4VPhysicalVolume* vol) { mExitHitWriteVolumes.insert(vol); }

        void UserSteppingAction(const G4Step*) override;

    private:
        g4e::RootOutputManager* mRootOutputManager;
        std::unordered_map<G4VPhysicalVolume*, std::function<void(const G4Step *)>> mEnterActions;
        std::unordered_map<G4VPhysicalVolume*, std::function<void(const G4Step *)>> mExitActions;
        std::unordered_set<G4VPhysicalVolume*> mEnterHitWriteVolumes;
        std::unordered_set<G4VPhysicalVolume*> mExitHitWriteVolumes;
    };
}



#endif //G4E_VOLUMECHANGESTEPPINGACTION_HH
