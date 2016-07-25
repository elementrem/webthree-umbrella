# Define color codes

if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
endif()

cmake_minimum_required(VERSION 3.0.0)

set(ELE_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/webthree-helpers/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${ELE_CMAKE_DIR})

set(UTILS_DIR "${CMAKE_CURRENT_LIST_DIR}/webthree-helpers/utils" CACHE PATH "The path to the utils libraries directory")
set(DEV_DIR "${CMAKE_CURRENT_LIST_DIR}/libweb3core" CACHE PATH "The path to the dev libraries directory")
set(ELE_DIR "${CMAKE_CURRENT_LIST_DIR}/libelementrem" CACHE PATH "The path to the ele libraries directory")
set(WEB3_DIR "${CMAKE_CURRENT_LIST_DIR}/webthree" CACHE PATH "The path to the webthree libraries directory")
set(SOL_DIR "${CMAKE_CURRENT_LIST_DIR}/solidity" CACHE PATH "The path to the solidity libraries directory")

# set cmake_policies

if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/webthree-helpers/cmake")
   message(FATAL_ERROR "${BoldRed}Could not find sub-modules ${BoldGreen}\nDid you use 'git clone --recursive' to checkout all repositories?${ColourReset}")
endif()

include(ElePolicy)
ele_policy()

# project name and version should be set after cmake_policy CMP0048
project(cpp-elementrem VERSION "1.2.9")

include(EleCompilerSettings)

include(EleExecutableHelper)

include(EleDependencies)

include(EleUtils)

include(EleOptions)
configure_project(GUI SOLIDITY)

add_subdirectory(webthree-helpers/utils)

add_subdirectory(libweb3core)
add_subdirectory(libelementrem)
add_subdirectory(webthree)

if (SOLIDITY)
	add_subdirectory(solidity)
endif()

if (GUI)
	add_subdirectory(alelezero)
	if (SOLIDITY)
		add_subdirectory(mix)
	endif()
	ele_appdmg()
	ele_nsis()
endif()
