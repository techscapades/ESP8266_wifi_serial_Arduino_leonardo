
void read_allow_HID_toggle() {
  if (digitalRead(allow_HID_toggle) == LOW) {
    write_HID = true;
  } else {
    write_HID = false;
  }
  if (write_HID != prev_write_HID) {
    oled_UI();
    prev_write_HID = write_HID;
    Serial.print("write_HID: ");
    Serial.println(write_HID);
  }
}

void send_keyboard_string(String input) {
  if (input != "null") {
    if (input != "" && write_HID) {
      Serial.print("input: ");
      Serial.println(input);
      Keyboard.print(input);
      previousMillis_keyboard = millis();
      while (true) {
        if (millis() - previousMillis_keyboard > interval_keyboard) {
          break;
        }
      }
    }
  }
}

void send_slash_string(String input) {
  if (input != "null") {
    if (input != "" && write_HID) {
      if (input == "/volume_up") {
        for (int i = 0; i < iterate_volume; i++) {
          Consumer.write(MEDIA_VOLUME_UP);
          delay(50);
        }
      }
      if (input == "/volume_down") {
        for (int i = 0; i < iterate_volume; i++) {
          Consumer.write(MEDIA_VOLUME_DOWN);
          delay(50);
        }
      }
      if (input == "/play_pause") {
        Consumer.write(MEDIA_PLAY_PAUSE);
        delay(50);
      }
      if (input == "/stop") {
        Consumer.write(MEDIA_STOP);
        delay(50);
      }
    }
  }
}

void send_tele_form_string() {
  send_keyboard_string(teleString);
  teleString = "";
  send_keyboard_string(formString);
  formString = "";
}

void send_slashTeleString() {
  send_slash_string(slashTeleString);
  slashTeleString = "";
}
