cmake_minimum_required(VERSION 3.5.0)
project(stm32-f103-exti)

enable_language(ASM)

IF(NOT CMAKE_CROSSCOMPILING)
    message(FATAL_ERROR "Cross compiling only. Please use -DCMAKE_TOOLCHAIN_FILE=/PATH/TO/TOOLCHAIN_FILE .")
ENDIF(NOT CMAKE_CROSSCOMPILING)

add_definitions(-DSTM32F1)

set(LIBOPENCM3_ROOT ${CMAKE_SOURCE_DIR}/lib/libopencm3)

#file(GLOB_RECURSE USER_SOURCES src/*.c)

include_directories(
        ${LIBOPENCM3_ROOT}/include
        inc)

link_directories(
        ${LIBOPENCM3_ROOT}/lib)

set(STM32F1_FLAGS "-mcpu=cortex-m3 -mthumb -mthumb-interwork -msoft-float")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall ${STM32F1_FLAGS} -Os -ffunction-sections -fdata-sections -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c++14 ${STM32F1_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/libopencm3.ld -lopencm3_stm32f1 -nostartfiles --specs=rdimon.specs -Wl,--gc-sections")


add_custom_target(libopencm3 make WORKING_DIRECTORY ${LIBOPENCM3_ROOT})

function(add_bin_from_elf bin elf)
    add_custom_target(${bin}
            ALL ${CMAKE_OBJCOPY}
            -Obinary ${elf} ${bin} DEPENDS ${elf})
endfunction(add_bin_from_elf)

add_subdirectory(src)
