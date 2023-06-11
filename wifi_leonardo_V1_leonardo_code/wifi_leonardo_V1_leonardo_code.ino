/*
   board: generic ESP8266 Module
   Builtin LED: "2"
   Upload speed: 921600
   CPU freq: 80MHz
   Crystal freq: 26MHz
   Flash size: 1MB
   Flash Mode: DOUT(compatible)
   Flash freq: 40MHz
   Reset method: no dtr (aka ck)
*/

/*
  sample commands to send:
  {"esp_mode":"1"} //either 1, 2, 3 or 4
  {"reboot":1}
  {"end_server":1}

  sample commands to recieve:
  {"esp_status":"rebooted"}, {"esp_status":"setup_complete"}
  {"tele_string": string_from_telegram}
  {"slash_tele_string": slash_string_from_telegram}
  {"form_string": string_from_form}
  {"ip_string": ip_addr}
  {"ntp_string": NTP_string}
  {"fw_string":firmware_number}
  consumer:
  "/volume_up" - increase volume
  "/volume_down" - decrease volume
  "/play_pause" - play or pause media
  "/stop" - stop media
*/

/*SOFTWARE SERIAL CODE*/
#include <Arduino.h>
#include <SoftwareSerial.h>
#define SSTX 9
#define SSRX 8
SoftwareSerial espSerial(SSRX, SSTX);

/*JSON CODE*/
#include <ArduinoJson.h>
StaticJsonDocument<512> doc; //initialise the JSON buffer to hold callback data
JsonObject serial_input_json;

/*Mode CODE*/
#define setup_timeout_counter_max_tries 3600
byte select_mode = 1; //default set mode
byte previous_mode = select_mode;
byte number_of_modes = 4;
String select_character = "";
const char* mode_names[] = {"telegram", "WiFi_AP", "AP_OTA", "Client_OTA"};
String serial_input;
unsigned long previousMillis_serial = 0;
unsigned long interval_serial = 10;
const byte allow_HID_toggle = 21; //a togle switch to allow/disallow HID for debugging
bool write_HID = false;
bool prev_write_HID = false; //to trigger oled update


/*ESP recieved JSON CODE*/
String espStatus, ntpString, teleString, slashTeleString, formString, fwVersion, ipAddr;

/*HID CODE*/
#include "HID-Project.h" //import nicohood's HID library
unsigned long previousMillis_keyboard = 0;
unsigned long interval_keyboard = 150;
byte iterate_volume = 10; //a variable for the number of times volume command is sent

/*BUTTON CODE*/
byte press_to_cycle_mode = 4;
byte press_to_change_mode = 5;
byte press_fastmode_1 = 6;
byte press_fastmode_2 = 7;
byte press_spare = 20;
bool isPressing = false;
bool isLongDetected = false;
byte mode_selector_delay_time = 500; //manual debouncing of short presses necessary

/*OLED CODE*/
#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL);
extern uint8_t SmallFont[];
bool show_oled = true;
unsigned long previousMillis_oled = 0;
unsigned long interval_oled_sleep = 60000; //put oled to sleep after 30 seconds

void setup() {
  delay(5000); //debug delay in case board bootloops
  Serial.begin(115200);
  pinMode(allow_HID_toggle, INPUT_PULLUP);
  espSerial.begin(9600);
  write_reboot();
  initialise_buttons();
  Keyboard.begin(); //begin HID library
  init_oled();
  shutoff_oled_after_time();
  read_allow_HID_toggle();
}

void loop() {
  read_esp_runtime();
  read_esp_status();
  press_fastmode_1_action();
  press_fastmode_2_action();
  press_to_change_mode_action();
  press_to_cycle_mode_action();
  press_spare_action();
  shutoff_oled_after_time();
  read_allow_HID_toggle();
  send_tele_form_string();
  send_slashTeleString();
}
