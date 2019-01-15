cmake_minimum_required(VERSION 3.5)

project(emusca C)

add_subdirectory(mbedtls EXCLUDE_FROM_ALL)

add_executable(aes-dec-test aes-dec-test.c)
target_link_libraries(aes-dec-test mbedcrypto)
target_include_directories(aes-dec-test PRIVATE ${CMAKE_SOURCE_DIR}/mbedtls/include)

set_target_properties(
    aes-dec-test
    PROPERTIES
    C_STANDARD 11
    C_EXTENSIONS ON
)
