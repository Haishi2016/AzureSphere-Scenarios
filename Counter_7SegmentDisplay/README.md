# Counter on 7-segment Display

This sample displays number 0 through F in a loop on a 7-segment display.

![Physical](../imgs/7_segment_display_physical.jpg)

## Required parts
| Part | Count |
|---------|---------|
| 7-segemnt display<br/>
![7-segment](../imgs/7-segment.png)| 1 |
| 74HC595<br/>
![74HC595](../imgs/74hc595.png)| 1 |
| Resistor 220Ω <br/>
![220ohm](../imgs/220ohm.png)| 8 | 
| Jumper (M-to-F) <br/>
![jumper](../imgs/jumper.png)| 5 |
| Jumper (M-to-M) <br/>
![jumper](../imgs/jumper-mm.png)| 13 |

## Required pins

| Pin | Purpose |
|---------|---------|
| Header 1 Pin 4 | GPIO output|
| Header 1 Pin 6 | GPIO output|
| Header 1 Pin 8 | GPIO output|

## Circuit

![Circuit](../imgs/counter_7segment_circuit.png)

## Notes (in case you needed them)

* **74HC595** is a chip that converts serial data into a byte. You send serial data through the **DS** port, and the output bits can be read from **Q0** to **Q7** ports. To write the bits, you first set **ST_CP** to *LOW*. Then, for each bit, you set **SH_CP** to *LOW*, write the bit to **DS**, and then set **SH_CP** to *HIGH*. Once you've written all bits, set **ST_CP** to *HIGH* to update the output ports.
* The **QE** port on the **74HC595** needs to be grounded to enable output ports.