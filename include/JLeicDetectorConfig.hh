//
// Created by yulia on 6/6/19.
//

#ifndef G4E_JLeicDetectorParameters_HH
#define G4E_JLeicDetectorParameters_HH

#include "cb_Solenoid/cb_Solenoid.hh"

#include "cb_VTX/cb_VTX.hh"
#include "cb_CTD/cb_CTD.hh"
#include "cb_DIRC/cb_DIRC.hh"
#include "cb_EMCAL/cb_EMCAL.hh"
#include "cb_HCAL/cb_HCAL.hh"

#include "ce_GEM/ce_GEM.hh"
#include "ce_MRICH/ce_MRICH.hh"
#include "ce_EMCAL/ce_EMCAL.hh"

#include "ffe_CPOL/ffe_CPOL.hh"

#include "ci_GEM/ci_GEM.hh"
#include "ci_DRICH/ci_DRICH.hh"
#include "ci_TRD/ci_TRD.hh"
#include "ci_EMCAL/ci_EMCAL.hh"
#include "ci_HCAL/ci_HCAL.hh"

#include "fi_TRKD1/fi_TRKD1.hh"
#include "fi_EMCAL/fi_EMCAL.hh"


#include "ffi_ZDC/ffi_ZDC.hh"         // Far-forward Ion   - HCAL ZDC


struct JLeicDetectorConfig {

    struct World_Config {
        double ShiftVTX = 40. * cm;
        double SizeR = 10000. * cm;
        double SizeZ = 20000. * cm;
    } World;

    struct ci_Endcap_Config {
        double RIn = 20. * cm;
        double ROut = NAN;
        double SizeZ = 250 * cm;
        double ShiftZ = 0. * cm;
        double PosX = 0. * cm;
        double PosZ = 0. * cm;
    } ci_Endcap;

    struct ce_Endcap_Config {
        double RIn = 20 * cm;
        double SizeZ = 60 * cm;
        double PosZ = NAN;
        double ROut = NAN;
    } ce_Endcap;


    //------------ BAREL-----
    cb_Solenoid_Config cb_Solenoid;

    cb_VTX_Config   cb_VTX;
    cb_CTD_Config   cb_CTD;
    cb_DIRC_Config  cb_DIRC;
    cb_EMCAL_Config cb_EMCAL;
    cb_HCAL_Config  cb_HCAL;
    //------------E-ENDCAP-----

    ce_GEM_Config  ce_GEM;
    ce_MRICH_Config  ce_MRICH;
    ce_EMCAL_Config  ce_EMCAL;

    //------------Far-Forward-Electron-----
    ffe_CPOL_Config ffe_CPOL;

    //------------H-ENDCAP-----
    ci_GEM_Config  ci_GEM;
    ci_DRICH_Config  ci_DRICH;
    ci_TRD_Config  ci_TRD;
    ci_EMCAL_Config  ci_EMCAL;
    ci_HCAL_Config  ci_HCAL;

    //------------Far-Forward-Ion-----
    fi_TRKD1_Config  fi_TRKD1;
    fi_EMCAL_Config  fi_EMCAL;

    //---------------HCAL -ZDC -------------------------
    ffi_ZDC_Config  ffi_ZDC;

} ;

#endif //G4E_JLeicDetectorParameters_HH