include_directories(
    ${CMAKE_SOURCE_DIR}/src/include
)

add_executable (main main.cpp)

target_link_libraries(main systempp)
