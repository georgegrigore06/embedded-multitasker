#ifndef MAIN_H
#define MAIN_H

#include "my_interrupts.h"
#include "oled.h"
#include "snake_logic.h"

/* Main Menu defines */
#define APP_1 (1U << 1)
#define APP_2 (1U << 2)
#define APP_3 (1U << 3)
#define STATUS_ON 0x01
#define STATUS_OFF 0
#define DISPLAY (1U << 3)

/* OLED defines */
#define APP_ERROR 0xFF
#define DIR_DOWN (1U << 4)
#define DIR_UP (1U << 5)
#define DIR_LEFT (1U << 6)
#define DIR_RIGHT (1U << 7)

/* APP 1 defines */
#define MAX_DELAY 200
#define MIN_DELAY 10
#define TIMER_DELAY_DONE (1U << 1)
#define CHANGED_DIRECTION (1U << 2)

/* APP 2 defines */
#define ENCODER_CHANGED_STATE (1U << 1)
#define ENCODER_BUTTON_PRESSED (1U << 2)

/* APP 3 defines */
#define TIMER_CHECK (1U << 1)

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
	{SHIELD_INITDIPSWITCH_SW_DIP_2_GPIO, SHIELD_INITDIPSWITCH_SW_DIP_2_GPIO_PIN},
	{SHIELD_INITDIPSWITCH_SW_DIP_3_GPIO, SHIELD_INITDIPSWITCH_SW_DIP_3_GPIO_PIN}
};
const uint8_t num_switches = sizeof(SWITCH) / sizeof(SHIELD_Pins_t);

SHIELD_Pins_t NAV_Switch[] = {
	{SHIELD_INITJOYSTICK_NAV_SW1_GPIO, SHIELD_INITJOYSTICK_NAV_SW1_GPIO_PIN}, // LEFT
	// {SHIELD_INITJOYSTICK_NAV_SW2_GPIO, SHIELD_INITJOYSTICK_NAV_SW2_GPIO_PIN}, // BUTTON
	{SHIELD_INITJOYSTICK_NAV_SW3_GPIO, SHIELD_INITJOYSTICK_NAV_SW3_GPIO_PIN}, // DOWN
	{SHIELD_INITJOYSTICK_NAV_SW4_GPIO, SHIELD_INITJOYSTICK_NAV_SW4_GPIO_PIN}, // RIGHT
	{SHIELD_INITJOYSTICK_NAV_SW6_GPIO, SHIELD_INITJOYSTICK_NAV_SW6_GPIO_PIN} // UP
};
const uint8_t num_nav = sizeof(NAV_Switch) / sizeof(SHIELD_Pins_t);

#endif

