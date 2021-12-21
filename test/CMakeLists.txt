include_directories(
    ${CMAKE_SOURCE_DIR}/src/include
)

add_executable (main main.cpp)
target_link_libraries(main systempp)

add_executable (console console.cpp)
target_link_libraries(console systempp)

add_executable (sysinfo sysinfo.cpp)
target_link_libraries(sysinfo systempp)

add_executable (thread thread.cpp)
target_link_libraries(thread systempp)
