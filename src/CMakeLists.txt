set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

IF(BUILD_SHARED_LIBS)
    add_definitions(-DBUILD_SHARED)
ENDIF()

include(GenerateExportHeader)

file(GLOB currentSrcs "*.cpp")
list(APPEND systemppSrcs ${currentSrcs})

file(GLOB currentSrcs "std/*.cpp")
list(APPEND systemppSrcs ${currentSrcs})

list(APPEND systemppSrcs "confdb/confdb.cpp")

include_directories(
    ${CMAKE_SOURCE_DIR}/src/include
)

#os dependencies
message("Build with Linux Support")
include_directories(
    "linux/include"
)
file(GLOB linuxSrcs 
    "linux/arch/${CMAKE_SYSTEM_PROCESSOR}/*.S"
    "linux/*.cpp"
    "linux/*.c"
)
configure_file(
    linux/arch/${CMAKE_SYSTEM_PROCESSOR}/syscall.h.in ${CMAKE_CURRENT_BINARY_DIR}/syscall.h
)

configure_file(
    linux/arch/${CMAKE_SYSTEM_PROCESSOR}/sysbits.h.in ${CMAKE_CURRENT_BINARY_DIR}/sysbits.h
)

list(APPEND systemppSrcs ${linuxSrcs})

install(FILES
        ${CMAKE_BINARY_DIR}/src/syscall.h
        ${CMAKE_BINARY_DIR}/src/sysbits.h
        DESTINATION include/systempp
        COMPONENT Devel
)


add_library(systempp        SHARED ${systemppSrcs} )
add_library(systempp-static STATIC ${systemppSrcs} )

add_executable(confdbd "confdb/confdbd.cpp")
add_executable(confcli "confdb/confcli.cpp")

generate_export_header(systempp)

set_property(TARGET systempp PROPERTY CMAKE_POSITION_INDEPENDENT_CODE ON)

target_link_libraries(systempp)
target_link_libraries(systempp-static)    

target_link_libraries(confdbd systempp-static)
target_link_libraries(confcli systempp-static)

SET(CMAKE_INSTALL_LIBDIR lib CACHE PATH "Output directory for libraries")

set_property(TARGET systempp PROPERTY VERSION ${Upstream_VERSION})
set_property(TARGET systempp PROPERTY SOVERSION ${LIBV})
set_property(TARGET systempp PROPERTY INTERFACE_httppp_MAJOR_VERSION ${LIBV})
set_property(TARGET systempp APPEND PROPERTY COMPATIBLE_INTERFACE_STRING httppp_MAJOR_VERSION)

install(TARGETS systempp EXPORT systemppTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(TARGETS systempp-static EXPORT systempp-staticTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(DIRECTORY include/
        DESTINATION include/systempp
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
