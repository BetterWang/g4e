target_sources_local(g4e
        PRIVATE
        JLeicCalorimeterHit.cc
        JLeicCalorimeterHit.hh

        BeamlineConstruction.cc
        BeamlineConstruction.hh

        BeamPipeConstruction.cc
        BeamPipeConstruction.hh

            JLeicCalorimeterSD.cc
            JLeicCalorimeterSD.hh



            JLeicDetectorConfig.hh

            JLeicDetectorConstruction.cc
            JLeicDetectorConstruction.hh

            BeamlineConstructionMessenger.cc
            BeamlineConstructionMessenger.hh

            JLeicDetectorMessenger.cc
            JLeicDetectorMessenger.hh


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

            TRTDetectorConstruction.hh
            TRTMaterials.hh

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