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
