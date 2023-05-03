#include <Arduino.h>
#include <SoftwareSerial.h>

#define BLUETOOTH_TRANSMITION_START_CHAR '@'
#define BLUETOOTH_TRANSMITION_END_CHAR '#'
#define BLUETOOTH_TRANSMITION_DEL_CHAR '/'
#define BLUETOOTH_RX 3
#define BLUETOOTH_TX 2
#define BLUETOOTH_BAUD_RATE 9600
#define PWM_MAX 64
#define PWM_MIN 0

String decision;
String bluetooth_buffer = "";
int pwm_counter = 0;
int pwm_velocity = 64;
int dc_pinA = 9;
int dc_pinB = 10;

void setup() {
  pinMode(dc_pinA,OUTPUT);
  pinMode(dc_pinB,OUTPUT);
  Serial.setTimeout(0);
  Serial.begin(BLUETOOTH_BAUD_RATE);
}

void handlePWM(){
    analogWrite(dc_pinA, pwm_velocity);
    digitalWrite(dc_pinB,0);
}

void readBluetooth(){
  if (Serial.available() > 1){
    String data = Serial.readString();
    if (data != ""){
      bluetooth_buffer += data;
      if(data.indexOf(BLUETOOTH_TRANSMITION_END_CHAR) != -1){
        String decision = bluetooth_buffer.substring(0, bluetooth_buffer.length() - 1);
        pwm_velocity = atoi(&decision[0]);
        bluetooth_buffer = "";
      }
    }
  }
}

void loop() {
  readBluetooth();
  handlePWM();
}
