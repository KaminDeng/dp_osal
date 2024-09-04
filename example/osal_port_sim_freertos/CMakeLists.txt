cmake_minimum_required(VERSION 3.15)

file(GLOB_RECURSE OSAL_PORT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(osal_port STATIC
        ${OSAL_PORT_SOURCES}
)


# 遍历子目录以确保包含所有头文件
foreach (subdir
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/cmsis_os2_freertos/cmsis_base
        ${CMAKE_CURRENT_LIST_DIR}/cmsis_os2_freertos
)
    target_include_directories(osal_port PUBLIC ${subdir})
endforeach ()

target_link_libraries(osal_port PUBLIC freertos_kernel freertos_config)

