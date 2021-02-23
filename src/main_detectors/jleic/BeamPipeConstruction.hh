//
// Created by romanov on 2/20/21.
//

#ifndef G4E_BEAMPIPECONSTRUCTION_H
#define G4E_BEAMPIPECONSTRUCTION_H

#include <G4VUserDetectorConstruction.hh>
#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <Materials.hh>
#include <subdetectors/ir_Beampipe/ir_Beampipe_Design.hh>
#include <subdetectors/ci_DRICH/ci_DRICH.hh>
#include "InitializationContext.hh"


class BeamPipeConstruction: public G4VUserDetectorConstruction {
public:
    BeamPipeConstruction(g4e::InitializationContext *initContext);
    G4VPhysicalVolume *Construct() override;
    G4VPhysicalVolume *Construct(G4VPhysicalVolume *worldPhysVol);
    void ConstructSDandField() override;

private:
    G4Box *fWorldSolidVol;                         // pointer to the solid World
    G4LogicalVolume *fWorldLogicVol;               // pointer to the logical World
    G4VPhysicalVolume *fWorldPhysicalVolume;              // pointer to the physical World
    G4Material *fWorldMaterial;
    g4e::Materials *fMaterials;
    g4e::InitializationContext *fInitContext;
    ir_Beampipe_Design ir_Beampipe;
    ir_Beampipe_Config ir_BeampipeConfig;

    ci_DRICH_Design m_ci_DRICH;
    ci_DRICH_Config m_ci_DRICH_Config;
};


#endif //G4E_BEAMPIPECONSTRUCTION_H
