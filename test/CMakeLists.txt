include_directories(
    ${CMAKE_SOURCE_DIR}/src/include
)

add_executable (main main.cpp)
target_link_libraries(main systempp)

add_executable (sysinfo sysinfo.cpp)
target_link_libraries(sysinfo systempp)

add_executable (time time.cpp)
target_link_libraries(time systempp)
