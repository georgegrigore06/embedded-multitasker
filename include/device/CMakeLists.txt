add_library(device STATIC)

target_sources(device
    PRIVATE
        system_MCXN947_cm33_core0.c
)

target_link_libraries(device
    PUBLIC
        drivers
)

target_include_directories(device
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/periph
        ${CMAKE_SOURCE_DIR}/include/CMSIS
        ${CMAKE_SOURCE_DIR}/include/drivers
)