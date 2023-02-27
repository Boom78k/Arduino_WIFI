#include <ESP8266WiFi.h>

int led_blue = 15;
int led_green = 13;
int led_red = 12;
int status_led1 = 0;
int status_led2 = 0;
int status_led3 = 0;

// Название сети WiFi и пароль
const char* ssid = "robots_school_students";
const char* password = "23111991";

// Поднимаем веб сервер
WiFiServer server(80);

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(led_blue, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_red, OUTPUT);

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

  //1
  int p = req.indexOf("txt1");
  String txt1 = "";
  if(p!= -1){
    p+=5;
    while((req[p] != '&') && (req[p] != ' ')){
      txt1+=req[p];
      p++;
    }
  }
  Serial.print("Number1: ");
  Serial.println(txt1);

  //2
  int p2 = req.indexOf("txt2");
  String txt2 = "";
  if(p2!= -1){
    p2+=5;
    while((req[p2] != '&') && (req[p2] != ' ')){
      txt2+=req[p2];
      p2++;
    }
  }
  Serial.print("Number2: ");
  Serial.println(txt2);

  //3
  int p3 = req.indexOf("txt3");
  String txt3 = "";
  if(p3!= -1){
    p3+=5;
    while((req[p3] != '&') && (req[p3] != ' ')){
      txt3+=req[p3];
      p3++;
    }
  }
  Serial.print("Number3: ");
  Serial.println(txt3);
  
  if (req.indexOf("favicon.ico")!= -1)
  {
    return; //Завершаем loop
  }

  int r = txt1.toInt();
  int g = txt2.toInt();
  int b = txt3.toInt();

  analogWrite(led_red, r);
  analogWrite(led_green, g);
  analogWrite(led_blue, b);

  // \r\n - следущая строка
  client.print("HTTP/1.1 200 OK\r\nConnect-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>");
  // Отправим "Шапку" HTML файла
  client.print("\r\n<center><h1><font size='7'>");
  client.print("<meta charset=\"utf-8\">");
  client.print("\r\n<form method=\"get\">");
  client.print("\r\n<INPUT type=\"text\" placeholder=\"красный\" name=\"txt1\"> ");
  client.print("\r\n<INPUT type=\"text\" placeholder=\"зеленый\" name=\"txt2\"> ");
  client.print("\r\n<INPUT type=\"text\" placeholder=\"синий\" name=\"txt3\">");
  client.print("\r\n<INPUT type=\"submit\" value=\"Отправить\">");
  client.print("\r\n</form>");
  
  delay(1);
  Serial.println("Client disconnected");

  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
