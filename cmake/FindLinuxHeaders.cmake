# Find the headers
set (LINUXHEADERS_DIR "/usr/src/linux-headers-${CMAKE_HOST_SYSTEM_VERSION}" CACHE STRING "Linux headers path common under debian")

find_path(LINUXHEADERS_DIR
    include/linux/mutex.h
  PATHS 
    ${LINUXHEADERS_DIR}
)

message(STATUS "Linux release: ${CMAKE_HOST_SYSTEM_VERSION}")
message(STATUS "Linux headers: ${LINUXHEADERS_DIR}")

if (EXISTS ${LINUXHEADERS_DIR})
  set(LINUXHEADERS_INCLUDE_DIRS
    ${LINUXHEADERS_DIR}
    ${LINUXHEADERS_DIR}/include
    ${LINUXHEADERS_DIR}/arch/x86/include/
    ${LINUXHEADERS_DIR}/arch/x86/include/generated/
  )
  set(LINUXHEADERS_FOUND 1 )
else()
  set(LINUXHEADERS_FOUND 0)
endif()

mark_as_advanced(LINUXHEADERS_FOUND)
 
