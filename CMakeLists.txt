cmake_minimum_required(VERSION 3.7)
project(Serializer)

option(BUILD_TEST "Builds tests" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(ADDITIONAL_INCLUDES "")
set(ADDITIONAL_LIBRARIES "")

if(NOT EXISTS ${CMAKE_SOURCE_DIR}"/Vendor/googletest")
    message("Dependencies not available, running git submodule update")
    set(ARGS "submodule" "update" "--init" "--recursive")
    execute_process(COMMAND git ${ARGS} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

find_package(ZLIB)
if (ZLIB_FOUND)
    message("Using ZLIB ${ZLIB_VERSION_STRING}")
    add_definitions(-DWITH_ZLIB)
    set(ADDITIONAL_INCLUDES ${ADDITIONAL_INCLUDES} ${ZLIB_INCLUDE_DIRS})
    set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} ${ZLIB_LIBRARIES})
endif()

set(SOURCE_FILES Source/SBuffer.h Source/Serializer.h Source/Serializer.cpp)
include_directories(Source/ ${ADDITIONAL_INCLUDES})

add_library(Serializer SHARED ${SOURCE_FILES})
add_library(Serializer_Static STATIC ${SOURCE_FILES})
target_link_libraries(Serializer ${ADDITIONAL_LIBRARIES})
target_link_libraries(Serializer_Static ${ADDITIONAL_LIBRARIES})

if (BUILD_TEST)
    message("Building tests")
    add_subdirectory(Vendor/googletest)
    set(TEST_FILES Test/Test.cpp Test/Serialization.h)
    set(TEST_INCLUDES Vendor/googletest/googletest/include)
    add_executable(SerializerTest ${TEST_FILES})
    target_link_libraries(SerializerTest PUBLIC Serializer gtest_main)
    include_directories(${TEST_INCLUDES})
else()
    message("Building example")
    add_executable(SerializerExample Example/example.cpp)
    target_link_libraries(SerializerExample PUBLIC Serializer_Static)
endif()
