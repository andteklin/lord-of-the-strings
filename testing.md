---
layout: default
title: Testing/Debugging Approach
nav_order: 4
---
# Testing and Debugging
## Mechanical Aspect
One of the main challenges we faced in this project was refining the design of the strumming arm. It took several iterations to resolve issues related to mechanical noise, instability, and ensuring the arm had the proper range of motion without interfering with the muting components. We also encountered clearance issues with the pick, as it initially couldn't reach the strings—this required careful adjustments to address positioning and reach. Assembling the servo mounts onto the main mounting plate was also a challenge; it took a while to finalize optimal locations for functionality and support. 

I wish we had more accurately measured where the servo mounts needed to go beforehand, as the mounting plate took a long time to print—making redesigns and reprints time-consuming.  Also, the guitar neck is not exactly flat, but rounded.  This was highlighted when our muting mechanism only really muted the top three strings and less on the bottom strings because they are positioned vertically lower due to the guitar’s design. Additionally, we had to CAD a custom servo mount to ensure the servo stood upright rather than laterally for proper strumming motion. One thing I really wish we had known before starting was the bioacoustic effect of the servo and pick—the vibrations produced a surprisingly loud mechanical noise. Unlike strumming by hand, where your fingers absorb a lot of that vibration and you mainly hear the strings, the strumming arm amplified both the string and mechanical sounds. Understanding that earlier would have informed our material and design choices for quieter operation. These experiences highlighted the importance of planning for mechanical tolerances, acoustic behavior, and clearance early in the design process.

## Electrical/Software Aspect
### Pulse-Width Modulation using Timer/PWM Module
Sending an accurate PWM signal to the servos was the primary goal of this project. To debug the signal, an oscilloscope was hooked up to the PWM signals on pins PTB18/PTB19, measuring the period, frequency, and width of the pulse. Early on, there were issues in deriving the correct clock period for the TPM module, and consequently the PWM signal. 

In theory, the external oscillator is more reliable as the core clock could potentially be affected by CPU processes. Thus, different base clock frequencies were tested with the TPM module, including the core clock and external 8MHz crystal oscillator. However, using the external oscillator did not produce any signal. The core clock remained the only option. 

Initially, TPM had been set up with a 20.97MHz core clock base with a 128 prescaler, producing tick division values that were not necessarily integer and were low resolution with higher prescalers. The image below shows the PWM signal using this clock source.
![image](images/163.84%20on%2020.97Mhz.PNG)

Instead, changing the program to run on a 48MHz core and 64 prescaler was more accurate, resulting in a wider range of precise duty cycles and a modulus value of ~15000 (which falls under the 16-bit modulus integer limit of 65535). However, the core clock was actually running at 47.972352MHz, so the modulus value and CnV values were not necessarily integer divisions and had to be rounded to the nearest integer, reducing its accuracy. Changing the clock frequency involved modifying the clock settings on the board, which is made easy by the clocks interface on the MCUXpresso IDE. More specifically, the FEI (FLL Engaged Internal) core clock was modified to ~48MHz. The MCUXpresso IDE then updates the respective board code files for adjusting the internal clock. With the new board frequency set, more precise PWM signals were achieved for servo motion using the TPM (Timer PWM Module). 

**Signal Analysis and Servo Testing**\
The initial `MOD` value of 14991 as derived from the formula produced an experimental PWM frequency of 50.3 Hz, which is slightly off. Using the oscilloscope, the frequency and pulse width was measured in real-time and the `MOD` value was adjusted via trial and error to achieve near-perfect 50Hz frequency for PWM, as seen in the image below.
![image](images/48MHz%20working!.PNG)

The CnV values were adjusted to correspond to pulse widths between 1-2ms, which provides the full range of motion for a servo. To simplify angle conversions to CnV, the helper function `angle_to_CnV(angle)` takes in an angle value from 0-180 degrees and converts the angle to the appropriate CnV value based on the TPM. This function helped with fine-tuning the strumming angles, using a protractor to calibrate and measure the angle limits. It also helped with setting the servo angles that convert to linear actuation with the muting mechanism. 

An Arduino program was designed to test the range of servo angles to linear acutation, using a potentiometer to change the pulse width and consequently the servo angle. The proper servo angle for the muting servo was determined experimentally such that the linearly actuated horizontal mute bar dampens the strings enough to produce the desired palm muting effect. 

### FSM and Scheduler Testing
As explained earlier, the `StrumState` struct contains four states: `STRUM_DOWN`, `STRUM_UP`, `MUTE_ON`, and `MUTE_OFF`. All four states in `StrumState` were tested and implemented under `state_update()`, and the corresponding motions were tested and confirmed by the changes in pulse width for both PWM channels as observed on the oscilloscope using a timestamped scheduler `TimeStep`. 

Later, the `StrumStep[]` scheduler was created and contains entries that use note durations rather than timestamps to produce the desired pattern. To test this, a typical 4/4 strumming pattern was played by hand, then observed and encoded as a sequence of strumming steps. The resultant strumming pattern corresponded to changes in pulse width and servo motion at the expected times. The image below illustrates the classic strumming pattern defined in `patterns.c`.
![Classic](/images/classic.png)

The final implementation fuses both muting and strumming into a single step, `ComboStep`. Since each step updates both strum and mute states at the same time, strumming and muting have limited independence in this design. That is, muting can only change as the program advances to another strum step. This can be mitigated by maintaining the strum state while changing the mute state, allowing for some independence of the muting servo from the strumming servo. However, utilizing this technique would require adjusting the duration of other steps to maintain the desired strumming pattern. This issue is discussed in **Future Improvements** and compared against having two independent FSMs instead.

**PIT Interrupts and Scheduling**\
Originally, we mistakenly assumed that the PIT timer was driven by the full system core clock. As a result, strumming patterns played at half speed, as a quarter note at 120 BPM was actually playing at 60 BPM. Dividing the system core clock by 2 and making the corresponding calculations resolved the timing discrepancies and restored the expected tempo.

### Support for multiple patterns and button toggles
With accurate timing and rhythms established by the PIT scheduler, the next goal involved switching between multiple strumming patterns and muting. Buttons wired to port interrupts were used to toggle between the patterns and muting states.

Initially, the button presses were not working due to incorrect wiring of the button to high voltage. Since the port interrupt vector is triggered when connected to ground, the button was wired to have one side connected to the port and the other side connected to ground. This rendered the button to work, but multiple button presses would be registered despite a single button press. A capacitor was wired in parallel to help debounce the button presses, which only alleviated the issue a bit. However, the button was functional and could switch between patterns.