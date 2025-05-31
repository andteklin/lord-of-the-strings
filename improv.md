---
layout: default
title: Future Improvements
nav_order: 5
---
# Future Improvements

While the final system successfully allows the user to control muting and switching between pre-programmed patterns, there are several opportunities for improvements, both in user interface design and in enhancing the realism and reliability of the strumming/muting mechanism.

## ON/OFF switch
For one, an ON/OFF switch can be used to turn on/off the power to the circuit, and another switch can be used to enable/disable the robot by calling `strummer_enable()` and `strummer_stop()` respectively. This would allow the user to have the software run indefinitely on the MCU without having to start/stop using the MCUXpresso debug tool.

## Rhythmic Precision
To achieve more precise delays for different tempos, an implementation at the assembly level might be explored. Firing PIT interrupts every 10ms may cause some small millisecond discrepancies in timing when driving a different state for the servos, as the PIT interrupt decrements a global remaining time variable and checks for whether this variable is less than the PIT interval every 10ms. Thus, using an assembly-level delay function may improve precision by using the CPU to run a delay by looping a certain number of cycles. However, with this approach, the CPU resources are taken up by this delay and no other functions can run in parallel as the delay is active. Thus, simply using PIT interrupts with a small interval may be sufficient without compromising too much on rhythmic precision and opens up room for concurrent processes.

### Independent Controls
In **Testing and Debugging** we discussed the downside of having a single finite state machine (FSM) that controls both muting and strumming at the same time. This coupling results in limited independence of the strumming and muting motions. In the future, implementing two independent FSMs to run a strumming schedule concurrently with a muting schedule could allow for greater freeedom in timing the two actions. This would more closely mimic a real-life guitarist in muting at certain times while strumming. However, this approach contains software challenges as well. First, the sychronization of processes are more complex, as creating combined effects like muting right after or before a strumming stroke becomes difficult. However, this effect is easier to achieve with two separate FSMs as the current implementation applies the mute state before the strumming stroke. In addition, the user must coordinate and encode the strumming and muting patterns separately, whereas the current implementation encodes both strumming and muting in the same step. This decoupled approach offers promising potential for more experienced musicians and improves flexibility with controlling the actions independently.

## Complex rhythms
The system is able to handle even divisions of notes in a simple duple meter. For the future, incorporating triplets or more complex rhytmic subdivisions within the same meter would expand the range of creativity and expression with more diverse strumming patterns. Such an implementation would require defining a new triplet time base interleaved with the duple sixteenth-note time base defined in `strummer.c`. Similar to how the length of a sixteenth note is defined, the length of a triplet can be defined by the formula $$\frac{60000}{\text{BPM} \times 3}$$. Then, a triplet multiplier can be defined in in the `NoteDiv` enumeration. The `ComboStep` struct would have to extend support for multiple timebases, so some timebase variable must be added to determine whether the current step is using sixteenth or triplet time bases. 

## Physical improvements
### Servo jitters
Momentum in the strumming arm caused the strumming servo to keep moving back and forth (jitterring) within ~5 degrees of the expected position. Ideally, the arm should simply remain in a single position for a specified duration, but the arm was not sufficiently damped to prevent the jitter from ocurring. A possible solution involves sending one or a few PWM impulse(s) to move the servo instead of sending a continuous PWM signal to the servo. This would reduce the jitterring but potentially reduces the accuracy of hitting the desired angle. 

### Pick placement
In addition, improvements can be made to the strumming arm itself with regards to the pick's angle of attack with the strings. Given the position of the servo right above the lowest string and the initially straight strumming arm, the pick's angle of attack with the strings was close to 30 degrees at the highest point and almost 60 degrees at the low point of the downstroke. Thus, such an improvement to reduce this angle of attack was made, but the angle point was set too close to the actual pivot point of the servo. This resulted in a few degrees of change with the pick's angle of attack. 

In the future, the angle point can be placed closer to the end of the arm near where the pick is located and farther away from the pivot point (servo horn). This way, the pick would make less of an angle with the strings and produce the desired tone quality.

{% include mathjax.html %}