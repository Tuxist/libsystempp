project(libsystempp CXX ASM)
cmake_minimum_required(VERSION 2.6)
enable_testing ()

set(LIBV "1.0.0")
set(Upstream_VERSION 1.0.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -nodefaultlibs -nostdlib")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -nodefaultlibs -nostdlib")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}  ${CMAKE_SOURCE_DIR}/cmake/)

set(CAN_USE_ASSEMBLER TRUE)
set(ASM_OPTIONS "-x assembler-with-cpp")
set(CMAKE_ASM_FLAGS "${CXXFLAGS} ${ASM_OPTIONS}" )

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(${CMAKE_BINARY_DIR})



add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(test)
