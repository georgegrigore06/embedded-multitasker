#include <stdint.h>
#include <limits.h>
// My header files
#include "main.h"

/* DMA Buffers for ADC values */
volatile uint16_t potValue;

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

uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/* Main Menu
	- Waits for user input and activates an app task using an event group through task notifications
	- Responsible for real-time synchronization between apps
*/
void main_menu_task(void *parameter)
{
	uint8_t active_app = 0;
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		vTaskDelay(1 / portTICK_PERIOD_MS);
		uint8_t app = (!(GPIO_PinRead(SWITCH[0].gpio, SWITCH[0].pin))) |
					   (!(GPIO_PinRead(SWITCH[1].gpio, SWITCH[1].pin)) << 1) |
					   (!(GPIO_PinRead(SWITCH[2].gpio, SWITCH[2].pin)) << 2);
		uint8_t nr_apps = bits_counter(app);
		if(nr_apps == 0)
		{
			if(active_app) xTaskNotify(app_handler[active_app], STATUS_OFF, eSetValueWithOverwrite);
			active_app = 0;
			xTaskNotify(oled_handler, 0, eSetValueWithOverwrite);
		}
		else if(nr_apps == 1) 
		{
			active_app = bit_position(app)+1;
			vTaskResume(app_handler[active_app]);
			xTaskNotify(app_handler[active_app], STATUS_ON | DISPLAY, eSetBits);
		}
		else
		{
			xTaskNotify(app_handler[active_app], STATUS_OFF, eSetValueWithOverwrite);
			active_app = 0;
			xTaskNotify(oled_handler, APP_ERROR, eSetValueWithOverwrite);
		}
	}
}

void oled_task(void *parameter)
{
	uint32_t notifBits;
	initOLED();
	bool ok = 1;
	uint8_t current_dir = RIGHT;
	sendOLED(nxp_logo_frame, 1024, OLED_DATA);
	vTaskDelay(1000 / portTICK_PERIOD_MS);
	resetOLED();
	printfOLED("App1: DP1\nApp2: DP2\nApp3: DP3");
	while(1)
	{
		xTaskNotifyWait(notifBits, ULONG_MAX, &notifBits, portMAX_DELAY);
		if((notifBits & (APP_3 | DIR_UP | DIR_DOWN | DIR_LEFT | DIR_RIGHT)) ^ notifBits) ok=1;
		if(ok) resetOLED();
		if(notifBits == APP_ERROR)
		{
			printfOLED("Error\nTurn off one switch");
		}
		else if(notifBits == 0)
		{
			printfOLED("App1: DP1\nApp2: DP2\nApp3: DP3");
		}
		else if(notifBits & APP_1)
		{
			printfOLED("Press SW1 to change  LEDs light direction.\nReset all DP switchesto go back to the    main menu.");
		}
		else if(notifBits & APP_2)
		{
			printfOLED("Use rotary encoder tochange LED direction.\nReset all DP switchesto go back to the    main menu.");
		}
		else if(notifBits & APP_3)
		{
			ok=0;
			if(notifBits & DIR_LEFT) current_dir = LEFT;
			else if(notifBits & DIR_RIGHT) current_dir = RIGHT;
			else if(notifBits & DIR_UP) current_dir = UP;
			else if(notifBits & DIR_DOWN) current_dir = DOWN;
			moveSnake(&mySnake, current_dir);
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
	uint32_t notifBits = 0;
	bool direction = 0;
	ctimer_match_config_t matchConfig = CTIMER0_Match_0_config;
	uint32_t delay = matchConfig.matchValue;
	while(1)
  	{
		xTaskNotifyWait(0, TIMER_DELAY_DONE | CHANGED_DIRECTION | DISPLAY, &notifBits, portMAX_DELAY);
		if(notifBits & STATUS_ON)
		{
			// CTIMER_DisableInterrupts(CTIMER0, kCTIMER_Match0InterruptEnable);
			CTIMER_StopTimer(CTIMER0);
			if(notifBits & DISPLAY) {
				xTaskNotify(oled_handler, APP_1, eSetBits);
			}
			if(notifBits & CHANGED_DIRECTION) direction = !direction;
			delay = map((uint32_t) potValue, 0, UINT16_MAX, MIN_DELAY, MAX_DELAY);
			if(abs(matchConfig.matchValue - delay) > 10)
			{
				matchConfig.matchValue = delay;
				CTIMER_SetupMatch(CTIMER0, CTIMER0_MATCH_0_CHANNEL, &matchConfig);
				CTIMER_Reset(CTIMER0);
			} 	
			GPIO_PinWrite(LEDs[old_led].gpio, LEDs[old_led].pin, 0);
			GPIO_PinWrite(LEDs[current_led].gpio, LEDs[current_led].pin, 1);
			old_led = current_led;
			if(direction) current_led = (current_led == 0) ? (num_leds - 1) : (current_led - 1);
			else current_led = (current_led+1)%num_leds;
			CTIMER_StartTimer(CTIMER0);
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
        xTaskNotifyWait(0, ENCODER_CHANGED_STATE | ENCODER_BUTTON_PRESSED | DISPLAY, &notifBits, portMAX_DELAY);

        if(notifBits & STATUS_ON) {
			if(notifBits & DISPLAY) {
				xTaskNotify(oled_handler, APP_2, eSetBits);
			}
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

/* 
Third App 
	- Dedicated to SW_DIP_3
	- Snake Game controlled via NAV Switch
*/
void app_3_task(void *parameter)
{
	uint32_t notifBits;
	DIRECTION direction = IDLE;
	while(1)
	{
		xTaskNotifyWait(0, TIMER_CHECK | DISPLAY, &notifBits, portMAX_DELAY);
		if(notifBits & STATUS_ON) {
			for(int i=0; i<num_nav; ++i)
			{
				if(!GPIO_PinRead(NAV_Switch[i].gpio, NAV_Switch[i].pin))
				{
					direction = i;
				}
			}
			switch (direction) {
				case LEFT:
					xTaskNotify(oled_handler, APP_3 | DIR_LEFT, eSetBits);
					direction = IDLE;
					break;
				case RIGHT:
					xTaskNotify(oled_handler, APP_3 | DIR_RIGHT, eSetBits);
					direction = IDLE;	
					break;
				case UP:
					xTaskNotify(oled_handler, APP_3 | DIR_UP, eSetBits);
					direction = IDLE;
					break;
				case DOWN:
					xTaskNotify(oled_handler, APP_3 | DIR_DOWN, eSetBits);
					direction = IDLE;
					break;
				case IDLE:	
					xTaskNotify(oled_handler, APP_3, eSetBits);
					break;
			}
		}
		else
		{
			vTaskSuspend(NULL);
		}
	}
}

int main() {

	xTaskCreate(main_menu_task, "Main Menu", 100, NULL, 3, &app_handler[0]);
  	xTaskCreate(app_1_task, "App 1", 50, NULL, 2, &app_handler[1]);
	xTaskCreate(app_2_task, "App 2", 50, NULL, 2, &app_handler[2]);
	xTaskCreate(app_3_task, "App 3", 50, NULL, 2, &app_handler[3]);
	xTaskCreate(oled_task, "OLED", 1024, NULL, 1, &oled_handler);

	BOARD_InitBootClocks();
  	BOARD_InitBootPins();
  	BOARD_InitPeripherals();

  	vTaskStartScheduler();

	while(1) {
		
	}

  return 0;
}
