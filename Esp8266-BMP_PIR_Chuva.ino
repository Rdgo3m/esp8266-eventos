#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>

// Nome da sua rede Wifi
const char* ssid = "xxx xxxx";
// Senha da rede
const char* password = "xxx xxxx";

#define sensePIR 2
#define sensibilidade2 A0

Adafruit_BMP085 bmp;

const char* host = "xxxxxxx.000webhostapp.com";
int count;
unsigned long changetime = 0;
int sensorState;
bool flag = false;
int sensorSensibilidade2;
float media=0;
int soma=0;
int countM =0;
int flagRain=0;

void setup() {
  // Iniciando o Serial
  Serial.begin(115200);
  changetime = millis();
  
  // Iniciando o BMP 
  bmp.begin();

  // Conectando na rede wifi
  Serial.println("");
  Serial.print("Conectando");
  
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a rede: ");
  Serial.println(ssid);

}
void loop() {
  
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    return;
  }

  // Lendo a temperatura em graus Celsius e pressão
int t = bmp.readTemperature();
int p = bmp.readPressure();
p=p/100;

  if (millis()-changetime >= 60000)
  {
  count++;
  changetime = millis();  
  }
  if (count>29) //original 30
{
 //Enviando p/ o host a temperatura e pressao.
 
  client.print(String("GET /add.php?MSG_Texto1=") + int(t) + String("C*_")  + int(p) + String("hPa(ESP8266)") +
                      " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  Serial.print("Temperatura Enviada: ");
  Serial.println(t);
  
count=0;
}
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C");
  Serial.print("\t Humidade: ");
  Serial.print(p);
  Serial.println(" hPa");
  
  //////PIR
   sensorState = digitalRead(sensePIR);

 if ((sensorState==1)&&(flag==false))
 {
client.print(String("GET /add.php?MSG_Texto1=") + String("PIR_Acionado") +
                      " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  flag=true;
  }
 if ((sensorState==0)&&(flag==true))
 {
 client.print(String("GET /add.php?MSG_Texto1=") + String("PIR_Desacionado") +
                      " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
  flag=false;
 
 }

   sensorSensibilidade2 = analogRead(sensibilidade2);
   soma = soma + sensorSensibilidade2;
   
   if (countM>9)
{
media = soma/10;

Serial.print("media:");
Serial.println(media);
       
        if (flagRain==0)
           {
                    if(media<960)
           {
client.print(String("GET /add.php?MSG_Texto1=") + String("Inicio_Chuva_2") +
                      " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
         flagRain=1;
          }                     
          }
           
           if (flagRain==1)           
           {
                    if(media>1005) 
           {
client.print(String("GET /add.php?MSG_Texto1=") + String("Sensor_Seco_2") +
                      " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
            flagRain=0;
          }
          }

soma=0;
countM=0;

}
countM++;
  
  Serial.println(sensorState);
  Serial.print("count_time ");
  Serial.println(count);

   delay(320); //delay original era 160ms
  
  Serial.println(sensorSensibilidade2);
}




 












 
