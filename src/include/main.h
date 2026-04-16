#ifndef MAIN_H
#define MAIN_H

#include "my_interrupts.h"
#include "oled.h"

/* Main Menu defines */
#define APP_1 1
#define APP_2 2
#define APP_ERROR 4
#define STATUS_ON 0x01
#define STATUS_OFF 0
#define STATUS_ERROR 4

/* APP 1 defines */
#define MAX_DELAY 200
#define MIN_DELAY 10
#define TIMER_DELAY_DONE (1U << 1)
#define CHANGED_DIRECTION (1U << 2)

/* APP 2 defines */
#define ENCODER_BUTTON_PRESSED 0x04
#define ENCODER_CHANGED_STATE 0x02

/* Structure for different I/O pins on the shield for quick access */
typedef struct {
  GPIO_Type *gpio;
  uint32_t pin;
} SHIELD_Pins_t; 

/* LED Ring Init */
SHIELD_Pins_t LEDs[] = {
		{GPIO4, SHIELD_INITLEDS_D1_GPIO_PIN},
		{GPIO0, SHIELD_INITLEDS_D2_GPIO_PIN},
		{GPIO0, SHIELD_INITLEDS_D3_GPIO_PIN},
		{GPIO0, SHIELD_INITLEDS_D4_GPIO_PIN},
		{GPIO2, SHIELD_INITLEDS_D5_GPIO_PIN},
		{GPIO2, SHIELD_INITLEDS_D6_GPIO_PIN},
		{GPIO2, SHIELD_INITLEDS_D7_GPIO_PIN},
		{GPIO2, SHIELD_INITLEDS_D8_GPIO_PIN}
}; 
const uint8_t num_leds = sizeof(LEDs) / sizeof(SHIELD_Pins_t); 

/* DIP Switches Init */
SHIELD_Pins_t SWITCH[] = {
	{SHIELD_INITDIPSWITCH_SW_DIP_1_GPIO, SHIELD_INITDIPSWITCH_SW_DIP_1_GPIO_PIN},
	{SHIELD_INITDIPSWITCH_SW_DIP_2_GPIO, SHIELD_INITDIPSWITCH_SW_DIP_2_GPIO_PIN}
};
const uint8_t num_switches = sizeof(SWITCH) / sizeof(SHIELD_Pins_t);

#endif

