cmake_minimum_required(VERSION 3.0.0)

set(ELE_CMAKE_DIR   "${CMAKE_CURRENT_LIST_DIR}/../webthree-helpers/cmake"   CACHE PATH "The the path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${ELE_CMAKE_DIR})

# Set cmake_policies
include(ElePolicy)
ele_policy()

# project name and version should be set after cmake_policy CMP0048
set(PROJECT_VERSION "0.3.5")
project(solidity VERSION ${PROJECT_VERSION})

# Let's find our dependencies
include(EleDependencies)

# Figure out what compiler and system are we using
include(EleCompilerSettings)

# Include helper macros
include(EleExecutableHelper)

# Include utils
include(EleUtils)

include(EleOptions)
configure_project(TESTS)

add_subdirectory(libevmasm)
add_subdirectory(libsolidity)
add_subdirectory(solc)

if (NOT EMSCRIPTEN)
	add_subdirectory(liblll)
	add_subdirectory(lllc)
endif()

if (TESTS AND NOT EMSCRIPTEN)
	add_subdirectory(test)
endif()

# TODO installation and packaging rules
