# womb
The whole system is designated to breed chicken eggs. The brain of the operations is an Arduino UNO or a compatible board. 

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
