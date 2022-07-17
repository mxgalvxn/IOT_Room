
#include <DHT.h>
#include <DHT_U.h>


#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

// Credenciales wifi
/* 1. Define the WiFi credentials */
#define ssid "Planta1 Casa21 2GHZ"
#define password "knife_6247"

/* 2. Define the API Key */
#define API_KEY "AIzaSyDondFuYb4j54jgjRlbg-1XQVDYV_XjiOA"

/* 3. Credenciales Proyecto Firebase */
const char *FIREBASE_HOST="https://base-de-datos-iot-18ce4-default-rtdb.firebaseio.com/"; 
const char *FIREBASE_AUTH="vsSga332yBprncRceWmmJG9ykvNeuMk6uX1CcFJP";

// Firebase Data object in the global scope
FirebaseData firebaseData;

bool iterar = true;
long inicio, finalizado;
  
const int LED = 5;
const int PIR = 4;
const int PRT = A0;
const int Trigger = 14;   
const int Echo = 12;
const int LED2 = 15;
const int LED3 = 13;

unsigned long inicioTemp = 0, inicioLuz = 0,inicioLuz1 = 0, inicioMovimiento = 0, finalizado1 = 0, finalizado2 = 0;

const int DHTPin = 0;  //Conectar sensor DHT a pin D3 = GPIO_0
#define DHTTYPE DHT11 //Definimos el tipo de sensor de humedad (DHT11 o DHT22)

int intento = 0;


const int threshold = 100;
float velocidad = 0.0343;
long duracion, distancia ;

DHT dht(DHTPin, DHTTYPE);

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(PIR, INPUT);
  pinMode(PRT, INPUT);

  pinMode(Trigger, OUTPUT); 
  pinMode(Echo, INPUT);  
  digitalWrite(Trigger, LOW);

 
  Serial.begin(9600);
   dht.begin();

WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(250);
  }
  Serial.print("\nConectado al Wi-Fi");
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true); 
}

void loop() {
 delay(500);
 String nodo = "Proyecto-iot";
  int input1 = analogRead(PRT) -30 ;
  int input = map(input1, 0, 1000, 150, 0);
  int value = digitalRead(PIR);
  
    digitalWrite(Trigger, LOW);        
    delayMicroseconds(2);              
    digitalWrite(Trigger, HIGH);   
    delayMicroseconds(10);           
    digitalWrite(Trigger, LOW);       
    duracion = pulseIn(Echo, HIGH) ; 
    distancia = velocidad* duracion / 2;
    
  
  float h = dht.readHumidity();  //Lectura de Humedad
  float t = dht.readTemperature(); //Lectura de Temperatura
 
   if (isnan(h) || isnan(t)) {
      Serial.println("No se puede leer el sensor");
   }
 
 
   Serial.print("\n Tem: ");
   Serial.print(t);
   Serial.print("\n Pir: ");
   Serial.print(value);
   Serial.print("\n Luz: ");
   Serial.print(input);
   Serial.print("\n Luz1: ");
   Serial.print(input1);
   Serial.print("\n Distancia: ");
   Serial.print(distancia);
   Serial.print("\n");
   
  if(distancia < 30){
    inicioMovimiento = millis();
    Firebase.pushInt(firebaseData, nodo + "/TiempoCasa" , inicioMovimiento/1000);
   digitalWrite(LED2,HIGH);
   delay(2000);
   digitalWrite(LED2,LOW);
  }
  
  if (value == 1 && input < 60){
    digitalWrite(LED,HIGH);
    inicioLuz = millis() - finalizado2;
    finalizado2 = millis();
    inicioLuz1 += inicioLuz,
    Serial.println("inicio fotoresistencia");
    }
  else if (millis() - finalizado2 - inicioLuz > 10000 || input > 100 ){
    digitalWrite(LED,LOW);
    finalizado2 = millis();
    Serial.println("fin fotoresistencia");
    }
    Serial.println(millis() - finalizado2 - inicioLuz);

    if (isnan(h)){
      digitalWrite(LED3,HIGH);
      delay(200);
      }
  inicio = millis();
  if (inicio - finalizado  > 6000 && intento < 10){
  Firebase.pushInt(firebaseData, nodo + "/Luz" , intento);
  Firebase.pushInt(firebaseData, nodo + "/Luz" ,input);
  Serial.println("Iteracion subida");
    delay(150);
    intento++;
    finalizado = millis();
  }

  iterar = false;
    if (intento == 10){
      Firebase.pushInt(firebaseData, nodo + "/Tiempo LUz" , inicioLuz1/1000);
      Firebase.pushInt(firebaseData, nodo + "/Temperatura" , t);
       Firebase.end(firebaseData);
       Serial.println("Acabo");
    }
}






//  if (distancia  <30){
//    servoMotor.write(180);
//    }
//   else{
//    servoMotor.write(0);
//    }
//}
