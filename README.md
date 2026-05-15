# Description
This project is made for learning purposes with no IDE, meaning that it's built from scratch with custom **startup code**, **linker script** and **CMake** files to understand how *building*, *flashing* and *booting* works on a microcontroller. Also, ***Real-Time Operating Systems*** are a must-know in embedded programming.  
Vendor tools such as MCUXPresso Config Tools and SDK drivers are still used for easier configuration.  
It's a remake of the **NXP MCX Embedded Programming** Course final project which implements **FreeRTOS**, **DMA** and various optimization changes on the *FRDM-MCXN947* board.  
## ***How it works?***
The system utilizes the MCXN947 I/O Shield **DP Switches** to toggle between three distinct applications. By having one task (named Main Menu), with the highest priority, wake-up everytime a DP Switch has changed it's state, it can safely choose on what task (application) should be suspended or resumed based on the current input. It also verifies if two or more applications are set ON, and sends an error, allowing the system to work only when one application is activated at a time.  
***Task Notifications*** are heavily used for inter-task communication and working the CPU only when it needs to.

### 1. LED Ring Speed Control - `DP Switch 1`
&emsp; Peripherals used:  
&emsp;   **Potentiometer** - Adjusts the rotation speed (the frequency at which the LEDs change between them).  
&emsp;   **LED Ring** - Displays the current speed pattern through LED output.  
&emsp;   **Button** - Changes rotation direction. 

In the background, ADC conversion for the potentiometer is activated by wiring a CTIMER0 Match 3 with a fixed time of 10 ms to the respective peripheral. After a successful conversion, the value is sent to a variable `potValue` through DMA.  
The Application 1 Task is woken up everytime CTIMER0 reaches Match 0 value and it updates the match value depending on how the potentiometer is currently set. Maximum delay is 100 ms and minimum delay is 20 ms.  

### 2. Rotary Encoder State Machine - `DP Switch 2`
&emsp; Peripherals used:  
&emsp;   **Rotary Encoder** - Handles state triggers and directional tracking.   
&emsp;   **LED Ring** - Displays the current selected state through LED output.  

The application operates based on two states controlled by the rotary encoder. In the **REDUCING** state, which is initiated by pressing the encoder switch, the entire LED ring illuminates completely. As the user rotates the encoder, the LEDs turn off one by one following the direction of the rotation. Once the reduction process leaves only a single active light, the system transitions into the **SWITCHING** state. In this mode, that single remaining LED acts as a traveling indicator, moving around the ring in real time to match the clockwise or counterclockwise rotation of the encoder.  

### 3. Snake Game - `DP Switch 3`
&emsp; Peripherals used:  
&emsp;   **NAV Switch** - Changes Snake's current direction     
&emsp;   **OLED** - Real-time display of the Snake   

The user controls a Snake that is fixed at a length of four blocks, navigating it across the screen in four directions using the NAV Switch. To ensure smooth performance without taxing the processor, the frame buffer data is transferred directly to the OLED display via DMA, minimizing CPU overhead during screen updates.






  
