cmake_minimum_required(VERSION 3.15)

set(ENV{OSAL_PORT_DIR} "example/osal_port_posix")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(../osal_port_posix build/osal_port_posix )
add_subdirectory(../../ build/osal)

add_executable(hello_world hello_word.cpp)
target_link_libraries(hello_world PRIVATE
    osal
    osal_port
)

add_executable(osal_test osal_test.cpp)
target_link_libraries(osal_test PRIVATE
    osal
    osal_port
)
