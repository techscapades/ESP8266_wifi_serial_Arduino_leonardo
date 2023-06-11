void init_oled() {
  if (!myOLED.begin(SSD1306_128X32)) {
    while (1);  // In case the library failed to allocate enough RAM for the display buffer
  }
  myOLED.setFont(SmallFont);
  oled_UI();
}

/*modify this to change the UI on the OLED*/
void oled_UI() {
  show_oled = true;
  previousMillis_oled = millis();
  myOLED.clrScr();
  myOLED.print("Mode: ", 0, 2); //syntax (string, x, y);
  myOLED.print(String(select_mode) , 35, 2);
  myOLED.print("/", 45, 2);
  myOLED.print(String(number_of_modes), 55, 2);
  myOLED.print(select_character, 68, 2);
  myOLED.print("HID: ", 85, 2);
  myOLED.print(String(write_HID), 115, 2);
  myOLED.print(mode_names[select_mode - 1], 0, 13);
  myOLED.print("FW: ", 85, 13);
  myOLED.print(fwVersion, 115, 13);
  myOLED.print("IP: ", 0, 24);
  myOLED.print(ipAddr, 20, 24);
  myOLED.update();
}

void clear_OLED_UI() {
  show_oled = false;
  myOLED.clrScr();
  myOLED.update();
}

void shutoff_oled_after_time() {
  /*
    I've programmed this to clear the OLED dispplay
    so as to not show it unnecessarily and prolong
    its usage life
  */
  if (show_oled) {
    if (millis() - previousMillis_oled >= interval_oled_sleep) {
      clear_OLED_UI();
      previousMillis_oled = millis();
    }
  }
}
