#include <SoftwareSerial.h>
#include <VoiceRecognitionV3.h>
#include <Servo.h>
#include "SD.h"
#define SD_ChipSelectPin 10 
#include "TMRpcm.h"
#include "SPI.h"

VR myVR(2,3);  
TMRpcm tmrpcm;
Servo motor, motor1, motor2;
int valorLasanha = 8, valorFeijao = 5, valorArroz = 1;
int segundos=0, minutos=0, i;
uint8_t records[7]; 
uint8_t buf[64];

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
  myVR.begin(9600);
  tmrpcm.speakerPin = 9; 
  Serial.begin(115200);
  motor.attach(5); //+1
  motor1.attach(6); // ligar
  motor2.attach(7); // cancelar
  motor.write(0);
  motor1.write(0);
  motor2.write(0);

  if(myVR.load((uint8_t)ESQUENTAR_ARROZ) >= 0){}
  if(myVR.load((uint8_t)DESCONGELAR_FEIJAO) >= 0){}
  if(myVR.load((uint8_t)FAZER_LASANHA) >= 0){}
  if(myVR.load((uint8_t)CANCELAR) >= 0){}
}

void loop()
{
  int valorModulo;
  valorModulo = myVR.recognize(buf, 50);
  if(valorModulo>0){
    switch(buf[1]){
      case ESQUENTAR_ARROZ:
        tmrpcm.setVolume(5);
        tmrpcm.play("feijao.wav");
        esquentarArroz();
        break;
      case DESCONGELAR_FEIJAO:
        tmrpcm.setVolume(5);
        tmrpcm.play("feijao.wav");
        descongelarFeijao();
        break;
      case FAZER_LASANHA:
        digitalWrite(led1, HIGH);
        tmrpcm.setVolume(5);
        tmrpcm.play("lasanha.wav");
        fazerLasanha();
        break;
      case CANCELAR:
        cancelar();
         tmrpcm.setVolume(5);
        tmrpcm.play("cancelar.wav");
        break;
      default:
        break;
    }
    printVR(buf);
  }
}
void esquentarArroz(){
      for(int i=0; i<valorArroz; i++){
        motor.write(12);
        delay(400);
        motor.write(0);
        delay(400); 
        }
         motor1.write(14);
         delay(1000);      
         motor1.write(0);
         delay(1000);     

        contador(30, 0);
  }
void descongelarFeijao(){
      for(int i=0; i<valorFeijao; i++){
        motor.write(40);
        delay(1000);
        motor.write(0);
        delay(1000);      
      }
       motor1.write(40);
       delay(1000);      
       motor1.write(0);
       delay(1000);    

       contador(30, 4);
    }
void fazerLasanha(){
    for(int i=0; i<valorLasanha; i++){
      motor.write(40);
      delay(1000);
      motor.write(0);
      delay(1000);      
    }
     motor1.write(40);
     delay(1000);      
     motor1.write(0);
     delay(1000);
     contador(30, 7);
  }

void contador(int valorCondicaoS, int valorCondicaoM){

  int seg = valorCondicaoS;
  int minu = valorCondicaoM;
  
      for(i=0; i<=60; i++){
      if(segundos==60){
        segundos=0;
        minutos++;
      }
      segundos++;
      
  if(minutos==minu and segundos==seg){
      cancelar();
      minutos=0, segundos=0;
  }
 }     
}
void cancelar(){
     motor2.write(10);
     delay(400);      
     motor2.write(0);
     delay(400);
  }
