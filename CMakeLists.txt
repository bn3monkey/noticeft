cmake_minimum_required(VERSION 3.24)

project(NoticeFT VERSION 1.0 LANGUAGES CXX)

option(USING_FTP "Implment file transfer using ftp" ON)
option(BUILD_TESTS "Build Test application for testing notice ft" ON)


include(FetchContent)

FetchContent_Declare(
    OpenSSL
    GIT_REPOSITORY https://github.com/janbar/openssl-cmake.git
    GIT_TAG 1.1.1w-20231130
    OVERRIDE_FIND_PACKAGE
)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(OpenSSL)
unset(BUILD_SHARED_LIBS CACHE)

add_library(OpenSSL::SSL UNKNOWN IMPORTED)

# Need to change for supporting multi-platform
set_target_properties(OpenSSL::SSL PROPERTIES
    IMPORTED_LOCATION "${openssl_BINARY_DIR}/ssl/libssl-1_1.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${openssl_BINARY_DIR}/include"
)

# Need to change for supporting multi-platform
add_library(OpenSSL::Crypto UNKNOWN IMPORTED)
set_target_properties(OpenSSL::Crypto PROPERTIES
    IMPORTED_LOCATION "${openssl_BINARY_DIR}/crypto/libcrypto-1_1.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${openssl_BINARY_DIR}/include"
)


FetchContent_Declare(
    curl
    GIT_REPOSITORY https://github.com/curl/curl.git
    GIT_TAG        curl-8_11_1 # 원하는 버전 태그
)

set(CURL_USE_OPENSSL ON)
set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(curl)

unset(BUILD_STATIC_LIBS CACHE)

SET(NOTICEFT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
SET(NOTICEFT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB_RECURSE NOTICEFT_SOURCE_FILES
    "${NOTICEFT_SOURCE_DIR}/*.cpp"
    "${NOTICEFT_SOURCE_DIR}/*.hpp"
    "${NOTICEFT_SOURCE_DIR}/*.h"
)

file(GLOB_RECURSE NOTICEFT_INCLUDE_FILES
    "${NOTICEFT_INCLUDE_DIR}/*.hpp"
    "${NOTICEFT_INCLUDE_DIR}/*.h"
)

message(STATUS "NOTICEFT_SOURCE_FILES : ${NOTICEFT_SOURCE_FILES}")
message(STATUS "NOTICEFT_INCLUDE_FILES : ${NOTICEFT_INCLUDE_FILES}")


add_library(
    alimftp
    SHARED
    ${NOTICEFT_SOURCE_FILES}
    ${NOTICEFT_INCLUDE_FILES}
)

target_include_directories(alimftp PRIVATE ${NOTICEFT_INCLUDE_DIR} ${curl_SOURCE_DIR}/include)
target_link_libraries(alimftp PRIVATE CURL::libcurl_static)
set_target_properties(alimftp PROPERTIES PUBLIC_HEADER ${NOTICEFT_INCLUDE_FILES})
target_compile_definitions(alimftp PUBLIC ALIMFTP_EXPORTS)

if (USING_FTP)
    target_compile_definitions(alimftp PRIVATE ALIMFTP_USING_FTP)
endif(USING_FTP)

install(TARGETS alimftp
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include)

install(TARGETS alimftp
    LIBRARY DESTINATION "E:/xHarbour/noticeft"
    ARCHIVE DESTINATION "E:/xHarbour/noticeft"
    RUNTIME DESTINATION "E:/xHarbour/noticeft"
    PUBLIC_HEADER DESTINATION include)

if (BUILD_TESTS)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1 # 원하는 버전
    )
    FetchContent_MakeAvailable(googletest)

    SET(NOTICEFT_TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
    file(GLOB_RECURSE NOTICEFT_TEST_SOURCES "${NOTICEFT_TEST_DIR}/*.cpp")

    add_executable(alimftp_tests 
        ${NOTICEFT_TEST_SOURCES})

    target_include_directories(alimftp_tests PRIVATE
        ${NOTICEFT_INCLUDE_DIR}
        ${gtest_SOURCE_DIR}/include
        ${gmock_SOURCE_DIR}/include)

    target_link_libraries(alimftp_tests
    alimftp
        gtest
        gtest_main)

    add_custom_command(
        TARGET alimftp_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${NOTICEFT_TEST_DIR}/resource" "${CMAKE_BINARY_DIR}/resource"
        COMMENT "Copying resource folder to binary directory"
    )

    enable_testing()
    add_test(NAME NoticeFTTests COMMAND alimftp_tests)

endif(BUILD_TESTS)