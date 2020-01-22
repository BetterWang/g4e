# - Locate HepMC library
# in a directory defined via  HEPMC_ROOT_DIR or HEPMC_DIR environment variable
# Defines:
#
#  HEPMC_FOUND
#  HEPMC_INCLUDE_DIR
#  HEPMC_INCLUDE_DIRS (not cached)
#  HEPMC_LIBRARIES
#  HEPMC_FIO_LIBRARIES

print_header(">>>>    H E P M C     <<<<" )

find_path(HEPMC_INCLUDE_DIR HepMC/GenEvent.h
        HINTS
        $ENV{HEPMC_DIR}/include
        $ENV{HEPMC_ROOT_DIR}/include
        ${HEPMC_DIRECTORY}/include
        ${HEPMC_DIR}/include)

find_library(HEPMC_LIBRARY
        NAMES HepMC
        HINTS
        $ENV{HEPMC_ROOT_DIR}
        $ENV{HEPMC_DIR}
        ${HEPMC_DIRECTORY}
        ${HEPMC_DIR}
        PATH_SUFFIXES lib lib64
        )

get_filename_component(HEPMC_LIBRARY_DIR ${HEPMC_LIBRARY} PATH)
set(HEPMC_FIO_LIBRARIES "-L${HEPMC_LIBRARY_DIR} -lHepMCfio")


# handle the QUIETLY and REQUIRED arguments and set HEPMC_FOUND to TRUE if
# all listed variables are TRUE
#INCLUDE(FindPackageHandleStandardArgs)

# find_package_handle_standard_args(HepMC DEFAULT_MSG HEPMC_LIBRARY HEPMC_INCLUDE_DIR)

mark_as_advanced(HepMC_FOUND HEPMC_INCLUDE_DIR HEPMC_LIBRARY)

set(HEPMC_INCLUDE_DIRS ${HEPMC_INCLUDE_DIR})
set(HEPMC_LIBRARIES ${HEPMC_LIBRARY} )
