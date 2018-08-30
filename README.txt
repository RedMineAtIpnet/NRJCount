Project: Energy counter

Context:

With this project, we aim to create a new generation of smart electric energy counters.
The first part of the project focuses on the energy acquisition and measurement.
We will measure the intensity and voltage of our installation to then deduce other indicators such as 
Active power (P), Reactive Power (Q) and Apparent Power (S).
We will, for that, use the STPM34 microchip, associated with arduino.

Hardware conception: acquisition.

STPM3x is equipped with
2 couples of voltage entries and 2 couples of current entries, each corresponding to a phase.
This feature is especially usefull in industrial settings where the 3-phase system is used.
Each couple has 2 entries, one for the phase, another for the neutral (ex: VIN1: Voltage Input Neutral on phase 1, VIP1: Voltage Input Phase on phase ).

These inputs on the stpm34 range from -0.3V to 0.3V. Thus, a shunt resistance and a voltage divider according to the joined diagram.

Digital pins have a 3.3 voltage input/output thus the associated microcontroller should have a 3.3V input/output.

Software: Reading

The STPM3x stores the data in registers you can find the whole list in the datasheet.
Communication between STPM34 and the microcontroller is done via SPI.


Code joined.