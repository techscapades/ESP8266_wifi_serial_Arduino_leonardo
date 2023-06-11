
void initialise_buttons() {
  /*
    initialise button pins
  */
  pinMode(press_to_cycle_mode, INPUT_PULLUP);
  pinMode(press_to_change_mode, INPUT_PULLUP);
  pinMode(press_fastmode_1, INPUT_PULLUP);
  pinMode(press_fastmode_2, INPUT_PULLUP);
  pinMode(press_spare, INPUT_PULLUP);

}

void press_spare_action() {

  if (digitalRead(press_spare) == LOW) {
    delay(mode_selector_delay_time);
    //do something you want
    oled_UI();
  }
}

void press_fastmode_1_action() {
  if (digitalRead(press_fastmode_1) == LOW) {
    delay(mode_selector_delay_time);
    select_mode = 1;
    select_character = "";
    fwVersion = "";
    ipAddr = "";
    previous_mode = select_mode;
    end_server();
    delay(100);
    write_reboot();
    oled_UI();
    Serial.println("fast mode 1 pressed");
  }
}

void press_fastmode_2_action() {
  if (digitalRead(press_fastmode_1) == LOW) {
    delay(mode_selector_delay_time);
    select_mode = 2;
    select_character = "";
    fwVersion = "";
    ipAddr = "";
    previous_mode = select_mode;
    write_reboot();
    oled_UI();
    Serial.println("fast mode 2 pressed");
  }
}

void press_to_change_mode_action() {
  /*
    This code is to set/reset the ESP
  */
  if (digitalRead(press_to_change_mode) == LOW) {
    delay(mode_selector_delay_time);
    select_character = "";
    fwVersion = "";
    ipAddr = "";
    if (select_mode != 1) {
      end_server();
      delay(100);
      write_reboot();
    } else {
      write_reboot();
    }
    oled_UI();
    previous_mode = select_mode;
    Serial.println("changed_mode");
  }

}

void press_to_cycle_mode_action() {
  /*
    This code is meant to allow short
    presses to increase mode by 1 and
    long presses to decrease mode by 1
  */

  if (digitalRead(press_to_cycle_mode) == LOW) {
    delay(mode_selector_delay_time);
    select_mode = select_mode + 1;
    if (select_mode > number_of_modes) {
      select_mode = 1;
    }
    if (previous_mode != select_mode) {
      select_character = "*";
    } else {
      select_character = "";
    }
    oled_UI();
    Serial.print("mode: ");
    Serial.println(select_mode);
  }

}
