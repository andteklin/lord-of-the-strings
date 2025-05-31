---
layout: default
title: Outside Resources
nav_order: 7
---

## Outside Resources

We drew inspiration for the mounting plate and strumming motion from this guide:  
🔗 [AutoStrummer – Instructables](https://www.instructables.com/AutoStrummer/)

To understand how push buttons work in embedded systems, we referred to:  
🔗 [How to Connect and Program Push Buttons – Circuit Basics](https://www.circuitbasics.com/how-to-connect-and-program-push-buttons-on-the-arduino/)

Servo motor specifications and limitations were referenced from the datasheet:  
🔗 [MG90S Metal Gear Servo – Components101](https://components101.com/motors/mg90s-metal-gear-servo-motor)

We used the FRDM-KL46Z schematic to understand pin mappings and electrical design:  
🔗 [FRDM-KL46Z Schematic – OpenHacks](https://www.openhacks.com/uploadsproductos/frdm-kl46z_sch.pdf)

For PWM signal generation using the TPM module, we referred to an NXP community discussion:  
🔗 [How to Set Timer PWM Module for Controlling ESC – NXP](https://community.nxp.com/t5/Kinetis-Software-Development-Kit/How-to-set-Timer-PWM-Module-for-controlling-ESC/m-p/532399)

In addition, we were inspired by a GitHub repo containing implementations of motor control on the FRDM board:
🔗 [Hardware Tinkering](https://github.com/KayeJD/FRDM-KL46Z-Programming)

An early source of project inspiration was this concept:  
🔗 [Strumbot – Hackaday](https://hackaday.com/2018/01/31/strumbot-the-guitar-that-strums-itself/)

Pin functions for the KL46Z board were cross-checked with the platform documentation:  
🔗 [FRDM-KL46Z Pinout – mbed OS](https://os.mbed.com/platforms/FRDM-KL46Z/)

---

We also used AI coding assistants like **ChatGPT** and **GitHub Copilot** to help write and debug parts of the code. These tools provided helpful high-level guidance and structure, but they lacked specific knowledge of hardware modules like TPM and clock configuration. As a result, significant effort was still required to debug, tune the PWM signals, and achieve reliable servo behavior.
