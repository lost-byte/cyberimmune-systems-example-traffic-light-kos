file(REMOVE_RECURSE
  "IDiagnostics.idl.h"
  "IMode.idl.h"
  "_headers_/traffic_light/IDiagnostics.idl"
  "_headers_/traffic_light/IDiagnostics.idl.h"
  "_headers_/traffic_light/IMode.idl"
  "_headers_/traffic_light/IMode.idl.h"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/traffic_light_idl_files.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
