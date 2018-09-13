# ESP8266 Slack Air-con Control
Utilizing an ESP8266 equipped with an IR blaster to control in-office AC units.

## Getting Started
You will need the following items in order to replicate this project:
* An ESP8266 Device, in our case, a [Wemos D1 Mini](https://www.diyelectronics.co.za/store/iot/1971-esp8266-wemos-d1-mini-wifi-dev-board.html?search_query=wemos+d1&results=2)
* A TTL Serial Module (If you are using an ESP8266 that does not have a built in serial chip)
* An IR Blaster
* Arduino IDE

### Boards and Libraries to be installed...
To install the ESP8266 boards manager, which is needed to flash the code onto the ESP8266 microcontroller, we will need to add the board to the Arduino IDE Boards Manager, to do this, Open the Arduino IDE, select `File -> Preferences`, and in the "Additional Boards Manager URLs:" paste: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`, and click 'OK' at the bottom.
Now select `Tools -> Board -> Boards Manager`, Search ESP8266, Select the first entry, and press Install.

To install all the libraries needed, select `Sketch -> Include Libraries -> Library Manager`, now search and install the following libraries (one-by-one).

* ArduinoOTA
* WebSocketsClient
* IRsend
* IRremoteESP8266
* ArduinoJson [!!!IMPORTANT!!!] When installing ArduinoJson, Make sure to install `version 5.13.2`, anything newer and the code will not work!
