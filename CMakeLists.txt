cmake_minimum_required(VERSION 3.21)

set(RUN_SET OFF CACHE BOOL [FORCE])
#-----------------------------------------------------------------------------------------

if (RUN_SET)
    project(set)
else()
    project(avl_tree)
endif()

#-----------------------------------------------------------------------------------------

set(TESTING_ENABLED ON CACHE BOOL [FORCE])

#-----------------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE "Release")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS  -fsanitize=address,leak)
    add_compile_definitions(MY_DEBUG)
    message("Debug_version")
endif ()

#-----------------------------------------------------------------------------------------

add_subdirectory(graph_lib/)
add_subdirectory(debug_utils/) #make compile only in debug mode
add_subdirectory(avl_tree/)

if (TESTING_ENABLED)
    add_subdirectory(./tests/)
endif()

unset(TESTING_ENABLED CACHE)
unset(RUN_SET CACHE)


