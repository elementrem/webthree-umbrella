aux_source_directory(. SRC_LIST)

set(EXECUTABLE ele)

file(GLOB HEADERS "*.h")

ele_simple_add_executable(${EXECUTABLE} ${SRC_LIST} ${HEADERS})

ele_use(${EXECUTABLE} REQUIRED Web3::web3jsonrpc JsonRpc::Client Web3::webthree Ele::elecore Ele::p2p Ele::devcrypto Ele::elementrem Ele::elhashseal)
ele_use(${EXECUTABLE} OPTIONAL Ele::evmjit)

if (APPLE)
	install(TARGETS ${EXECUTABLE} DESTINATION bin)
else()
	ele_install_executable(${EXECUTABLE})
endif()
