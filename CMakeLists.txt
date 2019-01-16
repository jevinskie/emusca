cmake_minimum_required(VERSION 3.5)

project(emusca C)

option(TARGET_ARM_BAREMETAL "Target ARM baremetal using arm-none-eabi-gcc toolchain" ON)

if(TARGET_ARM_BAREMETAL)
	set(CMAKE_EXECUTABLE_SUFFIX ".elf")
	set(CMAKE_TOOLCHAIN_FILE $ENV{KSDK_ROOT}/tools/cmake_toolchain_files/armgcc.cmake)
	set(CFLAGS -g -Os -mthumb -ffunction-sections -fdata-sections -DMBEDTLS_NO_PLATFORM_ENTROPY -UMBEDTLS_TIMING_C)
	set(LDFLAGS "-Wl,--gc-sections,-e,aes_do_dec_wrapped,-E")
	set(CMAKE_C_STANDARD_LIBRARIES "-Wl,--start-group -lgcc -lc_nano -lm -Wl,--end-group")
	set(ARCH_OPTS
		-mcpu=cortex-m4
		-mfpu=fpv4-sp-d16
		-mfloat-abi=hard
		-mthumb
	)
else()
	set(CFLAGS -g -Os)
	set(LDFLAGS -Wl,-dead_strip)
endif()

add_subdirectory(mbedtls EXCLUDE_FROM_ALL)

add_executable(aes-dec-test aes-dec-test.c)
target_link_libraries(aes-dec-test mbedcrypto)
target_include_directories(aes-dec-test PRIVATE ${CMAKE_SOURCE_DIR}/mbedtls/include)
target_compile_options(mbedcrypto PRIVATE ${CFLAGS})
target_compile_options(aes-dec-test PRIVATE ${CFLAGS})
set_target_properties(
    aes-dec-test
    PROPERTIES
    C_STANDARD 11
    C_EXTENSIONS ON
    LINK_FLAGS "${LDFLAGS}"
)
