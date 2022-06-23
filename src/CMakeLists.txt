set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

if(BUILD_SHARED_LIBS)
     add_definitions(-DBUILD_SHARED_LIB) 
endif()

set(EVENT "EPOLL")

include(GenerateExportHeader)

set(systemppSrcs
    "connection.cpp"
    "exception.cpp"
    "utils.cpp"
    "uuid.cpp"
    "string.cpp"
)

#os dependencies
message("Build with Linux Support")

list(APPEND crtSrcs

)

add_library(crt1 STATIC
    "linux/arch/${CMAKE_SYSTEM_PROCESSOR}/crtbegin.S"
    "linux/arch/${CMAKE_SYSTEM_PROCESSOR}/dso_handle.S"
    "linux/arch/${CMAKE_SYSTEM_PROCESSOR}/crti.S"
    "linux/arch/${CMAKE_SYSTEM_PROCESSOR}/crtn.S"
)

list(APPEND linuxSrcs
    "linux/alloc.cpp"
    "linux/arch/${CMAKE_SYSTEM_PROCESSOR}/clone.S"
    "linux/arch/${CMAKE_SYSTEM_PROCESSOR}/cpuid.cpp"
    "linux/console.cpp"
    "linux/cxa_exception.cpp"
    "linux/cxa_abi.cpp"
    "linux/errno.cpp"
    "linux/event.cpp"
    "linux/exec.cpp"
    "linux/file.cpp"
    "linux/mutex.cpp"
    "linux/signal.cpp"  
    "linux/socket.cpp"   
    "linux/sysinfo.cpp"    
    "linux/thread.cpp" 
    "linux/unwind.cpp"
    "linux/sleep.cpp"   
    "linux/sysprocess.cpp"  
    "linux/time.cpp"
    "linux/terminate.cpp"
    "linux/wchar.cpp"
)

configure_file(
    linux/arch/${CMAKE_SYSTEM_PROCESSOR}/syscall.h.in ${CMAKE_CURRENT_BINARY_DIR}/syscall.h
)

configure_file(
    linux/arch/${CMAKE_SYSTEM_PROCESSOR}/sysbits.h.in ${CMAKE_CURRENT_BINARY_DIR}/sysbits.h
)

list(APPEND systemppSrcs ${linuxSrcs} ${linuxstdSrcs})

install(FILES
        ${CMAKE_BINARY_DIR}/src/syscall.h
        ${CMAKE_BINARY_DIR}/src/sysbits.h
        ${CMAKE_BINARY_DIR}/config.h
        DESTINATION include/systempp
        COMPONENT Devel
)


add_library(systempp SHARED ${systemppSrcs} )

target_link_libraries(systempp)

add_library(
    cmd STATIC libcmd/cmd.cpp
)

generate_export_header(systempp)

set_property(TARGET systempp PROPERTY CMAKE_POSITION_INDEPENDENT_CODE ON)

target_link_libraries(cmd systempp)

SET(CMAKE_INSTALL_LIBDIR lib CACHE PATH "Output directory for libraries")

set_property(TARGET systempp PROPERTY VERSION ${Upstream_VERSION})
set_property(TARGET systempp PROPERTY SOVERSION ${LIBV})
set_property(TARGET systempp PROPERTY INTERFACE_httppp_MAJOR_VERSION ${LIBV})
set_property(TARGET systempp APPEND PROPERTY COMPATIBLE_INTERFACE_STRING httppp_MAJOR_VERSION)

set(LIBCXX_GENERATED_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include/systempp/c++")
set(LIBCXX_GENERATED_INCLUDE_TARGET_DIR ${LIBCXX_GENERATED_INCLUDE_DIR})


install(TARGETS systempp cmd crt1 EXPORT systemppTargets
    LIBRARY DESTINATION lib
    OBJECTS DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)


install(DIRECTORY include/systempp
        DESTINATION include
        COMPONENT Devel
)

install(FILES 
            include/stddef.h
            include/string.h
            include/time.h
            include/features.h
            include/atomic.h
            include/pthread.h
            include/errno.h
            include/wchar.h
            include/stdio.h
            include/stdlib.h
            include/stdint.h
            include/math.h
        DESTINATION include
        COMPONENT Devel
)

install(FILES libcmd/cmd.h
        DESTINATION include/libcmd
        COMPONENT Devel
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/systempp/systemppConfigVersion.cmake"
  VERSION ${Upstream_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT systemppTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/systempp/systemppTargets.cmake"
  NAMESPACE Upstream::
)
configure_file(cmake/systemppConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/systempp/systemppConfig.cmake"
  COPYONLY
)

set(ConfigPackageLocation lib/cmake/systempp)
install(EXPORT systemppTargets
  FILE
    systemppTargets.cmake
  NAMESPACE
    Upstream::
  DESTINATION
    ${ConfigPackageLocation}
)

install(
  FILES
    cmake/systemppConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/systempp/systemppConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)
