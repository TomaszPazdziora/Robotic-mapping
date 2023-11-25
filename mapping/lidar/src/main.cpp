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

struct Position {
  int x;
  int y;
  float angle;
};

#define BITMAP_SIZE 250
const int RESOLUTION = 40;
uint8_t occupancy_array[BITMAP_SIZE][BITMAP_SIZE];

const char* ssid = "* UPC6948437-2.4"; 
const char* password = "vESck6wbmddj"; 
const char* serverLidarData =          "http://192.168.0.38:5000/lidar_data";
const char* serverReadyToScanAddress = "http://192.168.0.38:5000/ready_to_scan";
const char* serverCurrentPosition =    "http://192.168.0.38:5000/current_position";
const char* serverOccupancy =          "http://192.168.0.38:5000/occupancy";
const char* serverScanSent =           "http://192.168.0.38:5000/scan_sent";


#define BYTES_READ 3000
uint8_t recived[BYTES_READ];
LiDARFrameTypeDef LidarMeasurements;

void clearLidarData();
void readLidarData();
void clearBitmap();
void sendOccupancyBitmapToServer();
LiDARFrameTypeDef getLidarFrame(int startIndex);
void sendLidarDataToServer(LiDARFrameTypeDef measurement);
String httpGETRequest(const char* serverAddress);
void updateCurrentPosition();
void setLidarDataSentFlag();
void postEndOfMeasFlag();
Position Current_Position;

void setup() {
  Serial.begin(230400);
  Serial2.begin(230400);
  clearLidarData();
  Current_Position.x = 0;
  Current_Position.y = 0;
  Current_Position.angle = 0;
  clearBitmap();
  delay(500);

  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wifi");

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConnected\n");
}

void loop() {
    if (httpGETRequest(serverReadyToScanAddress) == String("ready")) {
      updateCurrentPosition();
      delay(1000);
      readLidarData();
      for(int i=0; i<BYTES_READ-50; i++) {
        if(recived[i]==0x54 && recived[i+1]==0x2C) {
          LidarMeasurements = getLidarFrame(i);
          sendLidarDataToServer(LidarMeasurements);
        }
      }
      postEndOfMeasFlag();
      sendOccupancyBitmapToServer();
      setLidarDataSentFlag();
    }
    delay(1000);
  }

  void clearLidarData() {
    for(int i=0; i<BYTES_READ; i++) {
      recived[i] = 0;
    }
  }

  void clearBitmap() {
    for (int i=0; i <BITMAP_SIZE; i++) {
      for (int j=0; j <BITMAP_SIZE; j++) {
        occupancy_array[i][j] = 0;
      }
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

    float end_ang = Current_Position.angle + float(measurement.end_angle) / 100;
    float start_ang = Current_Position.angle + float(measurement.start_angle) / 100;
    // Serial.print("end : ");
    // Serial.println(end_ang);
    // Serial.print("start : ");
    // Serial.println(start_ang);

    float step = 0;
    int x[POINT_PER_PACK];
    int y[POINT_PER_PACK];

    if (end_ang > start_ang) step = (end_ang - start_ang) / POINT_PER_PACK;
    else step                     = (end_ang + 360 - start_ang) / POINT_PER_PACK;

    // Serial.print("step : ");
    // Serial.println(step);
    // Serial.println("---------------------------");

    for(int i=0; i<POINT_PER_PACK; i++) {
      float angle = start_ang + step*i;
      angle = angle * (PI/180); // in radians

      // Serial.print("angle : ");
      // Serial.println(angle);
      // Serial.print("$");

      if (measurement.point[i].distance == 0) continue;

      x[i] = Current_Position.x + measurement.point[i].distance * sin(angle);
      y[i] = Current_Position.y + measurement.point[i].distance * cos(angle);

      int occ_x = round((x[i]+5000) / RESOLUTION);
      int occ_y = round((y[i]+5000) / RESOLUTION);

      if (occ_x < 0 || occ_x > BITMAP_SIZE) continue;
      if (occ_y < 0 || occ_y > BITMAP_SIZE) continue;
      
      occupancy_array[occ_x][occ_y] = 1;

      /// y 5000 -5000
      /// x 5000 -5000

      // Serial.print("i: ");Serial.print(i);Serial.print(", x: ");Serial.print(x[i]);Serial.print(", y: ");Serial.println(y[i]);
    }
    Serial.print("\n");

    String dataMessege = "";

    for(int i=0; i<POINT_PER_PACK; i++) {
      dataMessege = dataMessege + String(x[i]);
      dataMessege = dataMessege + String(",");
      dataMessege = dataMessege + String(y[i]);
      dataMessege = dataMessege + String("\n");
    }
    // Serial.println(dataMessege);

    int httpResponseCode = http.POST(dataMessege);  

    // if (httpResponseCode > 0) {
    //   Serial.println("POST request successful");
    // }
    // else {
    //   Serial.println("Error on POST request");
    // }
    http.end();
  }
}

void postEndOfMeasFlag() {
  
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverLidarData);
    http.addHeader("Content-Type", "text/plain");

    int httpResponseCode = http.POST(String('e'));  
    http.end();
  }
}


String httpGETRequest(const char* serverAddress) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
    // Your IP address with path or Domain name with URL path 
    http.begin(client, serverAddress);
  
  
  // Send HTTP POST request
  int httpResponseCode = http.GET(); 
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    // Serial.print("HTTP Response code: ");
    // Serial.println(httpResponseCode);
    payload = http.getString();
  }
  
  // else {
  //   Serial.print("Error code: ");
  //   Serial.println(httpResponseCode);
  // }
  
  http.end();
  return payload;
  }
  else return "not connected";
}

void updateCurrentPosition() {
  String position = httpGETRequest(serverCurrentPosition);
  String fields[3];
  // for (int i=0; i < position.length(); i++) {
  //   fields[i] = "";
  // }
  // recieved message
  // x\ny\nangle\n
  int cnt = 0;
  for (int i=0; i < position.length(); i++) {
    if (position[i] == '\n') cnt++;
    else fields[cnt] += position[i]; 
  }
  // Serial.print("pos: ");
  // Serial.println(position);
  // Serial.print("f0: ");
  // Serial.println(fields[0]);
  // Serial.print("f1: ");
  // Serial.println(fields[1]);
  // Serial.print("f2: ");
  // Serial.println(fields[2]);

  Current_Position.x = fields[0].toInt();
  Current_Position.y = fields[1].toInt();
  Current_Position.angle = fields[2].toFloat();
  // Serial.print("x: ");
  // Serial.println(Current_Position.x);
  // Serial.print("y: ");
  // Serial.println(Current_Position.y);
  // Serial.print("angle: ");
  // Serial.println(Current_Position.angle);
}


void sendOccupancyBitmapToServer() {
  
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverOccupancy);
    http.addHeader("Content-Type", "text/plain");

    String dataMessege = "";
    
    for (int y=0; y <BITMAP_SIZE; y++) {
      for (int x=0; x <BITMAP_SIZE; x++) {
        if(occupancy_array[x][y] == 1) {
          dataMessege = dataMessege + String(x);
          dataMessege = dataMessege + String(",");
          dataMessege = dataMessege + String(y);
          dataMessege = dataMessege + String("\n");
        }
      }
    }

    int httpResponseCode = http.POST(dataMessege);
    http.end();
  }
}

  void setLidarDataSentFlag() {
  
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverScanSent);
    http.addHeader("Content-Type", "text/plain");

    String dataMessege = String("ready");
    int httpResponseCode = http.POST(dataMessege);
    http.end();
  }
}
