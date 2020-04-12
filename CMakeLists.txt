cmake_minimum_required(VERSION 2.8)

project(DataStructures)

aux_source_directory("./List" List)

add_executable(${PROJECT_NAME} "main.cpp" ${List})
