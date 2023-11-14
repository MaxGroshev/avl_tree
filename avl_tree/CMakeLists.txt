cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

project(avl_tree)

#-----------------------------------------------------------------------------------------

set(SOURCE
    ./ui/ui.cpp)

#-----------------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${SOURCE} ${DEBUG_UTILS} ./main.cpp)
add_executable(set ${SOURCE} ${DEBUG_UTILS} ./src/set_comparison.cpp)

target_link_libraries(${PROJECT_NAME} graphviz debug_utils)
target_link_libraries(set graphviz debug_utils)

target_include_directories(${PROJECT_NAME} PRIVATE ./ui ./include/)
target_include_directories(set PRIVATE ./ui ./include/)



