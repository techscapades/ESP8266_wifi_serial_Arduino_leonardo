# ESP8266_wifi_serial_Arduino_leonardo
A project which uses the ESP07 Serial WiFi module and an arduino leonardo to get texts and commands from a telegram bot a local AP server and it also has elegantasycOTA modes 

Note that if your code in the ESP07 takes more than 502KB, the OTA will fail and it will return with  "[HTTP ERROR] Bad Request" since it has insufficient space to host the code and the OTA code,
in this case its better to use an ESP8266 board with higher memory
