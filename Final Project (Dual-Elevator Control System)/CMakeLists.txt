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
project(ElevatorSystem C ASM)

# ============================================================
# Derive objcopy/size full paths from the resolved compiler location.
# Must be AFTER project() — that is when CMAKE_C_COMPILER is fully
# resolved to its absolute path.
get_filename_component(TOOLCHAIN_DIR "${CMAKE_C_COMPILER}" DIRECTORY)
if(TOOLCHAIN_DIR)
    set(CMAKE_OBJCOPY "${TOOLCHAIN_DIR}/arm-none-eabi-objcopy")
    set(CMAKE_SIZE    "${TOOLCHAIN_DIR}/arm-none-eabi-size")
else()
    set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
    set(CMAKE_SIZE    arm-none-eabi-size)
endif()

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
# STEP 4 — Source files
# ============================================================
set(SOURCES
    main/main.c
    communication/communication.c
    debounce/debounce.c
    diagnostics/diagnostics.c
    emergency/emergency.c
    exti/exti.c
    gpio/gpio.c
    interrupts/interrupts.c
    pwm/pwm.c
    queue/queue.c
    rcc/rcc.c
    scheduler/scheduler.c
    spi/spi.c
    state_machine/dispatcher.c
    state_machine/elevator_fsm.c
    timer/timer.c
    uart/uart.c
    STM32F401xE.s
)

# ============================================================
# STEP 5 — Create Master and Slave Targets
# ============================================================

# --- 5.1 MASTER TARGET ---
add_executable(MasterSystem ${SOURCES})
target_compile_definitions(MasterSystem PRIVATE MASTER_MCU)
set_target_properties(MasterSystem PROPERTIES OUTPUT_NAME "MasterSystem")

# --- 5.2 SLAVE TARGET ---
add_executable(SlaveSystem ${SOURCES})
target_compile_definitions(SlaveSystem PRIVATE SLAVE_MCU)
set_target_properties(SlaveSystem PROPERTIES OUTPUT_NAME "SlaveSystem")

# ============================================================
# STEP 6 — Common Configuration for both Targets
# ============================================================
set(TARGETS MasterSystem SlaveSystem)

foreach(TGT ${TARGETS})
    # Include paths
    target_include_directories(${TGT} PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/main
        ${CMAKE_SOURCE_DIR}/communication
        ${CMAKE_SOURCE_DIR}/debounce
        ${CMAKE_SOURCE_DIR}/diagnostics
        ${CMAKE_SOURCE_DIR}/emergency
        ${CMAKE_SOURCE_DIR}/exti
        ${CMAKE_SOURCE_DIR}/gpio
        ${CMAKE_SOURCE_DIR}/interrupts
        ${CMAKE_SOURCE_DIR}/pwm
        ${CMAKE_SOURCE_DIR}/queue
        ${CMAKE_SOURCE_DIR}/rcc
        ${CMAKE_SOURCE_DIR}/scheduler
        ${CMAKE_SOURCE_DIR}/spi
        ${CMAKE_SOURCE_DIR}/state_machine
        ${CMAKE_SOURCE_DIR}/timer
        ${CMAKE_SOURCE_DIR}/uart
    )

    # Compiler options
    target_compile_options(${TGT} PRIVATE
        ${CPU_FLAGS}
        -std=c11
        -Wall
        -ffunction-sections
        -fdata-sections
        -fno-common
    )

    # Linker options
    target_link_options(${TGT} PRIVATE
        ${CPU_FLAGS}
        -L ${CMAKE_SOURCE_DIR}
        -T ${CMAKE_SOURCE_DIR}/STM32F401xE.ld
        -Wl,--gc-sections
        -Wl,--no-warn-rwx-segments
        -Wl,-Map=${TGT}.map
        --specs=nosys.specs
        --specs=nano.specs
        -lm
    )

    # Post-build: HEX for Proteus + size summary
    add_custom_command(TARGET ${TGT} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${TGT} ${TGT}.hex
        COMMENT "Generating HEX for ${TGT}"
    )

    add_custom_command(TARGET ${TGT} POST_BUILD
        COMMAND ${CMAKE_SIZE} --format=berkeley ${TGT}
        COMMENT "ELF size for ${TGT}"
    )
endforeach()