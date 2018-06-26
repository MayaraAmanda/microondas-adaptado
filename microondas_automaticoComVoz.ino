#include <SoftwareSerial.h>
#include "VoiceRecognitionV3.h"
#include <Servo.h>
#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"

VR myVR(2,3);  
TMRpcm tmrpcm;
Servo motor, motor1, motor2;
int valorLasanha = 8, valorArroz = 2, valorFeijao = 5;
int segundos=0, minutos=0, i;
uint8_t records[7]; // save record
uint8_t buf[64];

int led1 = 6;
int led2 = 5;
//int led3 = 7;


#define ESQUENTAR_ARROZ    (0)
#define DESCONGELAR_FEIJAO   (1) 
#define FAZER_LASANHA   (2) 
#define CANCELAR   (3) 


void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}

void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF){
    Serial.print("NONE");
  }
  else if(buf[0]&0x80){
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else{
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  if(buf[3]>0){
    printSignature(buf+4, buf[3]);
  }
  else{
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}

void setup()
{
  /** initialize */
  myVR.begin(9600);

  tmrpcm.speakerPin = 9;
  //Serial.begin(9600);
  if (!SD.begin(SD_ChipSelectPin)) {
  Serial.println("SD fail");
  return;
  }
  Serial.begin(115200);
  motor.attach(7); //+1
  //motor1.attach(8); // ligar
  motor2.attach(10); // cancelar
  motor.write(0);
  motor1.write(0);
  motor2.write(0);
  //Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
     //pinMode(led3, OUTPUT);
    
  //if(myVR.clear() == 0){
    //Serial.println("Recognizer cleared.");
  //}else{
    //Serial.println("Not find VoiceRecognitionModule.");
    //Serial.println("Please check connection and restart Arduino.");
    //while(1);
  //}
  
  if(myVR.load((uint8_t)ESQUENTAR_ARROZ) >= 0){ // carrega o comando treinado no módulo.
  }
  
  if(myVR.load((uint8_t)DESCONGELAR_FEIJAO) >= 0){
  }
  if(myVR.load((uint8_t)FAZER_LASANHA) >= 0){
  }
  if(myVR.load((uint8_t)CANCELAR) >= 0){
  }
}

void loop()
{
  int valorModulo;
  valorModulo = myVR.recognize(buf, 50);
  if(valorModulo>0){
    switch(buf[1]){
      case ESQUENTAR_ARROZ:
        digitalWrite(led1, HIGH);
        esquentarArroz();
        break;
      case DESCONGELAR_FEIJAO:
      digitalWrite(led2, HIGH);
        tmrpcm.setVolume(5);
        tmrpcm.play("feijao.wav");
        //descongelarFeijao();
        break;
      case FAZER_LASANHA:
        /** turn off LED*/
        digitalWrite(led1, HIGH);
        tmrpcm.setVolume(5);
        tmrpcm.play("lasanha.wav");
        //fazerLasanha();
        break;
      case CANCELAR:
        /** turn off LED*/
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        //digitalWrite(led3, LOW);
        cancelar();
        break;
      default:
        break;
    }
    /** voice recognized */
    printVR(buf);
  }
}
void esquentarArroz(){
      for(int i=0; i<valorArroz; i++){
        motor.write(0);
        delay(1000);
        motor.write(20);
        delay(1000);      
        }
         motor1.write(0);
         delay(1000);      
         motor1.write(20);
         delay(1000);     
  }
void descongelarFeijao(){
      for(int i=0; i<valorFeijao; i++){
        motor.write(180);
        delay(1000);
        motor.write(120);
        delay(1000);      
      }
       motor1.write(180);
       delay(1000);      
       motor1.write(120);
       delay(1000);    
    }
void fazerLasanha(){
     //int valorSegundos=30, valorMinutos=1;
     for(int i=0; i<valorLasanha; i++){
      motor.write(180);
      delay(1000);
      motor.write(120);
      delay(1000);      
    }
     motor1.write(180);
     delay(1000);      
     motor1.write(120);
     delay(1000);

     contador();

  }

void contador(){
    for(i=0; i<=60; i++){
      if(segundos==60){
        segundos=0;
        minutos++;
      }
      segundos++;
      Serial.print("Minutos: ");
      Serial.print(minutos);
      Serial.print("\tSegundos: ");
      Serial.println(segundos);
      delay(1000);
      }
    
}
void cancelar(){
     motor2.write(180);
     delay(1000);      
     motor2.write(120);
     delay(1000);
  }
