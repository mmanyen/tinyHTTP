# WebServer/REST project
cmake_minimum_required (VERSION 3.11)

# Project setup, mostly boilerplate. 
# Project name and Versioning stuff here, change when changing the version

# Note: keep the project name lower case only for easy linux packaging support
project (prismax VERSION 4.0.0 LANGUAGES CXX C)

set(VERSION_TYPE "dev" CACHE STRING "version type" FORCE)
site_name(VERSION_HOST) # read hostname to VERSION_HOST
set(VERSION_HOST "${VERSION_HOST}" CACHE STRING "host of build" FORCE)

message(STATUS "")
message(STATUS "    == ${PROJECT_NAME} ${PLATFORM_NAME} Project configuration ==")
message(STATUS "")

# Set a default build type if none was specified
set(default_build_type "Debug")
# Check if a build type exists, else create one
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  message("Setting supported build types")
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}" )


# Local.cmake should never be checked into source control
# Default.cmake should be your project defaults and source controlled
include(Default.cmake)
include(Local OPTIONAL)

# Defs for all project compiles
add_compile_definitions(${PROJECT_OPTIONS_PLATFORM})

# Documentation build
add_subdirectory(doc)

# Testing
add_subdirectory(test)

# External assets
add_subdirectory(external)

# Project targets
add_subdirectory(main)

# Packaging stuff (deb, rpm, windows installer)
add_subdirectory(packaging)



