Author: Joshua Guillermo
Updated: 2/24/2023

The purpose of this code is to build a program that determines the blink speed of an LED by how long a button is held down. The system will start with the LED blinking at a certain speed, and the button press can alter that speed. Pressing another button will reset the blinking speed to the original.

The code utilizes 2 inputs in the 4.1 and 2.3 pins, one output in the 1.0 pin (red LED), and two timers for the blinking of the LED and length of the button press. The variable "press" is used to hold the button blink speed, while the variable "ifCount" is used to hold a 1 or 0 for whether or not Pin2.3 is being pressed. 

The system starts at a speed of "press = 3000." Here, Timer B0 CCR0 (timer for the LED blink speed) is also set to 3000, while Timer B1 CCR0 (timer for the length of button press) is set to 1. Ensure that a divider is used in both timer control registers.

If Pin2.3 is pressed, the Port 2 interrupt is fired. If a falling edge is detected: ifCount = 1, press will reset to 0, and the system will look for a rising edge. If a rising edge is detected: ifCount = 0, press will stay at its current value, and the system will look for a falling edge.

The Timer B1 interrupt works as follows: Its respective capture/compare register will have an offset of 1 added. If if ifCount = 1, press will increment and increase by 1: this is one part of how the blink speed of the LED changes. If ifCount = 0, press = press to stay at its current value. 

If Pin4.1 is pressed, the Port 4 interrupt is fired. Press will reset to its original speed of equal to 3000, and ifCount = 0. 

The Timer B0 interrupt works as follows: The red LED will be toggled, while its respective capture/compare register will have an offset equal to the value of press added, which is how the LED blink speed changes. 