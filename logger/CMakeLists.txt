project (logger)

# Toolchain for working with the NK-parser.
include (platform/nk)

# Set compiler flags.
project_header_default ("STANDARD_GNU_11:YES" "STRICT_WARNINGS:NO")

include_directories(${CMAKE_SOURCE_DIR}/resources/include)
add_executable (Logger "src/logger.c")
target_link_libraries (Logger ${vfs_CLIENT_LIB})
add_dependencies (Logger traffic_light_idl_files traffic_light_logger_edl_files)

# We do not need default VFS entity here, which comes from ${vfs_CLIENT_LIB}
set_target_properties (Logger PROPERTIES ${vfs_ENTITY}_REPLACEMENT "")
set_target_properties (Logger PROPERTIES LINK_FLAGS ${VULN_LDFLAGS})
