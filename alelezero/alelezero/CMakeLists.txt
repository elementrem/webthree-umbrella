set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB_RECURSE SRC_LIST "*.cpp" "plugins/*.cpp")
file(GLOB_RECURSE HEADERS "*.h" "plugins/*.h")
file(GLOB_RECURSE UI_RESOURCES "*.ui" "plugins/*.ui")

# required for ele_add_resources, TODO: remove this
if (DEFINED elementrem_SOURCE_DIR)
	set(ELE_DIR ${elementrem_SOURCE_DIR})
else()
	set(ELE_DIR "${CMAKE_CURRENT_LIST_DIR}/../cpp-elementrem"         CACHE PATH "The path to the cpp-elementrem directory")
endif()

ele_name(EXECUTABLE AleleZero)

# ele_add_executable is defined in cmake/EleExecutableHelper.cmake
ele_add_executable(${EXECUTABLE}
	ICON alelezero
	UI_RESOURCES ${UI_RESOURCES} alelezero.icns
	WIN_RESOURCES alelezero.rc
)

target_link_libraries(${EXECUTABLE} alele)
target_include_directories(${EXECUTABLE} PUBLIC ..)
ele_use(${EXECUTABLE} REQUIRED Qt::Core Qt::Widgets Ele::elementrem Web3::web3jsonrpc Ele::elhashseal Solidity::lll)
if (SOLIDITY)
	ele_use(${EXECUTABLE} REQUIRED Solidity::solidity)
endif()

# required because AUTOUIC can't handle multiple ui includes in one cpp file
qt5_wrap_ui(ui_AleleZero.h AleleZero.ui)
qt5_wrap_ui(ui_Connect.h Connect.ui)

ele_install_executable(${EXECUTABLE})