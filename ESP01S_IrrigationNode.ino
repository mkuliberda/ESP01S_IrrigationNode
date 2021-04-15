#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <time.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "Vectra-WiFi-84C84C"
#define STAPSK  "e63n6pb96ido2df7"
#endif

#define MY_NTP_SERVER "at.pool.ntp.org"           
#define MY_TZ "CET-1CEST,M3.5.0,M10.5.0/3"   

const char * ssid = STASSID; // your network SSID (name)
const char * pass = STAPSK;  // your network password

bool time_acquired = false;

void setup() {
  Serial.begin(115200);
  configTime(MY_TZ, MY_NTP_SERVER); // --> Here is the IMPORTANT ONE LINER needed in your sketch!

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  delay(5000);
  getTime();
  delay(1000);
}

void loop() {
  //handleCommunication();
  //todo: msgs broker
  getTime();
  delay(1000);
}

void handleCommunication(){
  // this snippet requires arduinojson ver 6, tbd
  // Check WiFi Status
  HTTPClient http;
  if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;  //Object of class HTTPClient
    http.begin("http://jsonplaceholder.typicode.com/users/1");
    int httpCode = http.GET();
    //Check the returning code                                                                  
    if (httpCode > 0) {
      // Parsing
      const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      /*DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      // Parameters
      int id = root["id"]; // 1
      const char* name = root["name"]; // "Leanne Graham"
      const char* username = root["username"]; // "Bret"
      const char* email = root["email"]; // "Sincere@april.biz"
      // Output to serial monitor
      Serial.print("Name:");
      Serial.println(name);
      Serial.print("Username:");
      Serial.println(username);
      Serial.print("Email:"); 
      Serial.println(email);*/
    }
    http.end();   //Close connection
  }
  // Delay
  delay(60000);
}

void getTime(){
  time_t now;                         // this is the epoch
  tm ts;
  char buf[80] = "";
  const size_t capacity = JSON_OBJECT_SIZE(1) + 80;
  StaticJsonDocument<capacity> doc;
  JsonObject obj = doc.createNestedObject("NTP999>SYS000");

  while(!time_acquired){
    time(&now);                       // read the current time
    localtime_r(&now, &ts);           // update the structure tm with the current time
    if(ts.tm_year + 1900 > 2020){
      time_acquired = true;
      }
      delay(1000);
  }

  time(&now);                       // read the current time
  localtime_r(&now, &ts);           // update the structure tm with the current time
  strftime(buf, sizeof(buf), "%y%m%d%a%H%M%S", &ts);
  obj["CTD"] = buf;
  serializeJson(doc, Serial);
  Serial.println();
  //Serial.println(buf);
  
}
