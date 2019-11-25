target_include_directories(g4e
        PUBLIC
            ./
            design_subdetectors/all
            generator
            helpers
            root_output

        SYSTEM
            external/spdlog/include     # Header only library for str formatting and fast logging
            external/argparse           # Header only library for argument parsing

        # TODO configure install and build interfaces right
        # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # $<INSTALL_INTERFACE:include/mylib>  # <prefix>/include/mylib
        )

target_sources_local(g4e
        PRIVATE

        g4e.cc
        InputProcessor.hh

        helpers/StringHelpers.cc
        helpers/StringHelpers.hh


        G4QuadrupoleMagField.cc
        G4QuadrupoleMagField.hh

        MulticastEventAction.hh
        MulticastSteppingAction.hh

        JLeicMaterials.cc
        JLeicMaterials.hh

        JLeicPhysicsList.cc
        JLeicPhysicsList.hh

        JLeicPhysicsListMessenger.cc
        JLeicPhysicsListMessenger.hh

        JLeicRunAction.cc

        JLeicRunAction.hh

        JLeicHistoMConfig.cc
        JLeicHistoMConfig.hh

        JLeicStepCut.cc
        JLeicStepCut.hh



        JLeicSteppingVerbose.cc
        JLeicSteppingVerbose.hh

        JLeicTrackingAction.cc
        JLeicTrackingAction.hh

        StepMax.cc
        StepMax.hh
        )

add_subdirectory(design_main)
add_subdirectory(generator)

