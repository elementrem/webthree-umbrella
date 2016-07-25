cmake_minimum_required(VERSION 3.0.0)

set(ELE_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/../webthree-helpers/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${ELE_CMAKE_DIR})

# set cmake_policies
include(ElePolicy)
ele_policy()

# project name and version should be set after cmake_policy CMP0048
project(dev VERSION "1.2.9")

include(EleCompilerSettings)

include(EleExecutableHelper)

include(EleDependencies)

include(EleUtils)

include(EleOptions)
configure_project(PARANOID TOOLS TESTS MINIUPNPC)

add_subdirectory(libdevcore)
add_subdirectory(libdevcrypto)
if (NOT EMSCRIPTEN)
	add_subdirectory(libp2p)

	if (TOOLS)
		add_subdirectory(rlp)
		add_subdirectory(bench)
	endif()
endif()

if (TESTS)
	add_subdirectory(test)
endif()
