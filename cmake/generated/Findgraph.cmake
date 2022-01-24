# AUTOGENERATED, DON'T CHANGE THIS FILE!

if (graph_FOUND)
  return()
endif()

if (TARGET graph)
  set(graph_FOUND ON)
  return()
endif()


set(FULL_ERROR_MESSAGE "Could not find `graph` package.\n\tDebian: sudo apt update && sudo apt install libboost-graph-dev\n\tMacOS: brew install boost")
if (graph_FIND_VERSION)
    set(FULL_ERROR_MESSAGE "${FULL_ERROR_MESSAGE}\nRequired version is at least ${graph_FIND_VERSION}")
endif()


include(FindPackageHandleStandardArgs)

find_library(graph_LIBRARIES_boost_graph
  NAMES boost_graph
)
list(APPEND graph_LIBRARIES ${graph_LIBRARIES_boost_graph})

find_path(graph_INCLUDE_DIRS_boost_graph_astar_search_hpp
  NAMES boost/graph/astar_search.hpp
)
list(APPEND graph_INCLUDE_DIRS ${graph_INCLUDE_DIRS_boost_graph_astar_search_hpp})



if (graph_FIND_VERSION)
if (UNIX AND NOT APPLE)
  find_program(DPKG_QUERY_BIN dpkg-query)
  if (DPKG_QUERY_BIN)
    execute_process(
      COMMAND dpkg-query --showformat=\${Version} --show libboost-graph-dev
      OUTPUT_VARIABLE graph_version_output
      ERROR_VARIABLE graph_version_error
      RESULT_VARIABLE graph_version_result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (graph_version_result EQUAL 0)
      set(graph_VERSION ${graph_version_output})
      message(STATUS "Installed version libboost-graph-dev: ${graph_VERSION}")
    endif(graph_version_result EQUAL 0)
  endif(DPKG_QUERY_BIN)
endif(UNIX AND NOT APPLE)
 
if (APPLE)
  find_program(BREW_BIN brew)
  if (BREW_BIN)
    execute_process(
      COMMAND brew list --versions boost
      OUTPUT_VARIABLE graph_version_output
      ERROR_VARIABLE graph_version_error
      RESULT_VARIABLE graph_version_result
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (graph_version_result EQUAL 0)
      if (graph_version_output MATCHES "^(.*) (.*)$")
        set(graph_VERSION ${CMAKE_MATCH_2})
        message(STATUS "Installed version boost: ${graph_VERSION}")
      else()
        set(graph_VERSION "NOT_FOUND")
      endif()
    else()
      message(WARNING "Failed execute brew: ${graph_version_error}")
    endif()
  endif()
endif()
 
endif (graph_FIND_VERSION)

 
find_package_handle_standard_args(
  graph
    REQUIRED_VARS
      graph_LIBRARIES
      graph_INCLUDE_DIRS
      
    FAIL_MESSAGE
      "${FULL_ERROR_MESSAGE}"
)
mark_as_advanced(
  graph_LIBRARIES
  graph_INCLUDE_DIRS
  
)


if (graph_FIND_VERSION)
  if (graph_VERSION VERSION_LESS graph_FIND_VERSION)
      message(STATUS
          "Version of graph is ${graph_VERSION}. "
          "Required version is ${graph_FIND_VERSION}. Ignoring found graph."
      )
      set(graph_FOUND OFF)
  endif()
endif()

if (NOT graph_FOUND)
  if (graph_FIND_REQUIRED)
      message(FATAL_ERROR "${FULL_ERROR_MESSAGE}")
  endif()

  return()
endif()

 
if (NOT TARGET graph)
  add_library(graph INTERFACE IMPORTED GLOBAL)

  if (TARGET Boost::graph)
    target_link_libraries(graph INTERFACE Boost::graph)
  endif()
  target_include_directories(graph INTERFACE ${graph_INCLUDE_DIRS})
  target_link_libraries(graph INTERFACE ${graph_LIBRARIES})
  
  # Target graph is created
endif()