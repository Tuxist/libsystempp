set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(LD_LLD lld)

project(libsystempp CXX ASM )
cmake_minimum_required(VERSION 2.6)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

enable_testing ()

set(LIBV "1.0.0")
set(Upstream_VERSION 1.0.0)

find_package(Clang REQUIRED)

set(CMAKE_CXX_FLAGS 
    "-fPIC\
    ${CMAKE_CXX_FLAGS} \
    -Wall \
    -nostdlib \
    -nodefaultlibs \
    -isysroot${CMAKE_SOURCE_DIR}/src/include"
)

set(CMAKE_C_FLAGS 
    "-fPIC\
    ${CMAKE_C_FLAGS} \
    -nostdlib \
    -nodefaultlibs\
    -Wall \
    -isysroot${CMAKE_SOURCE_DIR}/src/include"
)



set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}  ${CMAKE_SOURCE_DIR}/cmake/)

set(CAN_USE_ASSEMBLER TRUE)
set(ASM_OPTIONS "-x assembler-with-cpp")
set(CMAKE_ASM_FLAGS "${CXXFLAGS} ${ASM_OPTIONS}" )

configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

include_directories(
    ${CMAKE_SOURCE_DIR}/src/include/
    ${CLANG_INCLUDE_DIRS}
    ${CMAKE_BINARY_DIR}
)

add_definitions(
    ${CLANG_DEFINITIONS}
)

add_subdirectory(doc)
add_subdirectory(src)
add_subdirectory(test)
#add_subdirectory(examples)
