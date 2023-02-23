#include<ESP8266WiFi.h>
#include "DHT.h"

#define DHTPIN 3
DHT dht(DHTPIN, DHT11);


int val = 0;
int led = 3;

const char* ssid = "robots_school_students";
const char* password = "23111991";

WiFiServer server(80);

void setup()
{
  Serial.begin(9600);
  delay(10);

  pinMode(led, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.print("Connect to");
  Serial.print(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  Serial.println(WiFi.localIP());

  dht.begin();
}

void loop()
{
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  float t = dht.readTemperature();
  float h = dht.readHumidity();

  Serial.println("new client");
  while (!client.available())
  {
    delay(1);
  }
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  client.flush();
  client.print("HTTP/1.1 200 OK\r\nContent-Type:text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>");
  client.print("\r\n<center><h1><font size='8' color = 'blue'>TEMPERATURE: ");
  client.print(t + 'с');
  client.print("<center><h1><font size='8' color = 'red'>Humidity: ");
  client.print(h + '%');
 
  String s = "";

  s += "</font></h1</center><html>\n";

  client.print(s);
  delay(1);
  Serial.println("Client dissconnected");
}




