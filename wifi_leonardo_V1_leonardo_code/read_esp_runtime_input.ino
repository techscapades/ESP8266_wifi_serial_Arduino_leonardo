
void read_esp_status() {
  if (espStatus != "") {
    if (espStatus == "rebooted") {
      write_esp_mode(select_mode);
      delay(50);
    }
    if (espStatus == "setup_complete") {
      //do something here
    }
    espStatus = "";
  }
}

void read_esp_runtime() {
  if (millis() - previousMillis_serial >= interval_serial) {
    if (espSerial.available() > 0) {
      serial_input = espSerial.readStringUntil('\n');
      Serial.println(serial_input);
      DeserializationError error = deserializeJson(doc, serial_input);
      if (error) {
        Serial.println("error");
        return;
      } else {
        serial_input_json = doc.as<JsonObject>();
        String esp_status = serial_input_json["esp_status"].as<String>();
        String ipaddr = serial_input_json["ip_string"].as<String>();
        String ntp_string = serial_input_json["ntp_string"].as<String>();
        String tele_string = serial_input_json["tele_string"].as<String>();
        String form_string = serial_input_json["form_string"].as<String>();
        String slash_tele_string = serial_input_json["slash_tele_string"].as<String>();
        String fw_version = serial_input_json["fw_version"].as<String>();

        if (esp_status != "") {
          espStatus = esp_status;
        }
        if (ipaddr != "" && ipaddr.toInt()>0) {
          ipAddr = ipaddr;
          oled_UI();
        }
        if (ntp_string != "") {
          ntpString = ntp_string;
        }
        if (tele_string != "") {
          teleString = tele_string;
        }
        if (slash_tele_string != "") {
          slashTeleString = slash_tele_string;
        }
        if (form_string != "") {
          formString = form_string;
        }
        if (fw_version != "" && fw_version.toInt()>0) {
          fwVersion = fw_version;
          oled_UI();
        }
      }
    }
    previousMillis_serial = millis();
  }
}
