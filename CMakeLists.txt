cmake_minimum_required(VERSION 3.7)
project(Serializer)

option(BUILD_TEST "Builds tests" OFF)

set(CMAKE_CXX_STANDARD 11)

if(NOT EXISTS "Vendor/libarchive")
    message("Dependencies not available, running git submodule update")
    set(ARGS "submodule" "update" "--init" "--recursive")
    execute_process(COMMAND git ${ARGS} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()


add_subdirectory(Vendor/libarchive)

set(SOURCE_FILES Source/SBuffer.h Source/Serializer.h Source/Serializer.cpp)

add_library(Serializer SHARED ${SOURCE_FILES})
add_library(Serializer_Static STATIC ${SOURCE_FILES})
target_link_libraries(Serializer archive_static)
target_include_directories(Serializer PUBLIC Vendor/libarchive/libarchive)

if (BUILD_TEST)
    message("Building tests")
    add_subdirectory(Vendor/googletest)
    set(TEST_FILES Test/Test.cpp Test/Serialization.h)
    set(TEST_INCLUDES Vendor/googletest/googletest/include)
    add_executable(SerializerTest ${TEST_FILES})
    target_link_libraries(SerializerTest PUBLIC Serializer gtest_main)
    target_include_directories(SerializerTest PUBLIC ${TEST_INCLUDES} Source/)
else()
    message("Building example")
    add_executable(SerializerExample Example/example.cpp)
    target_link_libraries(SerializerExample PUBLIC Serializer)
    target_include_directories(SerializerExample PUBLIC Source/)
endif()
