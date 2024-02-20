file(REMOVE_RECURSE
  "CDmessage.cdl.h"
  "CMode.cdl.h"
  "_headers_/traffic_light/CDmessage.cdl"
  "_headers_/traffic_light/CDmessage.cdl.h"
  "_headers_/traffic_light/CMode.cdl"
  "_headers_/traffic_light/CMode.cdl.h"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/traffic_light_cdl_files.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
