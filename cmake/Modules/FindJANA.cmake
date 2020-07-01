
# ===              J A N A                  ===
# =============================================
print_header(">>>>    J A N A 2     <<<<" )


if(DEFINED JANA_DIR)
    message(STATUS "FindJANA: cmake JANA_DIR provided: ${JANA_DIR}")
elseif(DEFINED ENV{JANA_HOME})
    message(STATUS "FindJANA: JANA2 provided $ENV{JANA_HOME}")
    set(JANA_DIR $ENV{JANA_HOME})
else()
    message(WARNING "FindJANA: No cmake JANA_DIR nor env:JANA_HOME provided")
    message(WARNING "FindJANA: Build will use JanaConfig.cmake or FindJANA.cmake")
endif()

if(NOT EXISTS ${JANA_DIR})
    message(WARNING "FindJANA: JANA_DIR direcotry doesn't exist: '${JANA_DIR}'")
endif()

set(JANA_CONFIG_EXECUTABLE JANA_CONFIG_EXECUTABLE-NOTFOUND )
MARK_AS_ADVANCED( JANA_CONFIG_EXECUTABLE )

find_program( JANA_CONFIG_EXECUTABLE jana-config PATHS ${JANA_DIR}/bin NO_DEFAULT_PATH )

if( NOT JANA_CONFIG_EXECUTABLE )
    message( FATAL_ERROR "FindJANA: Didn't find JANA" )
else()

    # we JANA_CONFIG_EXECUTABLE is <jana-dir>/bin/jana-config so we call get_filename_component twice
    get_filename_component(JANA_DIR ${JANA_CONFIG_EXECUTABLE} DIRECTORY)
    get_filename_component(JANA_DIR ${JANA_DIR} DIRECTORY)
    message( STATUS "FindJANA: Found jana-config: ${JANA_CONFIG_EXECUTABLE}" )
endif()

if( JANA_CONFIG_EXECUTABLE )

    # JANA_LIBRARIES
    # get library dir from root-config output
    EXECUTE_PROCESS( COMMAND "${JANA_CONFIG_EXECUTABLE}" --libs
            OUTPUT_VARIABLE JANA_LIBS_FLAGS
            RESULT_VARIABLE _exit_code
            OUTPUT_STRIP_TRAILING_WHITESPACE
            )
    IF( NOT _exit_code EQUAL 0 )
        # clear variable if root-config exits with error
        # it might contain garbage
        SET( JANA_LIBS_FLAGS )
    ENDIF()

    # JANA_CFLAGS
    # get library dir from root-config output
    EXECUTE_PROCESS( COMMAND "${JANA_CONFIG_EXECUTABLE}" --cflags
            OUTPUT_VARIABLE JANA_CFLAGS
            RESULT_VARIABLE _exit_code
            OUTPUT_STRIP_TRAILING_WHITESPACE
            )
    IF( NOT _exit_code EQUAL 0 )
        # clear variable if root-config exits with error
        # it might contain garbage
        SET( JANA_CFLAGS )
    ENDIF()
endif()

#add_definitions(${JANA_LIBS_FLAGS})
#add_definitions(${JANA_CFLAGS})
set(JANA_INCLUDE_DIRS ${JANA_DIR}/include)
set(JANA_LIB_DIR ${JANA_DIR}/lib)
find_library(JANA_LIB JANA HINTS ${JANA_LIB_DIR})

message( STATUS "FindJANA: JANA_INCLUDE_DIRS:  ${JANA_INCLUDE_DIRS}")
message( STATUS "FindJANA: JANA_DIR set to: ${JANA_DIR}")
message( STATUS "FindJANA: JANA_LIB set to: ${JANA_LIB}")
