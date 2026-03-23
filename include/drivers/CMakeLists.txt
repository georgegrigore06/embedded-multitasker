add_library(drivers STATIC)

target_sources(drivers
    PRIVATE
        fsl_clock.c
        fsl_common_arm.c
        fsl_common.c
        fsl_gpio.c
        fsl_reset.c
        fsl_spc.c
        fsl_lpadc.c
        fsl_vref.c
        fsl_ctimer.c
        fsl_inputmux.c
        fsl_edma.c
        fsl_edma_soc.c
        fsl_lpi2c.c
        fsl_lpflexcomm.c
        fsl_lpi2c_edma.c
)

target_link_libraries(drivers PRIVATE device)