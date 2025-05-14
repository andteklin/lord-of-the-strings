# Lord of the Strings

#### Overview
The Lord of the Strings is more than just a simple guitar strummer. This robot attempts to implement palm muting, a technique for a more advanced guitarist. We wish to program the board to control the speed and strumming style. Much of the software will involve testing and debugging the sensor-to-arm control scheme and fine-tuning the movement of the strumming arm. We plan to add on a felt muting bar near the bridge to imitate the guitar palm muting technique.
YouTube Video to be embedded later. 
![Sketch](images/sketch.jpg)

#### System Diagram
![image](images/System%20Diagram.jpg)

#### Technical Approach
**Mechanical Approach**\
We will use two clamps to mount a PLA-printed plate onto the upper neck of the guitar. This plate will hold two servos: one for strumming and one for muting. The strumming servo will have a PLA-printed arm with a mounted pick to perform the strumming motion. The muting servo will have a PLA-printed arm with felt attached to the end to mute the strings. Both arms will extend over all six strings to provide strumming and muting capabilities. The plate, servo mounts, and arms will be CAD-designed by the team, while the clamps will be purchased.

**Electrical Approach**\
The electric components of the system include a 6v power supply, a switch, a buck converter, a capacitor, an SG90 servo, and the FRDM board. We will supply the system with 4x 1.5V AA batteries for a combined 6v. A switch will be connected from the output of the power supply to the rest of the circuit. We will then use a buck converter to step down the voltage from 6v to 5v. We will power the FRDM board directly from the 5v output and supply the servo power from the same 5v output through a decoupling capacitor. The FRDM board will output PWM signals to the servo, connected from a GPIO output pin on the FRDM to the signal input on the servo. 
![image](https://github.coecis.cornell.edu/ece3140-sp2025/ahl92-am2736-nsv23/assets/16551/48b23880-f1ca-4bda-b34f-a36f5a59727d)


**Software Approach**\
We will create PWM signaling for servo control similar to the PWM signals we sent to the LEDs in lab 2. We will also implement concurrency by having one servo controlling the strumming arm while the other controls the muting mechanism simultaneously. Timer-based scheduling will be used to run the servos in precise, overlapping coordination. The onboard button will be used to toggle between different programmed styles.

#### Testing and Debugging
**Mechanical Aspect**\
One of the main challenges we faced in this project was refining the design of the strumming arm. It took several iterations to resolve issues related to mechanical noise, instability, and ensuring the arm had the proper range of motion without interfering with the muting components. We also encountered clearance issues with the pick, as it initially couldn't reach the strings—this required careful adjustments to address positioning and reach. Assembling the servo mounts onto the main mounting plate was also a challenge; it took a while to finalize optimal locations for functionality and support. 
I wish we had more accurately measured where the servo mounts needed to go beforehand, as the mounting plate took a long time to print—making redesigns and reprints time-consuming.  Also, the guitar neck is not exactly flat, but rounded.  This was highlighted when our muting mechanism only really muted the top three strings and less on the bottom strings because they are positioned vertically lower due to the guitar’s design. Additionally, we had to CAD a custom servo mount to ensure the servo stood upright rather than laterally for proper strumming motion. One thing I really wish we had known before starting was the bioacoustic effect of the servo and pick—the vibrations produced a surprisingly loud mechanical noise. Unlike strumming by hand, where your fingers absorb a lot of that vibration and you mainly hear the strings, the strumming arm amplified both the string and mechanical sounds. Understanding that earlier would have informed our material and design choices for quieter operation. These experiences highlighted the importance of planning for mechanical tolerances, acoustic behavior, and clearance early in the design process.

#### Team Work 
How did you coordinate? Who was responsible for what? You can skip this section if you work alone. 
#### Outside Resources 
If you are using a code base from outside of class, or found a particular resource helpful, please put it here. Describe any use of generative AI tools here as well.
