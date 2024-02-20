file(REMOVE_RECURSE
  "Communication.edl.h"
  "_headers_/traffic_light/Communication.edl"
  "_headers_/traffic_light/Communication.edl.h"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/traffic_light_communication_edl_files.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
