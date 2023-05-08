#include <TM1637TinyDisplay.h>
#include<Servo.h>
#include <DHT.h>


boolean A;
int SENSOR = 6;
int temp, humedad1;
int sensorValue = 0;
int humedad = 0;
int barras = 0;
int nivel = 0;

#define CLK 4
#define DIO 5

Servo elRo;
DHT dht (SENSOR, DHT11);
TM1637TinyDisplay display(CLK, DIO);



void setup() {
  
   pinMode(0,INPUT);
   pinMode(1,OUTPUT);
   pinMode(A1,INPUT);

   elRo.attach(7);
   dht.begin();
   
   display.setBrightness(7);
   Serial.begin(9600);}

void loop() {

  
  sensorValue = (analogRead(A1));
  
  humedad = map(sensorValue,0,550,0,100);
  humedad1 = dht.readHumidity();
  temp = dht.readTemperature();
  nivel = map(humedad,0,100,0,8);

  elRo.write(180);
  delay(200);
  elRo.write(0);
  delay(200); 

  
   Serial.print("Temperatura: ");
   Serial.print(temp);
   Serial.print("ºC Humedad: ");
   Serial.print(humedad);
   Serial.println("%");

  
  display.showNumberDec(temp);
  delay(500); 
  
   digitalWrite(1,LOW);
  if (nivel==8){
    barras = 8888;}
  else if (nivel == 7){
    barras = 1888;}
  else if (nivel == 6){
    barras = 888;}
  else if (nivel == 5){
    barras = 188;}
  else if (nivel == 4){
    barras = 88;}
  else if (nivel == 3){
    barras = 18;}
  else if (nivel == 2){
    barras = 8;}
  else{
    digitalWrite(1,HIGH);
    barras = 1;} 
    
  display.showNumberDec(barras); 
  delay(200);

  }
