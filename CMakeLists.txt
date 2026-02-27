cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME SmartFramZET6)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    Core/App/Tasks/BleTask.c
    Core/BSP/oled/oled.c
    Core/BSP/oled/font.c
    Core/App/global/screen.c
    Core/App/global/screen.h
    Core/App/Tasks/ScreenTask.c
    Core/BSP/knob/knob.c
    Core/BSP/knob/knob.h
    Core/BSP/key/key.c
    Core/BSP/key/key.h
    Core/App/Tasks/InputTask.c
    Core/App/global/farmState.c
    Core/App/global/farmState.h
    Core/App/utils.c
    Core/App/utils.h
    Core/BSP/light/light.h
    Core/BSP/light/light.c
    Core/BSP/light/light_i2c.c
    Core/BSP/light/light_i2c.h
    Core/App/Tasks/SensorTask.c
    Core/BSP/soil_moisture/soil_moisture.c
    Core/BSP/soil_moisture/soil_moisture.h
    Core/BSP/rain/rain.c
    Core/BSP/rain/rain.h
    Core/BSP/pump/pump.c
    Core/BSP/pump/pump.h
    Core/App/Tasks/BeepTimer.c
    Core/App/Tasks/BeepTimer.h
    Core/App/global/adc_buffer.c
    Core/App/global/adc_buffer.h
    Core/BSP/aht20/aht20.c
    Core/BSP/aht20/aht20.h
    Core/BSP/BMP280/BMP280.c
    Core/BSP/BMP280/BMP280.h
    Core/App/StopModeRtc.c
    Core/App/StopModeRtc.h
    Core/BSP/debug_log/debug_log.c
    Core/BSP/debug_log/debug_log.h
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    Core/BSP/key
    Core/BSP/knob
    Core/BSP/oled
    Core/BSP/light
    Core/BSP/soil_moisture
    Core/BSP/rain
    Core/BSP/pump
    Core/BSP/aht20
    Core/BSP/BMP280
    Core/BSP/debug_log
    Core/App
    Core/App/Tasks
    Core/Middlewares
    Core/App/global
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries

)
