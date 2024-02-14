# Install script for directory: /home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "sim/../install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/opt/KasperskyOS-Community-Edition-1.1.1.40/toolchain/bin/aarch64-kos-objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnk_headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/traffic_light" TYPE FILE FILES
    "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/../traffic_light/IMode.idl"
    "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/../traffic_light/IMode.idl.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnk_headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/traffic_light" TYPE FILE FILES
    "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/../traffic_light/IDiagnostics.idl"
    "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/../traffic_light/IDiagnostics.idl.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnk_headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/traffic_light" TYPE FILE FILES
    "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/../traffic_light/CMode.cdl"
    "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/../traffic_light/CMode.cdl.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnk_headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/traffic_light" TYPE FILE FILES
    "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/../traffic_light/LightsGPIO.edl"
    "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/../traffic_light/LightsGPIO.edl.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnk_headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/traffic_light" TYPE FILE FILES
    "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/../traffic_light/ControlSystem.edl"
    "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/../traffic_light/ControlSystem.edl.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xnk_headersx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/traffic_light" TYPE FILE FILES
    "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/../traffic_light/Diagnostics.edl"
    "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/_headers_/traffic_light/../traffic_light/Diagnostics.edl.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/control_system/cmake_install.cmake")
  include("/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/lights_gpio/cmake_install.cmake")
  include("/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/diagnostics/cmake_install.cmake")
  include("/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/einit/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/master/Development/KasperskyOS/traffic-light-kos/myfork/cyberimmune-systems-example-traffic-light-kos/sim/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
