#include <stdint.h>

extern uint32_t _sdata, _edata, _sidata, __bss_start__, __bss_end__;
extern void _stackTop(void);
extern void main(void);
void Reset_Handler(void);
void Default_Handler(void) { while(1) {} } 

void NMI_Handler(void)          __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler(void)    { while(1) {} }
void MemManage_Handler(void)    __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler(void)     __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler(void)   __attribute__ ((weak, alias("Default_Handler")));
void SecureFault_Handler(void)  __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler(void)          __attribute__ ((weak, alias("Default_Handler")));
void DebugMon_Handler(void)     __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler(void)       __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler(void)      __attribute__ ((weak, alias("Default_Handler")));
void INT_OR_1_155_IRQHandler(void)   __attribute__ ((weak, alias("Default_Handler")));
void DMA0_0_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void EDMA_0_CH1_IRQHandler(void)     __attribute__ ((weak, alias("Default_Handler")));
void DMA0_2_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA0_3_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA0_4_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA0_5_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA0_6_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA0_7_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA0_8_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA0_9_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void DMA0_10_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void DMA0_11_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void DMA0_12_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void DMA0_13_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void DMA0_14_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void DMA0_15_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void GPIO00_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void GPIO01_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void GPIO10_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void GPIO11_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void GPIO20_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void GPIO21_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void GPIO30_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void GPIO31_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void GPIO40_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void GPIO41_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void GPIO50_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void GPIO51_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void UTICK0_IRQHandler(void)         __attribute__ ((weak, alias("Default_Handler")));
void MRT0_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void CTIMER0_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void CTIMER1_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));
void SCT0_IRQHandler(void)           __attribute__ ((weak, alias("Default_Handler")));
void CTIMER2_IRQHandler(void)        __attribute__ ((weak, alias("Default_Handler")));

__attribute__((used, section(".isr_vector"))) 
void (*const vectors[])(void) = {
    &_stackTop,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    SecureFault_Handler,
    0, 0, 0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    SysTick_Handler,
    INT_OR_1_155_IRQHandler,
    DMA0_0_IRQHandler,  
    EDMA_0_CH1_IRQHandler,    
    DMA0_2_IRQHandler,
    DMA0_3_IRQHandler,
    DMA0_4_IRQHandler,
    DMA0_5_IRQHandler,
    DMA0_6_IRQHandler,
    DMA0_7_IRQHandler,
    DMA0_8_IRQHandler,
    DMA0_9_IRQHandler,
    DMA0_10_IRQHandler,
    DMA0_11_IRQHandler,
    DMA0_12_IRQHandler,
    DMA0_13_IRQHandler,
    DMA0_14_IRQHandler,
    DMA0_15_IRQHandler,
    GPIO00_IRQHandler,
    GPIO01_IRQHandler,
    GPIO10_IRQHandler,
    GPIO11_IRQHandler,
    GPIO20_IRQHandler,
    GPIO21_IRQHandler,
    GPIO30_IRQHandler,
    GPIO31_IRQHandler,
    GPIO40_IRQHandler,
    GPIO41_IRQHandler,
    GPIO50_IRQHandler,
    GPIO51_IRQHandler,
    UTICK0_IRQHandler,
    MRT0_IRQHandler,
    CTIMER0_IRQHandler,
    CTIMER1_IRQHandler,
    SCT0_IRQHandler,
    CTIMER2_IRQHandler
};

__attribute__((naked, noreturn))
void Reset_Handler(void)
{
    uint32_t *dst = &_sdata;
    uint32_t *src = &_sidata;
    while(dst < &_edata)
    {
        *dst++ = *src++;
    }

    dst = &__bss_start__;
    while(dst < &__bss_end__)
    {
        *dst++ = 0;
    }
    
    main();

    while(1) {}
}