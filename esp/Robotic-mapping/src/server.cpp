#include "server.h"

String sensorReadings;
int sensorReadingsArr[3];

void readDataFromServer() {
  if(WiFi.status()== WL_CONNECTED){
    sensorReadings = httpGETRequest(serverName);
    Serial.println(sensorReadings);
    JSONVar myObject = JSON.parse(sensorReadings);
  
    // JSON.typeof(jsonVar) can be used to get the type of the var
    if (JSON.typeof(myObject) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }
  
    Serial.print("JSON object = ");
    Serial.println(myObject);
  
    // myObject.keys() can be used to get an array of all the keys in the object
    JSONVar keys = myObject.keys();
    Serial.print("Length = ");
    Serial.println(keys.length());

    for (int i = 0; i < keys.length(); i++) {
      Serial.print("i = ");
      Serial.println(i);
      JSONVar value = myObject[keys[i]];
      Serial.print(keys[i]);
      Serial.print(" = ");
      Serial.println(value);
      sensorReadingsArr[i] = (int)value;
    }

    state = sensorReadingsArr[0];
    LeftMotor.speed = sensorReadingsArr[1];
    RightMotor.speed = sensorReadingsArr[2];
  }

  else {
    Serial.println("WiFi Disconnected");
  }
}


String httpGETRequest(const char* serverName) {
  WiFiClient client;
  HTTPClient http;
  // Your IP address with path or Domain name with URL path 
  http.begin(client, serverName);
   
  // If you need Node-RED/server authentication, insert user and password below
  // http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
  
  // Send HTTP POST request
  int httpResponseCode = http.GET(); 
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
  return payload;
}