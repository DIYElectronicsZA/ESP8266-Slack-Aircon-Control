#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <IRsend.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
const char* myNum = "2";
#define SLACK_SSL_FINGERPRINT "C1 0D 53 49 D2 3E E5 2B A2 61 D5 9E 6F 99 0D 3D FD 8B B2 B3"
#define SLACK_BOT_TOKEN "xxxx" 
#define WIFI_SSID       "xxxx"
#define WIFI_PASSWORD   "xxxx"
#define WORD_SEPERATORS "., \"'()_[]<>;:-+&?!\n\t"
ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
long nextCmdId = 1;
bool connected = false;
bool me = false;
IRsend irsend(4);
uint16_t C16OnFH[211] = {8900, 4554,  526, 1692,  496, 1722,  472, 644,  464, 632,
                         466, 630,  466, 630,  490, 1728,  450, 1748,  470, 1750,  470, 1750,  466, 1752,
                         468, 650,  468, 628,  494, 602,  494, 1724,  448, 650,  492, 606,  462, 632,
                         494, 602,  494, 602,  494, 602,  494, 1724,  448, 1752,  466, 1752,  466, 650,
                         492, 606,  466, 628,  492, 604,  492, 606,  466, 630,  466, 632,  464, 632,
                         464, 632,  464, 632,  464, 634,  462, 634,  442, 654,  444, 1754,  528, 590,
                         444, 652,  446, 650,  448, 648,  450, 646,  450, 646,  450, 648,  474, 622,
                         450, 646,  476, 620,  504, 592,  502, 594,  504, 594,  504, 592,  532, 564,
                         534, 1644,  586, 554,  538, 558,  540, 556,  540, 556,  540, 556,  540, 556,
                         540, 556,  540, 556,  540, 556,  540, 556,  542, 554,  542, 556,  542, 554,
                         540, 556,  540, 556,  542, 554,  542, 554,  542, 556,  540, 554,  542, 554,
                         542, 554,  542, 556,  542, 554,  542, 1678,  536, 560,  540, 556,  540, 556,
                         542, 554,  540, 558,  564, 530,  540, 558,  540, 556,  538, 558,  538, 558,
                         540, 1680,  532, 564,  538, 1682,  536, 560,  538, 558,  538, 558,  538, 558,
                         538, 558,  538, 1682,  532, 1686,  532, 1686,  532, 1688,  534, 562,  536,
                         562,  536, 1684,  532, 564,  562
                        };  // 16'C Fan High On F638D84B
uint16_t C16OffFH[211] = {8886, 4566,  500, 1718,  474, 1746,  474, 626,  470, 626,  496, 598,  472, 624,  472, 1746,  472, 1746,  474, 1746,  474, 1744,  474, 1746,  474, 626,  470, 626,  470, 626,  470, 1746,  474, 626,  472, 624,  472, 626,  470, 624,  472, 626,  470, 624,  472, 1746,  474, 1746,  474, 1744,  474, 624,  474, 622,  474, 624,  474, 622,  472, 624,  472, 624,  472, 624,  472, 624,  472, 624,  474, 622,  472, 624,  474, 624,  472, 624,  472, 1744,  476, 624,  472, 624,  472, 624,  472, 624,  472, 624,  472, 624,  472, 624,  474, 622,  472, 624,  472, 622,  474, 624,  472, 624,  472, 624,  472, 624,  474, 622,  472, 1744,  476, 622,  474, 622,  472, 624,  472, 624,  498, 598,  472, 624,  472, 624,  472, 624,  472, 624,  472, 624,  472, 624,  472, 624,  472, 624,  472, 624,  472, 626,  472, 624,  470, 624,  472, 624,  470, 626,  472, 624,  472, 624,  472, 624,  472, 624,  470, 626,  472, 624,  472, 624,  470, 626,  472, 624,  472, 624,  470, 626,  498, 598,  470, 626,  470, 626,  472, 624,  470, 1746,  472, 626,  470, 1748,  472, 626,  470, 626,  470, 626,  470, 626,  496, 598,  470, 1748,  472, 1748,  472, 1746,  472, 1746,  474, 626,  468, 1750,  474, 624,  494, 602,  496};  // 16'C Fan High Off 62788A1D
void sendPing() {
  digitalWrite(LED_BUILTIN, LOW);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["type"] = "ping";
  root["id"] = nextCmdId++;
  String json;
  root.printTo(json);
  webSocket.sendTXT(json);
  digitalWrite(LED_BUILTIN, HIGH);
}
void processSlackMessage(char *payload) {
  char *nextWord = NULL;
  for (nextWord = strtok(payload, WORD_SEPERATORS); nextWord; nextWord = strtok(NULL, WORD_SEPERATORS)) {
    if (strcasecmp(nextWord, myNum) == 0) {
      me = true;
      Serial.println("ME!");
    }
    if (strcasecmp(nextWord, "on") == 0 || strcasecmp(nextWord, "On") == 0  ) {
      if (me) {
        Serial.println("16'C On Fan High");
        digitalWrite(2, HIGH);
        irsend.sendRaw(C16OnFH, 211, 38);  // Send a raw data capture at 38kHz.

        delay(5000);
        digitalWrite(2, LOW);
        me = false;
      }
    }
    if (strcasecmp(nextWord, "off") == 0 || strcasecmp(nextWord, "Off") == 0  ) {
      if (me) {
        digitalWrite(2, HIGH);
        Serial.println("16'C Off Fan High");
        irsend.sendRaw(C16OffFH, 211, 38);  // Send a raw data capture at 38kHz.

        delay(5000);
        digitalWrite(2, LOW);
        me = false;
      }
    }
  }
}
void webSocketEvent(WStype_t type, uint8_t *payload, size_t len) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[WebSocket] Disconnected :-( \n");
      connected = false;
      break;

    case WStype_CONNECTED:
      Serial.printf("[WebSocket] Connected to: %s\n", payload);
      sendPing();
      break;

    case WStype_TEXT:
      Serial.printf("[WebSocket] Message: %s\n", payload);
      processSlackMessage((char*)payload);
      break;
  }
}
bool connectToSlack() {
  // Step 1: Find WebSocket address via RTM API (https://api.slack.com/methods/rtm.connect)
  HTTPClient http;
  http.begin("https://slack.com/api/rtm.connect?token=" SLACK_BOT_TOKEN, SLACK_SSL_FINGERPRINT);
  int httpCode = http.GET();

  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("HTTP GET failed with code %d\n", httpCode);
    return false;
  }

  WiFiClient *client = http.getStreamPtr();
  client->find("wss:\\/\\/");
  String host = client->readStringUntil('\\');
  String path = client->readStringUntil('"');
  path.replace("\\/", "/");

  // Step 2: Open WebSocket connection and register event handler
  Serial.println("WebSocket Host=" + host + " Path=" + path);
  webSocket.beginSSL(host, 443, path, "", "");
  webSocket.onEvent(webSocketEvent);
  return true;
}
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  pinMode(2, OUTPUT);
  irsend.begin();
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }
  ArduinoOTA.setHostname("AC_02_ESP");
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
}
unsigned long lastPing = 0;
void loop() {
  webSocket.loop();
  ArduinoOTA.handle();
  if (connected) {
    // Send ping every 5 seconds, to keep the connection alive
    if (millis() - lastPing > 5000) {
      sendPing();
      lastPing = millis();
    }
  } else {
    // Try to connect / reconnect to slack
    connected = connectToSlack();
    if (!connected) {
      delay(500);
    }
  }
}