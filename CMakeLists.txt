cmake_minimum_required(VERSION 3.23)

project(app)

set(configTASK_NOTIFICATION_ARRAY_ENTRIES 2)
set(FREERTOS_PORT GCC_ARM_CM33_NTZ_NONSECURE CACHE STRING "")
set(FREERTOS_HEAP 4)
set(ARM_FLAGS -mcpu=cortex-m33 -mthumb)
add_compile_definitions(CPU_MCXN947VDF_cm33_core0)
add_compile_options(-g -O0 ${ARM_FLAGS})

add_executable(${PROJECT_NAME} "src/main.c")

add_subdirectory(include/drivers)
add_subdirectory(include/device)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/board)
add_subdirectory(src/startup)
add_subdirectory(include/freertos_kernel)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        device
        drivers
        board
        startup
        freertos_kernel
        my_lib
)

target_link_options(${PROJECT_NAME}
    PRIVATE 
        "-T${CMAKE_SOURCE_DIR}/src/startup/startup_MCXN947_linker.ld"
        ${ARM_FLAGS}
        --specs=nosys.specs
        --specs=nano.specs
)