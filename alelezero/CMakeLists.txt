cmake_minimum_required(VERSION 3.0.0)

# A place where should we look for *.cmake files
set(ELE_CMAKE_DIR   "${CMAKE_CURRENT_LIST_DIR}/../webthree-helpers/cmake"   CACHE PATH "The the path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${ELE_CMAKE_DIR})

# Set cmake_policies
include(ElePolicy)
ele_policy()

# project name and version should be set after cmake_policy CMP0048
project(alelezero VERSION "1.2.9")

# Let's find our dependencies
include(EleDependencies)

# Figure out what compiler and system are we using
include(EleCompilerSettings)

# Include helper macros
include(EleExecutableHelper)

# Include utils
include(EleUtils)

include(EleOptions)
configure_project(SOLIDITY)

# required for qt5_wrap_ui below, TODO: remove this
find_package(Qt5Widgets)

add_subdirectory(libalele)
add_subdirectory(alelezero)
