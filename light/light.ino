#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "Sapido_RB-1602G3_d526bf";
const char* password = "";
//const char* ssid = "AndroidAP0B53";
//const char* password = "123456788";
//const char* ssid = "thomas";
//const char* password = "1qaz2wsx";

String website;

bool ledstate=0;

int color=0;

WebServer server(80);
//int led =13;
//const int led1 = 13;
//const int led2 = 12;
//const int led3 = 23;

const int led[]={4,16,17};
int ledcount=3;

void createSite(){
  website+="<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <title>Diming LED</title> <script src=\"https://code.jquery.com/jquery-3.3.1.js\"></script> <script>$(document).ready(function(){$(\"[type=range]\").change(function(){var ledvalue=$(\"#led\").val(); $(\".ledvalue\").text(ledvalue);});}); </script> <style>body{background-color: rgb(108, 159, 189);}.container{display: flex; flex-direction: column; align-items: center;}.btn{padding: 0.3em 0.5em; font-size: 1.5em;}.title1{text-align:center;color:rgb(224, 224, 75);text-shadow:0px 3px 5px #000; margin-bottom: 50px;}a{display:block;text-decoration:none;background-color:#FFFFFF;radius:3px;width:150px;text-align:center;margin:0 auto;margin-bottom:30px;font-size:2em;box-shadow: 0px 3px 6px #000;}.btn1{color:black;}.btn2{color:black;}.title2{text-align:center;color:rgb(224, 224, 75);text-shadow:0px 3px 5px #000; margin-bottom: 50px;}a{display:block;text-decoration:none;background-color:#FFFFFF;radius:3px;width:150px;text-align:center;margin:0 auto;margin-bottom:30px;font-size:2em;box-shadow: 0px 3px 6px #000;}.btn3{color:red;}.btn4{color:orange;}.btn5{color:green;}</style></head><body>  <div class=\"container\"> <h1>LED Diming</h1> <h3>LED Light=<span class='ledvalue'>0</span></h3> <form action=\"ledDiming\" method=\"get\" id=\"form1\"> <input type=\"range\" name=\"ledval\" id=\"led\" min=\"0\" max=\"255\"> </form> <br><button type=\"submit\" form=\"form1\" value=\"Submit\" class=\"btn\">Submit</button><h1 class=\"title1\">LED Control</h1> <a class=\"btn1\" href=\"/H\">ON</a> <a class=\"btn2\" href=\"/L\">OFF</a><h1 class=\"title2\">LED Color</h1> <a class=\"btn3\" href=\"/RED\">RED</a> <a class=\"btn4\" href=\"/ORANGE\">ORANGE</a> <a class=\"btn5\" href=\"/GREEN\">GREEN</a> </div></body></html>";
}

void handleRoot() {
  //digitalWrite(led,1);
  server.send(200, "text/html",website); 
  //digitalWrite(led,0);
  
}
void ledDiming(){
  int led_Diming=server.arg("ledval").toInt();
  ledcWrite(0,led_Diming);  
  server.send(200,"text/html",website);
}
void LEDON(){
  digitalWrite(led[color], HIGH);
   server.send(200, "text/html",website);
  ledstate=1; 
  //ledcAttachPin(led[color],0);
  for(int i=0;i<ledcount;i++){
     ledcAttachPin(led[i],1);
     if(i==color){
      ledcAttachPin(led[i],0);
     }
  }
}
void LEDOFF(){
   server.send(200, "text/html",website);
  digitalWrite(led[color], LOW); 
  ledstate=0;
   for(int i=0;i<ledcount;i++){
     ledcAttachPin(led[i],1);
  }
}

void ledRED(){
  server.send(200, "text/html",website);
  for(int i=0;i<ledcount;i++){
    digitalWrite(led[i], LOW);
    if(i==0){
      if(ledstate){
        digitalWrite(led[i], HIGH);
          for(int i=0;i<ledcount;i++){
     ledcAttachPin(led[i],1);
     if(i==0){
      ledcAttachPin(led[i],0);
     }
  }
      }     
      color=i;
    }
  }
}

void ledORANGE(){
  server.send(200, "text/html",website);
  for(int i=0;i<ledcount;i++){
    digitalWrite(led[i], LOW);
    if(i==1){
      if(ledstate){
        digitalWrite(led[i], HIGH);
          for(int i=0;i<ledcount;i++){
     ledcAttachPin(led[i],1);
     if(i==1){
      ledcAttachPin(led[i],0);
     }
  }
      }  
      color=i;
    }
  }
}

void ledGREEN(){
  server.send(200, "text/html",website);
  for(int i=0;i<ledcount;i++){
    digitalWrite(led[i], LOW);
    if(i==2){
      if(ledstate){
        digitalWrite(led[i], HIGH);
          for(int i=0;i<ledcount;i++){
     ledcAttachPin(led[i],1);
     if(i==2){
      ledcAttachPin(led[i],0);
     }
  }
      }  
      color=i;
    }
  }
}
void handleNotFound() {
  //Serial.print("12345");
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
  //Serial.print("12345");
  for(int i=0;i<ledcount;i++){
    pinMode(led[i],OUTPUT);
  }
  //pinMode(led1, OUTPUT);
//  pinMode(led2, OUTPUT);
//  pinMode(led3, OUTPUT);
  ledcSetup(0,5000,8);
  
 // ledcAttachPin(led1,0);
//  ledcAttachPin(led2,0);
//  ledcAttachPin(led3,0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  createSite();
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

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/H", LEDON);
  server.on("/L", LEDOFF);
  server.on("/RED",ledRED);
  server.on("/ORANGE",ledORANGE);
  server.on("/GREEN",ledGREEN);
  server.on("/ledDiming",ledDiming);
  server.on("/inline", []() {
    server.send(200, "text/html", website);
  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}
