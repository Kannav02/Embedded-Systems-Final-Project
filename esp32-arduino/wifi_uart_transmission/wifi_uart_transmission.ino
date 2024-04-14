#include <WiFi.h>
#include <WiFiClient.h>

 const char* ssid = "Kinza125";
 const char* password = "67361923763";
 const char* host = "10.1.1.28"; // IP address of the server


const uint16_t port = 8080;

WiFiClient client;

void connect_to_wifi(){

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void clearSerialBuffer() {
    char dummy;
    while (Serial2.available()) {
        Serial2.read(&dummy, 1); // Read and discard the byte
    }
}

void setup() {
  // debug console
  Serial.begin(9600);  
  // esp32
  Serial2.begin(9600, SERIAL_8N1, 16, 17); 

  clearSerialBuffer();

  //connecting to wifi
  connect_to_wifi();
}

void loop() {
  if (Serial2.available()) {

    String incomingMessage = Serial2.readStringUntil('\n');
    incomingMessage.trim(); 
    
    if (incomingMessage.length() > 0) {

      //Serial.print("Sending message to server: ");
      Serial.println(incomingMessage);

      
      // Attempt to connect to the server
      if (client.connect(host, port)) {
        Serial.println("Connected to server");
        
        
        client.println(incomingMessage);  

        while (!client.available()) {
          delay(100);  
        }
        
        String serverResponse = client.readStringUntil('\n');
        Serial.print("Response from server: ");
        Serial.println(serverResponse);


        Serial2.println(serverResponse);
        
        client.stop(); 
      } 
      else {
        delay(100);
      }
    }
  }
  delay(1000); 

