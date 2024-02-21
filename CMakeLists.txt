cmake_minimum_required (VERSION 3.12)

project (traffic_light)

# Initialize CMake library for KasperskyOS SDK.
include (platform)
initialize_platform ()

# Add functions for installing artefacts.
include (platform/install)

# Tools for using NK parser.
include (platform/nk)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

nk_build_idl_files (traffic_light_idl_files 
                    NK_MODULE "traffic_light" 
                    IDL "resources/IMode.idl" 
                    "resources/IDiagnostics.idl"
                    "resources/ICSMode.idl"
                    "resources/IDiagComm.idl"
                    )

nk_build_cdl_files (traffic_light_cdl_files 
                    IDL_TARGET traffic_light_idl_files 
                    NK_MODULE "traffic_light" 
                    CDL "resources/CMode.cdl"
                    "resources/CDmessage.cdl"
                    "resources/CCSMode.cdl"
                    "resources/CDiagComm.cdl"
                    )

nk_build_edl_files (traffic_light_lights_gpio_edl_files 
                    CDL_TARGET traffic_light_cdl_files 
                    NK_MODULE "traffic_light" 
                    EDL "resources/LightsGPIO.edl")

nk_build_edl_files (traffic_light_control_system_edl_files 
                    CDL_TARGET traffic_light_cdl_files
                    NK_MODULE "traffic_light" 
                    EDL "resources/ControlSystem.edl")

nk_build_edl_files (traffic_light_diagnostics_edl_files 
                    CDL_TARGET traffic_light_cdl_files
                    NK_MODULE "traffic_light" 
                    EDL "resources/Diagnostics.edl")

nk_build_edl_files (traffic_light_communication_edl_files 
                    CDL_TARGET traffic_light_cdl_files
                    NK_MODULE "traffic_light" 
                    EDL "resources/Communication.edl")

# Add a package with the mosquitto component.
find_package (mosquitto REQUIRED COMPONENTS CXX_LIB)
include_directories (${mosquitto_INCLUDE})

# Add a package for working with the virtual file system.
find_package (vfs REQUIRED)
include_directories (${vfs_INCLUDE})
# Add a package with the dhcpcd program implementation.
find_package (rump REQUIRED COMPONENTS DHCPCD_ENTITY)
include_directories (${rump_INCLUDE})

# Add a package with the VFS program implementations
find_package (precompiled_vfs REQUIRED)
include_directories (${precompiled_vfs_INCLUDE})

find_package(nlohmann_json 3.2.0 REQUIRED)

# Choose the storage devices for the configuration files.
# If none of the options are selected, the ramfs device is used.
option (ROOTFS_SDCARD "Use SD card to mount root filesystem" OFF)
message (STATUS "Use SD card to mount root filesystem - ${ROOTFS_SDCARD}")

# Set mqtt broker ip address and port.
set (MQTT_BROKER_ADDRESS  10.0.2.2)
set (MQTT_BROKER_PORT     1883)

# Attach the directory with the generated config header files
include_directories (${CMAKE_CURRENT_BINARY_DIR})

# Attach the directories with programs.
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/resources/include)

# Recommended flags against vulnerabilities
set(VULN_LDFLAGS "\
    -Wl,-z,noexecstack \
    -Wl,-z,separate-code \
    -Wl,-z,now")
set(VULN_COMPILER_FLAGS "\
    -Wall -Wextra -Wconversion \
    -fPIE -pie -D_FORTIFY_SOURCE=2 -O2 \
    -fstack-protector-strong -fstack-clash-protection \
    -mbranch-protection=standard \
    -Wsign-conversion -Wformat=2 -Wformat-security -Werror=format-security \
    -fsanitize=undefined -fsanitize-undefined-trap-on-error")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${VULN_COMPILER_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VULN_COMPILER_FLAGS}")

add_subdirectory (communication)
add_subdirectory (diagnostics)
add_subdirectory (control_system)
add_subdirectory (lights_gpio)
add_subdirectory (einit)