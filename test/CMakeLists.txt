include_directories(
    ${CMAKE_SOURCE_DIR}/src/include
)

add_executable (main main.cpp)

target_link_libraries(main systempp-static)

#add_executable (iostream iostream.cpp)

#target_link_libraries(iostream systempp-static -nostdlib)
