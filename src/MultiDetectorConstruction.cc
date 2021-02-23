//
// Created by romanov on 2/20/21.
//

#include "MultiDetectorConstruction.hh"

MultiDetectorConstruction::MultiDetectorConstruction():
        fMessenger(this, "/eic/", "EIC related settings")
{
    fMessenger.DeclareProperty("detector", fSelectedName, "Default DetectorConstruction used as a main detector");
}

G4VPhysicalVolume *MultiDetectorConstruction::Construct() {

    return fDetConstByName[fSelectedName]->Construct();
}

void MultiDetectorConstruction::ConstructSDandField() {
    fDetConstByName[fSelectedName]->ConstructSDandField();
}

void MultiDetectorConstruction::RegisterDetectorConstruction(std::string name, G4VUserDetectorConstruction *detectorConstruction) {
    fDetConstByName[name] = detectorConstruction;
}


