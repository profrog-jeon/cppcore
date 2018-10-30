cmake_minimum_required(VERSION 2.8)
set(TARGET_NAME DummyDynamicLibrary)

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
## Utility functions
##############################################################
function(exclude_from_list SOURCES_LIST EXPRESSION)
	file(GLOB_RECURSE EXCLUDE ${EXPRESSION})	
	if(EXCLUDE)
		list(REMOVE_ITEM ${SOURCES_LIST} ${EXCLUDE})
	endif()
	set(${SOURCES_LIST} ${${SOURCES_LIST}} PARENT_SCOPE)
endfunction(exclude_from_list)

function(include_to_list SOURCES_LIST EXPRESSION)
	file(GLOB_RECURSE EXCLUDE ${EXPRESSION})	
	if(EXCLUDE)
		list(APPEND ${SOURCES_LIST} ${EXCLUDE})
	endif()
	set(${SOURCES_LIST} ${${SOURCES_LIST}} PARENT_SCOPE)
endfunction(include_to_list)


##############################################################
## Sources Filtering
##############################################################
file(GLOB_RECURSE SOURCES *.cpp)

if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
	exclude_from_list(SOURCES *_Mac.cpp)
	exclude_from_list(SOURCES *_Linux.cpp)
elseif(APPLE)
	exclude_from_list(SOURCES *_Win.cpp)
	exclude_from_list(SOURCES *_Linux.cpp)
	include_to_list(SOURCES *_Mac_Linux.cpp)
else()
	exclude_from_list(SOURCES *_Win.cpp)
	exclude_from_list(SOURCES *_Mac.cpp)
	include_to_list(SOURCES *_Linux_Mac.cpp)	
endif()
message(STATUS "SOURCES: " ${SOURCES})


##############################################################
## Build setting
##############################################################
if(MSVC)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Build)
	add_library(${TARGET_NAME} SHARED ${SOURCES})
	target_link_libraries(${TARGET_NAME} cppcore)
elseif(APPLE)
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Build/x64MacOSXRelease)
	add_library(${TARGET_NAME} SHARED ${SOURCES})
	target_compile_options(${TARGET_NAME} PUBLIC -fPIC)
	target_link_libraries(${TARGET_NAME} cppcore iconv)	
else()
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../Build/x64LinuxRelease)
	add_library(${TARGET_NAME} SHARED ${SOURCES})
	target_compile_options(${TARGET_NAME} PUBLIC -fPIC)
	target_link_libraries(${TARGET_NAME} cppcore pthread dl rt)	
endif()
