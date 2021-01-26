cmake_add_library(xtoolmain)


target_include_directories(common_interface
	INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
)
target_sources(xtoolmain
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/main.cc
	${CMAKE_CURRENT_SOURCE_DIR}/dlistbuf.cc
	${CMAKE_CURRENT_SOURCE_DIR}/override.cc
	${CMAKE_CURRENT_SOURCE_DIR}/serial.cc
	${CMAKE_CURRENT_SOURCE_DIR}/serialform.cc
	${CMAKE_CURRENT_SOURCE_DIR}/stp.cc
	${CMAKE_CURRENT_SOURCE_DIR}/transferform.cc
	${CMAKE_CURRENT_SOURCE_DIR}/xmlnode.cc
	${CMAKE_CURRENT_SOURCE_DIR}/xmlparse.cc
	${CMAKE_CURRENT_SOURCE_DIR}/xmlwidget.cc
	${CMAKE_CURRENT_SOURCE_DIR}/xtoolform.cc
	
)