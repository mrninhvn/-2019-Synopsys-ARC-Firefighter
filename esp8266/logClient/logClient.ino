//************************************************************
// this is a simple example that uses the painlessMesh library to 
// setup a node that logs to a central logging node
// The logServer example shows how to configure the central logging nodes
//************************************************************
#include "painlessMesh.h"

#define   MESH_PREFIX     "whateverYouLike"
#define   MESH_PASSWORD   "somethingSneaky"
#define   MESH_PORT       5555

Scheduler     userScheduler; // to control your personal task
painlessMesh  mesh;

// Prototype
void receivedCallback( uint32_t from, String &msg );
void sendBack();

size_t logServerId = 0;

void setup() {
  Serial.begin(115200);
    
  mesh.setDebugMsgTypes( ERROR | STARTUP | CONNECTION );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT, WIFI_AP_STA, 6 );
  mesh.onReceive(&receivedCallback);
//  mesh.onReceive(&sendBack);

}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
}

void receivedCallback( uint32_t from, String &msg ) {
  Serial.printf("logClient: Received from %u msg=%s\n", from, msg.c_str());

  // Saving logServer
#if ARDUINOJSON_VERSION_MAJOR==6
  DynamicJsonDocument jsonBuffer(1024 + msg.length());
  DeserializationError error = deserializeJson(jsonBuffer, msg);
  if (error) {
    Serial.printf("DeserializationError\n");
    return;
  }
  JsonObject root = jsonBuffer.as<JsonObject>();
#else
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(msg);
#endif
  if (root.containsKey("topic")) {
      if (String("logServer").equals(root["topic"].as<String>())) {
          // check for on: true or false
          logServerId = root["nodeId"];
          Serial.printf("logServer detected!!!\n");
      }
      Serial.printf("Handled from %u msg=%s\n", from, msg.c_str());
  }

  sendBack();
  
}

// Send message to the logServer
void sendBack() {
#if ARDUINOJSON_VERSION_MAJOR==6
        DynamicJsonDocument jsonBufferSend(1024);
        JsonObject msgSend = jsonBufferSend.to<JsonObject>();
#else
        DynamicJsonBuffer jsonBufferSend;
        JsonObject& msgSend = jsonBufferSend.createObject();
#endif
    msgSend["topic"] = "sensor";
    msgSend["value"] = random(0, 180);

    String str;
#if ARDUINOJSON_VERSION_MAJOR==6
    serializeJson(msgSend, str);
#else
    msgSend.printTo(str);
#endif
    if (logServerId == 0) // If we don't know the logServer yet
        mesh.sendBroadcast(str);
    else
        mesh.sendSingle(logServerId, str);

    // log to serial
#if ARDUINOJSON_VERSION_MAJOR==6
    serializeJson(msgSend, Serial);
#else
    msgSend.printTo(Serial);
#endif
    Serial.printf("\n");
}
