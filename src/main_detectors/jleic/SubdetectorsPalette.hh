#ifndef SubdetectorPalette_HH
#define SubdetectorPalette_HH

class SubdetectorPalette {
    // D e t e c t o r   D e s i g n s:
    // ========================================================================

    //  B E A M L I N E
    //==============================================
    ir_Beampipe_Design ir_Beampipe;

    //  C E N T R A L   (BARREL)
    //==============================================
    cb_Solenoid_Design cb_Solenoid;
    cb_VTX_Design      cb_VTX;         // Central Vertex
    cb_SiDISCS_Design  cb_SiDISCS;     // Silicon Vertex
    cb_CTD_Design      cb_CTD;         // CTD
    cb_DIRC_Design     cb_DIRC;        // DIRC
    cb_EMCAL_Design    cb_EMCAL;       // EMCAL
    cb_HCAL_Design     cb_HCAL;        // HCAL

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
    ffi_OFFM_TRK2_Design   ffi_OFFM_TRK2;      // virtual plane
    ffi_NEG_TRK_Design    ffi_NEG_TRK;      //  tracker for negarive particles lambda
    ffi_ZDC_Design      ffi_ZDC;

    // Roman Pot system
    ffi_RPOT_D2_Design  ffi_RPOT_D2;
    ffi_RPOT_D3_Design  ffi_RPOT_D3;
};
#endif //G4E_SUBDETECTORSPALETTE_HH
