#ifndef ir_Beampipe_Config_HH
#define ir_Beampipe_Config_HH

#include <G4PVDivision.hh>
#include <G4RotationMatrix.hh>
#include <G4Material.hh>
#include <G4Color.hh>
#include <G4VisAttributes.hh>
#include <G4SystemOfUnits.hh>
#include <G4Tubs.hh>
#include <G4LogicalVolume.hh>
#include <G4PVPlacement.hh>
#include <G4Polycone.hh>
#include <G4NistManager.hh>

struct ir_Beampipe_Config
{
    // Ir - Interaction region pipe
    double IrElectronSizeZ = 470 * mm;
    double IrHadronSizeZ = 1000 * mm;
    double IrInnerR = 10 * mm;
    double IrOuterR = 11.5 * mm;
};

class ir_Beampipe_Design
{
public:



    void Construct(ir_Beampipe_Config cfg, G4VPhysicalVolume *motherVolume, G4String cadDir);



private:

    G4Tubs *IrElectronSolid;
    ir_Beampipe_Config ConstructionConfig;
    G4LogicalVolume *IrElectronLogic;
    G4PVPlacement *IrElectronPhysical;
    G4VisAttributes *IrElectronVisual;
    G4Tubs *IrHadronSolid;
    G4LogicalVolume *IrHadronLogic;
    G4PVPlacement *IrHadronPhysical;
    G4VisAttributes *IrHadronVisual;
};

#endif //G4E_IR_BEAMPIPE_HH
