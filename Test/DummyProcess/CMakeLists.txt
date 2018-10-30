cmake_minimum_required(VERSION 2.8)
set(TARGET_NAME DummyProcess)

##############################################################
## Declaration of output path
##############################################################
if(MSVC)
	set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../Build)
else()
	set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../Build/Test)
endif()
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})
message(STATUS "OUTPUT_DIRECTORY: " ${OUTPUT_DIRECTORY})
message(STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME})
message(STATUS "TARGET_NAME: " ${TARGET_NAME})

##############################################################
## Sources Filtering
##############################################################
file(GLOB_RECURSE SOURCES *.cpp)
message(STATUS "SOURCES: " ${SOURCES})


##############################################################
## Build setting
##############################################################
if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Build)
	add_executable(${TARGET_NAME} ${SOURCES})
	target_link_libraries(${TARGET_NAME} cppcore)
elseif(APPLE)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Build/x64MacOSXRelease)
	add_executable(${TARGET_NAME} ${SOURCES})
	target_link_libraries(${TARGET_NAME} cppcore iconv)
else()
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Build/x64LinuxRelease)
	add_executable(${TARGET_NAME} ${SOURCES})
	target_link_libraries(${TARGET_NAME} cppcore pthread dl rt)
endif()
