---
layout: default
title: Technical Approach
nav_order: 3
---
## Technical Approach
### Mechanical Approach
This servo-controlled guitar robot integrates both strumming motion and palm muting through a compact mechanical system consisting of three main components. First, the **mounting structure** consists of a PLA-printed plate secured near the bridge of the guitar using two adjustable clamps. This plate serves as the base for securing two hobby servos: one for **strumming** and one for **muting**. 

The **strumming servo** is attached to a PLA-printed arm with a clamp for the guitar pick. This arm is designed to sweep across all six strings in upstroke/downstroke motions. The **muting servo** uses a 3D-printed linear actuator mechanism that moves a horizontal bar up and down to simulate palm muting. The bar contains a damping material (felt or foam pad) to mute the strings. 

Both arms extend over all six strings to provide strumming and muting capabilities. The plate, servo mounts, and actuator arms are desgined by the team using CAD, while the clamps will be purchased off-the-shelf.

![Mechanical System Diagram]

### Electrical Approach
The system is powered by a 5V power supply that feeds two MG90S hobby servos and the FRDM-KL46Z board. Each servo's signal is driven by TPM-generated PWM outputs via the GPIO pins. Four momentary tactile buttons are wired between ground and the internal pull-up resistors on the GPIO pin along with a 0.1 µF capacitor for debouncing presses. These buttons will control the tempo, pattern, and mute toggle. 
![GPIO Pull-Down](/images/GPIO%20pull-down.png)

Below is a system diagram of the pins used on the board.

![Electrical System Diagram]

### Software Approach
Continous, hardware-driven PWM signaling is used for servo control thorugh the KL46Z's **TPM (Timer/PWM Module)**. This module is standalone and offloaded from the CPU, resulting in continuous servo operation and parallel processing on the CPU. Initially, the team was inspired by Lab 2's implementation of an assembly-level PWM delay to achieve delay precision. However, the discovery of the TPM connected to certain GPIO ports seemed more promising and reliable as a hardware implementation of delays, as well as maintained continuous PWM signals without using the CPU. 

To switch between the different servo motions, a **PIT interrupt** is fired every 10ms. This interrupt drives a scheduler that runs a pre-programmed rhythmic pattern array consisting of `ComboStep`-type objects, which encodes strumming and muting motions using a time division of quarter, eighth, and sixteenth notes. These patterns can be found in `patterns.c`. 

The onboard buttons are used to toggle between different programmed styles, palm mute, and change tempo. Using the GPIO ISR, interrupts are fired on the falling edge (i.e. when the button is pressed and sending the signal to ground). These interrupts trigger the specific toggles as mentioned earlier. 

Switching between different patterns is similar to context switching with processes. Variables for controlling the step through the sequence need to be reset to advance the next sequence properly. For example, in `strummer_selectPattern()`, the current pattern is updated and switched to the next pattern in the list of patterns, then the state of the strumming and mute servo is updated.

![Software System Diagram]