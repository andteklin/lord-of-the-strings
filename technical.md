---
layout: default
title: Technical Approach
nav_order: 3
---

# Technical Approach

## Mechanical Approach
This servo-controlled guitar robot integrates both strumming motion and palm muting through a compact mechanical system consisting of three main components. First, the **mounting structure** consists of a PLA-printed plate secured near the bridge of the guitar using two adjustable clamps. This plate serves as the base for securing two hobby servos: one for **strumming** and one for **muting**.

The **strumming servo** is attached to a PLA-printed arm with a clamp for the guitar pick. This arm is designed to sweep across all six strings in upstroke/downstroke motions. The **muting servo** uses a 3D-printed linear actuator mechanism that moves a horizontal bar up and down to simulate palm muting. The bar contains a damping material (felt or foam pad) to mute the strings.

Both arms extend over all six strings to provide strumming and muting capabilities. The plate, servo mounts, and actuator arms are **designed** by the team using CAD, while the clamps will be purchased off-the-shelf.

![Mechanical System Diagram]

## Electrical Approach
The system is powered by a 5V power supply that feeds two MG90S hobby servos and the FRDM-KL46Z board. Each servo's signal is driven by TPM-generated PWM outputs via the GPIO pins. Four momentary tactile buttons are wired between ground and the internal pull-up resistors on the GPIO pins, along with a 0.1 µF capacitor for debouncing presses. These buttons control the tempo, pattern, and mute toggle.

![GPIO Pull-Down](/images/GPIO%20pulldown.png)

Below is a system diagram of the pins used on the board.

![Electrical System Diagram]

## Software Approach

### Driving PWM signals to servos
Continuous, hardware-driven PWM signaling is used for servo control through the KL46Z's **TPM (Timer/PWM Module)**. This module is standalone and offloaded from the CPU, resulting in continuous servo operation and parallel processing on the CPU. Initially, the team was inspired by Lab 2's implementation of an assembly-level PWM delay to achieve delay precision. However, the discovery of the TPM connected to certain GPIO ports seemed more promising and reliable as a hardware implementation of delays, as well as maintained continuous PWM signals without using the CPU. An implementation of PWM using TPM can be found in `pwm.c`.

With TPM, a prescaler and modulus (MOD) value can be set to define the frequency of the PWM signal. A clock source is chosen, then divided by the prescaler and modulus to produce the frequency. That is,  
$$f_{\text{PWM}}=\frac{f_{\text{clk}}}{\text{Prescaler}\times{\text{MOD} + 1}}$$  
The formula can be manipulated to find a good prescaler and modulus that produce a wide and reasonable range for the CnV value, which controls how long the signal remains high-true per period (1–2ms for servo pulse width). That is,  
$${\text{MOD}} = \frac{f_{\text{clk}}}{\text{Prescaler}\times{f_\text{PWM}}}$$

The final TPM module runs on a 48 MHz core clock and 64 prescaler. More precise PWM signals can be achieved for servo motion with this configuration.

To simplify servo angle conversions to CnV values for the TPM, the helper function `angle_to_CnV(angle)` takes in an angle value from 0–180 degrees and converts it to the appropriate CnV value based on the TPM. Since 1–2ms is 1/20 to 1/10 of the 20ms PWM period, this proportion can be applied to the total ticks (`MOD` value) to get the CnV value. ~750–1500 ticks serve as the range of the CnV value for pulse widths between 1–2ms given the final TPM clock **configuration**.

### FSM for Strumming/Muting
An incremental design approach was taken in designing a finite-state machine (FSM) and scheduler that would **accommodate** both strumming and muting motions. First, the strumming motion was isolated, defining a `StrumState` struct that contained four states: `STRUM_DOWN`, `STRUM_UP`, `MUTE_ON`, and `MUTE_OFF`. Then, a timestamp-based scheduler struct `TimeStep` was defined to schedule certain servo motions at a specified time starting from t = 0 ms. Using a timestamp-based scheduler is not necessarily convenient for modifying the movement patterns, since note lengths (or note values) make more sense to the everyday musician.

Consequently, the `StrumStep` struct in `strummer.h` was created, using note lengths as the duration for which the program should remain in a certain state. This scheduler uses a timebase of the value of a 16th note (in milliseconds). There are 60,000 ms in a minute, so dividing this value by the BPM value and again by 4 results in the duration of a sixteenth note (in ms). **Furthermore**, integer multipliers of the sixteenth note are used to define the 8th, dotted 8th, and quarter note length values, which are 2×, 3×, and 4× the ms value of a sixteenth note respectively. Most duple-time rhythms can be derived from a combination of these note lengths.

The final design involves one FSM defined by a `ComboStep[]` array (pattern), unifying strumming and muting motions under one schedule. The schedule contains `ComboStep` steps that control the strumming state (`StrumState`), muting state (`MuteState`), and duration of the step. As defined in `strummer.h`, the `StrumState` enumeration controls the strumming servo position and consists of three possible states: `STRUM_DOWN`, `STRUM_UP`, and `REST`. In the same header file, there are three possible `MuteState` states that control the position of the mute bar: `MUTE_NOCHANGE` (which is the default state if no state is defined at instantiation of `Step`), `MUTE_ON`, and `MUTE_OFF`.

To execute the schedule properly, a step counter `step_idx` tracks the current step in the pattern. The transitions between steps are scheduled and controlled by PIT interrupts to check whether the time elapsed has met the step duration (see section on **PIT Interrupts and Scheduling** below). Once a step has elapsed its duration, the program moves to the next step in the schedule, updating the mute servo position, then the strumming servo position immediately after.

### PIT Interrupts and Scheduling
The PIT timer is configured to trigger an interrupt every 10ms, serving as the clock for the scheduling of strumming/muting steps. To calculate the correct value of `LDVAL` for the 10ms interval, the formula  
$\text{LDVAL trigger}=(\text{source frequency}/\text{interrupt frequency}) - 1$  
is used. Since the PIT runs off of the bus clock (which is half the frequency of the system core clock), and the system core clock is 48 MHz, the effective bus clock is 24 MHz. Therefore, the PIT timer is loaded with 479,999 ticks to achieve a 10ms interval.

Inside the interrupt service routine (ISR), the scheduler periodically checks whether the current `ComboStep` is complete by checking `remaining_ms`. If there is still time left (>10 ms), then this variable is decremented by 10ms. Once the duration has been fulfilled, it advances to the next step in the pattern by calling `strummer_update()`, which updates the global states and variables for the step counter and duration. This acts as a real-time task scheduling algorithm, dealing with multiple real-time processes and context switching to other states by a defined schedule without necessarily busy-waiting on the CPU.

### Button Implementations
The onboard buttons are used to toggle between different programmed styles, palm mute, and change tempo. Using the GPIO ISR, interrupts are fired on the falling edge (i.e. when the button is pressed and sending the signal to ground). These interrupts trigger the specific toggles as mentioned earlier.

Similar to how context switches work with processes, the relevant global variables and states must be reset to adapt to the newly switched pattern context. To assist with pattern selection, a helper function `strummer_selectPattern(pid, doLoop)` takes in the process ID and whether to loop the strumming pattern.

Switching between different patterns is similar to context switching with processes. Variables for controlling the step through the sequence need to be reset to advance to the next sequence properly. The helper function `strummer_selectPattern(pid, doLoop)` updates the current global pattern by switching to the next pattern via a pattern ID, enumerated by the order of patterns in `patterns[]`. The associated button to cycle between patterns simply increments the pattern ID and calls `strummer_selectPattern()` with the new PID. Once the max-indexed ID has been reached, the PID resets back to 0 and the first pattern is called.

The mute toggle button updates the global mute state and overrides the previous mute state until the next step in the pattern where the mute state changes. If the pattern doesn't have mute changes, then the user has full control over the muting mechanism.

![Software System Diagram]