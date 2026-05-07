cmake_minimum_required(VERSION 3.10)

# ============================================================
# STEP 1 — Toolchain MUST come before project()
# ============================================================
set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# CACHE STRING lets the -DCMAKE_C_COMPILER passed by VS Code override this
# default. A plain set() would shadow the -D value and break path detection.
set(CMAKE_C_COMPILER   arm-none-eabi-gcc CACHE STRING "C compiler")
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc CACHE STRING "ASM compiler")

# Prevents CMake from trying to link a test executable using the cross-compiler
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ============================================================
# STEP 2 — Project declaration (uses toolchain set above)
# ============================================================
project(AutoCooler C ASM)

# ============================================================
# Derive objcopy/size full paths from the resolved compiler location.
# Must be AFTER project() — that is when CMAKE_C_COMPILER is fully
# resolved to its absolute path (e.g. P:/Setup/Arm GNU/bin/arm-none-eabi-gcc.exe).
# Before project() it may still be the bare name, giving an empty directory.
get_filename_component(TOOLCHAIN_DIR "${CMAKE_C_COMPILER}" DIRECTORY)
if(TOOLCHAIN_DIR)
    set(CMAKE_OBJCOPY "${TOOLCHAIN_DIR}/arm-none-eabi-objcopy")
    set(CMAKE_SIZE    "${TOOLCHAIN_DIR}/arm-none-eabi-size")
else()
    set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
    set(CMAKE_SIZE    arm-none-eabi-size)
endif()
message(STATUS "Toolchain dir : ${TOOLCHAIN_DIR}")
message(STATUS "objcopy       : ${CMAKE_OBJCOPY}")

# ============================================================
# STEP 3 — MCU compiler/linker flags
# ============================================================
set(CPU_FLAGS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

# ============================================================
# STEP 4 — Source files  (explicit list — matches folder layout)
# ============================================================
set(SOURCES
    main/main.c
    adc/adc.c
    gpio/gpio.c
    lcd/lcd.c
    pwm/pwm.c
    rcc/rcc.c
    state_machine/state_machine.c
    timer/timer.c
    nvic/nvic.c

    # Startup assembly for STM32F407 — rename/replace if yours differs
    STM32F401xE.s
)

# ============================================================
# STEP 5 — Executable
# ============================================================
add_executable(${PROJECT_NAME}.elf ${SOURCES})

# ============================================================
# STEP 6 — Include paths
# Headers live next to their .c files, so add each subfolder.
# The root is also included so "adc/adc.h" style paths resolve.
# ============================================================
target_include_directories(${PROJECT_NAME}.elf PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/main
    ${CMAKE_SOURCE_DIR}/adc
    ${CMAKE_SOURCE_DIR}/gpio
    ${CMAKE_SOURCE_DIR}/lcd
    ${CMAKE_SOURCE_DIR}/pwm
    ${CMAKE_SOURCE_DIR}/rcc
    ${CMAKE_SOURCE_DIR}/state_machine
    ${CMAKE_SOURCE_DIR}/timer
    ${CMAKE_SOURCE_DIR}/nvic
)

# ============================================================
# STEP 7 — Compiler options
# ============================================================
target_compile_options(${PROJECT_NAME}.elf PRIVATE
    ${CPU_FLAGS}
    -std=c11
    -Wall
    -ffunction-sections
    -fdata-sections
    -fno-common
)

# ============================================================
# STEP 8 — Linker options
# Use STM32F407 linker script (NOT F401 — different flash/RAM sizes)
# STM32F407VG : 1 MB Flash @ 0x08000000 | 192 KB RAM @ 0x20000000
# STM32F401xE : 512 KB Flash            | 96 KB RAM   <- WRONG for this project
# ============================================================
target_link_options(${PROJECT_NAME}.elf PRIVATE
    ${CPU_FLAGS}

    # -L: tell the linker where to search for INCLUDE'd scripts (e.g. common.ld)
    # Must appear BEFORE -T so the search path is set before the script is opened.
    -L
    ${CMAKE_SOURCE_DIR}

    # Option A: self-contained F407 script — no common.ld needed
    # -T
    # ${CMAKE_SOURCE_DIR}/STM32F407VGTx_FLASH.ld

    # Option B: original F401 script — common.ld must be in the project root
    -T
    ${CMAKE_SOURCE_DIR}/STM32F401xE.ld

    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map
    --specs=nosys.specs
    --specs=nano.specs
    -lm
)

# ============================================================
# STEP 9 — Post-build: HEX for Proteus + size summary
# CMAKE_OBJCOPY is defined above — safe to use here now
# ============================================================
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex
            ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Generating HEX for Proteus"
)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_SIZE} --format=berkeley ${PROJECT_NAME}.elf
    COMMENT "ELF size"
)