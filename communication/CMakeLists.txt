cmake_minimum_required(VERSION 3.12)

# Toolchain for working with the NK-parser.
include (platform/nk)

# Set compiler flags.
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wno-deprecated-declarations")

add_executable (Communication "src/main.cpp" "src/subscriber.cpp" )
add_dependencies (Communication traffic_light_communication_edl_files)
target_link_libraries(Communication 
                      ${mosquitto_CXX_LIB}
                      ${vfs_CLIENT_LIB}
)

# We do not need the default VFS entity which comes from ${vfs_CLIENT_LIB}.
set_target_properties (Communication PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
set_target_properties (Communication PROPERTIES LINK_FLAGS ${VULN_LDFLAGS})
