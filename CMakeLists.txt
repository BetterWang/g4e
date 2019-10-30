#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(g4e)

include(cmake/print_functions.cmake)                 # Helpers to print fancy headers, file names, etc
include(cmake/target_sources_local.cmake)            # Add relative source path independently of cmake version or policy
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH} )



# -----------------------------------------------------------------
print_grand_header("    F I N D   B A S I C   P A C K A G E S    ")
# -----------------------------------------------------------------

# Cern R O O T
# ------------
find_package(ROOT REQUIRED COMPONENTS Geom Core RIO Hist Tree)
if(${ROOT_VERSION} MATCHES "6.*")
    message(STATUS "Found Cern.ROOT version: ${ROOT_VERSION}")
else()
    message(FATAL_ERROR "GenFit and other components require CERN.ROOT version > 6.00.00")
endif()


# G E A N T 4
# -----------
# Can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)

if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
message(STATUS "g4e: Geant4_DIR: ${Geant4_DIR}")

# Setup Geant4 include directories and compile definitions
include(${Geant4_USE_FILE})

# >oO Debug:
message(DEBUG "g4e: Geant4_LIBS  :${Geant4_LIBRARIES}" )


# V  G  M
# --------
# TODO move to FindVGM.cmake

print_header(">>>>    V G M     <<<<" )

if(DEFINED VGM_DIRECTORY)
    message(STATUS "g4e: cmake VGM_DIRECTORY provided: ${VGM_DIRECTORY}")
elseif(DEFINED ENV{VGM_DIRECTORY})
    set(VGM_DIRECTORY $ENV{VGM_DIRECTORY})
    message(STATUS "g4e: VGM_DIRECTORY set from environemnt ${VGM_DIRECTORY}")
elseif(DEFINED ENV{VGM_DIR})
    set(VGM_DIRECTORY $ENV{VGM_DIR})
    message(STATUS "g4e: VGM_DIRECTORY set from environemnt ${VGM_DIRECTORY}")
else()
    message(STATUS "g4e: No cmake VGM_DIRECTORY nor env:VGM_DIRECTORY provided")
    message(STATUS "g4e: Build will try to locate VGMConfig.cmake or FindVGM.cmake")
endif()

find_library(VGM_Geant4GM NAMES Geant4GM HINTS ${VGM_DIRECTORY} PATH_SUFFIXES lib lib64)
find_library(VGM_BaseVGM  NAMES BaseVGM  HINTS ${VGM_DIRECTORY} PATH_SUFFIXES lib lib64)
find_library(VGM_RootGM   NAMES RootGM   HINTS ${VGM_DIRECTORY} PATH_SUFFIXES lib lib64)
find_library(VGM_XmlVGM   NAMES XmlVGM   HINTS ${VGM_DIRECTORY} PATH_SUFFIXES lib lib64)

message(STATUS "g4e: VGM_Geant4GM :${VGM_Geant4GM} ")
message(STATUS "g4e: VGM_BaseVGM  :${VGM_BaseVGM}  ")
message(STATUS "g4e: VGM_RootGM   :${VGM_RootGM}   ")
message(STATUS "g4e: VGM_XmlVGM   :${VGM_XmlVGM}   ")

# H E P  M C
# ----------

find_package(HepMC REQUIRED)    # easy one =)

message(STATUS "g4e: HEPMC_INCLUDE_DIR :${HEPMC_INCLUDE_DIR}")
message(STATUS "g4e: HEPMC_INCLUDE_DIR :${HEPMC_INCLUDE_DIR}")
message(STATUS "g4e: HEPMC_LIBRARY_DIR  :${HEPMC_LIBRARY_DIR}")
message(STATUS "g4e: HEPMC_LIBRARIES  :${HEPMC_LIBRARIES}")


# ---------------------------------------------------------------------
print_grand_header("    G E N E R A L   C O N F I G U R A T I O N    ")
# ---------------------------------------------------------------------

# C++ standard to use (controlled with -DCPP_STANDARD flag)
if(NOT DEFINED CMAKE_CXX_STANDARD)
    SET(CMAKE_CXX_STANDARD 11)
    message(STATUS "g4e: Setting default to ${CPP_STANDARD}")
endif()
message(STATUS "g4e: CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

# General include directories
# TODO move to targets whatever is possible
include_directories(
        BEFORE
            ${PROJECT_SOURCE_DIR}/include
            ${PROJECT_SOURCE_DIR}/src/design/all
            ${PROJECT_SOURCE_DIR}/src/generator
            ${PROJECT_SOURCE_DIR}/src/generator/beagle_reader
            ${PROJECT_SOURCE_DIR}/src/generator/hepmc2_reader
            ${PROJECT_SOURCE_DIR}/src/generator/pythia_ascii_reader
            ${PROJECT_SOURCE_DIR}/src/generator/generic_lund_reader
            ${PROJECT_SOURCE_DIR}/src/generator/root_output
            ${PROJECT_SOURCE_DIR}/src/helpers
            ${PROJECT_SOURCE_DIR}/src/root_output

        SYSTEM
            ${ROOT_INCLUDE_DIRS}
            ${PROJECT_SOURCE_DIR}/src/external/spdlog/include
            ${Geant4_INCLUDE_DIR}
            ${HEPMC_INCLUDE_DIR}
            ${VGM_DIRECTORY}/include
            ${VGM_DIRECTORY}/test/include
)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
ENDIF("${isSystemDir}" STREQUAL "-1")



# TODO this is terrible!
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)
list(APPEND sources

        #    H E L P E R S
        src/helpers/StringHelpers.cc
        src/helpers/StringHelpers.hh


        #
        # Generator
        #

        # Primary user action and a messenger
        src/generator/PrimaryGeneratorAction.cc
        src/generator/PrimaryGeneratorAction.hh
        src/generator/PrimaryGeneratorMessenger.cc
        src/generator/PrimaryGeneratorMessenger.hh

        # Generic lund reader
        src/generator/generic_lund_reader/TextEventFileReader.hh

        # Beagle event generator reader
        src/generator/beagle_reader/BeagleReader.cc
        src/generator/beagle_reader/BeagleReader.hh
        src/generator/beagle_reader/BeagleInterfaceMessenger.cc
        src/generator/beagle_reader/BeagleInterfaceMessenger.hh
        src/generator/beagle_reader/BeagleInterface.cc
        src/generator/beagle_reader/BeagleInterface.hh

        # HepMC2 event generator reader
        src/generator/hepmc2_reader/HepMCG4AsciiReader.cc
        src/generator/hepmc2_reader/HepMCG4AsciiReader.hh
        src/generator/hepmc2_reader/HepMCG4AsciiReaderMessenger.cc
        src/generator/hepmc2_reader/HepMCG4AsciiReaderMessenger.hh
        src/generator/hepmc2_reader/HepMCG4Interface.cc
        src/generator/hepmc2_reader/HepMCG4Interface.hh

        # Pythia 6 lund format reader
        src/generator/pythia_ascii_reader/PythiaAsciiReaderMessenger.hh
        src/generator/pythia_ascii_reader/PythiaAsciiReader.cc
        src/generator/pythia_ascii_reader/PythiaAsciiReader.hh
        src/generator/pythia_ascii_reader/PythiaAsciiReaderMessenger.cc

        )
#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(g4e g4e.cc ${sources} ${headers})
target_link_libraries(g4e
        ${Geant4_LIBRARIES}
        ${HEPMC_LIBRARIES}
        ${VGM_Geant4GM}
        ${VGM_BaseVGM}
        ${VGM_RootGM}
        ${VGM_XmlVGM}
        ${ROOT_LIBRARIES}
        )

#${HEPMC_FIO_LIBRARIES}
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build JLEICgeant4. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(g4e_SCRIPTS
       jleic.mac
       jleicvis.mac
  )

foreach(_script ${JLEICgeant4_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()

# Copy resources to a destination
# https://stackoverflow.com/questions/13429656/how-to-copy-contents-of-a-directory-into-build-directory-after-make-with-cmake
add_custom_command(TARGET g4e PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:g4e>)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS g4e DESTINATION bin)

