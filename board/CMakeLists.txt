add_library(board STATIC)

target_sources(board
    PRIVATE
        clock_config.c
        peripherals.c
        pin_mux.c
)

target_include_directories(board
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include/drivers
)

target_link_libraries(board 
    PUBLIC 
        device
        drivers
)