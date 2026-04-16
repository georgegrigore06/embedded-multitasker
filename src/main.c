#include <stdint.h>
#include <limits.h>
// My header files
#include "main.h"

/* DMA Buffers for ADC values */
volatile uint16_t potValue, lightValue;

uint8_t bits_counter(uint8_t bits)
{
	uint8_t counter = 0;
	while(bits)
	{
		counter += bits & 1;
		bits >>= 1;
	}
	return counter;
}

uint8_t bit_position(uint8_t bits)
{
	uint8_t counter = 0;
	while(bits)
	{
		if(bits & 1) break;
		counter++;
		bits >>= 1;
	}
	return counter;
}

/* Main Menu
	- Waits for user input and activates an app task using an event group through task notifications
	- Responsible for real-time synchronization between apps
*/
void main_menu_task(void *parameter)
{
	uint8_t active_app = 0;
	CTIMER_StartTimer(CTIMER0);
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(1)); // Added delay to prevent switch debouncing
		uint8_t app = (!(GPIO_PinRead(SWITCH[0].gpio, SWITCH[0].pin))) |
					   (!(GPIO_PinRead(SWITCH[1].gpio, SWITCH[1].pin)) << 1);
		uint8_t nr_apps = bits_counter(app);
		if(nr_apps == 0)
		{
			xTaskNotify(app_handler[active_app], STATUS_OFF, eSetValueWithOverwrite);
			active_app = 0;
			xTaskNotify(oled_handler, STATUS_OFF, eSetValueWithOverwrite);
		}
		else if(nr_apps == 1) 
		{
			active_app = bit_position(app)+1;
			vTaskResume(app_handler[active_app]);
			xTaskNotify(app_handler[active_app], STATUS_ON, eSetBits);
			xTaskNotify(oled_handler, app, eSetValueWithOverwrite);
		}
		else
		{
			xTaskNotify(app_handler[active_app], STATUS_OFF, eSetValueWithOverwrite);
			active_app = 0;
			xTaskNotify(oled_handler, STATUS_ERROR, eSetValueWithOverwrite);
		}
	}
}

void oled_task(void *parameter)
{
	uint32_t notifBits;
	initOLED();
	sendOLED(nxp_logo_frame, 1024, OLED_DATA);
	while(1)
	{
		xTaskNotifyWait(notifBits, ULONG_MAX, &notifBits, portMAX_DELAY);
		resetOLED();
		if(notifBits == APP_ERROR)
		{
			printfOLED("Error\nTurn off one switch");
		}
		else if(notifBits == 0)
		{
			sendOLED(nxp_logo_frame, 1024, OLED_DATA);
		}
		else if(notifBits == APP_1)
		{
			printfOLED("Press SW1 to change  LEDs light direction.\nReset all DP switchesto go back to the    main menu.");
		}
		else if(notifBits == APP_2)
		{
			printfOLED("Use rotary encoder tochange LED direction.\nReset all DP switchesto go back to the    main menu.");
		}
	}
}

/* 
First app  
	- Dedicated to SW_DIP_1
	- LED Ring speed switching control via potentiometer
*/
void app_1_task(void *parameter) {
	/* Logic implementation for switching LEDs */
	uint8_t current_led = 0; 
	uint8_t old_led = 0; 
	uint32_t delay = 0;
	uint32_t notifBits;
	bool direction = 0;
	while(1)
  	{
		xTaskNotifyWait(0, TIMER_DELAY_DONE | CHANGED_DIRECTION, &notifBits, portMAX_DELAY);
		if(notifBits & STATUS_ON)
		{
			if(notifBits & CHANGED_DIRECTION) direction = !direction;
			uint32_t delay = MAX_DELAY - (uint32_t)((potValue * (MAX_DELAY - MIN_DELAY)) / UINT16_MAX);
			CTIMER0->MR[CTIMER0_MATCH_0_CHANNEL] = delay;
			if(CTIMER0->TC > delay) CTIMER_Reset(CTIMER0);
			GPIO_PinWrite(LEDs[old_led].gpio, LEDs[old_led].pin, 0);
			GPIO_PinWrite(LEDs[current_led].gpio, LEDs[current_led].pin, 1);
			old_led = current_led;
			if(direction) current_led = (current_led == 0) ? (num_leds - 1) : (current_led - 1);
			else current_led = (current_led+1)%num_leds;
		}
		else
		{
			for(int i=0; i<num_leds; ++i)
			{
				GPIO_PinWrite(LEDs[i].gpio, LEDs[i].pin, 0);
			}
			vTaskSuspend(NULL);
		}
  	}
}

/* 
Second App
	- Dedicated to SW_DIP_2
	- Rotary Encoder state machine application with LED ring 		
*/
void app_2_task(void *parameter)
{
    uint32_t notifBits;
    bool lastStateA = GPIO_PinRead(SHIELD_INITROTARYENCODER_SW6_A_GPIO, SHIELD_INITROTARYENCODER_SW6_A_GPIO_PIN), stateA, stateB;
    uint8_t current_led = 0; 
    uint8_t old_led = 0;
    bool direction = 0;
    uint8_t ledsOn = 0;
    int8_t offDirection = 0;

    typedef enum {
        REDUCING,
        SWITCHING,
		IDLE
    } AppMode;

    AppMode currentMode = IDLE;

    while(1)
    {
        xTaskNotifyWait(0, ENCODER_CHANGED_STATE | ENCODER_BUTTON_PRESSED, &notifBits, portMAX_DELAY);

        if(notifBits & STATUS_ON) {
            if(notifBits & ENCODER_BUTTON_PRESSED)
            {
                for(int i=0; i<num_leds; ++i)
                {
                    GPIO_PinWrite(LEDs[i].gpio, LEDs[i].pin, 1);
                }
                ledsOn = num_leds;
                current_led = 0;
                old_led = 0;
                offDirection = 0;
                currentMode = REDUCING;
            }
            else 
            {
                stateA = GPIO_PinRead(SHIELD_INITROTARYENCODER_SW6_A_GPIO, SHIELD_INITROTARYENCODER_SW6_A_GPIO_PIN);
                stateB = GPIO_PinRead(SHIELD_INITROTARYENCODER_SW6_B_GPIO, SHIELD_INITROTARYENCODER_SW6_B_GPIO_PIN);
                
                if(stateA != lastStateA)
                {
                    direction = (stateB != stateA);
                    lastStateA = stateA;

					/* Button has been pressed and there is more than one LED turned on */
                    if(currentMode == REDUCING)
                    {
                        int8_t currentDir = direction ? 1 : -1;
                        
                        if(offDirection == 0) offDirection = currentDir;

						/* The Rotary Encoder is rotated in the same direction that initialized the reducing direction */
                        if(currentDir == offDirection)
                        {
                            if(ledsOn > 1)
                            {
                                int target = (offDirection == 1) ? (num_leds - ledsOn + 1) : (ledsOn - 1);
                                GPIO_PinWrite(LEDs[target].gpio, LEDs[target].pin, 0);
                                ledsOn--;
                            }
                        }
						/* Else, the LEDs that were turned off on the way should be turned back on with each rotation */
                        else
                        {
                            if(ledsOn < num_leds)
                            {
                                ledsOn++;
                                int target = (offDirection == 1) ? (num_leds - ledsOn + 1) : (ledsOn - 1);
                                GPIO_PinWrite(LEDs[target].gpio, LEDs[target].pin, 1);
                                if(ledsOn == num_leds) offDirection = 0;
                            }
                        }

						/* If we have only one LED on, we need to change states */
                        if(ledsOn == 1) 
                        {
                            currentMode = SWITCHING;
                            current_led = 0;
                            old_led = 0;
                        }
                    }
                    else if(currentMode == SWITCHING)
                    {
                        old_led = current_led;
                        if(direction)
                        {
                            current_led = (current_led + 1) % num_leds;
                        }
                        else
                        {
                            current_led = (current_led == 0) ? (num_leds - 1) : (current_led - 1);
                        }
                        GPIO_PinWrite(LEDs[old_led].gpio, LEDs[old_led].pin, 0);
                        GPIO_PinWrite(LEDs[current_led].gpio, LEDs[current_led].pin, 1);
                    }
                }
            }
        }
        else
        {
            ledsOn = 0;
			currentMode = IDLE;
            for(int i=0; i<num_leds; ++i)
            {
                GPIO_PinWrite(LEDs[i].gpio, LEDs[i].pin, 0);
            }
            vTaskSuspend(NULL);
        }
    }
}

int main() {

	xTaskCreate(main_menu_task, "Main Menu", 50, NULL, 3, &app_handler[0]);
  	xTaskCreate(app_1_task, "App 1", 50, NULL, 2, &app_handler[1]);
	xTaskCreate(app_2_task, "App 2", 50, NULL, 2, &app_handler[2]);
	xTaskCreate(oled_task, "OLED Display", 500, NULL, 1, &oled_handler);

	BOARD_InitBootClocks();
  	BOARD_InitBootPins();
  	BOARD_InitPeripherals();

  	vTaskStartScheduler();

	while(1) {
		
	}

  return 0;
}
