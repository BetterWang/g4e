#ifndef ModularDetector_HH
#define ModularDetector_HH

#include <InitializationContext.hh>
#include "globals.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4ios.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Polycone.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4Region.hh"
#include "G4UniformMagField.hh"

//-- Quadrupole --
#include "G4FieldManager.hh"
#include "G4ChordFinder.hh"
#include "G4QuadrupoleMagField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4ClassicalRK4.hh"
#include "G4FieldManager.hh"


#include "Materials.hh"
#include "JLeicDetectorMessenger.hh"
#include "JLeicDetectorConfig.hh"
#include "VolumeChangeSteppingAction.hh"


#include "ce_EMCAL/ce_EMCAL.hh"         // Central Electron Endcap - EMCAL

#include "ci_EMCAL/ci_EMCAL.hh"         // Central Ion Endcap - EMCAL



class JLeicDetectorConstruction : public G4VUserDetectorConstruction
{
public:

    //--------BEAM elements------
    bool USE_FFQs = true;

    // --------BARREL------
    //===========================================

    bool USE_BARREL = true;
    bool USE_BARREL_det = true;

    bool USE_BEAMPIPE = false;

    //------- subdetector-volumes  barrel ----- 

    bool USE_CB_VTX = true;
    bool USE_CB_VTX_LADDERS = true;
    bool USE_CB_VTX_ENDCAPS = false;  // for vxt endcaps ladders
    bool USE_CB_SiDISCS  = true;  // for vxt discs along beampipe
    //bool USE_VTX_E 1   // for vxt endcaps 

    bool USE_CB_CTD = true;
    // use one of the following options for CTD:
    bool USE_CB_CTD_Si = true;       // silicon version of CTD
    bool USE_CB_CTD_Straw = false;   // straw version of CTD

    bool USE_CB_DIRC = true;
    bool USE_CB_DIRC_bars = false;   // bars for DIRC

    bool USE_CB_EMCAL = true ;
    bool USE_CB_HCAL = true;
    bool USE_CB_HCAL_D = true; // hcal detector ( granularity)

    // Electron endcap
    // ==============================================
    bool USE_E_ENDCAP = true;

    //------- subdetector-volumes E-encap ----- 
    bool USE_CE_GEM = true;
    bool USE_CE_EMCAL = true;
    bool USE_CE_MRICH = true;

    // -------- polarimeter ------------
    bool USE_FFE_CPOL = false;
    bool USE_FFE_LUMI =false;
    bool USE_FFE_LOWQ2 =true;

    //==============================================
    //--------H-encap------
    //==============================================
    bool USE_CI_ENDCAP = true;

    //------- subdetector-volumes H-encap -----
    bool USE_CI_GEM = true;
    bool USE_CI_DRICH = true;
    bool USE_CI_TRD = true;
    bool USE_CI_EMCAL = true;
    bool USE_CI_HCAL = true;
    bool USE_CI_HCAL_D = true;

    //--------- Forward D1 ------
    bool USE_FI_B0_TRK = true;
    bool USE_FI_B0_EMCAL = false;
    //--------FARFORWARD HADRON------

    bool USE_FFI_OFFM_TRK = true;
    bool USE_FFI_NEG_TRK = true;
    // 
    bool USE_FFI_ZDC = true;
    bool USE_FFI_ZDC_ALICE = false;
    bool USE_FFI_ZDC_GLASS = false; 
    bool USE_FFI_ZDC_CRYSTAL = true; 

    //---
    bool USE_FFI_RPOT_D2 = true;
    bool USE_FFI_RPOT_D3 = false;
    //bool USE_FARFORWARD_GEM
    //bool USE_FARFORWARD_VP


    explicit JLeicDetectorConstruction(g4e::InitializationContext *);

    ~JLeicDetectorConstruction() override;

public:
    void SetAbsorberMaterial(G4String);

    void SetAbsorberThickness(G4double);

    void SetAbsorberZpos(G4double);

    void SetRadiatorMaterial(G4String);

    void SetWorldMaterial(G4String);

    void SetWorldSizeZ(G4double);

    void SetWorldSizeR(G4double);

    G4VPhysicalVolume *Construct() override;

    void ConstructSDandField() override ;

    void UpdateGeometry();

public:

    void PrintGeometryParameters();

    JLeicDetectorConfig &GetConfigRef() { return fConfig; }

    G4Material *GetWorldMaterial() { return World_Material; };

    void checkVolumeOverlap();

    const G4VPhysicalVolume *GetWorldPhysicalVolume() { return World_Phys; }

    const G4VPhysicalVolume *GetAbsorberPhysicalVolume() { return fPhysicsAbsorber; }


    // TODO: REMOVE from global JLeic DETECOTR CONSTRUCTION (Argh!!!)

    G4Material *GetAbsorberMaterial() { return fConfig.ci_TRD.det_Material; }

    //TRD- related ---- needs to be moved move

    G4LogicalVolume *GetLogicalRadiator() { return ci_TRD.fLogicRadiator; }

    G4Material *GetFoilMaterial() { return ci_TRD.fFoilMat; }

    G4Material *GetGasMaterial() { return ci_TRD.fGasMat; }

    G4double GetFoilThick() { return fConfig.ci_TRD.fRadThickness; }

    G4double GetGasThick() { return fConfig.ci_TRD.fGasGap; }

    G4int GetFoilNumber()
    {
        //std::cout << " foil number4 = " << ci_TRD.ConstructionConfig.fFoilNumber << std::endl;
        return ci_TRD.ConstructionConfig.fFoilNumber;
    }

    G4double fadc_slice;

    //---  fsv move to public ---
    G4Material *fAbsorberMaterial;

    G4int fModuleNumber;   // the number of Rad-et modules

    JLeicDetectorConfig fConfig;

    void SetUpJLEIC2019();

private:

    void Create_ci_Endcap(JLeicDetectorConfig::ci_Endcap_Config cfg);

    void Create_ce_Endcap(JLeicDetectorConfig::ce_Endcap_Config dfg);


private:

    // D e t e c t o r   D e s i g n s:
    // ========================================================================

    //  B E A M L I N E
    //==============================================
    ir_Beampipe_Design ir_Beampipe;

    //  C E N T R A L   (BARREL)
    //==============================================
    cb_Solenoid_Design cb_Solenoid;
    cb_VTX_Design     cb_VTX;         // Central Vertex
    cb_SiDISCS_Design cb_SiDISCS;     // Silicon Vertex
    cb_CTD_Design     cb_CTD;         // CTD
    cb_DIRC_Design    cb_DIRC;        // DIRC
    cb_EMCAL_Design   cb_EMCAL;       // EMCAL
    cb_HCAL_Design    cb_HCAL;        // HCAL

    //  E - E N D C A P
    //==============================================
    ce_GEM_Design   ce_GEM;    // GEM volume
    ce_MRICH_Design ce_MRICH;  // MRICH volume
    ce_EMCAL_Design ce_EMCAL;  // EMCAL volume

    // I O N  -  E N D C A P
    //==============================================
    ci_GEM_Design   ci_GEM;     //GEM
    ci_DRICH_Design ci_DRICH;   //DRICH
    ci_TRD_Design   ci_TRD;     //TRD
    ci_EMCAL_Design ci_EMCAL;   //EMCAL
    ci_HCAL_Design  ci_HCAL;    //HCAL

    // Far-Forward Electron
    //==============================================
    ffe_CPOL_Design ffe_CPOL;       // CPOL volume
    ffe_LUMI_Design ffe_LUMI;       // Luminosity monitor
    ffe_LOWQ2_Design ffe_LOWQ2;     // Low Q2 tagger

    // Far-Forward ION
    //==============================================

    fi_B0_TRK_Design     fi_B0_TRK;       // Tracking
    fi_B0_EMCAL_Design   fi_B0_EMCAL;     // EMCAL
    ffi_OFFM_TRK_Design   ffi_OFFM_TRK;      // after B1 (in D2) Off-momentum Tracking
    ffi_NEG_TRK_Design    ffi_NEG_TRK;      //  tracker for negarive particles lambda
    ffi_ZDC_Design      ffi_ZDC;

    // Roman Pot system
    ffi_RPOT_D2_Design  ffi_RPOT_D2;
    ffi_RPOT_D3_Design  ffi_RPOT_D3;

    //=========================================================================

    G4Box *World_Solid;                         // pointer to the solid World
    G4LogicalVolume *World_Logic;               // pointer to the logical World
    G4VPhysicalVolume *World_Phys;              // pointer to the physical World
    G4Material *World_Material;

    // Interaction region
   // ir_LatticeDesign ir_Lattice;
    AcceleratorMagnets *ion_line_magnets;

    // Hadron ENDCAP volume
    G4VisAttributes *ci_ENDCAP_GVol_VisAttr;
    G4Tubs *ci_ENDCAP_GVol_Solid;               // pointer to the solid  ENDCAP-H volume
    G4LogicalVolume *ci_ENDCAP_GVol_Logic;      // pointer to the logical  ENDCAP-H  volume
    G4VPhysicalVolume *ci_ENDCAP_GVol_Phys;     // pointer to the physical ENDCAP-H  volume

    // Electron ENDCAP  volume-
    G4VisAttributes *ce_ENDCAP_VisAttr;
    G4Tubs *ce_ENDCAP_GVol_Solid;               // pointer to the solid  ENDCAP-E volume
    G4LogicalVolume *ce_ENDCAP_GVol_Logic;      // pointer to the logical ENDCAP-E  volume
    G4VPhysicalVolume *ce_ENDCAP_GVol_Phys;     // pointer to the physical ENDCAP-E  volume
    //==============================================


    g4e::Materials *fMat;
    g4e::InitializationContext *fInitContext;
    JLeicDetectorMessenger *fDetectorMessenger;
    JLeicCalorimeterSD *fCalorimeterSD;
    JLeicVertexSD *fVertexSD;
    G4VPhysicalVolume *fPhysicsAbsorber;
    AcceleratorMagnets* fElectronLineMagnets;
    AcceleratorMagnets* fIonLineMagnets;


};

inline void JLeicDetectorConstruction::PrintGeometryParameters()
{
    G4cout << "\n The  WORLD   is made of " << fConfig.World.SizeZ / mm << "mm of " << World_Material->GetName();
    G4cout << ", the transverse size (R) of the world is " << fConfig.World.SizeR / mm << " mm. " << G4endl;
    G4cout << "WorldMaterial = " << World_Material->GetName() << G4endl;
    //  G4cout<<"fVTX_END_Z = "<<fVTX_END_Z/mm<<" mm"<<G4endl;
    G4cout << G4endl;
}

inline void JLeicDetectorConstruction::SetAbsorberMaterial(G4String materialChoice)
{
    // get the pointer to the material table
    const G4MaterialTable *theMaterialTable = G4Material::GetMaterialTable();

    // search the material by its name
    G4Material *pttoMaterial;

    for (size_t J = 0; J < theMaterialTable->size(); J++) {
        pttoMaterial = (*theMaterialTable)[J];

        if (pttoMaterial->GetName() == materialChoice) {
            fAbsorberMaterial = pttoMaterial;
            // fLogicAbsorber->SetMaterial(pttoMaterial);
            // PrintCalorParameters();
        }
    }
}

inline void JLeicDetectorConstruction::SetRadiatorMaterial(G4String materialChoice)
{
    // get the pointer to the material table

    const G4MaterialTable *theMaterialTable = G4Material::GetMaterialTable();

    // search the material by its name

    G4Material *pttoMaterial;
    for (size_t J = 0; J < theMaterialTable->size(); J++) {
        pttoMaterial = (*theMaterialTable)[J];

        if (pttoMaterial->GetName() == materialChoice) {
            fConfig.ci_TRD.fRadiatorMat = pttoMaterial;
            //fLogicRadSlice->SetMaterial(pttoMaterial);
            // PrintCalorParameters();
        }
    }
}

inline void JLeicDetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
    // get the pointer to the material table
    const G4MaterialTable *theMaterialTable = G4Material::GetMaterialTable();

    // search the material by its name
    for (auto material : *theMaterialTable) {
        if (material->GetName() == materialChoice) {
            World_Material = material;
            World_Logic->SetMaterial(material);
        }
    }
}

inline void JLeicDetectorConstruction::SetWorldSizeZ(G4double val)
{
    fConfig.World.SizeZ = val;
}

inline void JLeicDetectorConstruction::SetWorldSizeR(G4double val)
{
    fConfig.World.SizeR = val;
}

inline void JLeicDetectorConstruction::SetAbsorberZpos(G4double val)
{
    fConfig.ci_TRD.fAbsorberZ = val;
}

inline void JLeicDetectorConstruction::checkVolumeOverlap()
{
    // loop inside all the daughters volumes
    G4cout << " loop inside all the daughters volumes" << G4endl;
    //        bool bCheckOverlap;
    //        bCheckOverlap=false;

    int nSubWorlds, nSubWorlds2;
    for (int i = 0; i < (int) World_Phys->GetLogicalVolume()->GetNoDaughters(); i++) {
        World_Phys->GetLogicalVolume()->GetDaughter(i)->CheckOverlaps();
        nSubWorlds = (int) World_Phys->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetNoDaughters();
        for (int j = 0; j < nSubWorlds; j++) {
            World_Phys->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->CheckOverlaps();
            nSubWorlds2 = (int) World_Phys->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetNoDaughters();
            for (int k = 0; k < nSubWorlds2; k++) {
                World_Phys->GetLogicalVolume()->GetDaughter(i)->GetLogicalVolume()->GetDaughter(j)->GetLogicalVolume()->GetDaughter(k)->CheckOverlaps();
            }
        }
    }
    G4cout << G4endl;
}

#endif







