# Description
This project is made for learning purposes with no IDE, meaning that it's built from scratch with my own **startup code**, **linker script** and **CMake** files to understand how *booting* and *flashing* works on a microcontroller.   
I'm still using vendor tools such as MCUXPresso Config Tools and SDK drivers for easier configuration.  
It's a remake of the **NXP MCX Embedded Programming** Course final project which implements **FreeRTOS**, **DMA** and various optimization changes on the *FRDM-MCXN947* board.  
## ***How it works?***
The system utilizes the onboard **DP Switches** to toggle between three distinct applications. By having one task (named Main Menu), with the highest priority, wake-up everytime a DP Switch has changed it's state, it can safely choose on what task (application) should be suspended or resumed based on the current input.  
***Task Notifications*** are heavily used for inter-task communication and working the CPU only when it needs to.

### 1. LED Ring Speed Control — `DP Switch 1`
&emsp; Peripherals used:  
&emsp;   **Potentiometer** — Adjusts the rotation speed (the frequency at which the LEDs change between them).  
&emsp;   **LED Ring** — Displays the current speed pattern through LED output.  
&emsp;   **Button** — Changes rotation direction. 

In the background, ADC conversion for the potentiometer is activated by wiring a CTIMER0 Match 3 with a fixed time of 10 ms to the respective peripheral. After a successful conversion, the value is sent to a variable `potValue` through DMA.  
The Application 1 Task is woken up everytime CTIMER0 reaches Match 0 value and it updates the match value depending on how the potentiometer is currently set. Maximum delay is 100 ms and minimum delay is 20 ms.  

### 2. Rotary Encoder State Machine - `DP Switch 2`

  
