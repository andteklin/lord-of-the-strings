## Technical Approach
### Mechanical Approach
This servo-controlled guitar robot integrates both strumming motion and palm muting through a compact mechanical system consisting of three main components. First, the **mounting structure** consists of a PLA-printed plate secured near the bridge of the guitar using two adjustable clamps. This plate serves as the base for securing two hobby servos: one for **strumming** and one for **muting**. 

The **strumming servo** will have a PLA-printed arm with a mounted pick to perform the strumming motion. The muting servo will have a PLA-printed arm with felt attached to the end to mute the strings. Both arms will extend over all six strings to provide strumming and muting capabilities. The plate, servo mounts, and arms will be CAD-designed by the team, while the clamps will be purchased.

### Electrical Approach
The electric components of the system include a 5V power supply, two SG90 hobby servos, buttons, and the FRDM board. A switch will be connected from the output of the power supply to the rest of the circuit. The FRDM board will output PWM signals to the two servos sequentially, connected from a GPIO output pin on the FRDM to the signal input on the servo. 

### Software Approach
We use PWM signaling for servo control similar to the PWM signals we sent to the LEDs in lab 2. However, this time we use TPM (Timer PWM Module) to have the PWM signals continously sent without using the CPU. Different rhythms can be programmed using a time division of quarter, eighth, and sixteenth notes along with muting. The onboard buttons can be used to toggle between different programmed styles and palm mute.