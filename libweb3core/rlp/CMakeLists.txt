aux_source_directory(. SRC_LIST)

set(EXECUTABLE rlp)

add_executable(${EXECUTABLE} ${SRC_LIST})

ele_use(${EXECUTABLE} REQUIRED Dev::devcrypto)

if (APPLE)
	install(TARGETS ${EXECUTABLE} DESTINATION bin)
else()
	ele_install_executable(${EXECUTABLE})
endif()

