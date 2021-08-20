include_directories(
    ${CMAKE_SOURCE_DIR}/src
)

add_executable (main main.cpp)

target_link_libraries(main systempp-static -nostdlib)

#add_executable (iostream iostream.cpp)

#target_link_libraries(iostream systempp-static -nostdlib)
