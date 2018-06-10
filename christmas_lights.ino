#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <FS.h> // SPIFFS library
#include <WebSocketsServer.h>

/* Hex command to open relay */
byte relON[] = {0xA0, 0x01, 0x01, 0xA2};
/* Hex command to close relay */
byte relOFF[] = {0xA0, 0x01, 0x00, 0xA1}; 

const char ssid[10] = "";     //Add here the SSID to which the ESP will connnect to
const char *password = "";    //Add here the password of the SSID to which the ESP will connect to
const char esp_ssid[20] = ""; //Add here the name of ESP and how it will be found using WiFi
const char esp_password[10] = ""; //Add here password which will be used for the WiF Manager
int counter = 0;

WiFiManager wifiManager;

/* Create a webserver object that listens for HTTP request on port 80 */
ESP8266WebServer server(80);
/*WebSocket support */
WebSocketsServer webSocket = WebSocketsServer(81);

/* convert the file extension to the MIME type */
String getContentType(String filename);
/* send the right file to the client (if it exists) */
bool handleFileRead(String path);

void startWebSocket();

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght);

void setup() {
  /* For communication with the uc controlling the relay */
  Serial.begin(9600);
  
  /* Connect to WiFi network with SSID and password */
  WiFi.begin(ssid, password);

  /* Check if able to connect, if unable enter WiFi Manager mode */
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    counter++;
    if (counter > 20){
      wifiManager.autoConnect(esp_ssid, esp_password);
    }
  }

  SPIFFS.begin();

  startWebSocket();

  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  server.begin();
}

void loop() {

  while(WiFi.status() != WL_CONNECTED){
      wifiManager.autoConnect(esp_ssid, esp_password);
  }
  
  webSocket.loop();
  server.handleClient();
}

String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  return "text/plain";
}

bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}

/*WebSocket function */
void startWebSocket() { // Start a WebSocket server
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
  Serial.println("WebSocket server started");
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_TEXT:                     // if new text data is received
    Serial.printf("[%u] get Text: %s\n", num, payload);
      if (payload[0] == 'O' && payload[1] == 'N'){
        Serial.write(relON,sizeof(relON));
      } else if (payload[0] == 'O' && payload[1] == 'F'){
        Serial.write(relOFF, sizeof(relOFF));
      }
      break;
  }
}
