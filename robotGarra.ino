#include <SparkFunSerialGraphicLCD.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "config.h"
#include "utils.h"

Adafruit_PWMServoDriver mux = Adafruit_PWMServoDriver(0x40);
LCD MyLCD;


void setup() {
  
  //Inicializa porta série
  Serial.begin(9600);
  delay(100);

  
  //Define Pinos do sensor US
  pinMode(ULTRA_S_ECHO, INPUT);
  pinMode(ULTRA_S_PING, OUTPUT);
  //Define Pinos dos sensores IR
  pinMode(LEFT_IR_SENS, INPUT);
  pinMode(MIDD_IR_SENS, INPUT);
  pinMode(RIGHT_IR_SENS, INPUT);
  //Define Pinos do sensor de cor
  pinMode(COLOR_SENSOR_S2,   OUTPUT);
  pinMode(COLOR_SENSOR_S3,   OUTPUT);
  pinMode(COLOR_SENSOR_READ, INPUT);
  //Define pinos do motor
  pinMode(MA_DIR,OUTPUT); 
  pinMode(MA_SPEED,OUTPUT);
  pinMode(MB_DIR ,OUTPUT);
  pinMode(MB_SPEED ,OUTPUT);

  //Pára motores
  movimenta('p');

  //Inicializa servo multiplexor
  mux.begin();
  mux.setPWMFreq(PWM_FREQ);
  delay(1000);

  //Coloca braço na posição incial
  armToInitPosition(mux);

}

void loop() {

  distancia();
  detetaCor();

  if (AUTO_MODE){
    detetaLinha();
    if(left == 1 || rigth == 1){
      corrigeTragetoria();
    }
  }

  
  // apanhaObjeto(mux);
 
  if (DEBUG) printValues();

  MyLCD.setHome();
MyLCD.clearScreen();
MyLCD.printStr("Toggle reverse mode");
delay(2000);
  
MyLCD.clearScreen();
MyLCD.toggleReverseMode();
MyLCD.printStr("Reverse Mode On");
delay(1000);

}

