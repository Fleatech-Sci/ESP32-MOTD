#include <WiFi.h>
#include <WebServer.h>
#include <pgmspace.h>
#include "wifi_secrets.h"

// TODO: gather your wifi credentials here (and keep it private)
char ssid[] = SECRET_SSID;
char password[] = SECRET_PASS;

WebServer server(80);

// Version information
#define VERSION_FILENAME __FILE__
#define VERSION_DATE __DATE__
#define VERSION_TIME __TIME__
#define VERSION_GCC __VERSION__
char VERSION_FUNCTION[] = MOTD;
char VERSION_AUTHOR[] = AUTHOR;

// Store the HTML template in PROGMEM
// The "R" before the first quotation mark enables raw string literals in C++
// which preserves formatting and allows for multi-line strings without escape sequences
const char MOTD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Message of the Day</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 20px;
            background-color: #f0f0f0;
            color: #333;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
            background-color: #fff;
            padding: 20px;
            border-radius: 8px;
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
        }
        h1 {
            color: #2c3e50;
            text-align: center;
        }
        .motd {
            padding: 15px;
            background-color: #e8f4f8;
            border-left: 4px solid #3498db;
            margin: 20px 0;
            font-size: 1.1em;
            line-height: 1.5;
        }
        .version-info {
            font-family: monospace;
            background-color: #f8f9fa;
            padding: 12px;
            border-radius: 4px;
            font-size: 0.9em;
            margin-top: 20px;
            white-space: pre-wrap;
        }
        .timestamp {
            text-align: right;
            color: #7f8c8d;
            font-size: 0.9em;
            margin-top: 20px;
        }
        .footer {
            text-align: center;
            margin-top: 30px;
            font-size: 0.8em;
            color: #95a5a6;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>System Information</h1>
        
        <div class="motd">
            Basic MOTD Template Function for ESP32 Devices, Tested on XIAO ESP32 Modules [C3 or S3].
        </div>
        
        <div class="version-info">
Function: %FUNCTIONNAME%"
Sketch:   %FILENAME%
Compiled: %DATE% %TIME%
GCC:      %VERSION%
Core Functional Code Designed & Rewritten by: %AUTHOR%
        </div>
        
        <div class="timestamp">
            Current time: <span id="datetime">-</span>
        </div>
        
        <div class="footer">
            Powered by ESP32 Webserver
        </div>
    </div>

    <script>
        // Simple script to display current date
        document.getElementById('datetime').textContent = new Date().toLocaleString();
    </script>
</body>
</html>
)rawliteral";

// Function to replace placeholders with actual version data
String processHTML() {
  String html = FPSTR(MOTD_HTML);  // Get HTML from PROGMEM
  
  // Replace placeholders with actual values
  html.replace("%FUNCTIONNAME%", VERSION_FUNCTION);
  html.replace("%FILENAME%", VERSION_FILENAME);
  html.replace("%DATE%", VERSION_DATE);
  html.replace("%TIME%", VERSION_TIME);
  html.replace("%VERSION%", VERSION_GCC);
  html.replace("%AUTHOR%", VERSION_AUTHOR);
  
  return html;
}

void handleRoot() {
  server.send(200, "text/html", processHTML());
}

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  // Print IP address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
  // Set up web server routes
  server.on("/", handleRoot);
  server.begin();
  
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}