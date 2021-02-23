/**
 * This class allows to manage multiple detector construction classes and select via messenger what to use
 *
 * Users use this class in runManager->SetUserInitialization
 * 1. Then register other G4VUserDetectorConstruction* classes by name via RegisterDetectorConstruction
 * 2. Then select one of the detectors by that name using SelectDetectorConstruction
 * 3. When Construct() or ConstructSDandField() are called they are forwarded to the selected DetectorConstruction
 *
 * It is assumed that a messenger can be used for SelectDetectorConstruction method.
 * And that messenger should select the detector construction before Construction
 * i.e. this class does nothing if the selected detector construction is changed during a run
 */
#ifndef MultiDetectorConstruction_HEADER
#define MultiDetectorConstruction_HEADER

#include <G4VUserDetectorConstruction.hh>
#include <G4GenericMessenger.hh>

class MultiDetectorConstruction: public G4VUserDetectorConstruction {

public:

    MultiDetectorConstruction();

    G4VPhysicalVolume* Construct() override;

    void ConstructSDandField() override;

    /// Registers user detector construction by name
    void RegisterDetectorConstruction(std::string, G4VUserDetectorConstruction*);

    /// Selects detector  construction by name
    void SelectDetectorConstruction(std::string name) {
        fSelectedName = name;
    }

private:
    std::map<std::string, G4VUserDetectorConstruction*> fDetConstByName;
    std::string fSelectedName;
    G4GenericMessenger fMessenger;
};


#endif //MultiDetectorConstruction_HEADER
