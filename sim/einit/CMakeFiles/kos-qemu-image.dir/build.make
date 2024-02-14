# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/KasperskyOS-Community-Edition-1.1.1.40/toolchain/bin/cmake

# The command to remove a file.
RM = /opt/KasperskyOS-Community-Edition-1.1.1.40/toolchain/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim

# Utility rule file for kos-qemu-image.

# Include any custom commands dependencies for this target.
include einit/CMakeFiles/kos-qemu-image.dir/compiler_depend.make

# Include the progress variables for this target.
include einit/CMakeFiles/kos-qemu-image.dir/progress.make

einit/CMakeFiles/kos-qemu-image: einit/kos-qemu-image

einit/kos-qemu-image: diagnostics/Diagnostics
einit/kos-qemu-image: control_system/ControlSystem
einit/kos-qemu-image: lights_gpio/LightsGPIO
einit/kos-qemu-image: einit/EinitQemu
einit/kos-qemu-image: einit/EinitQemu-kss/ksm.module
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating KasperskyOS image: /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit/kos-qemu-image"
	cd /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit && /opt/KasperskyOS-Community-Edition-1.1.1.40/toolchain/bin/makeimg --target=aarch64-kos --with-extra-ldflags=-no-pie --sys-root=/opt/KasperskyOS-Community-Edition-1.1.1.40/sysroot-aarch64-kos --with-toolchain=/opt/KasperskyOS-Community-Edition-1.1.1.40/toolchain --with-python= --ldscript=/opt/KasperskyOS-Community-Edition-1.1.1.40/sysroot-aarch64-kos/../libexec/aarch64-kos/kos-qemu.ld --img-src=/opt/KasperskyOS-Community-Edition-1.1.1.40/sysroot-aarch64-kos/../libexec/aarch64-kos/kos-qemu --img-dst=/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit/kos-qemu-image --aboot_dtb_file= --aboot_mkboot= --aboot_config_file= --with-init=/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit/EinitQemu /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/diagnostics/Diagnostics /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/control_system/ControlSystem /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/lights_gpio/LightsGPIO /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit/EinitQemu-kss/ksm.module

einit/EinitQemu-kss/ksm.module: einit/EinitQemu-kss/src/security.psl
einit/EinitQemu-kss/ksm.module: einit/CMakeFiles/kos-qemu-image.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating EinitQemu-kss/ksm.module, EinitQemu-kss/security.psl.audit.c"
	cd /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit/EinitQemu-kss && /opt/KasperskyOS-Community-Edition-1.1.1.40/toolchain/bin/makekss --target=aarch64-kos --module=-lksm_kss --base="0xFFFFC00000000000" --with-cc="/opt/KasperskyOS-Community-Edition-1.1.1.40/toolchain/bin/aarch64-kos-gcc" --with-cflags="-g -I/opt/KasperskyOS-Community-Edition-1.1.1.40/sysroot-aarch64-kos/include -D__KOS_KERNEL__" --with-nk="/opt/KasperskyOS-Community-Edition-1.1.1.40/toolchain/bin/nk-psl-gen-c" --with-nkflags="-I /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_ -I /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/.. -I sim/../install/include -I /opt/KasperskyOS-Community-Edition-1.1.1.40/sysroot-aarch64-kos/include -I /opt/KasperskyOS-Community-Edition-1.1.1.40/toolchain/aarch64-kos/include -I /sysroot-aarch64-kos/include -I /opt/KasperskyOS-Community-Edition-1.1.1.40/toolchain/include -P /opt/KasperskyOS-Community-Edition-1.1.1.40/sysroot-aarch64-kos/include/system.platform -I /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/einit/src -I /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit/EinitQemu-kss -I /opt/KasperskyOS-Community-Edition-1.1.1.40/common" --with-nktype="psl" --psl-build-type="debug" --save-audit-to="/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit/EinitQemu-kss" --audit-output-to-console="n" /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit/EinitQemu-kss/src/security.psl

einit/EinitQemu-kss/security.psl.audit.c: einit/EinitQemu-kss/ksm.module
	@$(CMAKE_COMMAND) -E touch_nocreate einit/EinitQemu-kss/security.psl.audit.c

kos-qemu-image: einit/CMakeFiles/kos-qemu-image
kos-qemu-image: einit/EinitQemu-kss/ksm.module
kos-qemu-image: einit/EinitQemu-kss/security.psl.audit.c
kos-qemu-image: einit/kos-qemu-image
kos-qemu-image: einit/CMakeFiles/kos-qemu-image.dir/build.make
.PHONY : kos-qemu-image

# Rule to build all files generated by this target.
einit/CMakeFiles/kos-qemu-image.dir/build: kos-qemu-image
.PHONY : einit/CMakeFiles/kos-qemu-image.dir/build

einit/CMakeFiles/kos-qemu-image.dir/clean:
	cd /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit && $(CMAKE_COMMAND) -P CMakeFiles/kos-qemu-image.dir/cmake_clean.cmake
.PHONY : einit/CMakeFiles/kos-qemu-image.dir/clean

einit/CMakeFiles/kos-qemu-image.dir/depend:
	cd /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/einit /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit/CMakeFiles/kos-qemu-image.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : einit/CMakeFiles/kos-qemu-image.dir/depend

