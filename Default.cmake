# Default.cmake for Webserver/REST project
#------------------------------------------------------------------------------
# General settings

# visual studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
if(WIN32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(WIN32)

# export compile commands by default, this is handy for 
# IDEs (VSCode), clang-tidy, and for other tools.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# We can use include() and find_package() for our scripts in there
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# global 
set(PROJECT_LIBS "")                    
set(PROJECT_LIBS_FOLDERS "")           
set(PROJECT_INCLUDE_FOLDERS "${PROJECT_HOME}/external")        
set(PROJECT_OPTIONS "")                  

# platform 
if (WIN32)                          
    set(PROJECT_PLATFORM_LIBS "")          
    set(PROJECT_PLATFORM_LIBS_FOLDERS "")   
    set(PROJECT_PLATFORM_INCLUDE_FOLDERS "")                     
    set(PROJECT_OPTIONS_PLATFORM PLATFORM_WINDOWS)
endif(WIN32)

if (UNIX)                          
    set(PROJECT_PLATFORM_LIBS "")          
    set(PROJECT_PLATFORM_LIBS_FOLDERS "")   
    set(PROJECT_PLATFORM_INCLUDE_FOLDERS "")             
    set(PROJECT_OPTIONS_PLATFORM PLATFORM_LINUX)
endif(UNIX)


#-------------------------------------------------------------------------------
# Set default install location to dist folder in top dir
# we do not want to install to /usr by default
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set (CMAKE_INSTALL_PREFIX "${PROJECT_HOME}/dist" CACHE PATH
    "Install path prefix, prepended onto install directories." FORCE )
endif()
