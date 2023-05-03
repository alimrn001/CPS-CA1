#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#define SHT25_ADDR 0x40
#define HUMIDITY_CMD 0xF5
#define TEMPERATURE_CMD 0xF3

#define BLUETOOTH_TRANSMITION_START_CHAR '@'
#define BLUETOOTH_TRANSMITION_END_CHAR '#'
#define BLUETOOTH_TRANSMITION_DEL_CHAR '/'
#define BLUETOOTH_RX 3
#define BLUETOOTH_TX 2
#define BLUETOOTH_BAUD_RATE 9600

String decision;

long double last_tick;

void setup() {
  Wire.begin();
  Serial.setTimeout(0);
  Serial.begin(BLUETOOTH_BAUD_RATE);
  last_tick = millis();
}

void sendCMD(int cmd){
  Wire.beginTransmission(SHT25_ADDR);
  Wire.write(cmd);
  Wire.endTransmission();
}

float receiveData(unsigned int data[2]){
  Wire.requestFrom(SHT25_ADDR,2);
  if (Wire.available() == 2) {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
}

boolean has_ticked(){
  long double now = millis();
  if (now - last_tick >= 4000){
    last_tick = now;
    return true;
  }
  return false;
}

float getHumidity(){
  unsigned int data[2];
  sendCMD(HUMIDITY_CMD);
  delay(500);
  receiveData(data);
  return (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
}

float getCTemperature(){
  unsigned int data[2];
  sendCMD(TEMPERATURE_CMD);
  delay(500);
  receiveData(data);
  return (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
}

void sendWithBluetooth(float temperature,float humidity){
  String message = String(humidity) + BLUETOOTH_TRANSMITION_DEL_CHAR + String(temperature) + BLUETOOTH_TRANSMITION_END_CHAR;
  Serial.println(message);
}

void loop() {
  if(has_ticked()){
    float humidity = getHumidity();
    float temperature = getCTemperature();
    sendWithBluetooth(temperature, humidity);
  }
}
