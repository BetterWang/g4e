#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(g4e)

#include(cmake/Modules/FindGeant4.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()
message(STATUS "g4e: Geant4_DIR: ${Geant4_DIR}")

set (CMAKE_CXX_STANDARD 11)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

##---- ROOT ---

Execute_process(
    COMMAND root-config --cflags 
    OUTPUT_VARIABLE rootflags
    OUTPUT_STRIP_TRAILING_WHITESPACE)

Execute_process(
    COMMAND root-config --libs
    OUTPUT_VARIABLE rootlibs
    OUTPUT_STRIP_TRAILING_WHITESPACE)

        EXECUTE_PROCESS(COMMAND root-config --cflags OUTPUT_VARIABLE ROOT_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ROOT_CXX_FLAGS}")

        EXECUTE_PROCESS(COMMAND root-config --libs OUTPUT_VARIABLE ROOT_LD_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
        set(CMAKE_EXE_LINKER_FLAGS ${ROOT_LD_FLAGS})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#

if(EXISTS ${VGM_DIR})
    message(STATUS "jleic-geant: cmake VGM_DIR provided: ${VGM_DIR}")
elseif(EXISTS $ENV{VGM_DIR})
    set(VGM_DIR $ENV{VGM_DIR})
    message(STATUS "g4e: VGM_DIR set from environemnt ${VGM_DIR}")
else()
    message(STATUS "g4e: No cmake VGM_DIR nor env:VGM_DIR provided")
    message(STATUS "g4e: Build will try to locate VGMConfig.cmake or FindVGM.cmake")
endif()

find_library(VGM_Geant4GM NAMES Geant4GM HINTS ${VGM_DIR} PATH_SUFFIXES lib lib64)
find_library(VGM_BaseVGM  NAMES BaseVGM  HINTS ${VGM_DIR} PATH_SUFFIXES lib lib64)
find_library(VGM_RootGM   NAMES RootGM   HINTS ${VGM_DIR} PATH_SUFFIXES lib lib64)
find_library(VGM_XmlVGM   NAMES XmlVGM   HINTS ${VGM_DIR} PATH_SUFFIXES lib lib64)

message(STATUS "g4e: VGM_Geant4GM :${VGM_Geant4GM} ")
message(STATUS "g4e: VGM_BaseVGM  :${VGM_BaseVGM}  ")
message(STATUS "g4e: VGM_RootGM   :${VGM_RootGM}   ")
message(STATUS "g4e: VGM_XmlVGM   :${VGM_XmlVGM}   ")
message(STATUS "g4e: Geant4_LIBS  :${Geant4_LIBRARIES}" )

IF(DEFINED ENV{HEPMC_DIR})
 SET(HEPMC_DIR $ENV{HEPMC_DIR})
ELSE()
 set(HEPMC_DIR /home/yulia/EIC-soft/HepMC-2.06.09/install )
ENDIF()

#SET(HEPMC_INCLUDE_DIR  ${HEPMC_DIR}/include )

find_package(HepMC REQUIRED)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src/design
        ${Geant4_INCLUDE_DIR}
		${HEPMC_INCLUDE_DIR}
        ${VGM_DIR}/include
        ${VGM_DIR}/test/include
)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(g4e g4e.cc ${sources} ${headers} include/JLeicRootOutput.hh)
target_link_libraries(g4e
        ${Geant4_LIBRARIES}
        ${HEPMC_LIBRARIES}
        ${VGM_Geant4GM}
        ${VGM_BaseVGM}
        ${VGM_RootGM}
        ${VGM_XmlVGM}
        # TODO replace for NORMAL root config
        $ENV{ROOTSYS}/lib/libGeom.so
        $ENV{ROOTSYS}/lib/libCore.so
        $ENV{ROOTSYS}/lib/libRIO.so
        $ENV{ROOTSYS}/lib/libHist.so
        $ENV{ROOTSYS}/lib/libTree.so
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

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS g4e DESTINATION bin)

