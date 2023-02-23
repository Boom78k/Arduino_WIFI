#include <ESP8266WiFi.h>

int led = 2;
int status_led = 0;

// Название сети WiFi и пароль
const char* ssid = "robots_school_students";
const char* password = "23111991";

// Поднимаем веб сервер
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(led, OUTPUT);
  digitalWrite(led, status_led);

  // Подключаемся к сети WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP addres
  Serial.println(WiFi.localIP());
}
void loop() {
  // Если клиент НЕ подключается
  WiFiClient client = server.available();
  if (!client) {
    return; //Завершаем loop
  }

  // Если клиент подключился печатаем
  Serial.println("new client");
  // Ждем, пока клиент начнет передавать данные
  while (!client.available()) {
    delay(1);
  }

  // Читаем данные
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush(); // Очищаем буфер от мусора

  if (req.indexOf("favicon.ico")!=-1)
  {
    return; //Завершаем loop
  }

  // \r\n - следущая строка
  client.print("HTTP/1.1 200 OK\r\nConnect-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>");
  // Отправим "Шапку" HTML файла
  client.print("\r\n<center><h1><font size='7'>");

  client.print("<meta charset=\"utf-8\">");
  client.print("\r\n<form method=\"get\">");
  
  if (req.indexOf("/?cb1=on") != -1) {
    client.print("\r\nВЫКЛ/ВКЛ: <INPUT type=\"checkbox\" name=\"cb1\" checked><br>");
  }
  else {
    client.print("\r\nВЫКЛ/ВКЛ: <INPUT type=\"checkbox\" name=\"cb1\"><br>");
  }
  
  client.print("\r\n<INPUT type=\"submit\" value=\"Отправить\">");
  client.print("\r\n</form>");

  if(req.indexOf("cb1=on")!=-1) {
    status_led = 1;
    digitalWrite(led, status_led);
  }
  else {
    status_led = 0;
    digitalWrite(led, status_led);
  }
  
  delay(1);
  Serial.println("Client disconnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
