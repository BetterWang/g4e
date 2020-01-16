target_include_directories(g4e
        PUBLIC
            ./
            subdetectors/all
            generator
            helpers
            root_output

        SYSTEM
            external/spdlog/include     # Header only library for str formatting and fast logging

            external/CLI11           # Header only library for argument parsing

        # TODO configure install and build interfaces right
        # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # $<INSTALL_INTERFACE:include/mylib>  # <prefix>/include/mylib
        )

# This definition will turn fmt lib into header only version
target_compile_definitions(g4e PRIVATE FMT_HEADER_ONLY)

target_sources_local(g4e
        PRIVATE

        g4e.cc
        InputProcessor.hh

        helpers/StringHelpers.cc
        helpers/StringHelpers.hh

        InitializationContextInterface.hh
        LoggingMessenger.hh
        LogLevels.hh
        Logging.hh


        G4QuadrupoleMagField.cc
        G4QuadrupoleMagField.hh

        MulticastEventAction.hh
        MulticastSteppingAction.hh

        Materials.cc
        Materials.hh

        JLeicPhysicsList.cc
        JLeicPhysicsList.hh

        JLeicPhysicsListMessenger.cc
        JLeicPhysicsListMessenger.hh

        JLeicHistoMConfig.cc
        JLeicHistoMConfig.hh

        JLeicStepCut.cc
        JLeicStepCut.hh

        JLeicSteppingVerbose.cc
        JLeicSteppingVerbose.hh

        JLeicTrackingAction.cc
        JLeicTrackingAction.hh
        )

add_subdirectory(main_detectors)
add_subdirectory(generator)

