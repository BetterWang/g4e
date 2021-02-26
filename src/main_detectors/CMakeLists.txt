target_sources_local(g4e
        PRIVATE
        CommonCalorimeterHit.cc
        CommonCalorimeterHit.hh

        BeamlineConstruction.cc
        BeamlineConstruction.hh

        BeamPipeConstruction.cc
        BeamPipeConstruction.hh

        CommonCalorimeterSD.cc
        CommonCalorimeterSD.hh


        DetectorConfig.hh

        ReferenceDetectorConstruction.cc
        ReferenceDetectorConstruction.hh

        BeamlineConstructionMessenger.cc
        BeamlineConstructionMessenger.hh

        ReferenceDetectorMessenger.cc
        ReferenceDetectorMessenger.hh


        ReferenceDetectorEventAction.hh
        ReferenceDetectorEventAction.cc

        ReferenceDetectorRunAction.cc
        ReferenceDetectorRunAction.hh

        ../subdetectors/cb_Solenoid/ReferenceDetectorSolenoid3D.cc
        ../subdetectors/cb_Solenoid/ReferenceDetectorSolenoid3D.hh

        ReferenceDetectorSteppingAction.cc
        ReferenceDetectorSteppingAction.hh

        CommonVertexSD.cc
        CommonVertexSD.hh

        CommonVertexHit.cc
        CommonVertexHit.hh

        ReferenceDetectorStepCut.cc
        ReferenceDetectorStepCut.hh

        ReferenceDetectorSteppingVerbose.cc
        ReferenceDetectorSteppingVerbose.hh

        ReferenceDetectorTrackingAction.cc
        ReferenceDetectorTrackingAction.hh

        CommonTrackInformation.hh
        CommonTrackInformation.cc
        )

target_include_directories(g4e PUBLIC jleic)