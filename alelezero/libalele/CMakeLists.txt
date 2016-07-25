set(CMAKE_INCLUDE_CURRENT_DIR ON)

aux_source_directory(. SRC_LIST)

file(GLOB HEADERS "*.h")
file(GLOB UI_RESOURCES "*.ui")

# This is a hack until someone proposes a better solution. When building split repos
# ELE_DIR has not been set yet but is nonetheless used in the ele_add_resources() call.
# TODO: fix properly
set(ELE_DIR "${ELE_CMAKE_DIR}/../../libelementrem" CACHE PATH "The path to the elementrem directory")
ele_add_resources("${CMAKE_CURRENT_SOURCE_DIR}/JSResources.cmake" "JSRES" "${ELE_DIR}/..")

set(EXECUTABLE alele)

# it should be static library, cause there is a problem with creating QWidgets from dynamic libraries
# it fails with "Must construct a QApplication before a QWidget"
# https://forum.qt.io/topic/39042/can-t-create-a-qwidget-from-dynamic-library-must-construct-a-qapplication-before-a-qwidget
add_library(${EXECUTABLE} STATIC ${SRC_LIST} ${JSRES} ${HEADERS} ${UI_RESOURCES})

set_target_properties(${EXECUTABLE} PROPERTIES AUTOUIC ON AUTOMOC ON)
ele_use(${EXECUTABLE} REQUIRED Qt::Core Qt::Widgets Qt::Network Web3::webthree Ele::elecore Ele::p2p Ele::devcrypto Ele::elementrem Web3::web3jsonrpc Ele::natspec Ele::elhashseal)
