//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//    JLeicMaterials.cc 66587 2019-02-21
//
/// \file electromagnetic/VertexEIC/src/JLeicMaterials.cc
/// \brief Implementation of the JLeicMaterials class


#include "JLeicMaterials.hh"

#include "G4UnitsTable.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4SystemOfUnits.hh"
#include <G4NistManager.hh>

JLeicMaterials::JLeicMaterials()
{
    Initialise();
}

JLeicMaterials::~JLeicMaterials() = default;

void JLeicMaterials::Initialise()
{
    G4String name, symbol;
    G4double a, z;
    G4double density, fractionmass;
    G4int nel, ncomponents;

    // define Elements

    a = 1.01 * g / mole;
    G4Element *elH = new G4Element(name = "Hydrogen", symbol = "H", z = 1., a);

    a = 6.94 * g / mole;
    G4Element *elLi = new G4Element(name = "Lithium", symbol = "Li", z = 3., a);

    a = 9.01 * g / mole;
    G4Element *elBe = new G4Element(name = "Berillium", symbol = "Be", z = 4., a);

    // a =19.32*g/mole;
    // G4Element* elAu  = new G4Element(name="Gold",symbol="Au" , z= 79., a);

    a = 12.01 * g / mole;
    G4Element *elC = new G4Element(name = "Carbon", symbol = "C", z = 6., a);

    a = 14.01 * g / mole;
    G4Element *elN = new G4Element(name = "Nitrogen", symbol = "N", z = 7., a);

    a = 16.00 * g / mole;
    G4Element *elO = new G4Element(name = "Oxygen", symbol = "O", z = 8., a);

    a = 39.948 * g / mole;
    G4Element *elAr = new G4Element(name = "Argon", symbol = "Ar", z = 18., a);



    /*
    a = 131.29*g/mole;
    G4Element* elXe = new G4Element(name="Xenon", symbol="Xe", z=54., a);

    a = 19.00*g/mole;
    G4Element* elF  = new G4Element(name="Fluorine", symbol="F", z=9., a);
    */

    //////////////
    //
    // Detector windows, electrodes
    // Al for electrodes

    density = 2.700 * g / cm3;
    a = 26.98 * g / mole;
    new G4Material(name = "Al", z = 13., a, density);


    /////////
    //
    // Materials for popular X-ray TR radiators
    //

    // TRT_CH2

    density = 0.935 * g / cm3;
    auto TRT_CH2 = new G4Material(name = "TRT_CH2", density, nel = 2);
    TRT_CH2->AddElement(elC, 1);
    TRT_CH2->AddElement(elH, 2);

    // Radiator

    density = 0.059 * g / cm3;
    auto Radiator = new G4Material(name = "Radiator", density, nel = 2);
    Radiator->AddElement(elC, 1);
    Radiator->AddElement(elH, 2);

    // Carbon Fiber

    density = 0.145 * g / cm3;
    auto CarbonFiber = new G4Material(name = "CarbonFiber", density, nel = 1);
    CarbonFiber->AddElement(elC, 1);

    // Lithium

    density = 0.534 * g / cm3;
    auto Li = new G4Material(name = "Li", density, nel = 1);
    Li->AddElement(elLi, 1);

    // Beryllium

    density = 1.848 * g / cm3;
    auto Be = new G4Material(name = "Be", density, nel = 1);
    Be->AddElement(elBe, 1);


    // Mylar

    density = 1.39 * g / cm3;
    auto Mylar = new G4Material(name = "Mylar", density, nel = 3);
    Mylar->AddElement(elO, 2);
    Mylar->AddElement(elC, 5);
    Mylar->AddElement(elH, 4);

    // Kapton Dupont de Nemur (density: 1.396-1.430, get middle )

    density = 1.413 * g / cm3;
    auto Kapton = new G4Material(name = "Kapton", density, nel = 4);
    Kapton->AddElement(elO, 5);
    Kapton->AddElement(elC, 22);
    Kapton->AddElement(elN, 2);
    Kapton->AddElement(elH, 10);

    // Kapton (polyimide) ??? since = Mylar C5H4O2

    // density = 1.39*g/cm3;
    // auto kapton = new G4Material(name="kapton", density, nel=3);
    // Kapton->AddElement(elO,2);
    // Kapton->AddElement(elC,5);
    // Kapton->AddElement(elH,4);

    // Polypropelene

    auto CH2 = new G4Material("CH2", 0.91 * g / cm3, 2);
    CH2->AddElement(elH, 2);
    CH2->AddElement(elC, 1);

    ////////////////////////////
    //
    // Noble gases , STP conditions

    // Helium as detector gas, STP

    density = 0.178 * mg / cm3;
    a = 4.0026 * g / mole;
    auto He = new G4Material(name = "He", z = 2., a, density);

    // Neon as detector gas, STP

    density = 0.900 * mg / cm3;
    a = 20.179 * g / mole;
    new G4Material(name = "Ne", z = 10., a, density);

    // Argon as detector gas, STP

    density = 1.7836 * mg / cm3;       // STP
    auto Argon = new G4Material(name = "Argon", density, ncomponents = 1);
    Argon->AddElement(elAr, 1);

    // Krypton as detector gas, STP

    density = 3.700 * mg / cm3;
    a = 83.80 * g / mole;
    auto Kr = new G4Material(name = "Kr", z = 36., a, density);

    // Xenon as detector gas, STP

    density = 5.858 * mg / cm3;
    a = 131.29 * g / mole;
    auto Xe = new G4Material(name = "Xenon", z = 54., a, density);

/////////////////////////////////
//
// Hydrocarbones, metane and others

    // Metane, STP

    density = 0.7174 * mg / cm3;
    auto metane = new G4Material(name = "CH4", density, nel = 2);
    metane->AddElement(elC, 1);
    metane->AddElement(elH, 4);

    // Propane, STP

    density = 2.005 * mg / cm3;
    auto propane = new G4Material(name = "C3H8", density, nel = 2);
    propane->AddElement(elC, 3);
    propane->AddElement(elH, 8);

    // iso-Butane (methylpropane), STP

    density = 2.67 * mg / cm3;
    auto isobutane = new G4Material(name = "isoC4H10", density, nel = 2);
    isobutane->AddElement(elC, 4);
    isobutane->AddElement(elH, 10);

    /////////////////////////
    //
    // Molecular gases

    // Carbon dioxide, STP

    density = 1.977 * mg / cm3;
    auto CO2 = new G4Material(name = "CO2", density, nel = 2, kStateGas, 273.15 * kelvin, 1. * atmosphere);
    CO2->AddElement(elC, 1);
    CO2->AddElement(elO, 2);

    // Carbon dioxide, STP

    density = 1.977 * 273. * mg / cm3 / 293.;
    auto CarbonDioxide = new G4Material(name = "CO2", density, nel = 2);
    CarbonDioxide->AddElement(elC, 1);
    CarbonDioxide->AddElement(elO, 2);

    // Nitrogen, STP

    density = 1.25053 * mg / cm3;       // STP
    auto Nitrogen = new G4Material(name = "N2", density, ncomponents = 1);
    Nitrogen->AddElement(elN, 2);

    // Oxygen, STP

    density = 1.4289 * mg / cm3;       // STP
    auto Oxygen = new G4Material(name = "O2", density, ncomponents = 1);
    Oxygen->AddElement(elO, 2);

    /* *****************************
    density = 1.25053*mg/cm3;       // STP
    a = 14.01*g/mole ;       // get atomic weight !!!
    //  a = 28.016*g/mole;
    auto N2  = new G4Material(name="Nitrogen", z= 7.,a,density) ;

    density = 1.25053*mg/cm3;       // STP
    auto anotherN2 = new G4Material(name="anotherN2", density,ncomponents=2);
    anotherN2->AddElement(elN, 1);
    anotherN2->AddElement(elN, 1);

    // air made from oxigen and nitrogen only

    density = 1.290*mg/cm3;  // old air from elements
    auto air = new G4Material(name="air"  , density, ncomponents=2);
    air->AddElement(elN, fractionmass=0.7);
    air->AddElement(elO, fractionmass=0.3);
    ******************************************** */

    // Dry Air (average composition with Ar), STP

    density = 1.2928 * mg / cm3;       // STP
    auto Air = new G4Material(name = "Air", density, ncomponents = 3);
    Air->AddMaterial(Nitrogen, fractionmass = 0.7557);
    Air->AddMaterial(Oxygen, fractionmass = 0.2315);
    Air->AddMaterial(Argon, fractionmass = 0.0128);

    ////////////////////////////////////////////////////////////////////////////
    //
    // MWPC mixtures


    // 80% Xe + 20% CO2, STP

    density = 5.0818 * mg / cm3;
    auto Xe20CO2 = new G4Material(name = "Xe20CO2", density, ncomponents = 2);
    Xe20CO2->AddMaterial(Xe, fractionmass = 0.922);
    Xe20CO2->AddMaterial(CarbonDioxide, fractionmass = 0.078);

    // 70% Xe + 27% CO2 + 3% O2, 20 1 atm ATLAS straw tube mixture

    density = 4.358 * mg / cm3;
    auto Xe27CO23O2 = new G4Material(name = "Xe27CO23O2", density, ncomponents = 3);
    Xe27CO23O2->AddMaterial(Xe, fractionmass = 0.87671);
    Xe27CO23O2->AddMaterial(CarbonDioxide, fractionmass = 0.11412);
    Xe27CO23O2->AddMaterial(Oxygen, fractionmass = 0.00917);

    // 80% Kr + 20% CO2, STP

    density = 3.601 * mg / cm3;
    auto Kr20CO2 = new G4Material(name = "Kr20CO2", density, ncomponents = 2);
    Kr20CO2->AddMaterial(Kr, fractionmass = 0.89);
    Kr20CO2->AddMaterial(CarbonDioxide, fractionmass = 0.11);

    // Xe + 55% He + 15% CH4 ; NIM A294 (1990) 465-472; STP

    density = 1.963 * 273. * mg / cm3 / 293.;
    auto Xe55He15CH4 = new G4Material(name = "Xe55He15CH4", density, ncomponents = 3);
    Xe55He15CH4->AddMaterial(Xe, 0.895);
    Xe55He15CH4->AddMaterial(He, 0.050);
    Xe55He15CH4->AddMaterial(metane, 0.055);

    // 90% Xe + 10% CH4, STP ; NIM A248 (1986) 379-388

    density = 5.344 * mg / cm3;
    auto Xe10CH4 = new G4Material(name = "Xe10CH4", density, ncomponents = 2);
    Xe10CH4->AddMaterial(Xe, fractionmass = 0.987);
    Xe10CH4->AddMaterial(metane, fractionmass = 0.013);

    // 95% Xe + 5% CH4, STP ; NIM A214 (1983) 261-268

    density = 5.601 * mg / cm3;
    auto Xe5CH4 = new G4Material(name = "Xe5CH4", density, ncomponents = 2);
    Xe5CH4->AddMaterial(Xe, fractionmass = 0.994);
    Xe5CH4->AddMaterial(metane, fractionmass = 0.006);

    // 80% Xe + 20% CH4, STP ; NIM A253 (1987) 235-244

    density = 4.83 * mg / cm3;
    auto Xe20CH4 = new G4Material(name = "Xe20CH4", density, ncomponents = 2);
    Xe20CH4->AddMaterial(Xe, fractionmass = 0.97);
    Xe20CH4->AddMaterial(metane, fractionmass = 0.03);

    // 93% Ar + 7% CH4, STP ; NIM 107 (1973) 413-422

    density = 1.709 * mg / cm3;
    auto Ar7CH4 = new G4Material(name = "Ar7CH4", density, ncomponents = 2);
    Ar7CH4->AddMaterial(Argon, fractionmass = 0.971);
    Ar7CH4->AddMaterial(metane, fractionmass = 0.029);

    // 93% Kr + 7% CH4, STP ; NIM 107 (1973) 413-422

    density = 3.491 * mg / cm3;
    auto Kr7CH4 = new G4Material(name = "Kr7CH4", density, ncomponents = 2);
    Kr7CH4->AddMaterial(Kr, fractionmass = 0.986);
    Kr7CH4->AddMaterial(metane, fractionmass = 0.014);

    // 0.5*(95% Xe + 5% CH4)+0.5*(93% Ar + 7% CH4), STP ; NIM A214 (1983) 261-268

    density = 3.655 * mg / cm3;
    auto XeArCH4 = new G4Material(name = "XeArCH4", density, ncomponents = 2);
    XeArCH4->AddMaterial(Xe5CH4, fractionmass = 0.766);
    XeArCH4->AddMaterial(Ar7CH4, fractionmass = 0.234);


    // 90% Ar + 10% CO2, STP

    density = 1.802 * mg / cm3;
    auto Ar10CO2 = new G4Material(name = "Ar10CO2", density, ncomponents = 2);
    Ar10CO2->AddMaterial(Argon, fractionmass = 0.891); // 1.78*0.9 / density
    Ar10CO2->AddMaterial(CarbonDioxide, fractionmass = 0.109); // 1 - frmAr


    // 85% Xe + 15% CO2, STP

    density = 4.9 * mg / cm3;
    auto Xe15CO2 = new G4Material(name = "Xe15CO2", density, ncomponents = 2);
    Xe15CO2->AddMaterial(Xe, fractionmass = 0.979);
    Xe15CO2->AddMaterial(CarbonDioxide, fractionmass = 0.021);

    // Silicon as detector material

    density = 2.330 * g / cm3;
    a = 28.09 * g / mole;
    new G4Material(name = "Si", z = 14., a, density);

    density = 19.32 * g / cm3;
    a = 196.97 * g / mole;
    new G4Material(name = "Au", z = 79., a, density);


    G4int natoms;
    // G4Element* elO  = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);
    a = 183.84 * g / mole;
    G4Element *elW = new G4Element(name = "Tungsten", symbol = "W", z = 74., a);

    a = 207.20 * g / mole;
    G4Element *elPb = new G4Element(name = "Lead", symbol = "Pb", z = 82., a);

    //------------------ PWO EMCAL crystals ------------------------------
    density = 8.280 * g / cm3;
    auto PbWO4 = new G4Material(name = "PbWO4", density, ncomponents = 3);
    PbWO4->AddElement(elO, natoms = 4);
    PbWO4->AddElement(elW, natoms = 1);
    PbWO4->AddElement(elPb, natoms = 1);

    //------------------ GLASS EMCAL  ------------------------------------
    // a = 16.00*g/mole;    G4Element* elO  = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);
    a = 137.33 * g / mole;
    G4Element *elBa = new G4Element(name = "Barium", symbol = "Ba", z = 56., a);
    a = 28.09 * g / mole;
    G4Element *elSi = new G4Element(name = "Silicone", symbol = "Si", z = 14., a);

    density = 3.8 * g / cm3;
    auto DSBCe = new G4Material(name = "DSBCe", density, ncomponents = 3);
    DSBCe->AddElement(elO, natoms = 5);
    DSBCe->AddElement(elBa, natoms = 1);
    DSBCe->AddElement(elSi, natoms = 2);

    //-------------- DIRC bars --------------------------------------------
    density = 2.200 * g / cm3;
    auto SiO2 = new G4Material("quartz", density, ncomponents = 2);
    SiO2->AddElement(elSi, natoms = 1);
    SiO2->AddElement(elO, natoms = 2);
//-----------------------------------------------------------------------

    density = 2.700 * g / cm3;
    a = 26.98 * g / mole;
    new G4Material(name = "Al", z = 13., a, density);

    density = 7.87 * g / cm3;
    a = 55.85 * g / mole;
    new G4Material(name = "Iron", z = 26., a, density);


    density = 70000.87 * g / cm3;
    a = 5005.85 * g / mole;
    new G4Material(name = "IronAll", z = 26., a, density);

    G4NistManager *man = G4NistManager::Instance();
    G4Element *C = man->FindOrBuildElement("C");
    G4Element *Si = man->FindOrBuildElement("Si");
    G4Element *Cr = man->FindOrBuildElement("Cr");
    G4Element *Mn = man->FindOrBuildElement("Mn");
    G4Element *Fe = man->FindOrBuildElement("Fe");
    G4Element *Ni = man->FindOrBuildElement("Ni");

    auto StainlessSteel = new G4Material("StainlessSteel", density = 8.06 * g / cm3, ncomponents = 6);
    StainlessSteel->AddElement(C,  fractionmass = 0.001);
    StainlessSteel->AddElement(Si, fractionmass = 0.007);
    StainlessSteel->AddElement(Cr, fractionmass = 0.18);
    StainlessSteel->AddElement(Mn, fractionmass = 0.01);
    StainlessSteel->AddElement(Fe, fractionmass = 0.712);
    StainlessSteel->AddElement(Ni, fractionmass = 0.09);


    //    density = 1e-25*cm3/g;
    //AddMaterial("G4_Galactic", density, 1, 21.8);
    auto empty = man->FindOrBuildMaterial("G4_Galactic");




}


G4Material* JLeicMaterials::GetMaterial(const G4String &name)
{
    return G4Material::GetMaterial(name);
}        

  






