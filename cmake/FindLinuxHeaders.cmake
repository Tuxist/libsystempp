# Find the kernel release
execute_process(
  COMMAND uname -r
  OUTPUT_VARIABLE LINUX_RELEASE
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Find the headers
find_path(LINUXHEADERS_DIR
  include/linux/mutex.h
  PATHS /usr/src/linux-headers-${LINUX_RELEASE}
)

message(STATUS "Linux release: ${LINUX_RELEASE}")
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
 
