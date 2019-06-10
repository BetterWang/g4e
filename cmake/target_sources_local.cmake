# the problem this function is to solve:
# in CMAKE 3.12 target_sources relative paths are from starts_with.cmake that declared a target
# in CMAKE 3.13 target_sources relative paths are from starts_with.cmake calling target_sources
# Guess what CMAKE versions are installed on RHEL 7? Guess what version is on other designed-for-humans systems?
# You can read more here:
# https://crascit.com/2016/01/31/enhanced-source-file-handling-with-target_sources/
# What we want is something like this (not depending on CMake version):
#   target_sources_local(myLib
#      PRIVATE
#         foo.cpp
#         foo_p.cpp
#         foo_p.h
#      PUBLIC
#         foo.h
#   )

# NOTE: This helper function assumes no generator expressions are used
#       for the source files
function(target_sources_local target)
    if(POLICY CMP0076)
        # New behavior is available, so just forward to it by ensuring
        # that we have the policy set to request the new behavior, but
        # don't change the policy setting for the calling scope
        cmake_policy(PUSH)
        cmake_policy(SET CMP0076 NEW)
        target_sources(${target} ${ARGN})
        cmake_policy(POP)
        return()
    endif()

    # Must be using CMake 3.12 or earlier, so simulate the new behavior
    unset(_srcList)
    get_target_property(_targetSourceDir ${target} SOURCE_DIR)

    foreach(src ${ARGN})
        if(NOT src STREQUAL "PRIVATE" AND
                NOT src STREQUAL "PUBLIC" AND
                NOT src STREQUAL "INTERFACE" AND
                NOT IS_ABSOLUTE "${src}")
            # Relative path to source, prepend relative to where target was defined
            file(RELATIVE_PATH src "${_targetSourceDir}" "${CMAKE_CURRENT_LIST_DIR}/${src}")
        endif()
        list(APPEND _srcList ${src})
    endforeach()
    target_sources(${target} ${_srcList})
endfunction()