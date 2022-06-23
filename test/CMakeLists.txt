include_directories(
    ${CMAKE_SOURCE_DIR}/src/include
)

add_executable (main main.cpp)
target_link_libraries(main systempp crt1)

add_executable (sysinfo sysinfo.cpp)
target_link_libraries(sysinfo systempp crt1)

add_executable (time time.cpp)
target_link_libraries(time systempp crt1)

add_executable (sleep sleep.cpp)
target_link_libraries(sleep systempp crt1)

add_executable (testc test.c)
target_link_libraries(testc systempp crt1)
