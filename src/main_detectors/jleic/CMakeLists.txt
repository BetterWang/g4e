target_sources_local(g4e
        PRIVATE
        JLeicCalorimeterHit.cc
        JLeicCalorimeterHit.hh

            JLeicCalorimeterSD.cc
            JLeicCalorimeterSD.hh
            
            JLeicCe_emcalSD.cc
            JLeicCe_emcalSD.hh
            
            JLeicCe_emcalHit.cc
            JLeicCe_emcalHit.hh
            
            JLeicCe_emcalDigiHit.hh
            JLeicCe_emcalDigiHit.cc

            JLeicDetectorConfig.hh

            JLeicDetectorConstruction.cc
            JLeicDetectorConstruction.hh

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

            JLeicTrackInformation.hh
            JLeicTrackInformation.cc
        )

target_include_directories(g4e PUBLIC ./)