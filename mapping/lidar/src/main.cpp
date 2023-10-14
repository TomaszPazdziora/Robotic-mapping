#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const int POINT_PER_PACK = 12;
#define HEADER 0x54

typedef struct __attribute__((packed)) {
   uint16_t distance;
   uint8_t intensity;
} LidarPointStructDef;

typedef struct __attribute__((packed)) {
   uint8_t header;
   uint8_t ver_len;
   uint16_t speed;
   uint16_t start_angle;
   LidarPointStructDef point[POINT_PER_PACK];
   uint16_t end_angle;
   uint16_t timestamp;
   uint8_t crc8;
} LiDARFrameTypeDef;

const char* ssid = "TP-Link_292F"; 
const char* password = "64857817"; 
const char* serverLidarData = "http://192.168.0.108:5000/lidar_data";

#define BYTES_READ 3000
uint8_t recived[BYTES_READ];
LiDARFrameTypeDef LidarMeasurements;

void clearLidarData();
void readLidarData();
LiDARFrameTypeDef getLidarFrame(int startIndex);
void sendLidarDataToServer(LiDARFrameTypeDef measurement);

void setup() {
  Serial.begin(230400);
  Serial2.begin(230400);
  clearLidarData();
  delay(1000);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wifi");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected\n");
}

void loop() {
    readLidarData();

    for(int i=0; i<BYTES_READ-50; i++) {
      if(recived[i]==0x54 &&recived[i+1]==0x2C) {
        LidarMeasurements = getLidarFrame(i);
        sendLidarDataToServer(LidarMeasurements);
      }
    }
    while(1){}
  }

  void clearLidarData() {
    for(int i=0; i<BYTES_READ; i++) {
      recived[i] = 0;
    }
  }

  void readLidarData() {
    for(int i=0; i<BYTES_READ; i++) {
      while(!Serial2.available());
      recived[i] = Serial2.read();
    }
  }

  LiDARFrameTypeDef getLidarFrame(int i) {
    LiDARFrameTypeDef Frame;
    Frame.speed = ((uint16_t)recived[i+3] << 8) | recived[i+2];
    Frame.start_angle = ((uint16_t)recived[i+5] << 8) | recived[i+4];
    for(int j=0; j<POINT_PER_PACK; j++) {
      Frame.point[j].distance = ((uint16_t)recived[i+7+j*3] << 8) | recived[i+6+j*3];
      Frame.point[j].intensity = recived[i+8+j*3];
    }
    Frame.end_angle = ((uint16_t)recived[i+7+POINT_PER_PACK*3] << 8) | recived[i+6+POINT_PER_PACK*3];
    return Frame;
  }

  void sendLidarDataToServer(LiDARFrameTypeDef measurement) {
  
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverLidarData);
    http.addHeader("Content-Type", "text/plain");

    float end_ang = float(measurement.end_angle) / 100;
    float start_ang = float(measurement.start_angle) / 100;
    Serial.print("end : ");
    Serial.println(end_ang);
    Serial.print("start : ");
    Serial.println(start_ang);

    float step = 0;
    int x[POINT_PER_PACK];
    int y[POINT_PER_PACK];

    if (end_ang > start_ang) step = (end_ang - start_ang) / POINT_PER_PACK;
    else step = (end_ang + 360 - start_ang) / POINT_PER_PACK;

    Serial.print("step : ");
    Serial.println(step);
    Serial.println("---------------------------");

    for(int i=0; i<POINT_PER_PACK; i++) {
      float angle = start_ang + step*i;
      angle = angle * (PI/180); // in radians

      Serial.print("angle : ");
      Serial.println(angle);

      x[i] = measurement.point[i].distance * sin(angle);
      y[i] = measurement.point[i].distance * cos(angle);
      Serial.print("i: ");Serial.print(i);Serial.print(", x: ");Serial.print(x[i]);Serial.print(", y: ");Serial.println(y[i]);
    }

    String dataMessege = "";

    for(int i=0; i<POINT_PER_PACK; i++) {
      dataMessege = dataMessege + String(x[i]);
      dataMessege = dataMessege + String(",");
      dataMessege = dataMessege + String(y[i]);
      dataMessege = dataMessege + String("\n");
    }

    Serial.println(dataMessege);

    int httpResponseCode = http.POST(dataMessege);

    if (httpResponseCode > 0) {
      Serial.println("POST request successful");
    }
    else {
      Serial.println("Error on POST request");
    }
    http.end();
  }
}