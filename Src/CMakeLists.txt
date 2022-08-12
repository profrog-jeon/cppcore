cmake_minimum_required(VERSION 2.8)
set(TARGET_NAME cppcore)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -O3 -fPIC")

if(APPLE)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../Build/MacOSXRelease)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../Build/MacOSXRelease)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../Build/MacOSXRelease)
else()
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../Build/LinuxRelease)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../Build/LinuxRelease)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../Build/LinuxRelease)
endif()

message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY: " ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY: " ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY: " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

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
## Source Filtering
##############################################################
# Glob all source files and filt-out later
file(GLOB_RECURSE SOURCES *.cpp *.c)

if(APPLE)
	exclude_from_list(SOURCES *_Win.cpp)
	exclude_from_list(SOURCES *_Linux.cpp)
	include_to_list(SOURCES *_Mac_Linux.cpp)
else()
	exclude_from_list(SOURCES *_Win.cpp)
	exclude_from_list(SOURCES *_Mac.cpp)
	include_to_list(SOURCES *_Linux_Mac.cpp)
endif()

foreach(SOURCE IN LISTS SOURCES)
	message(STATUS "SOURCE - " ${SOURCE})
endforeach(SOURCE)

##############################################################
## Build setting
##############################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Inc)
add_library(${TARGET_NAME}A OBJECT ${SOURCES})
add_library(${TARGET_NAME}W OBJECT ${SOURCES})
target_compile_options(${TARGET_NAME}A PUBLIC -fPIC -Wno-narrowing)
target_compile_options(${TARGET_NAME}W PUBLIC -fPIC -Wno-narrowing)
target_compile_definitions(${TARGET_NAME}W PUBLIC -DUNICODE PUBLIC -D_UNICODE)
add_library(${TARGET_NAME} STATIC $<TARGET_OBJECTS:cppcoreA> $<TARGET_OBJECTS:cppcoreW>)