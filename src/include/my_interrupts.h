#ifndef MY_INTERRUPTS_H
#define MY_INTERRUPTS_H

#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "oled.h"

/* Task Handlers for: Main Menu and Apps, OLED Display */
extern TaskHandle_t app_handler[3], oled_handler;
extern volatile uint8_t direction;

void GPIO1_INT_0_IRQHANDLER(void);
void GPIO2_INT_0_IRQHANDLER(void);
void GPIO4_INT_0_IRQHANDLER(void);
void CTIMER0_IRQHandler(void);
void ctimer0_callback(uint32_t flags);
void lpi2c_master_callback(LPI2C_Type *base, lpi2c_master_edma_handle_t *handle, status_t completionStatus, void *userData);

#endif