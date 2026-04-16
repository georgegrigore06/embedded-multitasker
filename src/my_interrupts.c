#include "my_interrupts.h"

TaskHandle_t app_handler[3], oled_handler;

void EDMA_0_CH1_IRQHandler(void)
{
  DMA0->CH[1].CH_INT = 1;
  uint32_t status;
  status = LPI2C_MasterCheckAndClearError(I2C_OLED, LPI2C_MasterGetStatusFlags(I2C_OLED));
  lpi2c_master_callback(I2C_OLED, &LP_FLEXCOMM2_master_handle, status, NULL);
}

/*!
 * @brief LP_FLEXCOMM2 leader eDMA completion callback function.
 * 
 * This callback is used only for the non-blocking leader transfer API. * 
 * @param base The LPI2C peripheral base address.
 * @param handle Handle associated with the completed transfer.
 * @param completionStatus Either kStatus_Success or an error code describing how the transfer completed.
 * @param userData Arbitrary pointer-sized value passed from the application.
 */
void lpi2c_master_callback(LPI2C_Type *base, lpi2c_master_edma_handle_t *handle, status_t completionStatus, void *userData) {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  
  if (completionStatus == kStatus_Success) {
      handle->isBusy = false;
      vTaskNotifyGiveFromISR(oled_handler, &xHigherPriorityTaskWoken);
  }
  portYIELD_FROM_ISR( xHigherPriorityTaskWoken ); 
  
}

/* GPIO00_IRQn interrupt handler */
void GPIO0_INT_0_IRQHANDLER(void) {
  /* Get pin flags 0 */
  uint32_t pin_flags0 = GPIO_GpioGetInterruptChannelFlags(GPIO0, 0U);

  /* Place your interrupt code here */
  xTaskNotifyFromISR(app_handler[2], 0x04, eSetBits, NULL);
  /* Clear pin flags 0 */
  GPIO_GpioClearInterruptChannelFlags(GPIO0, pin_flags0, 0U); 

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}



/* GPIO10_IRQn interrupt handler */
void GPIO1_INT_0_IRQHANDLER(void) {
  /* Get pin flags 0 */
  uint32_t pin_flags0 = GPIO_GpioGetInterruptChannelFlags(GPIO1, 0U);

  /* Place your interrupt code here */
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(app_handler[0], &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR( xHigherPriorityTaskWoken ); 

  /* Clear pin flags 0 */
  GPIO_GpioClearInterruptChannelFlags(GPIO1, pin_flags0, 0U); 

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

/* GPIO20_IRQn interrupt handler */
void GPIO2_INT_0_IRQHANDLER(void) {
  /* Get pin flags 0 */
  uint32_t pin_flags0 = GPIO_GpioGetInterruptChannelFlags(GPIO2, 0U);

  /* Place your interrupt code here */
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(app_handler[0], &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR( xHigherPriorityTaskWoken ); 
  
  /* Clear pin flags 0 */
  GPIO_GpioClearInterruptChannelFlags(GPIO2, pin_flags0, 0U); 
}

/* GPIO40_IRQn interrupt handler */
void GPIO4_INT_0_IRQHANDLER(void) {
  /* Get pin flags 0 */
  uint32_t pin_flags0 = GPIO_GpioGetInterruptChannelFlags(GPIO4, 0U);

  /* Place your interrupt code here */
  xTaskNotifyFromISR(app_handler[1], (1 << 2), eSetBits, NULL);
  
  /* Clear pin flags 0 */
  GPIO_GpioClearInterruptChannelFlags(GPIO4, pin_flags0, 0U); 

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}

/* GPIO30_IRQn interrupt handler */
void GPIO3_INT_0_IRQHANDLER(void) {
  /* Get pin flags 0 */
  uint32_t pin_flags0 = GPIO_GpioGetInterruptChannelFlags(GPIO3, 0U);

  /* Place your interrupt code here */
  xTaskNotifyFromISR(app_handler[2], 0x02, eSetBits, NULL);
  /* Clear pin flags 0 */
  GPIO_GpioClearInterruptChannelFlags(GPIO3, pin_flags0, 0U); 

  /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
  #if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
  #endif
}


void CTIMER0_IRQHandler(void)
{
	uint32_t flags = CTIMER_GetStatusFlags(CTIMER0);

    CTIMER_ClearStatusFlags(CTIMER0, flags);

	if(flags & kCTIMER_Match0Flag) ctimer0_callback(flags);
}

void ctimer0_callback(uint32_t flags) {
  BaseType_t xHigherPriorityWasWoken = pdFALSE;
	xTaskNotifyFromISR(app_handler[1], (1 << 1), eSetBits, &xHigherPriorityWasWoken);
  portYIELD_FROM_ISR(xHigherPriorityWasWoken);
}