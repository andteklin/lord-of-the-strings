# Project Descriptions for Team al2245-jrv67-mpm297

Please finalize your project and team by April 18th and update this README.md file accordingly.

## al2245
## jrv67
Overview

 

The objective of our final project is to implement mental tasks, including reaction time and memory testers. The reaction time task will test a user's ability to rapidly respond to visual stimuli presented by means of LEDs. And the memory task will assess a user’s ability to remember sequences of LED flashes in forward and reverse order. Ultimately, at the end of both tests, a final score will be calculated providing the user with a score representing the performance achieved on all tasks.  To implement the tests, an external circuit consisting of various LEDs (potentially 3-5 LEDs) on a breadboard will be constructed as outputs for the frdm46z MCU board and a series of corresponding buttons will be used as inputs to be read by the board - the user being tested should press the buttons in response to the visual stimuli of the LEDs. A set of pins on the MCU will be connected to the buttons and will be configured as inputs to the board, and a set of pins on the MCU will be connected to the LEDs and will be configured as outputs of the board. Current-limiting resistors and proper circuit implementation (e.g., connections to GND) will be utilized to ensure proper functionality. 

 

Reaction Time Tester

 

We’ll have a series of LEDs toggle red one by one in a line with 0.5 seconds in between, and then after a “random” amount of time ranging from 0.5 to 3 seconds (for example), we change all of the LEDs to show a green light. Upon this change, a timer will start and will only stop when a certain button is pressed. In addition to the LEDs turning green, we could also control an external speaker to make a sound simultaneously to test the user’s reaction time to sound, as well as their visual reaction time. Once the user stops the timer by pressing a specific button, their time will be displayed on the board’s built-in LCD display. 

 

We were also considering tracking the player’s fastest time, slowest time, and average time for multiple rounds done for the player. The game can be based on a game with a set number of rounds, like five rounds, where there would also be a button to reset the game being played. This would allow us to add more input type of handling for buttons as well as the timing.

 

Memory Tester

 

For the memory tester, the hardware we will use is a series of LEDs, and a series of corresponding buttons. We will have a series of levels (ex. 1 through 10) that each display a specific pattern of LED flashes upon any button being pressed. Once the LEDs simultaneously flash red, indicating the end of the pattern, the user will have to press the corresponding buttons in the matching order in order to move on to the next level. If the user fails to correctly press the buttons in the same pattern, they will restart to the first level. The built-in LCD display will show which level the user is currently on, and also show a message if the user fails, as well as display a congratulatory message when the user passes the final level.

 

We plan to implement two types of memory games - forward and reverse memory. Forward memory will consist of the user repeating the sequence of LED flashes in the order in which they occurred. Reverse memory will consist of the user repeating the sequence of LED flashes in the reverse order in which they occurred (starting with the last LED flash and ending with the first LED flash).## mpm297
Partner submitted this part.
# Feedback

# Project Web-Page

The project web-pages will also be hosted on github in this repo in the "page" branch. You can edit it by switching branches and modifying the files, or by pushing to the branch. Here is a link to a minimal web-page that you can edit and modify: [https://pages.github.coecis.cornell.edu/ece3140-sp2025/al2245-jrv67-mpm297](https://pages.github.coecis.cornell.edu/ece3140-sp2025/al2245-jrv67-mpm297)
