#include <CAN.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>



const char* ssid = "S10";
const char* password = "yasser1234";

char *packet = NULL;

/*! String serverName = "**";*/


AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
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
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  // register the receive callback
  CAN.onReceive(onReceive);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      if(packet == NULL) request->send(404, "text/plain");
        request->send(200, "text/plain", String(*packet));
        free(packet);
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

  // only print packet data for non-RTR packets
  packet = (char*)malloc(packetSize);
  for(int i =0; i<packetSize; i++) {
    *(packet+1) = (char)CAN.read();
    Serial.print(*packet);
  }
  Serial.println();

  Serial.println();
}

