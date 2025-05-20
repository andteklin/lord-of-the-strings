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
The system is powered by a 5V power supply that feeds two MG90S hobby servos and the FRDM-KL46Z board. Each servo's signal is driven by TPM-generated PWM outputs via the GPIO pins. Four momentary tactile buttons are wired between ground and the internal pull-ups on the GPIO pin along with a 0.1 µF capacitor for debouncing presses. These buttons will control the tempo, pattern, and mute toggle. Below is a system diagram of the pins used on the board.

![Electrical System Diagram]

### Software Approach
Continous, hardware-driven PWM signaling is used for servo control thorugh the KL46Z's **TPM (Timer/PWM Module)**. This module is standalone and offloaded from the CPU, resulting in continuous servo operation and parallel processing on the CPU. Initially, the team was inspired by Lab 2's implementation of an assembly-level PWM delay to achieve delay precision. However, the discovery of the TPM connected to certain GPIO ports seemed more promising and reliable as a hardware implementation of delays, as well as maintained continuous PWM signals without using the CPU. 

A **PIT interrupt** is fired every 10ms drives a scheduler that runs a pre-programmed rhythmic pattern.

Different rhythmic patterns can be programmed using a time division of quarter, eighth, and sixteenth notes along with muting. The onboard buttons can be used to toggle between different programmed styles and palm mute.

![Software System Diagram]