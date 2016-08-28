#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

const char* ssid     = "TP";
const char* password = "12345678";

const char* host = "www.cricapi.com"; 

#include <Wire.h>  // This library is already built in to the Arduino IDE
#include <LiquidCrystal_I2C.h> //Download :: https://codeload.github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library/zip/master
LiquidCrystal_I2C lcd(0x27, 20, 4);



void setup() {
  Serial.begin(115200);
  delay(100);
  lcd.begin();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight
  lcd.setCursor(0, 0);
  lcd.print("Wi-Fi >>>"); // Printing Line
  delay(1000);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(" ");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi  OK");
}

int value = 0;

void loop() {
  StaticJsonBuffer<1200> jsonBuffer;
  delay(5000);
  ++value;
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  
  String url = "/api/cricketScore/?unique_id=995461&apikey=xxxxxxxxxxxxxx"; // pleas insert API Key in to xxxxxxxxxx
  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  delay(500);

  // Read all the lines of the reply from server and print them to Serial
  
  while (client.available()) {
    line = client.readStringUntil('\r');
    line.trim();
    if (line.indexOf("Sri Lanka") > 0) {
      JsonObject& root = jsonBuffer.parseObject(line);
      root.prettyPrintTo(Serial);
      
      String score  = root["score"]; // Score data 
      String inning  = root["innings-requirement"]; // Inning data
      
      int in_l = inning.length() / 16+1; // length of the characters ( for display) 16 x 2 
      int s_l =  score.length() / 16;
      
      int h = 0; // String pos
      
      int b = 16; // String pos
      
      for ( int i = 1; i <= in_l; i++) {
      
        String r = score.substring(0, 15) ;
        
        r += ')';
        
        String m = inning.substring(h, (b * i));
        
        h = b * i; // updating new String Pos
        
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(m) ;
        lcd.setCursor(0, 1);
        lcd.print(r) ;
        delay(3000);
      }
    }

  }
  Serial.println();
  Serial.println("closing connection");

}
