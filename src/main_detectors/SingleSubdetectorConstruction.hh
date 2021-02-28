/**
 * This class aims to construct and show single sub-detectors
 * For debugging and many other development purposes it is convenient to be able
 * to work with a singe subdetector alone.
 * Users can choose :
 *    /eic/detector subdetector
 *    /eic/subdetector ci_DIRC
 * To render ci_DIRC alone
 *
 * Currently this class has kind of naive approach and could be redone in future
 */
#ifndef SingleSubdetectorConstruction_HH
#define SingleSubdetectorConstruction_HH

#include <G4VUserDetectorConstruction.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4GenericMessenger.hh>

#include <Materials.hh>
#include <subdetectors/ir_Beampipe/ir_Beampipe_Design.hh>
#include <subdetectors/ci_DRICH/ci_DRICH_Config.hh>
#include <subdetectors/ci_DRICH/ci_DRICH_Design.hh>
#include "InitializationContext.hh"



class SingleSubdetectorConstruction: public G4VUserDetectorConstruction {
public:
    SingleSubdetectorConstruction(g4e::InitializationContext *initContext, DetectorConfig &config);
    G4VPhysicalVolume *Construct() override;
    G4VPhysicalVolume *Construct_Beampipe();
    G4VPhysicalVolume *Construct_Beamline();
    G4VPhysicalVolume *Construct_ci_DRICH();

    void ConstructSDandField() override;

private:
    G4Box *fWorldSolidVol;                         // pointer to the solid World
    G4LogicalVolume *fWorldLogicVol;               // pointer to the logical World
    G4VPhysicalVolume *fWorldPhysicalVolume;       // pointer to the physical World
    G4Material *fWorldMaterial;                    // pointer to world material
    g4e::Materials *fMaterials;                    // pointer to g4e materials collection
    g4e::InitializationContext *fInitContext;      // G4E Initialization context


    ir_Beampipe_Design ir_Beampipe;
    ci_DRICH_Design ci_DRICH;


    G4GenericMessenger fMessenger;
    G4String fSubdetectorName;
    DetectorConfig &fConfig;
};


#endif //SingleSubdetectorConstruction_HH
