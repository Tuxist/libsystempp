include_directories(
    ${CMAKE_SOURCE_DIR}/src/include
)

add_subdirectory(libc)

add_executable (main main.cpp)
target_link_libraries(main systempp)

add_executable (sysinfo sysinfo.cpp)
target_link_libraries(sysinfo systempp)
