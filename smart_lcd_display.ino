#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LiquidCrystal.h>  
//configuring 16x2 lcd dislplay
const int rs = D1, en = D2, d4 = D3, d5 = D4, d6 = D5, d7 = D6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
 
AsyncWebServer server(80);
 
const char* ssid = "sandra";
const char* password = "kuru1234";
 
const char* PARAM_INPUT_1 = "input1";
 
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Smart Notice Board</title>
  <meta name="viewport" content="width=device-width, initial-scale=1.0">  <style>
    body {
      font-family: Arial, sans-serif;  margin: 0;
      background-color: #f5f5f5;  display: flex;
      flex-direction: column;
      justify-content: center;
      align-items: center;
      min-height: 100vh;  }

    h1 {
      color: #333;
      margin: 20px 0;
      font-size: 3em;  text-align: center;
    }

    h3 {
      color: #333;
      margin-bottom: 15px;
      font-size: 1.2em;  text-align: center;
    }

    .notice-board {
      background-color: #fff;
      border-radius: 10px;
      padding: 30px;
      box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
      display: flex;
      flex-direction: column;
      align-items: center;
    }

    .form-container {
      display: flex;
      flex-direction: column;
      margin-bottom: 20px;
    }

    .input-text {
      width: 100%;
      padding: 10px;
      border: 1px solid #ccc;
      border-radius: 4px;
      font-size: 1em;
      margin-bottom: 10px;
    }

    .button {
      display: inline-block;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      font-size: 1em;
      cursor: pointer;
      border-radius: 4px;
      transition: all 0.2s ease-in-out;
    }

    .button-on {
      background-color: #1abc9c;
      color: white;
    }

    .button-on:hover {
      background-color: #17a2b8;
    }

    .button-off {
      background-color: #34495e;
      color: white;
    }

    .button-off:hover {
      background-color: #2c3e50;
    }

    .info-text {
      font-size: 0.8em;
      color: #aaa;
      margin-top: 10px;
    }
  </style>
</head>
<body>
  <center> <div class="notice-board">
      <h1>Smart Notice Board!</h1>
      <div class="form-container">
        <h3>Enter Text to Display:</h3>
        <form action="/get">
        <input type="text" name="input1" class="input-text">
        <input type="submit" value="Send" class="button button-on">
        </form>
      </div>
      <br>
    </div>
  </center>
</body>
</html>
)rawliteral";
 
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}
 
void setup() {
  Serial.begin(115200);
   lcd.begin(16, 2);
   lcd.clear();
   lcd.setCursor(0, 0);
  lcd.print("Smart Notice");
  lcd.setCursor(0, 1);
  lcd.print("    Board");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Success");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  
 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Message would be");
    lcd.setCursor(0,1);
    lcd.print("  Shown here!");
  });
 
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String message;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_1)) {
      message = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
       lcd.clear();
       lcd.setCursor(0,0);
      
      lcd.print(message);
    }
    else {
      message = "No message sent";
      inputParam = "none";
    }
    Serial.println(message);
   
  request->send(200, "text/html", index_html);
  });
  server.onNotFound(notFound);
  server.begin();
}
 
void loop() {
    
}