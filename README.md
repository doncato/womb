# womb
The whole system is designated to breed chicken eggs. The brain of the operations is an Arduino UNO or a compatible board. 

## Parts
+ Heating Element (any)
+ Motor (To turn the eggs) (any)
+ Multiple Temperature Sensors (Adafruit MCP9808)
+ Humidity Sensor (Adafruit DHT11)
+ Text Display (Grove - LCD RGB)
+ Relais (any, e.g. DEBO RELAIS 4ch)
+ 4 Buttons (any, but they should have NO and NC)

## Libraries
The program needs the following libraries to work:
+ [Adafruit MCP 9808 Library](https://www.arduino.cc/reference/en/libraries/adafruit-mcp9808-library/)
+ [Grove LCD RGB Backlight Library](https://www.arduino.cc/reference/en/libraries/grove-lcd-rgb-backlight/)
+ [DHT Sensor Library](https://www.arduino.cc/reference/en/libraries/dht-sensor-library/)

## Wiring
The schematics were created with https://www.circuit-diagram.org/ if you have improvements, you can import
the existing from this repo, edit it and make a pull request if you like.
<img src="https://raw.githubusercontent.com/doncato/womb/main/circuit.png">
