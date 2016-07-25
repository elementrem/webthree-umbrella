cmake_minimum_required(VERSION 3.0.0)

# A place where should we look for *.cmake files
set(ELE_CMAKE_DIR   "${CMAKE_CURRENT_LIST_DIR}/../webthree-helpers/cmake"   CACHE PATH "The the path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${ELE_CMAKE_DIR})

# Set cmake_policies
include(ElePolicy)
ele_policy()

# project name and version should be set after cmake_policy CMP0048
set(PROJECT_VERSION "1.0.5")
project(mix VERSION ${PROJECT_VERSION})

# Let's find our dependencies
include(EleDependencies)

# Figure out what compiler and system are we using
include(EleCompilerSettings)

# Include helper macros
include(EleExecutableHelper)

# Include utils
include(EleUtils)

include(EleOptions)
configure_project()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
aux_source_directory("./src" SRC_LIST)
file(GLOB HEADERS "src/*.h")

ele_name(EXECUTABLE Mix-ide)
find_package(Qt5Core)

if (APPLE)
	qt5_add_resources(UI_RESOURCES osx.qrc)
endif()

qt5_add_resources(UI_RESOURCES res.qrc qml.qrc)
qt5_add_resources(UI_RESOURCES web.qrc)
qt5_add_resources(UI_RESOURCES ${CMAKE_CURRENT_BINARY_DIR}/web3.qrc)

if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DQT_QML_DEBUG)
endif()

# ele_add_executable is defined in cmake/EleExecutableHelper.cmake
ele_add_executable(${EXECUTABLE} 
	ICON mix
	UI_RESOURCES ${UI_RESOURCES} mix.icns
	WIN_RESOURCES mix.rc
)

ele_use(${EXECUTABLE} REQUIRED Qt::Core Qt::Widgets Qt::WebEngine Ele::elementrem Web3::web3jsonrpc Ele::elhashseal)
ele_use(${EXECUTABLE} OPTIONAL Solidity::solidity)

# ele_install_executable is defined in cmake/EleExecutableHelper.cmake
ele_install_executable(${EXECUTABLE}
	QMLDIR ${CMAKE_CURRENT_SOURCE_DIR}/qml
)

#generate web3.qrc
set(ELE_WEB3JS_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/../webthree-helpers/js/setup.js ${CMAKE_CURRENT_SOURCE_DIR}/../webthree-helpers/js/admin.js ${CMAKE_CURRENT_SOURCE_DIR}/../web3.js/dist/web3.js)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/web3.qrc
	DEPENDS ${ELE_WEB3JS_SOURCES}
	COMMAND ${CMAKE_COMMAND} -DELE_RES_FILES="${ELE_WEB3JS_SOURCES}" -DELE_QRC_DEST="${CMAKE_CURRENT_BINARY_DIR}/web3.qrc" -P "${ELE_SCRIPTS_DIR}/genqrc.cmake"
)

#add qml asnd stdc files to project tree in Qt creator
file(GLOB_RECURSE QMLFILES "qml/*.*")
file(GLOB_RECURSE TESTFILES "test/qml/*.*")
file(GLOB_RECURSE SOLFILES "stdc/*.*")
add_custom_target(mix_qml SOURCES ${QMLFILES} ${SOLFILES} ${TESTFILES})

#test target
find_package(Qt5QuickTest REQUIRED)
find_package(Qt5Test REQUIRED)
set(TEST_EXECUTABLE mix_test)
list(APPEND LIBRARIES "Qt5::QuickTest")
list(APPEND LIBRARIES "Qt5::Test")
list(REMOVE_ITEM SRC_LIST "src/main.cpp")
aux_source_directory(test SRC_LIST)
file(GLOB HEADERS "test/*.h")
add_executable(${TEST_EXECUTABLE} ${SRC_LIST} ${HEADERS})
target_link_libraries(${TEST_EXECUTABLE} ${LIBRARIES})
set_target_properties(${TEST_EXECUTABLE} PROPERTIES EXCLUDE_FROM_ALL 1 EXCLUDE_FROM_DEFAULT_BUILD 1)



