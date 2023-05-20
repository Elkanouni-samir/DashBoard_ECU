#include <CAN.h>
#include <WiFi.h>
#include <stdint.h>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>



const char* ssid = "S10";
const char* password = "yasser1234";

char data[3];
int RPM_CAN = 0;
int TEMP_CAN = 0;

/*! String serverName = "**";*/


AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "not found");
}

void setup() {

  Serial.begin(9600);
  while (!Serial);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  // put your setup code here, to run once

  Serial.println("CAN Receiver Callback");

  // start the CAN bus at 500 kbps
  CAN.setPins(22, 21);
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  // register the receive callback
  CAN.onReceive(onReceive);

  server.on("/RPM", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(RPM_CAN));
    });

   server.on("/TEMP", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(TEMP_CAN));
    });


  server.onNotFound(notFound);

   server.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

}


void onReceive(int packetSize) {
  // received a packet
  Serial.print("Received ");
  Serial.print("packet with id 0x");
  Serial.print(CAN.packetId(), HEX);

    Serial.print(" and length ");
    Serial.println(packetSize);

    data[0] = (char)CAN.read();
    data[1] = (uint8_t)CAN.read();
    data[2] = (uint8_t)CAN.read();
    switch(data[0])
    {
      case 'R':
      RPM_CAN = data[1]*256 + data[2];
        Serial.print(RPM_CAN);
        Serial.print(" RPM");
        break;
      case 'T':
        TEMP_CAN = data[1]*256 + data[2];
        Serial.print(TEMP_CAN);
        Serial.print(" C");
        break;
      default:
        Serial.print("not recognized");
        break;
    }
    Serial.println();
    Serial.println();

  Serial.println();
}

