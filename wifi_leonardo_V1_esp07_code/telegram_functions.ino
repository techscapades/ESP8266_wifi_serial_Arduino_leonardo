void print_tele_JSON(String tele_string) {
  DynamicJsonDocument text_doc(512);
  String output_string;
  text_doc["tele_string"] = tele_string;
  serializeJson(text_doc, output_string);
  Serial.println(output_string);
  delay(50);
}

void print_slash_tele_JSON(String slash_tele_string) {
  DynamicJsonDocument slash_text_doc(512);
  String output_string;
  slash_text_doc["slash_tele_string"] = slash_tele_string;
  serializeJson(slash_text_doc, output_string);
  Serial.println(output_string);
  delay(50);
}

void print_WiFi_JSON(String ip_string) {
  DynamicJsonDocument ip_doc(32);
  String output_string;
  ip_doc["ip_string"] = ip_string;
  serializeJson(ip_doc, output_string);
  Serial.println(output_string);
  delay(50); 
}

void print_NTP_JSON(String ntp_string) {
  DynamicJsonDocument ntp_doc(32);
  String output_string;
  ntp_doc["ntp_string"] = ntp_string;
  serializeJson(ntp_doc, output_string);
  Serial.println(output_string);
  delay(50);
}


void handleNewMessages(int numNewMessages) {
  /*
    This function will take a message sent from
    telegram, check the username and then if the
    username matches what this is paired to, it
    will serialise it in a JSON doc and println
    it over serial for the leonardo to capture
  */
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    if (chat_id == CHATID) {
      //Serial.println(text);
      if (text.startsWith("/")) {
        print_slash_tele_JSON(text);
      } else {
        print_tele_JSON(text);
      }
      bot.sendMessage(chat_id, "message sent over");
    } else {
      bot.sendMessage(chat_id, "~ unauthorised user >:( ~");
    }
  }
}

void begin_tele_wifi_ntp() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setTrustAnchors(&cert); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  ipaddr = WiFi.localIP().toString().c_str();
  print_WiFi_JSON(ipaddr);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  configTime(0, 0, "pool.ntp.org");      // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    delay(100);
    now = time(nullptr);
  }
  ntpnow = String(now);
  print_NTP_JSON(ntpnow);
}

void tele_runtime() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    bot_lasttime = millis();
  }

  if ((WiFi.status() != WL_CONNECTED) && (millis() - previousMillis_wifi >= interval_wifi)) {
    begin_tele_wifi_ntp();
    previousMillis_wifi = millis();
  }
}
