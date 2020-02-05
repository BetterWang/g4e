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
/// \file electromagnetic/VertexEIC/src/JLeicDetectorMessenger.cc
/// \brief Implementation of the JLeicDetectorMessenger class
//
//
// $Id: JLeicDetectorMessenger.cc 67268 2013-02-13 11:38:40Z ihrivnac $
//
// 

#include "JLeicDetectorMessenger.hh"


#include "JLeicDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//////////////////////////////////////////////////////////////////////////////

JLeicDetectorMessenger::JLeicDetectorMessenger(JLeicDetectorConstruction *JLeicDet) : G4UImessenger(), JLeicDetector(JLeicDet)
{
    DetDir = new G4UIdirectory("/detsetup/");
    DetDir->SetGuidance("Detector control.");

    EbeamECmd = new G4UIcmdWithAnInteger("/detsetup/eBeam", this);
    EbeamECmd->SetGuidance("Electron beam energy settings");
    EbeamECmd->SetParameterName("EbeamE", false, false);
    EbeamECmd->SetDefaultValue(10);
    EbeamECmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    PbeamECmd = new G4UIcmdWithAnInteger("/detsetup/pBeam", this);
    PbeamECmd->SetGuidance("Ion/proton beam energy settings");
    PbeamECmd->SetParameterName("PbeamE", false, false);
    PbeamECmd->SetDefaultValue(10);
    PbeamECmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    BeamlineNameCmd =  new G4UIcmdWithAString("/detsetup/beamlineName", this);
    BeamlineNameCmd->SetGuidance("Select beamline erhic or jlab");
    BeamlineNameCmd->SetParameterName("beamlineName", true);
    BeamlineNameCmd->SetDefaultValue("erhic");

    TRDdetDir = new G4UIdirectory("/XTRdetector/");
    TRDdetDir->SetGuidance("TRD detector control.");

    AbsMaterCmd = new G4UIcmdWithAString("/XTRdetector/setAbsMat", this);
    AbsMaterCmd->SetGuidance("Select Material of the Absorber.");
    AbsMaterCmd->SetParameterName("choice", true);
    AbsMaterCmd->SetDefaultValue("Xe");
    AbsMaterCmd->AvailableForStates(G4State_Idle);

    RadiatorMaterCmd = new G4UIcmdWithAString("/XTRdetector/setRadMat", this);
    RadiatorMaterCmd->SetGuidance("Select Material of the XTR radiator.");
    RadiatorMaterCmd->SetParameterName("choice", true);
    RadiatorMaterCmd->SetDefaultValue("CH2");
    RadiatorMaterCmd->AvailableForStates(G4State_Idle);

    ModelCmd = new G4UIcmdWithAnInteger("/XTRdetector/setModel", this);
    ModelCmd->SetGuidance("Select Model for XTR");
    ModelCmd->SetParameterName("choice", true);
    ModelCmd->SetDefaultValue(0);
    // ModelCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    ModelCmd->AvailableForStates(G4State_Idle);

    FoilNumCmd = new G4UIcmdWithAnInteger("/XTRdetector/setFoilNum", this);
    FoilNumCmd->SetGuidance("Select foil number for XTR");
    FoilNumCmd->SetParameterName("choice", true);
    FoilNumCmd->SetDefaultValue(0);
    FoilNumCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    WorldMaterCmd = new G4UIcmdWithAString("/XTRdetector/setWorldMat", this);
    WorldMaterCmd->SetGuidance("Select Material of the World.");
    WorldMaterCmd->SetParameterName("wchoice", true);
    WorldMaterCmd->SetDefaultValue("Air");
    WorldMaterCmd->AvailableForStates(G4State_Idle);

    AbsThickCmd = new G4UIcmdWithADoubleAndUnit("/XTRdetector/setAbsThick", this);
    AbsThickCmd->SetGuidance("Set Thickness of the Absorber");
    AbsThickCmd->SetParameterName("SizeZ", false, false);
    AbsThickCmd->SetDefaultUnit("mm");
    AbsThickCmd->SetRange("SizeZ>0.");
    AbsThickCmd->AvailableForStates(G4State_Idle);

    RadiatorThickCmd = new G4UIcmdWithADoubleAndUnit("/XTRdetector/setRadThick", this);
    RadiatorThickCmd->SetGuidance("Set Thickness of XTR radiator");
    RadiatorThickCmd->SetParameterName("SizeZ", false, false);
    RadiatorThickCmd->SetDefaultUnit("mm");
    RadiatorThickCmd->SetRange("SizeZ>0.");
    RadiatorThickCmd->AvailableForStates(G4State_Idle);

    GasGapThickCmd = new G4UIcmdWithADoubleAndUnit("/XTRdetector/setGasGapThick", this);
    GasGapThickCmd->SetGuidance("Set Thickness of XTR gas gaps");
    GasGapThickCmd->SetParameterName("SizeZ", false, false);
    GasGapThickCmd->SetDefaultUnit("mm");
    GasGapThickCmd->SetRange("SizeZ>0.");
    GasGapThickCmd->AvailableForStates(G4State_Idle);

    AbsRadCmd = new G4UIcmdWithADoubleAndUnit("/XTRdetector/setAbsRad", this);
    AbsRadCmd->SetGuidance("Set radius of the Absorber");
    AbsRadCmd->SetParameterName("SizeR", false, false);
    AbsRadCmd->SetDefaultUnit("mm");
    AbsRadCmd->SetRange("SizeR>0.");
    AbsRadCmd->AvailableForStates(G4State_Idle);

    AbsZposCmd = new G4UIcmdWithADoubleAndUnit("/XTRdetector/setAbsZpos", this);
    AbsZposCmd->SetGuidance("Set Z pos. of the Absorber");
    AbsZposCmd->SetParameterName("Zpos", false, false);
    AbsZposCmd->SetDefaultUnit("mm");
    AbsZposCmd->AvailableForStates(G4State_Idle);

    WorldZCmd = new G4UIcmdWithADoubleAndUnit("/XTRdetector/setWorldZ", this);
    WorldZCmd->SetGuidance("Set Z size of the World");
    WorldZCmd->SetParameterName("WSizeZ", false, false);
    WorldZCmd->SetDefaultUnit("mm");
    WorldZCmd->SetRange("WSizeZ>0.");
    WorldZCmd->AvailableForStates(G4State_Idle);

    WorldRCmd = new G4UIcmdWithADoubleAndUnit("/XTRdetector/setWorldR", this);
    WorldRCmd->SetGuidance("Set R size of the World");
    WorldRCmd->SetParameterName("WSizeR", false, false);
    WorldRCmd->SetDefaultUnit("mm");
    WorldRCmd->SetRange("WSizeR>0.");
    WorldRCmd->AvailableForStates(G4State_Idle);

    UpdateCmd = new G4UIcmdWithoutParameter("/XTRdetector/update", this);
    UpdateCmd->SetGuidance("Update calorimeter geometry.");
    UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
    UpdateCmd->SetGuidance("if you changed geometrical value(s).");
    UpdateCmd->AvailableForStates(G4State_Idle);

    MagFieldCmd = new G4UIcmdWithADoubleAndUnit("/XTRdetector/setField", this);
    MagFieldCmd->SetGuidance("Define magnetic field.");
    MagFieldCmd->SetGuidance("Magnetic field will be in Z direction.");
    MagFieldCmd->SetParameterName("Bz", false, false);
    MagFieldCmd->SetDefaultUnit("tesla");
    MagFieldCmd->AvailableForStates(G4State_Idle);
    /*
    ElectronCutCmd = new G4UIcmdWithADoubleAndUnit("/XTRdetector/setElectronCut",this);
    ElectronCutCmd->SetGuidance("Set electron cut in mm for vertex region");
    ElectronCutCmd->SetParameterName("ElectronCut",false,false);
    ElectronCutCmd->SetDefaultUnit("mm");
    ElectronCutCmd->SetRange("ElectronCut>0.");
    ElectronCutCmd->AvailableForStates(G4State_Idle);


    PositronCutCmd = new G4UIcmdWithADoubleAndUnit("/XTRdetector/setPositronCut",this);
    PositronCutCmd->SetGuidance("Set positron cut in mm for vertex region");
    PositronCutCmd->SetParameterName("PositronCut",false,false);
    PositronCutCmd->SetDefaultUnit("mm");
    PositronCutCmd->SetRange("PositronCut>0.");
    PositronCutCmd->AvailableForStates(G4State_Idle);


    GammaCutCmd = new G4UIcmdWithADoubleAndUnit("/XTRdetector/setGammaCut",this);
    GammaCutCmd->SetGuidance("Set gamma cut in mm for vertex region");
    GammaCutCmd->SetParameterName("GammaCut",false,false);
    GammaCutCmd->SetDefaultUnit("mm");
    GammaCutCmd->SetRange("GammaCut>0.");
    GammaCutCmd->AvailableForStates(G4State_Idle);
    */

}

///////////////////////////////////////////////////////////////////////////////

JLeicDetectorMessenger::~JLeicDetectorMessenger()
{
    delete AbsMaterCmd;
    delete ModelCmd;
    delete FoilNumCmd;
    delete AbsThickCmd;
    delete RadiatorThickCmd;
    delete GasGapThickCmd;
    delete AbsRadCmd;
    delete AbsZposCmd;
    delete WorldMaterCmd;
    delete RadiatorMaterCmd;
    delete WorldZCmd;
    delete WorldRCmd;
    delete UpdateCmd;
    delete MagFieldCmd;

    delete TRDdetDir;
    delete EbeamECmd;
    delete PbeamECmd;

    delete DetDir;
}

/////////////////////////////////////////////////////////////////////////////

void JLeicDetectorMessenger::SetNewValue(G4UIcommand *command, G4int newValue)
{
    if (command == ModelCmd && newValue == 0) {
        //  JLeicDetector->SetParametrisationModel(newValue);
        JLeicDetector->Construct();
    }
}
////////////////////////////////////////////////////////////////////////////
//
//

void JLeicDetectorMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == ModelCmd) {
        // JLeicDetector->SetParametrisationModel(ModelCmd->GetNewIntValue(newValue));
        // JLeicDetector->ParametrisationModel();
    }
    // if( command == FoilNumCmd )
//  {
//    JLeicDetector->SetFoilNumber(FoilNumCmd->GetNewIntValue(newValue));
//  }
    if (command == EbeamECmd) {
        JLeicDetector->GetConfigRef().ElectronBeamEnergy = G4UIcmdWithAnInteger::GetNewIntValue(newValue);
    }
    if (command == PbeamECmd) {
        JLeicDetector->GetConfigRef().IonBeamEnergy = G4UIcmdWithAnInteger::GetNewIntValue(newValue);
    }
    if (command == BeamlineNameCmd) {
        JLeicDetector->GetConfigRef().BeamlineName = newValue;
    }

    if (command == AbsMaterCmd) { JLeicDetector->SetAbsorberMaterial(newValue); }


    if (command == RadiatorMaterCmd) { JLeicDetector->SetRadiatorMaterial(newValue); }

    if (command == WorldMaterCmd) { JLeicDetector->SetWorldMaterial(newValue); }

    if (command == AbsThickCmd) {
        JLeicDetector->GetConfigRef().ci_TRD.fAbsorberThickness = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    }

    if (command == RadiatorThickCmd) {
        JLeicDetector->GetConfigRef().ci_TRD.fRadThickness =  G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    }

    if (command == GasGapThickCmd) {
        JLeicDetector->GetConfigRef().ci_TRD.fGasGap = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    }

    if (command == AbsRadCmd) {
        JLeicDetector->GetConfigRef().ci_TRD.fAbsorberRadius = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    }

    if (command == AbsZposCmd) { JLeicDetector->SetAbsorberZpos(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue)); }

    if (command == WorldZCmd) { JLeicDetector->SetWorldSizeZ(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue)); }

    if (command == WorldRCmd) { JLeicDetector->SetWorldSizeR(G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue)); }

    if (command == UpdateCmd) { JLeicDetector->UpdateGeometry(); }

    if (command == MagFieldCmd) {
        // TODO Remove it probably? Or make option for uniform magnetic field
        //JLeicDetector->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));
    }
    /*
    if( command == ElectronCutCmd )
    {
      JLeicDetector->SetElectronCut(WorldRCmd->GetNewDoubleValue(newValue));
    }
    if( command == PositronCutCmd )
    {
      JLeicDetector->SetPositronCut(WorldRCmd->GetNewDoubleValue(newValue));
    }
    if( command == GammaCutCmd )
    {
      JLeicDetector->SetGammaCut(WorldRCmd->GetNewDoubleValue(newValue));
    }
    */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
