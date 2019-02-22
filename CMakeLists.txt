#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(JLEICgeant4)

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
IF(DEFINED ENV{VGM_SOURCE_DIR})
 SET(VGM_SOURCE_DIR $ENV{VGM_SOURCE_DIR})
ELSE()
 set(VGM_SOURCE_DIR /home/yulia/VGM)
ENDIF()

IF(DEFINED ENV{HEPMC_DIR})
 SET(HEPMC_DIR $ENV{HEPMC_DIR})
ELSE()
 set(HEPMC_DIR /home/yulia/EIC-soft/HepMC-2.06.09/install )
ENDIF()

#SET(HEPMC_INCLUDE_DIR  ${HEPMC_DIR}/include )

find_package(HepMC REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
		    ${HEPMC_INCLUDE_DIR}
   ${VGM_SOURCE_DIR}/include
   ${VGM_SOURCE_DIR}/test/include)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(JLEICgeant4 JLEICgeant4.cc ${sources} ${headers})
target_link_libraries(JLEICgeant4 ${Geant4_LIBRARIES} ${VGM_SOURCE_DIR}/lib64/libGeant4GM.so ${VGM_SOURCE_DIR}/lib64/libBaseVGM.so
                       ${VGM_SOURCE_DIR}/lib64/libRootGM.so ${HEPMC_LIBRARIES} $ENV{ROOTSYS}/lib/libGeom.so ) 
#${HEPMC_FIO_LIBRARIES}
#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build JLEICgeant4. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(JLEICgeant4_SCRIPTS
       vis.mac jleic.mac jleicvis.mac
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
install(TARGETS JLEICgeant4 DESTINATION bin)

