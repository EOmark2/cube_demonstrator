#include "Wire.h"
#include <WiFi.h>
#include <MPU6050_light.h>
#include <Adafruit_NeoPixel.h>

#define PIN 13
MPU6050 mpu(Wire);
const unsigned long eventInterval = 500;
unsigned long previousTime = 0;
bool led_status = false;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(19, PIN, NEO_GRB + NEO_KHZ800);

enum discrete_maneuver{
  LANE_CHANGE_LEFT,
  LANE_CHANGE_RIGHT,
  FASTER_10,
  SLOWER_10,
  unknown
};

enum discrete_maneuver man = unknown;
const char* ssid = "Insert SSID";
const char* password =  "Insert Password";
 
const uint16_t port = 1228;
const char * host = "Insert TCP Server";

void setup() {
  Serial.begin(9600);
  pixels.begin();
  Wire.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("...");
  }
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculating offsets, do not move MPU6050"));
  delay(1000);
  mpu.calcOffsets(); // gyro and accelero
  Serial.println("Done!\n");

  Serial.print("WiFi connected with IP: ");
  Serial.println(WiFi.localIP());

  //Turn led on for connected status
  blink_both(255,255,255); 
}

void loop() {
  mpu.update();
  WiFiClient client;
 
  unsigned long currentTime = millis();

  
  if (!client.connect(host, port)) {
 
        Serial.println("Connection to host failed");
        blink_both(255,0,0);
        return;
  }
  if(currentTime-previousTime >= eventInterval){ // print data every 10ms{
    man = unknown;
    if(mpu.getAngleX() >= 15){
      man = LANE_CHANGE_LEFT;
      client.print(man);
      Serial.println(man);
      if(!led_status){
        set_left_on(0,255,0);
      }else{
        set_left_off();
      }
      led_status = !led_status;
    }else{
        if(mpu.getAngleX() <= -15){
        man = LANE_CHANGE_RIGHT;
        client.print(man);
        Serial.println(man);
        if(!led_status){
          set_right_on(0,255,0);
        }else{
          set_right_off();
        }
        led_status = !led_status; 
        }else{
          if(mpu.getAngleY() <= -15){
          man = FASTER_10;
          client.print(man);
          Serial.println(man);
          if(!led_status){
            set_right_on(0,0,255);
            set_left_on(0,0,255);
          }else{
            set_right_off();
            set_left_off();
          }
          led_status = !led_status; 
          }else{
            if(mpu.getAngleY() >= 15){
            man = SLOWER_10;
            client.print(man);
            Serial.println(man);
            if(!led_status){
              set_right_on(255,0,0);
              set_left_on(255,0,0);
            }else{
              set_right_off();
              set_left_off();
            }
            led_status = !led_status; 
            }else{
              client.print(man);
              set_left_off();
              set_right_off();
          }
        }
      }
    }
    previousTime = currentTime;
  }
  client.stop();
}

void set_right_on(int r, int g, int b){
  for (int i = 0; i < 10; i++) {
  pixels.setPixelColor(i, pixels.Color(r, g, b)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  //delay(200); // Delay for a period of time (in milliseconds).
  }
}

void set_right_off(){
  for (int i = 0; i < 10; i++) {
  pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  //delay(200); // Delay for a period of time (in milliseconds).
  }
}

void set_left_on(int r, int g, int b){
  for (int i = 10; i < 20; i++) {
  pixels.setPixelColor(i, pixels.Color(r, g, b)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  //delay(200); // Delay for a period of time (in milliseconds).
  }
}

void set_left_off(){
  for (int i = 10; i < 20; i++) {
  pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright green color.
  pixels.show(); // This sends the updated pixel color to the hardware.
  //delay(200); // Delay for a period of time (in milliseconds).
  }
}

void blink_both(int r, int g, int b){
  for(int i =0; i < 3; i++){
    set_left_on(r,g,b);
    set_right_on(r,g,b);
    delay(500);
    set_left_off();
    set_right_off();
    delay(500);
  }
}
