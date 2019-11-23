# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(event)


if(NOT DEFINED ROOT_LIBRARIES)
    # You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
    #   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
    #   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
    list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

    #---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
    find_package(ROOT REQUIRED COMPONENTS RIO)

    #---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
    #include(${ROOT_USE_FILE})

    #include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
    #ROOT_GENERATE_DICTIONARY(G__Event Event.h LINKDEF EventLinkDef.h)

    #---Create a shared library with geneated dictionary
    #add_library(Event SHARED Event.cxx G__Event.cxx)
    #target_link_libraries(Event ${ROOT_LIBRARIES})
endif()

#---Create  a main program using the library
add_executable(tests_g4e main.cpp catch.hh)
target_link_libraries(tests_g4e ${ROOT_LIBRARIES})

target_link_libraries(tests_g4e
        ${Geant4_LIBRARIES}
        ${HEPMC_LIBRARIES}
        ${VGM_Geant4GM}
        ${VGM_BaseVGM}
        ${VGM_RootGM}
        ${VGM_XmlVGM}
        ${ROOT_LIBRARIES}
        )

target_include_directories(tests_g4e
        PUBLIC
        SYSTEM
        ${ROOT_INCLUDE_DIRS}
        ${Geant4_INCLUDE_DIR}
        ${HEPMC_INCLUDE_DIR}
        ${VGM_DIRECTORY}/include
        ${VGM_DIRECTORY}/test/include
        )