target_sources_local(g4e
        PRIVATE
        JLeicCalorimeterHit.cc
        JLeicCalorimeterHit.hh

#        BeamlineConstruction.cc
#        BeamlineConstruction.hh

            JLeicCalorimeterSD.cc
            JLeicCalorimeterSD.hh

        ../../subdetectors/ce_EMCAL/JLeicCe_emcalSD.cc
        ../../subdetectors/ce_EMCAL/JLeicCe_emcalSD.hh

        ../../subdetectors/ce_EMCAL/JLeicCe_emcalHit.cc
        ../../subdetectors/ce_EMCAL/JLeicCe_emcalHit.hh

        ../../subdetectors/ce_EMCAL/JLeicCe_emcalDigiHit.hh
        ../../subdetectors/ce_EMCAL/JLeicCe_emcalDigiHit.cc

            JLeicDetectorConfig.hh

            JLeicDetectorConstruction.cc
            JLeicDetectorConstruction.hh

        BeamlineConstructionMessenger.cc
        BeamlineConstructionMessenger.hh

        JLeicDetectorMessenger.cc
        JLeicDetectorMessenger.hh

            JLeicHistogramManager.hh
            JLeicHistogramManager.cc

            JLeicEventAction.hh
            JLeicEventAction.cc

            JLeicRunAction.cc
            JLeicRunAction.hh

            JLeicSolenoid3D.cc
            JLeicSolenoid3D.hh

            JLeicSteppingAction.cc
            JLeicSteppingAction.hh

            JLeicVertexSD.cc
            JLeicVertexSD.hh

            JLeicVTXHit.cc
            JLeicVTXHit.hh

        ../../subdetectors/ci_TRD/ci_TRD_TransparentRegRadModel.cc
        ../../subdetectors/ci_TRD/ci_TRD_TransparentRegRadModel.hh

            TRTDetectorConstruction.hh
            TRTMaterials.hh

            JLeicHistoMConfig.hh
            JLeicHistoMConfig.cc

            JLeicStepCut.cc
            JLeicStepCut.hh

            JLeicSteppingVerbose.cc
            JLeicSteppingVerbose.hh

            JLeicTrackingAction.cc
            JLeicTrackingAction.hh

            JLeicTrackInformation.hh
            JLeicTrackInformation.cc
        )

target_include_directories(g4e PUBLIC ./)