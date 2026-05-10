add_library(my_lib STATIC my_interrupts.c oled.c
    snake_logic.c
    include/snake_logic.h)

target_link_libraries(my_lib
    PRIVATE
        board
        device
        drivers
        freertos_kernel
)

target_include_directories(my_lib
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)