# 1 "/home/pmleite/Documents/dev/00_enta_cprob/robotGarra/robotGarra.ino"
# 2 "/home/pmleite/Documents/dev/00_enta_cprob/robotGarra/robotGarra.ino" 2
# 3 "/home/pmleite/Documents/dev/00_enta_cprob/robotGarra/robotGarra.ino" 2
# 4 "/home/pmleite/Documents/dev/00_enta_cprob/robotGarra/robotGarra.ino" 2
# 5 "/home/pmleite/Documents/dev/00_enta_cprob/robotGarra/robotGarra.ino" 2
# 6 "/home/pmleite/Documents/dev/00_enta_cprob/robotGarra/robotGarra.ino" 2
# 7 "/home/pmleite/Documents/dev/00_enta_cprob/robotGarra/robotGarra.ino" 2
# 8 "/home/pmleite/Documents/dev/00_enta_cprob/robotGarra/robotGarra.ino" 2

Adafruit_PWMServoDriver mux = Adafruit_PWMServoDriver(0x40);
LCD MyLCD;


void setup() {

  //Inicializa porta série
  Serial.begin(9600);
  delay(100);


  //Define Pinos do sensor US
  pinMode(A0, 0x0);
  pinMode(A1, 0x1);
  //Define Pinos dos sensores IR
  pinMode(A2, 0x0);
  pinMode(8, 0x0);
  pinMode(9, 0x0);
  //Define Pinos do sensor de cor
  pinMode(6, 0x1);
  pinMode(7, 0x1);
  pinMode(A3, 0x0);
  //Define pinos do motor
  pinMode(2,0x1);
  pinMode(3,0x1);
  pinMode(4 ,0x1);
  pinMode(5 ,0x1);

  //Pára motores
  movimenta('p');

  //Inicializa servo multiplexor
  mux.begin();
  mux.setPWMFreq(60);
  delay(1000);

  //Coloca braço na posição incial
  armToInitPosition(mux);

}

void loop() {

  distancia();
  detetaCor();

  if (true /* false se tiver implementado o controlo remoto PS2*/){
    detetaLinha();
    if(left == 1 || rigth == 1){
      corrigeTragetoria();
    }
  }


  // apanhaObjeto(mux);

  if (false) printValues();

  MyLCD.setHome();
MyLCD.clearScreen();
MyLCD.printStr("Toggle reverse mode");
delay(2000);

MyLCD.clearScreen();
MyLCD.toggleReverseMode();
MyLCD.printStr("Reverse Mode On");
delay(1000);

}
