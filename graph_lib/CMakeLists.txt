cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

project(graphviz)

#-----------------------------------------------------------------------------------------

add_library(${PROJECT_NAME} INTERFACE .)

target_include_directories(${PROJECT_NAME} INTERFACE .)




