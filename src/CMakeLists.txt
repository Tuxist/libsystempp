set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

add_definitions(-DBUILD_SHARED_LIB) 
add_definitions(-D_LIBCPP_DEBUG_LEVEL=2)
add_definitions(-D_LIBCPP_BUILDING_LIBRARY)

set(EVENT "EPOLL")

include(GenerateExportHeader)

set(systemppSrcs
    "connection.cpp"
    "exception.cpp"
    "utils.cpp"
    "uuid.cpp"
)

#os dependencies
message("Build with Linux Support")

list(APPEND linuxSrcs
    "linux/arch/${CMAKE_SYSTEM_PROCESSOR}/start.S"
    "linux/arch/${CMAKE_SYSTEM_PROCESSOR}/clone.S"
    "linux/arch/${CMAKE_SYSTEM_PROCESSOR}/cpuid.cpp"
    "linux/arch/${CMAKE_SYSTEM_PROCESSOR}/dso_handle.S"
    "linux/algorithm.cpp"
    "linux/any.cpp"
    "linux/assert.cpp"
    "linux/atomic.cpp"
    "linux/barrier.cpp"
    "linux/charconv.cpp"
    "linux/chrono.cpp"
    "linux/condition_variable.cpp"
    "linux/condition_variable_destructor.cpp"
    "linux/cstdio.cpp"
    "linux/ctrlhandler.cpp"
    "linux/debug.cpp"
    "linux/epoll.cpp"
    "linux/errno.cpp"
    "linux/exception.cpp"
    "linux/exec.cpp"
    "linux/format.cpp"
    "linux/functional.cpp"
    "linux/future.cpp"
    "linux/hash.cpp"
    "linux/ios.cpp"
    "linux/ios.instantiations.cpp"
    "linux/iostream.cpp"
    "linux/legacy_debug_handler.cpp"
    "linux/legacy_pointer_safety.cpp"
    "linux/locale.cpp"
    "linux/mutex.cpp"
    "linux/mutex_destructor.cpp"
    "linux/new.cpp"
    "linux/optional.cpp"
    "linux/random.cpp"
    "linux/regex.cpp"
    "linux/shared_mutex.cpp"
    "linux/signal.cpp"
    "linux/sleep.cpp"
    "linux/socket.cpp"
    "linux/stdexcept.cpp"
    "linux/string.cpp"
    "linux/strstream.cpp"
    "linux/sysinfo.cpp"
    "linux/sysprocess.cpp"
    "linux/system_error.cpp"
    "linux/thread.cpp"
    "linux/time.cpp"
    "linux/typeinfo.cpp"
    "linux/valarray.cpp"
    "linux/variant.cpp"
    "linux/vector.cpp"
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


install(TARGETS systempp cmd EXPORT systemppTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)


install(DIRECTORY include/systempp
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
