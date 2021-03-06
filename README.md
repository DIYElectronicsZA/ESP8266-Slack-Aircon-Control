# ESP8266 Slack AC Control, with OTA!

![IMAGE](https://raw.githubusercontent.com/DIYElectronicsZA/ESP8266-Slack-Aircon-Control/master/Emitters2.jpg)

Utilizing an ESP8266 equipped with an IR blaster to control in-office AC units via Slack, (Specifically "AUX" model AC units). The Code features OTA "Over-the-Air" code uploads and multiple emitters.

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
* ArduinoJson **!!!IMPORTANT!!!** When installing ArduinoJson, Make sure to install `version 5.13.2`, anything newer and the code will not work!

Once all of the above is done, we can continue to the next section

## Modifying The Code
The credentials you will need to insert into the code:
* WiFi SSID
* WiFi Passphrase
* Slack Bot Token, available [here](https://my.slack.com/services/new/bot)

Copy and paste the SlackESP.ino code into a new sketch in the Arduino IDE, and replace 'xxxx' in `WIFI_SSID`, `WIFI_PASSWORD`, and `SLACK_BOT_TOKEN` with your correct credential.

You can now flash the code to the ESP8266, to do this, select `Tools -> Board` and select your board, in our case, the **LOLIN(Wemos) DI R2 & Mini**, then in `Tools -> Port` select the COM port inwhich the ESP8266 or programmer is plugged into your PC via. Usually there is only one COM port to choose from, but if you have more, make sure there are not other usb serial devices plugged into your PC. Then in `Tools -> Upload Speed`, select 921600. You can now press the right arrow near the top left of the Arduino IDE to upload your code!

## Making the Physical Connections
The Following connections need to be made:
* **VCC of ESP8266** to **VCC of IR LED**
* **GPIO 4 or D2 of ESP8266** to **Signal of IR LED**
* **GND of ESP8266** to **GND of IR LED**

## Final Setup and Usage
Now to give your ESP8266 to power, ensure the IR Blaster is pointing at your AC unit and open Slack!
In Slack, Press the `+` next to **Direct Messages**, select the bot you made previously, and message it `1 on`.
The device should emit an IR signal and turn on the AC!
You can use the commands: `1 on, 1 On, 1 off, 1 Off`

## Advanced (Multiple Emitters and OTA)
### Multiple Emitters
To use mulitple emitters, for multiple AC units, you can use the exact same code, only changing `const char* myNum = "1";` to `const char* myNum = "2";` and `ArduinoOTA.setHostname("AC_01_ESP");` to `ArduinoOTA.setHostname("AC_02_ESP");` on the new emitter. Now in slack you can specify `1 on` or `2 on` to turn either the first or second emitter on.

### OTA (Over-The-Air Code Updates
When an emitter is plugged into power (not from your PC), in Arduino IDE, select `Tools -> Port`, and you should see **AC_01_ESP** under 'Network ports'. Select this as the port and now you can upload code over the air! No need to connect the ESP8266 to your PC!

## Contributions
The following sources were used in the making of this code
* [Urish's Arduino Slack Bot](https://github.com/urish/arduino-slack-bot/blob/master/slackbot/slackbot.ino)
* [Arduino OTA](https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA)
