//
// Created by romanov on 6/7/19.
//

#ifndef G4E_CB_SOLENOID_H
#define G4E_CB_SOLENOID_H

#include <G4Color.hh>
#include <G4VisAttributes.hh>
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

struct cb_Solenoid_Parameters
{
    G4bool fPipeField = true;                       // field in helium pipe used?
    double Solenoid_Field_Strength = -2.0 * tesla;
    double Solenoid_AlphaB = 0. * degree;
    double Solenoid_SizeZ = 400. * cm;
    double Solenoid_ROut = 144. * cm;
    double Solenoid_RIn = 0.0 * cm;
};

class cb_Solenoid_Design {
public:

    void Create(cb_Solenoid_Parameters &p, double shiftZ, G4Material *material, G4VPhysicalVolume *motherVolume)
    {
        printf("Solenoid_SizeZ=%f", p.Solenoid_SizeZ);

        Solenoid_Solid = new G4Tubs("Solenoid_Solid", p.Solenoid_RIn, p.Solenoid_ROut, p.Solenoid_SizeZ / 2., 0., 360 * deg);
        Solenoid_Logic = new G4LogicalVolume(Solenoid_Solid, material, "Solenoid_Logic");
        Solenoid_Phys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, shiftZ), "Solenoid_Phys", Solenoid_Logic, motherVolume, false, 0);

        if (p.fPipeField) {
            G4cout << "Set Magnetic field = " << p.Solenoid_Field_Strength << G4endl << G4endl;
            delete fMagField;   //delete the existing mag field

            auto direction = G4ThreeVector(p.Solenoid_Field_Strength * std::sin(p.Solenoid_AlphaB),
                                           0.,
                                           p.Solenoid_Field_Strength * std::cos(p.Solenoid_AlphaB));
            fMagField = new G4UniformMagField(direction);

            G4FieldManager *fieldMgr = new G4FieldManager(fMagField);
            fieldMgr->SetDetectorField(fMagField);
            fieldMgr->CreateChordFinder(fMagField);
            Solenoid_Logic->SetFieldManager(fieldMgr, true);
        }
        else {
            G4cout << "No Magnetic field " << G4endl << G4endl;
        }

        attr_Solenoid = new G4VisAttributes(G4Color(0.1, 0, 0.1, 0.4));
        attr_Solenoid->SetLineWidth(1);
        attr_Solenoid->SetForceSolid(false);
        Solenoid_Logic->SetVisAttributes(attr_Solenoid);
    }

    G4VisAttributes *attr_Solenoid;
    G4Tubs *Solenoid_Solid;      //pointer to the solid
    G4LogicalVolume *Solenoid_Logic;    //pointer to the logical
    G4VPhysicalVolume *Solenoid_Phys;  //pointer to the physical
    //----
    G4UniformMagField *fMagField;      //pointer to the magnetic field
private:


};


#endif //G4E_CB_SOLENOID_H
