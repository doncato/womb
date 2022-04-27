# W.O.M.B.
The *Willingly Overengineered Mechanical Breeder* is a 'highly' advanced system designed to breed chicken eggs.
The brain of the operations is an Arduino UNO or a compatible board. 

## Parts
+ Heating Element (any)
+ Motor (To turn the eggs) (any, can be a servo but the code is designed for a very slow motor)
+ Multiple Temperature Sensors (Adafruit MCP9808, can be others, but the code is designed for those)
+ Humidity Sensor (Adafruit DHT11, can be any, but the code is designed for this)
+ Text Display (Grove - LCD RGB, can be any LCD or different screen but the code is... you get the idea)
+ Relais (any, e.g. DEBO RELAIS 4ch)
+ 4 Buttons (any)

## Libraries
The program needs the following libraries to work:
+ [Adafruit MCP 9808 Library](https://www.arduino.cc/reference/en/libraries/adafruit-mcp9808-library/)
+ [Grove LCD RGB Backlight Library](https://www.arduino.cc/reference/en/libraries/grove-lcd-rgb-backlight/)
+ [DHT Sensor Library](https://www.arduino.cc/reference/en/libraries/dht-sensor-library/)

## Wiring
The schematics were created with https://www.circuit-diagram.org/ if you have improvements, you can import
the existing from this repo, edit it and make a pull request if you like.
<img src="https://raw.githubusercontent.com/doncato/womb/main/circuit.png">
### Wiring Description
- The four Integrated Circuits on the far left are the various temperature sensors, you can use more or less than currently displayed.
- The single Integrated Circuit on the bottom left is the I²C compatible LCD display
- The switches in the bottom center are simple switches to adjust the behaviour of the machine
- The single Integrated Circuit on the right side is an analog humidity Sensor
- The two relais on the upper right corner are to control the heater and the motor. (The System is designed with 4 relais while only 2 are actually used, you can expand the code to use all four or even more.)
- The Arduino Uno sits in the middle on the wiring diagram
