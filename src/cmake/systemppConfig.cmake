include("${CMAKE_CURRENT_LIST_DIR}/systemppTargets.cmake")

get_filename_component(systempp_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
if(EXISTS "${systempp_CMAKE_DIR}/CMakeCache.txt")
  # In build tree
  include("${systempp_CMAKE_DIR}/systemppBuildTreeSettings.cmake")
else()
  set(systempp_INCLUDE_DIRS "${systempp_CMAKE_DIR}/@CONF_REL_INCLUDE_DIR@")
endif()

set(systempp_LIBRARIES systempp)
set(systempp_INCLUDE_DIR "systempp")
