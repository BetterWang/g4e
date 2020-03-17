#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(g4e)

include(cmake/print_functions.cmake)                 # Helpers to print fancy headers, file names, etc
include(cmake/target_sources_local.cmake)            # Add relative source path independently of cmake version or policy

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules ${CMAKE_MODULE_PATH} )
include(cotire)


# -----------------------------------------------------------------
print_grand_header("    F I N D   B A S I C   P A C K A G E S    ")
# -----------------------------------------------------------------

# Cern R O O T
# ------------
print_header(">>>>    C E R N    R O O T    <<<<")

# Some debug text
if(EXISTS "$ENV{ROOTSYS}")
    message(STATUS "g4e: ROOTSYS provided $ENV{ROOTSYS}")
    set(CERNROOT_DIR $ENV{ROOTSYS})
elseif(CERNROOT_DIR)
    message(STATUS "ejana: CERNROOT_DIR provided ${CERNROOT_DIR}")
else()
    message(STATUS "ejana warning: No ROOTSYS or CERNROOT_DIR provided")
    message(STATUS "ejana warning: Build will try to use ROOTConfig.cmake or FindROOT.cmake")
endif()

# You need to tell CMake where to find the ROOT installation.
#   - ROOT built with CMake. CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH ${CERNROOT_DIR})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS Geom Core RIO Hist Tree)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})



# G E A N T 4
# -----------
# Can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)

if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
message(STATUS "g4e: GEANT4_DIR: ${GEANT4_DIR}")

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
message(STATUS "g4e: HEPMC_LIBRARY_DIR  :${HEPMC_LIBRARY_DIR}")
message(STATUS "g4e: HEPMC_LIBRARIES  :${HEPMC_LIBRARIES}")


# ---------------------------------------------------------------------
print_grand_header("    G E N E R A L   C O N F I G U R A T I O N    ")
# ---------------------------------------------------------------------

# >oO profiling
# add_definitions("-ftime-trace")
# >oO profiling add_definitions("-ftime-trace -ftime-report")
# set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

# C++ standard to use (controlled with -DCPP_STANDARD flag)
if(NOT DEFINED CMAKE_CXX_STANDARD)
    SET(CMAKE_CXX_STANDARD 11)
    message(STATUS "g4e: Setting default to ${CMAKE_CXX_STANDARD}")
endif()
message(STATUS "g4e: CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")


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

if(POLICY CMP0079)
    cmake_policy(SET CMP0079 NEW)
endif()

add_subdirectory(src/external/spdlog)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(g4e "")

target_link_libraries(g4e
        ${Geant4_LIBRARIES}
        ${HEPMC_LIBRARIES}
        ${VGM_Geant4GM}
        ${VGM_BaseVGM}
        ${VGM_RootGM}
        ${VGM_XmlVGM}
        ${ROOT_LIBRARIES}
        spdlog
        )

#set_property(TARGET g4e APPEND_STRING PROPERTY CXX_FLAGS "")

target_include_directories(g4e
        SYSTEM
        PRIVATE
            ${ROOT_INCLUDE_DIRS}
            ${Geant4_INCLUDE_DIR}
            ${HEPMC_INCLUDE_DIR}
            ${VGM_DIRECTORY}/include
            ${VGM_DIRECTORY}/test/include
            src/external/spdlog/include     # Header only library for str formatting and fast logging
            src/external/CLI11              # Header only library for argument parsing
        )

# All sources and includes for the project are there:
add_subdirectory(src)

#cotire(g4e)

# This is good for in JupyterLab Build
if(${G4E_SILENCE_WARNINGS})
    if ( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR (CMAKE_C_COMPILER_ID MATCHES "Clang") OR (CMAKE_CXX_COMPILER_ID MATCHES "Clang") )
        target_compile_options(g4e PRIVATE "-w")
    endif()
endif()
#${HEPMC_FIO_LIBRARIES}
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build JLEICgeant4. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(g4e_SCRIPTS
        resources/jleic/jleic.mac
        resources/jleic/jleicvis.mac
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
#add_custom_command(TARGET g4e PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/resources/ $<TARGET_FILE_DIR:g4e>)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS g4e DESTINATION bin)

add_subdirectory(test/unit_tests)
