target_sources_local(g4e
        PRIVATE

            JLeicActionInitialization.hh
            JLeicActionInitialization.cc

            JLeicCalorHit.cc
            JLeicCalorHit.hh

            JLeicCalorimeterSD.cc
            JLeicCalorimeterSD.hh

            JLeicDetectorConfig.hh

            JLeicDetectorConstruction.cc
            JLeicDetectorConstruction.hh

            JLeicDetectorMessenger.cc
            JLeicDetectorMessenger.hh

        JLeicHistogramManager.hh
        JLeicHistogramManager.cc

            JLeicEventAction.hh
            JLeicEventAction.cc

            JLeicEventActionMessenger.cc
            JLeicEventActionMessenger.hh

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

            JLeicXTRphysics.cc
            JLeicXTRphysics.hh

            JLeicXTRphysicsMessenger.cc
            JLeicXTRphysicsMessenger.hh

            JLeicXTRTransparentRegRadModel.cc
            JLeicXTRTransparentRegRadModel.hh

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
        )

target_include_directories(g4e PUBLIC ./)