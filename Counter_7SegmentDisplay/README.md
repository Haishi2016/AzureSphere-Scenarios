# Counter on 7-segment Display

This sample displays number 0 through F in a loop on a 7-segment display.

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

![Circuit](../imgs/blinking_led_circuit.png)

## Notes (in case you needed them)

* The longer leg of a LED is the positive pole.
* You can connect to any GND pins
