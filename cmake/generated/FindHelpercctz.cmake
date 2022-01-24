# AUTOGENERATED, DON'T CHANGE THIS FILE!

if (cctz_FOUND)
  return()
endif()

if (TARGET cctz)
  set(cctz_FOUND ON)
  return()
endif()


set(FULL_ERROR_MESSAGE "Could not find `cctz` package.\n\tDebian: sudo apt update && sudo apt install libcctz-dev\n\tMacOS: brew install cctz-dev")
if (cctz_FIND_VERSION)
    set(FULL_ERROR_MESSAGE "${FULL_ERROR_MESSAGE}\nRequired version is at least ${cctz_FIND_VERSION}")
endif()


include(FindPackageHandleStandardArgs)

find_library(cctz_LIBRARIES_cctz
  NAMES cctz
  PATHS /usr/lib/x86_64-linux-gnu
)
list(APPEND cctz_LIBRARIES ${cctz_LIBRARIES_cctz})

find_path(cctz_INCLUDE_DIRS_cctz_civil_time_h
  NAMES cctz/civil_time.h
)
list(APPEND cctz_INCLUDE_DIRS ${cctz_INCLUDE_DIRS_cctz_civil_time_h})



if (cctz_FIND_VERSION)
if (UNIX AND NOT APPLE)
  find_program(DPKG_QUERY_BIN dpkg-query)
  if (DPKG_QUERY_BIN)
    execute_process(
      COMMAND dpkg-query --showformat=\${Version} --show libcctz-dev
      OUTPUT_VARIABLE cctz_version_output
      ERROR_VARIABLE cctz_version_error
      RESULT_VARIABLE cctz_version_result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (cctz_version_result EQUAL 0)
      set(cctz_VERSION ${cctz_version_output})
      message(STATUS "Installed version libcctz-dev: ${cctz_VERSION}")
    endif(cctz_version_result EQUAL 0)
  endif(DPKG_QUERY_BIN)
endif(UNIX AND NOT APPLE)
 
if (APPLE)
  find_program(BREW_BIN brew)
  if (BREW_BIN)
    execute_process(
      COMMAND brew list --versions cctz-dev
      OUTPUT_VARIABLE cctz_version_output
      ERROR_VARIABLE cctz_version_error
      RESULT_VARIABLE cctz_version_result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (cctz_version_result EQUAL 0)
      if (cctz_version_output MATCHES "^(.*) (.*)$")
        set(cctz_VERSION ${CMAKE_MATCH_2})
        message(STATUS "Installed version cctz-dev: ${cctz_VERSION}")
      else()
        set(cctz_VERSION "NOT_FOUND")
      endif()
    else()
      message(WARNING "Failed execute brew: ${cctz_version_error}")
    endif()
  endif()
endif()
 
endif (cctz_FIND_VERSION)

 
find_package_handle_standard_args(
  cctz
    REQUIRED_VARS
      cctz_LIBRARIES
      cctz_INCLUDE_DIRS
      
    FAIL_MESSAGE
      "${FULL_ERROR_MESSAGE}"
)
mark_as_advanced(
  cctz_LIBRARIES
  cctz_INCLUDE_DIRS
  
)


if (cctz_FIND_VERSION)
  if (cctz_VERSION VERSION_LESS cctz_FIND_VERSION)
      message(STATUS
          "Version of cctz is ${cctz_VERSION}. "
          "Required version is ${cctz_FIND_VERSION}. Ignoring found cctz."
      )
      set(cctz_FOUND OFF)
  endif()
endif()

if (NOT cctz_FOUND)
  if (cctz_FIND_REQUIRED)
      message(FATAL_ERROR "${FULL_ERROR_MESSAGE}")
  endif()

  return()
endif()

 
if (NOT TARGET cctz)
  add_library(cctz INTERFACE IMPORTED GLOBAL)

  target_include_directories(cctz INTERFACE ${cctz_INCLUDE_DIRS})
  target_link_libraries(cctz INTERFACE ${cctz_LIBRARIES})
  
  # Target cctz is created
endif()