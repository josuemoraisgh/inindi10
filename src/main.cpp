#include <Arduino.h>
#include <math.h>
#include "filtro_iir.h"
#define NUMTHREADS 2 // Deve-se sempre declarar o NUMTHREADS antes de "inindThread.h"
#include "inindThread.h"

DigitalFilter filter1;
#define FILTER_ORDER_NUM1 1
const double filterTapsNum1[FILTER_ORDER_NUM1] = {
 0.01
};
#define FILTER_ORDER_DEN1 1
const double filterTapsDen1[FILTER_ORDER_DEN1] = {
 -0.99
};

#define pinANALOG A5                        //Configura o pino de leitura do LUXÍMETRO
#define pinPWM 6                            //Configura o pino de Saida do PWM
#define PERIODO 100                        //Em us 100ms

unsigned int amplitude = 127;
unsigned int count = 0;
byte bLeitura[4] = {0, 0, 0, 0};

void analogReadFunc()                      // Faz a leitura do sinal Analogico
{ 
  Serial.println(DigitalFilter_put(&filter1,abs(analogRead(pinANALOG)-512)));
}

void pwmFunc()                            //Faz a leitura e escrita da serial
{ 
  analogWrite(pinPWM,amplitude*sin(2*PI*count/PERIODO)+127);
  if(++count==10*PERIODO){
    count=0;
    amplitude=amplitude-27;
  }
}

void setup() {                              // Codigo de configuração
  Serial.begin(9600);
  pinMode(pinANALOG, INPUT);
  pinMode(pinPWM, OUTPUT);
  threadSetup(analogReadFunc, 1,pwmFunc,1);
  DigitalFilter_init(&filter1,filterTapsNum1,FILTER_ORDER_NUM1,filterTapsDen1,FILTER_ORDER_DEN1);  
}

void loop() { }