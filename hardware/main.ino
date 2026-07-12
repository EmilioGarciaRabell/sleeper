#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // 1. Point to your endpoint
    http.begin("http://your-server-endpoint.com/upload");
    
    // 2. Specify content type (e.g., if you are sending JSON)
    http.addHeader("Content-Type", "application/json");
    
    // 3. Send the POST request
    int httpResponseCode = http.POST("{\"data\": \"hello_from_nano\"}");
    
    // 4. Check the response
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(response);
    }
    
    http.end(); // Free resources
  }
  delay(10000); // Wait 10 seconds before next call
}