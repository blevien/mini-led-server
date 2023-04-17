#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Adafruit_NeoPixel.h>

// Resources
// https://davidjwatts.com/youtube/esp8266/esp-convertHTM.html#

#define STASSID "YOUR_SSID"
#define STAPSK  "YOUR_PASSWORD"
#define NUM_PIXELS 2

const char* ssid = STASSID;
const char* password = STAPSK;

String newHostname = "your-device-name";

ESP8266WebServer server(80);

Adafruit_NeoPixel pixels(NUM_PIXELS, D4, NEO_GRB | NEO_KHZ800);


void setColor(uint32_t color) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    pixels.setPixelColor(i, color);
    pixels.show();
    delay(50);
  }
}

void handleRoot() {

  String myVariable = "TESTING TEXT";
  String html ="<!doctype html> <html lang=\"en\"> <head> <meta charset=\"utf-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <title>Kai Hex Tiles</title> <link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-KK94CHFLLe+nY2dmCWGMq91rCGa5gtU4mk92HdvYe+M/SXH301p5ILy+dN9+nJOZ\" crossorigin=\"anonymous\"> </head>";
          html += "<div class=\"container-fluid\">";
          html += "<body> <h1 class=\"text-center\">What do you want to do with your lights...?</h1>";
          html += "<div class=\"d-grid gap-1 col-8 mx-auto\">";
          html += "<a class=\"btn btn-danger \" href=\"red\" role=\"button\">Red</a>";
          html += "<a class=\"btn btn-success\" href=\"green\" role=\"button\">Green</a>";
          html += "<a class=\"btn btn-primary\" href=\"blue\" role=\"button\">Blue</a>";
          html += "</div>";
          html += "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha3/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-ENjdO4Dr2bkBIFxQpeoTz1HIcje39Wm4jDKdf19U8gI4ddQ3GYNS7NTKfAdVQSZe\" crossorigin=\"anonymous\"></script> </body> </html>";

  server.send(200, "text/html", html);

}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  pixels.begin();

  Serial.begin(115200);
  WiFi.hostname(newHostname.c_str());
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/red", []() {
    setColor(pixels.Color(100, 0, 0));
    server.sendHeader("Location","/");
    server.send(303); 
  });

  server.on("/green", []() {
    setColor(pixels.Color(0, 100, 0));
    server.sendHeader("Location","/");
    server.send(303); 
  });

  server.on("/blue", []() {
    setColor(pixels.Color(0, 0, 100));
    server.sendHeader("Location","/");
    server.send(303); 
  });


  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
