/*
  board: generic ESP8266 Module
  Builtin LED: "2"
  Upload speed: 921600
  CPU freq: 80MHz
  Crystal freq: 26MHz
  Flash size: 1MB (FS: none, OTA: ~502KB)
  Flash Mode: DOUT(compatible)
  Flash freq: 40MHz
  Reset method: no dtr (aka ck)
*/

/*
  sample commands:
  {"esp_mode":"1"} //either 1, 2, 3 or 4
  {"reboot":1}
  {"end_server":1}
*/

/*Wifi client and Telegram CODE*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

//use wifimanager later to set these as variables
#define WIFI_SSID "XXXXXXXXXXX" 
#define WIFI_PASSWORD "XXXXXXXXXXXXX"
#define BOT_TOKEN "XXXXXXXXXXXXXXXXXXXX" //get from bot father
#define USERNAME "XXXXXX" //not really necessary
#define CHATID "XXXXXXXXXXX" //go to @myidbot to get this
const String ESPHostname = "WiFi-keyboard";
const unsigned long BOT_MTBS = 20; // mean time between scan messages
String ipaddr;
String ntpnow;
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;
unsigned long previousMillis_wifi = 0;
unsigned long interval_wifi = 5000;
unsigned long previousMillis_serial = 0;
unsigned long interval_serial = 10;
bool Start = true;

/*ASYNC and OTA CODE*/
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
AsyncWebServer server(80);
#define APSSID "wifileo" //can set whatever APSSID and password you want
#define APPASS "wifileo123"
const char* text_box_input_1 = "text_box_1";
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>WiFiLeo Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head>
  <body>
  <h3>Enter text to send WiFiLeo</h3>
  <form action="/get">
    <textarea id="largeTB" name="text_box_1" maxlength="512" rows="4" cols="50"></textarea><br><br>
    <input type="submit" value="Submit">
  </form>
  </body></html>)rawliteral";
String text_output;
#define OTAUSER "wifileouser" //username and password for elegantasyncOTA
#define OTAPASS "wifileopass" //to prevent unauthorised OTA 

/*JSON CODE*/
#include <ArduinoJson.h>
StaticJsonDocument<512> doc; //initialise the JSON buffer to hold callback data
JsonObject serial_input_json;

/*Mode CODE*/
#define setup_timeout_counter_max_tries 3600
byte select_mode = 0;
String serial_input;
unsigned long previousMillis_leonardo = 0;
unsigned long interval_leonardo = 4000;

/*FW VERSION*/
String fw_version = "1";

void setup() {
  delay(100);
  Serial.begin(9600);
  await_leonardo_mode_message();

  if (select_mode > 0) {
    switch (select_mode) {
      case 1:
        begin_tele_wifi_ntp();
        break;
      case 2:
        begin_AP_form_server();
        break;
      case 3:
        begin_AP_OTA_server();
        break;
      case 4:
        begin_client_OTA_server();
        break;
      default:
        begin_tele_wifi_ntp();
    }
  }

}

void loop() {

  if (select_mode > 0) {
    switch (select_mode) {
      case 1:
        tele_runtime();
        break;
      case 2:
        //AP_form_server
        break;
      case 3:
        //AP OTA server
        break;
      case 4:
        //client OTA server
        break;
      default:
        tele_runtime();
    }
  }

  read_leonardo_runtime();

}
