cmake_minimum_required(VERSION 3.12)

# Toolchain for working with the NK-parser.
include (platform/nk)

# Set compiler flags.
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wno-deprecated-declarations")

find_package(nlohmann_json 3.2.0 REQUIRED)

#include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/src/nlohmann)
add_executable (Communication "src/main.cpp" "src/mqtt-comm.cpp")

# target_include_directories(
#                 Communication
#                 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/nlohmann
#                 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/nlohmann/detail
#                 )


add_dependencies (Communication traffic_light_communication_edl_files)
target_link_libraries(Communication 
                      ${mosquitto_CXX_LIB}
                      ${vfs_CLIENT_LIB}
                      nlohmann_json::nlohmann_json
)



# We do not need the default VFS entity which comes from ${vfs_CLIENT_LIB}.
set_target_properties (Communication PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
set_target_properties (Communication PROPERTIES LINK_FLAGS ${VULN_LDFLAGS})

