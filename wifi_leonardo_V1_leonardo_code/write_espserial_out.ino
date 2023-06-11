void write_esp_mode(byte esp_mode) {
    DynamicJsonDocument esp_doc(32);
    String output_string;
    esp_doc["esp_mode"] = esp_mode;
    serializeJson(esp_doc, output_string);
    espSerial.println(output_string);
    Serial.println(output_string);
}

void write_reboot() {
    DynamicJsonDocument esp_doc(32);
    String output_string;
    esp_doc["reboot"] = 1;
    serializeJson(esp_doc, output_string);
    espSerial.println(output_string);
    Serial.println(output_string);
}

void end_server() {
    DynamicJsonDocument esp_doc(32);
    String output_string;
    esp_doc["end_server"] = 1;
    serializeJson(esp_doc, output_string);
    espSerial.println(output_string);
    Serial.println(output_string);
}
