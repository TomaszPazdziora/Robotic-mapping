#include "server.h"

String serverData;
int serverDataArr[3];

void readDataFromServer() {
  if(WiFi.status()== WL_CONNECTED){
    serverData = httpGETRequest(serverDataAddress);
    Serial.println(serverData);
    JSONVar myObject = JSON.parse(serverData);
  
    // JSON.typeof(jsonVar) can be used to get the type of the var
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }
  
    // Serial.print("JSON object = ");
    // Serial.println(myObject);
  
    // myObject.keys() can be used to get an array of all the keys in the object
    JSONVar keys = myObject.keys();
    // Serial.print("Length = ");
    // Serial.println(keys.length());

    for (int i = 0; i < keys.length(); i++) {
      JSONVar value = myObject[keys[i]];
      serverDataArr[i] = (int)value;
    }

    state = serverDataArr[0];
    LeftMotor.setSpeed(serverDataArr[1]);
    RightMotor.setSpeed(serverDataArr[2]);
  }

  else {
    Serial.println("WiFi Disconnected");
  }
}


String httpGETRequest(const char* serverAddress) {
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
  
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
  return payload;
}

void readyForScan() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverLidarReadyAddress);
    http.addHeader("Content-Type", "text/plain");

    String dataMessege = "ready";
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

void sendCurrPosToServer(Position CurrentPosition){
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, serverCurrentPosition);
    http.addHeader("Content-Type", "text/plain");

    String dataMessege = "";
    dataMessege += String(CurrentPosition.x);
    dataMessege += String("\n");
    dataMessege += String(CurrentPosition.y);
    dataMessege += String("\n");
    dataMessege += String(CurrentPosition.angle);
    dataMessege += String("\n");

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

bool isLidarDataSent() {
  String response = httpGETRequest(serverLidarDataSent);
  if (response == String("ready")) return true;
  else return false;
}