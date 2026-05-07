cmake_minimum_required(VERSION 3.10)
project(STM32_Project C ASM)

# 1. Compiler Flags
set(CPU_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPU_FLAGS} -Wall -fdata-sections -ffunction-sections")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${CPU_FLAGS} -x assembler-with-cpp -I\"${CMAKE_CURRENT_SOURCE_DIR}\" -I\"${CMAKE_CURRENT_SOURCE_DIR}/src\" -I\"${CMAKE_CURRENT_SOURCE_DIR}/include\"")

# 2. Linker Settings
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${CPU_FLAGS} -Wl,--gc-sections -L\"${CMAKE_CURRENT_SOURCE_DIR}\" --specs=nosys.specs --specs=nano.specs -T \"${CMAKE_CURRENT_SOURCE_DIR}/STM32F401xE.ld\"")

# 3. Include Directories
include_directories(include)

# 4. Source Files
file(GLOB SOURCES "src/*.c")
list(APPEND SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/STM32F401xE.s")

# 5. Create the ELF executable
add_executable(STM32_Project.elf ${SOURCES})

# 6. Generate the HEX file for Proteus (THIS WAS THE BROKEN PART)
# Generate the HEX file for Proteus using the full path
add_custom_command(TARGET STM32_Project.elf POST_BUILD
    COMMAND "${CMAKE_OBJCOPY}" -O ihex STM32_Project.elf STM32_Project.hex
    COMMENT "Generating STM32_Project.hex for Proteus"
)