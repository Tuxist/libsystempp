project(libsystempp CXX ASM)
cmake_minimum_required(VERSION 2.6)
enable_testing ()

set(LIBV "1.0.0")
set(Upstream_VERSION 1.0.0)

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
  if( CMAKE_CXX_COMPILER_ID MATCHES GNU )
     SET(CMAKE_CXX_FLAGS "-g -Wall")
  endif( CMAKE_CXX_COMPILER_ID MATCHES GNU )
  if(CMAKE_CXX_COMPILER_ID MATCHES ".*clang")
    SET(CMAKE_CXX_FLAGS "-g -Wthread-safety -Wall")
  endif( CMAKE_CXX_COMPILER_ID MATCHES ".*clang" )
ELSE(CMAKE_BUILD_TYPE MATCHES DEBUG )
  if( CMAKE_CXX_COMPILER_ID MATCHES GNU )
    SET(CMAKE_CXX_FLAGS "-O0 -Wall")
  endif( CMAKE_CXX_COMPILER_ID MATCHES GNU )
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}  ${CMAKE_SOURCE_DIR}/cmake/)

IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
  list(APPEND ${CMAKE_CXX_FLAGS} "-msse2")
  add_definitions( -Dx86_64)
ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )

set(CAN_USE_ASSEMBLER TRUE)
set(ASM_OPTIONS "-x assembler-with-cpp")
set(CMAKE_ASM_FLAGS "${CFLAGS} ${ASM_OPTIONS}" )

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(test)
