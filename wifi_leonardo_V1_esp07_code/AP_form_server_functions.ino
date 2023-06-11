void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void print_form_JSON(String form_string) {
  DynamicJsonDocument text_doc(512);
  String output_string;
  text_doc["form_string"] = form_string;
  serializeJson(text_doc, output_string);
  Serial.println(output_string);
  delay(50);
}

void begin_AP_form_server() {
  WiFi.mode(WIFI_STA);
  WiFi.softAP(APSSID, APPASS);
  String IP = WiFi.softAPIP().toString().c_str();
  print_WiFi_JSON(IP);
  //Serial.print("AP IP address: ");
  //Serial.println(IP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest * request) {
    if (request->hasParam(text_box_input_1)) {
      text_output = request->getParam(text_box_input_1)->value();
    }
    //Serial.println(text_output);
    print_form_JSON(text_output);
    request->send_P(200, "text/html", index_html);
  });

  server.onNotFound(notFound);
  server.begin();
}

void end_AP_server() {
  server.end();
}

void begin_AP_OTA_server() {
  
  WiFi.mode(WIFI_STA);
  WiFi.softAP(APSSID, APPASS);
  String IP = WiFi.softAPIP().toString().c_str();
  print_WiFi_JSON(IP);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", "<br><a href=\"/update\">Click to enter update portal</a>");
  });

  AsyncElegantOTA.begin(&server, OTAUSER, OTAPASS);    // Start ElegantOTA
  server.begin();
}

void begin_client_OTA_server() {

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  ipaddr = WiFi.localIP().toString().c_str();
  print_WiFi_JSON(ipaddr);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/html", "<br><a href=\"/update\">Click to enter update portal</a>");
  });

  AsyncElegantOTA.begin(&server, OTAUSER, OTAPASS);    // Start ElegantOTA
  server.begin();
}
