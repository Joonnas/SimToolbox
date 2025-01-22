cmake_minimum_required(VERSION 3.4)

if(POLICY CMP0079)
    cmake_policy(SET CMP0079 NEW)
endif()

project(SimToolbox VERSION 0.1)

add_executable(SimToolbox Main.cpp)

target_include_directories(SimToolbox
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_subdirectory(${CMAKE_SOURCE_DIR}/ext)
add_subdirectory(${CMAKE_SOURCE_DIR}/utils)
add_subdirectory(${CMAKE_SOURCE_DIR}/gui)
add_subdirectory(${CMAKE_SOURCE_DIR}/physics)