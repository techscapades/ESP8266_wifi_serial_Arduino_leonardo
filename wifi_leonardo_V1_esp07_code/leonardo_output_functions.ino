void send_leonardo_startup_message() {
  /*
     Send leonardo a message to let it
     know that ESP has just rebooted
  */
  DynamicJsonDocument startup_doc(32);
  String output_string;
  startup_doc["esp_status"] = "rebooted";
  serializeJson(startup_doc, output_string);
  Serial.println(output_string);
  delay(50);
}

void send_leonardo_success_message() {
  /*
    Send leonardo a message to let it
    know that the mode has been set
  */
  DynamicJsonDocument startup_doc(32);
  String output_string;
  startup_doc["esp_status"] = "setup_complete";
  serializeJson(startup_doc, output_string);
  Serial.println(output_string);
  delay(50);
}

void send_leonardo_fw_version() {
  DynamicJsonDocument fw_doc(32);
  String output_string;
  fw_doc["fw_version"] = fw_version;
  serializeJson(fw_doc, output_string);
  Serial.println(output_string);
  delay(50);
}

void send_leonardo_current_mode() {
  DynamicJsonDocument curr_mode_doc(32);
  String output_string;
  curr_mode_doc["esp_current_mode"] = String(select_mode);
  serializeJson(curr_mode_doc, output_string);
  Serial.println(output_string);
  delay(50);
}

void await_leonardo_mode_message() {
  send_leonardo_startup_message();
  delay(50);
  byte setup_timeout_counter;
  while (select_mode == 0) {
    if (Serial.available() > 0) {
      serial_input = Serial.readStringUntil('\n');
      Serial.println(serial_input);
      DeserializationError error = deserializeJson(doc, serial_input);
      if (error) {
        Serial.println("error");
        return;
      } else {
        serial_input_json = doc.as<JsonObject>();
        select_mode = serial_input_json["esp_mode"];
      }
    }

    if (select_mode > 0) {
      break;
    }

    if (millis() - previousMillis_leonardo >= interval_leonardo) {
      send_leonardo_startup_message();
      previousMillis_leonardo = millis();
    }

    setup_timeout_counter++;

    if (setup_timeout_counter > setup_timeout_counter_max_tries) {
      /*
        enter setup anyway once setup times out
      */
      break;
    }

  }
  send_leonardo_success_message();
  send_leonardo_fw_version();
}

void read_leonardo_runtime() {
  if (millis() - previousMillis_serial >= interval_serial) {
    if (Serial.available() > 0) {
      serial_input = Serial.readStringUntil('\n');
      Serial.println(serial_input);
      DeserializationError error = deserializeJson(doc, serial_input);
      if (error) {
        Serial.println("error");
        return;
      } else {
        serial_input_json = doc.as<JsonObject>();
        int reboot = serial_input_json["reboot"];
        int end_server = serial_input_json["end_server"];

        if (reboot == 1) {
          /*
            Send this when user wants to switch mode
          */
          ESP.restart();
        }

        if (end_server == 1) {
          /*
            send this to close the AP server
          */
          end_AP_server();
        }

      }
    }
    previousMillis_serial = millis();
  }
}
