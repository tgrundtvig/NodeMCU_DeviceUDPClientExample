#include <Arduino.h>
#include <DeviceUDPClient.h>
#include <ESP8266WiFi.h>        // Include the Wi-Fi library

const char* ssid     = "SSID";         // The SSID (name) of the Wi-Fi network you want to connect to
const char* password = "PASSWORD";     // The password of the Wi-Fi network

//The client will begin by sending a broadcast packet to find the server.
const uint16_t local_port = 2323; //the local port on the NodeMCU
const uint16_t server_port = 3377; //The port on the server to connect to. 

unsigned long last_send = 0;

DeviceUDPClient client(ESP.getChipId(), "TestDevice", 1);

void setup() {
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  
  WiFi.begin(ssid, password);             // Connect to the network
  Serial.print("Connecting to ");
  Serial.print(ssid); Serial.println(" ...");

  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print(' ');
  }

  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer

  //Start client
  client.begin(local_port, server_port);
}

void loop()
{
  unsigned long cur_time = millis();
  client.update(cur_time);

  if(cur_time - last_send > 10000)
  {
    client.sendPacketToServer(42, 1, 2);
    last_send = cur_time;
  }
}
