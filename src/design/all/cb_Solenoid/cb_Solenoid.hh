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
#include "G4UImessenger.hh"
#include "G4SystemOfUnits.hh"

struct cb_Solenoid_Config
{
    bool UseMagneticField = true;                       // field in helium pipe used?
    double FieldStrength = -2.0 * tesla;
    double AlphaB = 0. * degree;
    double SizeZ = 400. * cm;
    double ROut = 144. * cm;
    double RIn = 0.0 * cm;
    double ShiftZ = 0.;
};

class cb_Solenoid_Messenger: G4UImessenger {
public:
    explicit cb_Solenoid_Messenger(cb_Solenoid_Config * parameters, G4UIdirectory *parentDirectory) {

    }
};


class cb_Solenoid_Design {
public:


    explicit cb_Solenoid_Design()
    {
    }


    void Construct(cb_Solenoid_Config p, G4Material *material, G4VPhysicalVolume *motherVolume)
    {
        ConstructionConfig = p;

        printf("SizeZ=%f", p.SizeZ);

        // Global placement
        Solid = new G4Tubs("Solid", p.RIn, p.ROut, p.SizeZ / 2., 0., 360 * deg);
        Logic = new G4LogicalVolume(Solid, material, "Logic");
        Phys = new G4PVPlacement(nullptr, G4ThreeVector(0, 0, p.ShiftZ), "Phys", Logic, motherVolume, false, 0);

        // Visual attributes
        VisAttributes = new G4VisAttributes(G4Color(0.1, 0, 0.1, 0.4));
        VisAttributes->SetLineWidth(1);
    //    VisAttributes->SetForceSolid(false);
        VisAttributes->SetForceSolid(true);
        Logic->SetVisAttributes(VisAttributes);

        if (p.UseMagneticField) {
            G4cout << "Set Magnetic field = " << p.FieldStrength << G4endl << G4endl;
            CreateMagneticField(p);
        }
        else {
            G4cout << "No Magnetic field " << G4endl << G4endl;
        }
    }

    void CreateMagneticField(const cb_Solenoid_Config &p)
    {
        delete MagneticField;   //delete the existing mag field

        auto direction = G4ThreeVector(p.FieldStrength * std::sin(p.AlphaB),
                                       0.,
                                       p.FieldStrength * std::cos(p.AlphaB));
        MagneticField = new G4UniformMagField(direction);

        auto *fieldMgr = new G4FieldManager(MagneticField);
        fieldMgr->SetDetectorField(MagneticField);
        fieldMgr->CreateChordFinder(MagneticField);
        Logic->SetFieldManager(fieldMgr, true);

    }

    G4VisAttributes *VisAttributes;
    G4Tubs *Solid;      //pointer to the solid
    G4LogicalVolume *Logic;    //pointer to the logical
    G4VPhysicalVolume *Phys;  //pointer to the physical
    G4UniformMagField *MagneticField;      //pointer to the magnetic field

    /// Parameters that was used in the moment of construction
    cb_Solenoid_Config ConstructionConfig;
private:


};


#endif //G4E_CB_SOLENOID_H