target_include_directories(g4e
        PUBLIC
            ./
            subdetectors/all
            generator
            helpers
            root_output
        # TODO configure install and build interfaces right
        # $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # $<INSTALL_INTERFACE:include/mylib>  # <prefix>/include/mylib
        )

# This definition will turn fmt lib into header only version
target_compile_definitions(g4e PRIVATE FMT_HEADER_ONLY)

target_sources_local(g4e
        PRIVATE

        g4e.cc
        ArgumentProcessor.hh

        helpers/StringHelpers.cc
        helpers/StringHelpers.hh

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

        GeometryExport.hh
        GeometryExport.cc

        UserActionInitialization.hh
        UserActionInitialization.cc
        )

add_subdirectory(main_detectors)
add_subdirectory(generator)

