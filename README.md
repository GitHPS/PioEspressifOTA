# Espressif (esp8266-12E) Modules, Platformio and OTA

[![N|Solid](http://www.esp8266.com/wiki/lib/exe/fetch.php?w=400&tok=defe10&media=esp8266-12_mod.png)](http://www.esp8266.com/wiki/doku.php?id=esp8266-dev-boards)

### Material

### Preparation 
```sh
$ mkdir esp_test
$ pio platform install expressif
$ pio init board nodemcuv2
```


### Make a test program (blink led)

```sh
$ vim src/main.cpp
```
main.cpp:
```cpp
/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"

#define TESTPIN D0
#define INTERVAL 100

void setup()
{
  // initialize LED digital pin as an output.
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TESTPIN, OUTPUT);
}

void loop()
{
  // turn the LED on (HIGH is the voltage level)
  //digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(TESTPIN, HIGH);

  // wait for a second
  delay(INTERVAL);

  // turn the LED off by making the voltage LOW
  //digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(TESTPIN, LOW);

   // wait for a second
  delay(INTERVAL);
}

```
### compile and upload

1st connect a ftdi serial/usb to your pc and link wires like this:

ftdi  <-> esp8266-12E AI thinker
Txd   <-> Rxd
Rxd   <-> Txd
Gnd   <-> Gnd
3v3   <-> Vcc

Note that ftdi must be running 3v3 IO

Test compiling:
```sh
$ pio run -e nodemcuv2
```
Upload:
```sh
pio run -t upload
```

### OTA

src/main.cpp:
```cpp
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
 
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
 
void setup() {
 
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
 
  ArduinoOTA.setHostname("espota");
  // No authentication by default
  // ArduinoOTA.setPassword("admin");
 
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
 
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
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
  Serial.println("Ready OTA ESP8266");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void loop() {
  ArduinoOTA.handle();
}
```

download libraries

```sh
$ cd src
$ wget https://raw.githubusercontent.com/esp8266/Arduino/master/libraries/ArduinoOTA/ArduinoOTA.cpp
$ wget https://raw.githubusercontent.com/esp8266/Arduino/master/libraries/ArduinoOTA/ArduinoOTA.h
```
Test compiling (it should work)
```sh
$ cd ..
$ pio run
```
Upload 
```sh
$ pio run -t upload
```
Check if any espressif device is present on your networrk

```sh
$ sudo arp-scan --interface=eth0 --localnet
```

include the folowing line in platformio.ini:

***upload_port=espota.local*** or ***upload_port=192.168.x.y***

### blink with OTA
src/main.cpp
```cpp
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

/*****************************************************************\
*   O T A   c o m p i l e r   d i r e c t i v e s                 *
\*****************************************************************/

// OTA Debug mode 
// Change to 0 for no debug info via serial
#ifndef OTA_DEBUG
#define OTA_DEBUG 1
#endif

// OTA MACROS
#define __OTA_HOOK__ ArduinoOTA.handle() 

// OTA Constants
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";



/*****************************************************************\
*   P r o g r a m   c o m p i l e r   d i r e c t i v e s        *
\*****************************************************************/

// program specific definitions 
#define TESTPIN D0
#define INTERVAL 500

void setup() {

/*****************************************************************\
*                                                                 *
*   O T A   C O D E                                               *
*                                                                 *
*   F l a s h   y o u r   d e v i c e   o v e r   t h e   a i r   *
*                                                                 *
\*****************************************************************/

  #if OTA_DEBUG==1 
  Serial.begin(115200);
  Serial.println("Booting");
  #endif
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    #if OTA_DEBUG==1
    Serial.println("Connection Failed! Rebooting...");
    #endif
    delay(5000);
    ESP.restart();
  }
 
  ArduinoOTA.setHostname("espota");
  // No authentication by default
  // ArduinoOTA.setPassword("admin");
 
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";
 
    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    #if OTA_DEBUG==1
    Serial.println("Start updating " + type);
    #endif
  });
  ArduinoOTA.onEnd([]() {
    #if OTA_DEBUG==1
    Serial.println("\nEnd");
    #endif
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    #if OTA_DEBUG==1
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    #endif
  });
  ArduinoOTA.onError([](ota_error_t error) {
    #if OTA_DEBUG==1
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    #endif
  });
  ArduinoOTA.begin();
  #if OTA_DEBUG==1
  Serial.println("Ready OTA ESP8266");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  #endif

/*****************************************************************\
*   O T A   C O D E   E N D S   H E R E                           *
\*****************************************************************/

  // initialize LED digital pin as an output.
  //pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TESTPIN, OUTPUT);
}
 
void loop() {

/*****************************************************************\
*   O T A   C O D E   H O O K                                     *
\*****************************************************************/
  __OTA_HOOK__;


/*****************************************************************\
*   M a i n   C o d e   S t a r t s   h e r e                    *
\*****************************************************************/

  // turn the LED on (HIGH is the voltage level)
  //digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(TESTPIN, HIGH);

  // wait for a second
  delay(INTERVAL);

  // turn the LED off by making the voltage LOW
  //digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(TESTPIN, LOW);

   // wait for a second
  delay(INTERVAL);
}
```
