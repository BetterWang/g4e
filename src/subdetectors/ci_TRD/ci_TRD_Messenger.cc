#include "ci_TRD_Messenger.hh"

#include "ci_TRD_Design.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//////////////////////////////////////////////////////////////////////////////

ci_TRD_Messenger::ci_TRD_Messenger(ci_TRD_Design *trdDesign) : G4UImessenger(),
                                                               fTrd(trdDesign)
{
    TRDdetDir = new G4UIdirectory("/XTRdetector/");
    TRDdetDir->SetGuidance("TRD detector control.");

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


    cutGCmd = new G4UIcmdWithADoubleAndUnit("/calor/cutG", this);
    cutGCmd->SetGuidance("Set cut values by RANGE for Gamma.");
    cutGCmd->SetParameterName("range", true);
    cutGCmd->SetDefaultValue(1.);
    cutGCmd->SetDefaultUnit("mm");
    cutGCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    cutECmd = new G4UIcmdWithADoubleAndUnit("/calor/cutE", this);
    cutECmd->SetGuidance("Set cut values by RANGE for e- e+.");
    cutECmd->SetParameterName("range", true);
    cutECmd->SetDefaultValue(1.);
    cutECmd->SetDefaultUnit("mm");
    cutECmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    setMaxStepCmd = new G4UIcmdWithADoubleAndUnit("/step/setMaxStep", this);
    setMaxStepCmd->SetGuidance("Set max. step length in the detector");
    setMaxStepCmd->SetParameterName("mxStep", true);
    setMaxStepCmd->SetDefaultUnit("mm");

    ElectronCutCmd = new G4UIcmdWithADoubleAndUnit("/emphyslist/setElectronCut", this);
    ElectronCutCmd->SetGuidance("Set electron cut in mm for vertex region");
    ElectronCutCmd->SetParameterName("ElectronCut", false, false);
    ElectronCutCmd->SetDefaultUnit("mm");
    ElectronCutCmd->SetRange("ElectronCut>0.");
    ElectronCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    PositronCutCmd = new G4UIcmdWithADoubleAndUnit("/emphyslist/setPositronCut", this);
    PositronCutCmd->SetGuidance("Set positron cut in mm for vertex region");
    PositronCutCmd->SetParameterName("PositronCut", false, false);
    PositronCutCmd->SetDefaultUnit("mm");
    PositronCutCmd->SetRange("PositronCut>0.");
    PositronCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    GammaCutCmd = new G4UIcmdWithADoubleAndUnit("/emphyslist/setGammaCut", this);
    GammaCutCmd->SetGuidance("Set gamma cut in mm for vertex region");
    GammaCutCmd->SetParameterName("GammaCut", false, false);
    GammaCutCmd->SetDefaultUnit("mm");
    GammaCutCmd->SetRange("GammaCut>0.");
    GammaCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    RadiatorCutCmd = new G4UIcmdWithADoubleAndUnit("/emphyslist/setRadiatorCuts", this);
    RadiatorCutCmd->SetGuidance("Set radiator cut in mm for vertex region");
    RadiatorCutCmd->SetParameterName("RadiatorCuts", false, false);
    RadiatorCutCmd->SetDefaultUnit("mm");
    RadiatorCutCmd->SetRange("RadiatorCuts > 0.");
    RadiatorCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    DetectorCutCmd = new G4UIcmdWithADoubleAndUnit("/emphyslist/setDetectorCuts", this);
    DetectorCutCmd->SetGuidance("Set radiator cut in mm for vertex region");
    DetectorCutCmd->SetParameterName("DetectorCuts", false, false);
    DetectorCutCmd->SetDefaultUnit("mm");
    DetectorCutCmd->SetRange("DetectorCuts > 0.");
    DetectorCutCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

    XTRModelCmd = new G4UIcmdWithAString("/emphyslist/setXTRModel", this);
    XTRModelCmd->SetGuidance("Set XTR model");
    XTRModelCmd->SetParameterName("XTRModel", false);
    XTRModelCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

///////////////////////////////////////////////////////////////////////////////

ci_TRD_Messenger::~ci_TRD_Messenger()
{
    delete TRDdetDir;

    delete cutGCmd;
    delete cutECmd;

    delete setMaxStepCmd;

    delete ElectronCutCmd;
    delete PositronCutCmd;
    delete GammaCutCmd;
    delete XTRModelCmd;

}


void ci_TRD_Messenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == ModelCmd) {
        // JLeicDetector->SetParametrisationModel(ModelCmd->GetNewIntValue(newValue));
        // JLeicDetector->ParametrisationModel();
    }
    // if( command == FoilNumCmd )
//  {
//    JLeicDetector->SetFoilNumber(FoilNumCmd->GetNewIntValue(newValue));
//  }




    if (command == RadiatorMaterCmd) {
        fTrd->SetRadiatorMaterial(newValue);
    }


    if (command == AbsThickCmd) {
        fTrd->ConstructionConfig.fAbsorberThickness = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    }

    if (command == RadiatorThickCmd) {
        fTrd->ConstructionConfig.fRadThickness =  G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    }

    if (command == GasGapThickCmd) {
        fTrd->ConstructionConfig.fGasGap = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    }

    if (command == AbsRadCmd) {
        fTrd->ConstructionConfig.fAbsorberRadius = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    }

    //printf("JLeicXTRphysicsMessenger::SetNewValue %p %p string= %s \n",command,GammaCutCmd,newValue.c_str());

    if (command == cutGCmd) { fTrd->PhysicsConstructor->SetGammaCut(cutGCmd->GetNewDoubleValue(newValue)); }
    if (command == cutECmd) { fTrd->PhysicsConstructor->SetElectronCut(eCmd->GetNewDoubleValue(newValue)); }
    if (command == setMaxStepCmd) { fTrd->PhysicsConstructor->SetMaxStep(setMaxStepCmd->GetNewDoubleValue(newValue)); }

    if (command == ElectronCutCmd) {
        fTrd->PhysicsConstructor->SetRegElectronCut(ElectronCutCmd->GetNewDoubleValue(newValue));
    }
    if (command == PositronCutCmd) {
        fTrd->PhysicsConstructor->SetRegPositronCut(PositronCutCmd->GetNewDoubleValue(newValue));
    }
    if (command == GammaCutCmd) {

        fTrd->PhysicsConstructor->SetRegGammaCut(GammaCutCmd->GetNewDoubleValue(newValue));
    }
    if (command == RadiatorCutCmd) {
        fTrd->PhysicsConstructor->SetRadiatorCuts();
    }
    if (command == DetectorCutCmd) {
        fTrd->PhysicsConstructor->SetDetectorCuts();
    }
    if (command == XTRModelCmd) {
        fTrd->PhysicsConstructor->SetXTRModel(newValue);
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
