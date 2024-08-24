cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(SYSTEM_IMPL "CMSIS_OS")
set(SYSTEM_IMPL "POSIX")

add_compile_options(
        $<IF:$<STREQUAL:${SYSTEM_IMPL},CMSIS_OS>,-DTEST_CONFIG_CMSIS_OS,>
        $<IF:$<STREQUAL:${SYSTEM_IMPL},POSIX>,-DTEST_CONFIG_POSIX,>
)

set(PORT_PATH "port")
set(TEST_SUITE_PATH "test")
set(OSAL_IMPL_PATH "src/impl")


file(GLOB_RECURSE PORT_SOURCES "${PORT_PATH}/*.c" "${PORT_PATH}/*.cpp")
file(GLOB_RECURSE TEST_SUITE_SOURCES "${TEST_SUITE_PATH}/*.cpp")

if (SYSTEM_IMPL STREQUAL "POSIX")
    file(GLOB_RECURSE SYSTEM_IMPL_SOURCES "${OSAL_IMPL_PATH}/posix/*.cpp")
elseif (SYSTEM_IMPL STREQUAL "CMSIS_OS")
    file(GLOB_RECURSE SYSTEM_IMPL_SOURCES "${OSAL_IMPL_PATH}/cmsis_os/*.cpp")
else ()
    message(FATAL_ERROR "Unsupported SYSTEM_IMPL: ${SYSTEM_IMPL}")
endif ()

# 添加库
add_library(osal_lib STATIC
        ${TEST_SUITE_SOURCES}
        ${PORT_SOURCES}
        ${SYSTEM_IMPL_SOURCES}
)

# 遍历子目录以确保包含所有头文件
foreach (subdir
        src/interface
        ${PORT_PATH}
        ${PORT_PATH}/cmsis_os2_freertos
        ${PORT_PATH}/cmsis_os2_freertos/cmsis_base
        $<IF:$<STREQUAL:${SYSTEM_IMPL},CMSIS_OS>,src/impl/cmsis_os/include,>
        $<IF:$<STREQUAL:${SYSTEM_IMPL},POSIX>,src/impl/posix/include,>

)
    target_include_directories(osal_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${subdir})
endforeach ()

target_link_libraries(osal_lib freertos_kernel freertos_config)
