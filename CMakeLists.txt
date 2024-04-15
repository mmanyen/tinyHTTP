# WebServer/REST project
cmake_minimum_required (VERSION 3.20)

# Project setup, mostly boilerplate. 
# Project name and Versioning stuff here, change when changing the version

# Note: keep the project name lower case only for easy linux packaging support
project (tinyHTTP VERSION 4.0.0 LANGUAGES CXX C)

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

# Defs for all project compiles
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      add_definitions(-ggdb -O0)
    endif()

endif(WIN32)

# export compile commands by default, this is handy for 
# IDEs (VSCode), clang-tidy, and for other tools.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# We can use include() and find_package() for our scripts in there
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#-------------------------------------------------------------------------------
# Set default install location to dist folder in top dir
# we do not want to install to /usr by default
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set (CMAKE_INSTALL_PREFIX "${PROJECT_HOME}/dist" CACHE PATH
    "Install path prefix, prepended onto install directories." FORCE )
endif()

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



