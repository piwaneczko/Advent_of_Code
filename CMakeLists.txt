cmake_minimum_required(VERSION 3.10)

project(AdventOfCode)

set (CMAKE_CXX_STANDARD 14)

set(RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../install/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/../install/bin/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/../install/bin/)

################ MSVC #################

if (MSVC)
    # For multithreaded building
    add_compile_options(/MP)
endif (MSVC)

#######################################

add_subdirectory(src)


############ Installation #############

# Automatically install bin/msvcp140.dll and bin/vcruntime140.dll in ${CMAKE_INSTALL_PREFIX}
include(InstallRequiredSystemLibraries)

#######################################