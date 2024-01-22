cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

project(avl_tree)

#-----------------------------------------------------------------------------------------

set(TESTING_ENABLED ON CACHE BOOL [FORCE])

#-----------------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE "Debug")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    # set(CMAK E_CXX_FLAGS  -fsanitize=address,leak)
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


