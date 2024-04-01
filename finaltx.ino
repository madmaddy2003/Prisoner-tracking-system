#include <SoftwareSerial.h>
int vs =9;
#define IR_SENSOR_RIGHT 11
#define L_RX 3
#define L_TX 2
SoftwareSerial LoRaSerial(L_RX, L_TX); // RX, TX
#define BAUDRATE 115200


void setup() {
  pinMode(vs, INPUT); 
  pinMode(IR_SENSOR_RIGHT, INPUT);
  Serial.begin(BAUDRATE); // USB Serial
  delay(1000);
  LoRaSerial.begin(BAUDRATE);

}

void loop() {
  //initializations
  String data1;
  String data2;

  // vibration sensor
  long measurement=pulseIn (vs, HIGH);  //wait for the pin to get HIGH and returns measurement
  if (measurement>40000)
  {
    Serial.println("abnormal vibration detected");
    data1="DATA1:1";
  }
  else
  {
    Serial.println("normal condition");
    data1="DATA1:0";
  }
  // IR sensor
  int rightIRSensorValue = digitalRead(IR_SENSOR_RIGHT);
  if (rightIRSensorValue == LOW )
  {
    Serial.println("properly weared");
    data2="DATA2:0";
  }
  else{
    Serial.println("device removed");
    data2="DATA2:1";
  }
  
  char charArray1[20];
  char charArray2[20];
  data1.toCharArray(charArray1, sizeof(charArray1));
  data2.toCharArray(charArray2, sizeof(charArray2));
    sendLoraData(data1, 2);
    Serial.println(data1);
    delay(10);
    sendLoraData(data2, 2);
    Serial.println(data2);
    Serial.flush();
    delay(500);

}
void sendLoraData(String data, int address) {
  String myString = "AT+SEND=" + String(address) + "," + String(data.length()) + "," + data + "\r\n";
  char* buf = (char*) malloc(sizeof(char) * myString.length() + 1);
  Serial.println(myString);
  myString.toCharArray(buf, myString.length() + 1);
  LoRaSerial.write(buf);
  free(buf);
}
