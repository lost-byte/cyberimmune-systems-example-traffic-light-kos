project (LightsGPIO)

# Tools for using NK parser.
include (platform/nk)

# Set compile flags.
project_header_default ("STANDARD_GNU_11:YES" "STRICT_WARNINGS:NO")

add_executable (LightsGPIO "src/lights_gpio.c")

#target_link_libraries (LightsGPIO ${gpio_CLIENT_LIB})
#if ("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm|aarch64")
#    target_link_libraries (LightsGPIO ${bsp_CLIENT_LIB})
#endif ()

add_dependencies (LightsGPIO traffic_light_lights_gpio_edl_files)
