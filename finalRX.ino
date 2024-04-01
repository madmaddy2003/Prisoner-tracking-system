#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#define RX 5
#define TX 4
SoftwareSerial LoRaSerial(RX, TX); // RX, TX

String apiKey = "XC15GK9JLTQI9LPV";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "BSNL-FTTH@WIFI";     // replace with your wifi ssid and wpa2 key
const char *pass =  "4132190577";
const char* server = "api.thingspeak.com";
WiFiClient client;

void setup() {
  Serial.begin(115200); // USB Serial
  delay(1000);
  LoRaSerial.begin(115200); 
       Serial.println("Connecting to ");
       Serial.println(ssid);
       WiFi.begin(ssid, pass);
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");// put your setup code here, to run once:

}

void loop() {
  String data1,data2;
  // Check if data is available on Software Serial
  while (LoRaSerial.available()) {
    String receivedData = LoRaSerial.readStringUntil('\n'); // Read until a newline character is encountered.
     Serial.println(receivedData);
    // Now, you can discriminate between data1 and data2 based on the received data.
    if (receivedData.startsWith("DATA1:")) {
      data1 = receivedData.substring(6);
      // Serial.println(data1);
       
      // Extract the actual data after "DATA1:"
      // Handle data1 as needed.
    } else if (receivedData.startsWith("DATA2:")) {
      data2 = receivedData.substring(6);
      // Serial.println(data2); // Extract the actual data after "DATA2:"
      // Handle data2 as needed.
    }
  }


  float lat=100;
  float log=200;
  
  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
  {  
                            
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += data1;
    postStr +="&field2=";
    postStr += data2;
    postStr +="&field3=";
    postStr += String(log);
    postStr += "\r\n\r\n";
 
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    Serial.println("%. Send to Thingspeak.");
  }
    client.stop();
    Serial.println("Waiting...");
  
  // thingspeak needs minimum 15 sec delay between updates
delay(10000);
// delay(1000);
}
