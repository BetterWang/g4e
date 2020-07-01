#include <vector>

#include "JLeicDetectorConstruction.hh"
#include "JLeicCalorimeterSD.hh"
#include "JLeicVertexSD.hh"

#include "G4SDManager.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"
#include "G4RegionStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4ProductionCuts.hh"
#include "G4VisAttributes.hh" 
#include "G4Colour.hh"
#include "G4ios.hh" 
#include "G4SystemOfUnits.hh"

// export geometry through VGM
#include "GeometryExport.hh"
#include "VolumeChangeSteppingAction.hh"

JLeicDetectorConstruction::JLeicDetectorConstruction(g4e::InitializationContext *initContext) : 
    fInitContext(initContext)
{
    fDetectorMessenger = new JLeicDetectorMessenger(this);
    fMat = new g4e::Materials();

    initContext->ActionInitialization->AddUserActionGenerator([initContext](){
        auto action = new g4e::VolumeChangeSteppingAction(initContext->RootManager);
        return static_cast<G4UserSteppingAction*>(action);
    });
}


JLeicDetectorConstruction::~JLeicDetectorConstruction()
{
    delete fDetectorMessenger;
    delete fMat;
}


G4VPhysicalVolume *JLeicDetectorConstruction::Construct()
{
    using namespace fmt;

    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    //===================================================================================
    //==                    create a world                                            ==
    //===================================================================================

    // World_Material    = Air;
    World_Material = fMat->GetMaterial("G4_Galactic");
    World_Solid = new G4Box("World_Solid", fConfig.World.SizeR, fConfig.World.SizeR, fConfig.World.SizeZ / 2.);
    World_Logic = new G4LogicalVolume(World_Solid, World_Material, "World_Logic");
    World_Phys = new G4PVPlacement(nullptr, G4ThreeVector(), "World_Phys", World_Logic, nullptr, false, 0);

    fmt::print("Initializing WORLD. x:{:<10}m y:{:<10}m z:{:<10}m", fConfig.World.SizeR * 2 / m, fConfig.World.SizeR * 2 / m, fConfig.World.SizeZ / m);

    // beam line flag, that is going to be used in future
    auto beamLine = fConfig.BeamlineName == "erhic" ? BeamLines::ERHIC : BeamLines::JLEIC;

    //=================================================================================
    //                         CE_EMCAL
    //=================================================================================
    if (USE_CE_EMCAL) {
        fConfig.ce_EMCAL.PosZ = -fConfig.ce_Endcap.SizeZ / 2 + fConfig.ce_EMCAL.Thickness / 2.;
        fConfig.ce_EMCAL.ROut = fConfig.ce_Endcap.ROut -3*cm;

        ce_EMCAL.Construct(fConfig.ce_EMCAL, World_Material, ce_ENDCAP_GVol_Phys);
        ce_EMCAL.ConstructCrystals(); // --- inner detector with Crystals
        ce_EMCAL.ce_EMCAL_detPWO_Logic->SetSensitiveDetector(fCalorimeterSD);
        ce_EMCAL.ConstructGlass();    // --- outer part with Glass
        ce_EMCAL.ce_EMCAL_detGLASS_Logic->SetSensitiveDetector(fCalorimeterSD);
    }

    //================================================================================
    // ==                      CI_EMCAL    Hadron endcap
    //================================================================================
    if (USE_CI_EMCAL) {
        fConfig.ci_EMCAL.PosZ = -fConfig.ci_Endcap.SizeZ / 2 + fConfig.ci_DRICH.ThicknessZ + fConfig.ci_TRD.ThicknessZ + fConfig.ci_EMCAL.ThicknessZ / 2;
        if(beamLine == BeamLines::JLEIC)  {
            fConfig.ci_EMCAL.USE_JLEIC = true;
            fConfig.ci_EMCAL.det_Rin1 = 20*cm;
            fConfig.ci_EMCAL.det_Rin2 = 55*cm;
        } else {
            fConfig.ci_EMCAL.USE_ERHIC=true;
            fConfig.ci_EMCAL.det_Rin1=30*cm;
            fConfig.ci_EMCAL.det_Rin2=30*cm;
        }

        ci_EMCAL.Construct(fConfig.ci_EMCAL, World_Material, ci_ENDCAP_GVol_Phys);
        ci_EMCAL.ConstructDetectors();    // --- outer part with Glass
        ci_EMCAL.ci_EMCAL_det_Logic->SetSensitiveDetector(fCalorimeterSD);
    } // end USE_CI_EMCAL


   //===================================================================================
   //                     END detector construction.... Exporting geometry
   //===================================================================================

    spdlog::info(" - exporting geometry");
    g4e::GeometryExport::Export(fInitContext->Arguments->OutputBaseName, World_Phys);
    PrintGeometryParameters();
}


void JLeicDetectorConstruction::ConstructSDandField()
{
    // fVolChangeAction.OnEnterVolumeWriteHit(ffi_ZDC.Phys);

    //fVolChangeAction =

//G4PhysicalVolumeStore::GetInstance()->

    if( USE_FFI_ZDC){ fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_ZDC.Phys);}
    //  fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_RPOT_D2.lay_Phys[0]);
    if( USE_FFI_RPOT_D3)  { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_RPOT_D2.Phys); }
    if( USE_FFI_RPOT_D3)  { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_RPOT_D3.Phys); }
    if( USE_FFI_OFFM_TRK) { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_OFFM_TRK.Phys); }
    if( USE_FFI_NEG_TRK)  { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->ffi_NEG_TRK.Phys); }
    if( USE_FI_B0_TRK)    { fInitContext->ActionInitialization->OnEnterVolumeWriteHit(this->fi_B0_TRK.Phys); }
}


void JLeicDetectorConstruction::UpdateGeometry()
{
    G4RunManager::GetRunManager()->DefineWorldVolume(World_Phys);
}

