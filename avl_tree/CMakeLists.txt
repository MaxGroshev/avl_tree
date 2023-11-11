cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

if (RUN_SET)
    project(set)
else()
    project(avl_tree)
endif()

#-----------------------------------------------------------------------------------------

set(SOURCE
    ./ui/ui.cpp)

if(RUN_SET)
    set(MAIN ./src/set_comparison.cpp)
    message(${RUN_SET})
else()
    set(MAIN  ./main.cpp)
endif()

#-----------------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} ${SOURCE} ${DEBUG_UTILS} ${MAIN})

target_link_libraries(${PROJECT_NAME} graphviz debug_utils)

target_include_directories(${PROJECT_NAME} PRIVATE ./ui
                                                    ./include/)



