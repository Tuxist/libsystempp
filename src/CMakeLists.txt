find_package(LinuxHeaders REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

include(GenerateExportHeader)

file(GLOB systemppSrcs "*.cpp" ".S")

file(GLOB libcSrcs "libc/*.cpp")

#os dependencies
if(${LINUXHEADERS_FOUND})
    message("Build with Linux Support")
    include_directories(
        ${LINUXHEADERS_INCLUDE_DIRS}
    )
    file(GLOB linuxSrcs "linux/*.cpp" "linux/*.S")
    list(APPEND systemppSrcs "linux/wrapper.c")
    list(APPEND systemppSrcs ${linuxSrcs})
endif()

include_directories(
  ${OPENSSL_INCLUDE_DIR}
)



add_library(systempp        SHARED ${systemppSrcs} )
add_library(systempp-static STATIC ${systemppSrcs} )

add_library(c SHARED ${libcSrcs})

generate_export_header(systempp)

target_link_libraries(systempp -e,"_start" -nostdlib)
target_link_libraries(systempp-static -e,"_start" -nostdlib)    

target_link_libraries(c -nostdlib)

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
