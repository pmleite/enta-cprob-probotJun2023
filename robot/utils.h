/**
* função que atualiza as variaveis de deteção de linha
*/
void detetaLinha(){
  left   = !digitalRead(LEFT_IR_SENS);
  center = !digitalRead(MIDD_IR_SENS);
  rigth  = !digitalRead(RIGHT_IR_SENS);
}

/**
* função que mede a distância e atualiza a variavel
*/
void distancia(){
  //Envio de um sinal
  digitalWrite(ULTRA_S_PING, LOW);
  delayMicroseconds(2);
  digitalWrite(ULTRA_S_PING, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRA_S_PING, LOW);

  //Detecao do echo do sinal e conversão para cm
  currDist = pulseIn(ULTRA_S_ECHO, HIGH) * 0.034 / 2;
}


/**
* função que converte angulo para pulsos
*
* @param  angulo o valor do angulo a converter para pulsos
*/
int angleToPulse(int angle){
  return map(angle, 0, 180, minPulseValue, maxPulseValue);
}


/**
* função que executa um movimento de um servo
*
* @param  servo  o servo que se quer fazer mover
* @param  angulo a posisão (angulo) que se quer no servo.
* @param  speed  a velocidade do movimento (definido em constantes nos settings)
*/
void moveServo(Adafruit_PWMServoDriver drv, int servo, int angulo, long speed=armSpeed){

        //Variavel para guardar a última posição
        int lastPos;

        //Guarda valor da ultima posição em função do servo indicado como argumento
        if (servo == 0) lastPos = servBaseLastPos;
        if (servo == 1) lastPos = servGarraLastPos;
        if (servo == 2) lastPos = servVertLastPos;
        if (servo == 3) lastPos = servHoriLastPos;

        //Verifica se a trajetória á crescente
        if (lastPos < angulo){
              for (int n = lastPos; n<= angulo; n++){
                drv.writeMicroseconds(servo,angleToPulse(n));
                delay(speed);
              }
        //Se a trajetória for decrescente
        }else{
          for (int n = lastPos; n>= angulo; n--){
                drv.writeMicroseconds(servo,angleToPulse(n));
                delay(speed);
              }
        }

        //Guarda a última posição do respetivo servo.
        if (servo == 0) servBaseLastPos   = angulo;
        if (servo == 1) servGarraLastPos  = angulo;
        if (servo == 2) servVertLastPos   = angulo;
        if (servo == 3) servHoriLastPos   = angulo;
        
}


/**
* função que coloca o braço numa posição inicial
*
* @param  angulo o valor do angulo a converter para pulsos
*/
void armToInitPosition(Adafruit_PWMServoDriver mux){
  moveServo(mux, clawServo, clawClosedAngle);
  moveServo(mux, baseServo, 90);
  moveServo(mux, vertServo, 100);
  moveServo(mux, horiServo, 90);
}


/**
* função que transforma os valores de pulsos para um escalar
* cujo o limite inferior e superior é definido pelas
* variaveis "scaleLowVal" e "scaleHighVal" nos settings
*
* @param pulseVal valor da leitura do pulso.
* @return um INTEIRO compreendido entre o valor de scaleLowVal e scaleHighVal
*/
int convertPulseValue(int pulseVal, int minPulse, int maxPulse, bool calibrate=false){

  //Se está em modo de calibração só apresenta o valor de retorno sem map
  if (calibrate) return pulseVal;
  //Se não está em modo de calibração devolve o valor de retorno mapeado na
  //escala definida por scaleLowVal e scaleHighVal
  return map(pulseVal,minPulse,maxPulse,scaleLowVal,scaleHighVal);
 
}


/**
*  função que obtém os pulsos de cor aplicando
*  o respetivo filtro do fotodiodo pela ativação das
*  seguinte portas:
*
*      S2	     S3	      Photodiode type
*      LOW	  LOW	      Red
*      LOW	  HIGH	    Blue
*      HIGH	  LOW	      Clear (No filter)
*      HIGH	  HIGH	    Green
*  
*/
void atualizaValoresDasCores(bool calibrate = CALIBRATE_COLOR_DETECTOR) {

    /*
      S2	  S3	    Photodiode type
      LOW	  LOW	      Red
      LOW	  HIGH	    Blue
      HIGH	LOW	      Clear (No filter)
      HIGH	HIGH	    Green
    */

    //Muda a configuração do sensor para o filtro vermelho
    digitalWrite(COLOR_SENSOR_S2,LOW);
    digitalWrite(COLOR_SENSOR_S3,LOW);
    //Le o valor do sensor e atualiza a variavel redValue
    redValue = convertPulseValue(pulseIn(COLOR_SENSOR_READ, LOW), redMin, redMax, calibrate);
    delay(COLOR_READ_DELAY);

    //Muda a configuração do sensor para o filtro Verde
    digitalWrite(COLOR_SENSOR_S2,HIGH);
    digitalWrite(COLOR_SENSOR_S3,HIGH);
    //Le o valor do sensor e atualiza a variavel greenValue
    greenValue = convertPulseValue(pulseIn(COLOR_SENSOR_READ, LOW), greenMin, greenMax, calibrate);
    delay(COLOR_READ_DELAY);
    
    //Muda a configuração do sensor para o filtro Azul
    digitalWrite(COLOR_SENSOR_S2,LOW);
    digitalWrite(COLOR_SENSOR_S3,HIGH);
    //Le o valor do sensor e atualiza a variavel blueValue
    blueValue = convertPulseValue(pulseIn(COLOR_SENSOR_READ, LOW), blueMin, blueMax, calibrate);
    delay(COLOR_READ_DELAY);

    //Muda a configuração do sensor para o filtro Branco
    digitalWrite(COLOR_SENSOR_S2,HIGH);
    digitalWrite(COLOR_SENSOR_S3,LOW);
    //Le o valor do sensor e atualiza a variavel whiteValue
    whiteValue = convertPulseValue(pulseIn(COLOR_SENSOR_READ, LOW), whiteMin, whiteMax, calibrate);
    delay(COLOR_READ_DELAY);
}


/**
* função que intrepreta os valores e devolve a cor detetada
*
* @param r valor da leitura dos pulsos do RED, convertido em escala de 0 a 255
* @param g valor da leitura dos pulsos do GREEN, convertido em escala de 0 a 255
* @param b valor da leitura dos pulsos do BLUE, convertido em escala de 0 a 255
* @param w valor da leitura dos pulsos do WHITE, convertido em escala de 0 a 255
*
* @return 0=Red, 1=Green, 2=Blue, 3=White, 4=Preto   
*/
void detetaCor(){
  atualizaValoresDasCores();
  if ((whiteValue == -255 ) || ( whiteValue >0 && redValue < 0 && redValue <0 && blueValue <0)) detectedColor = 4;
  else if ((redValue   == 255 || redValue   == 0 ) || (redValue   < greenValue && redValue   < blueValue))   detectedColor = 0;
  else if ((greenValue == 255 || greenValue == 0 ) || (greenValue < redValue   && greenValue < blueValue))   detectedColor = 1;
  else if ((blueValue  == 255 || blueValue  == 0 ) || (blueValue  < redValue   && blueValue  < greenValue))  detectedColor = 2;
  else  detectedColor = 99;
}


/**
* função que faz movimentar o motor  
* @param dir Char que indica a direção, nomeadamente:
*            a - avança
*            r - recua
*            e - esquerda
*            d - direita
*            p - para
* @param speed char que indica a velocidade
* @param dif   factor diferencial de rotaçao entre motores        
*/
void movimenta(char dir, char speed=120, char diff=100){

  if(dir == 'a'){
    digitalWrite(MA_DIR,HIGH); 
    digitalWrite(MB_DIR,LOW); 
    analogWrite(MA_SPEED,speed); mASpeed = speed;
    analogWrite(MB_SPEED,speed); mBSpeed = speed;
  }

  if(dir == 'r'){
    digitalWrite(MA_DIR,LOW); 
    digitalWrite(MB_DIR,HIGH); 
    analogWrite(MA_SPEED,speed); mASpeed = speed;
    analogWrite(MB_SPEED,speed); mBSpeed = speed;
  }

  if(dir == 'e'){
    digitalWrite(MA_DIR,HIGH); 
    digitalWrite(MB_DIR,LOW); 
    analogWrite(MA_SPEED,speed); mASpeed = speed;
    analogWrite(MB_SPEED,speed-diff); mBSpeed = speed;
  }

  if(dir == 'd'){
    digitalWrite(MA_DIR,HIGH); 
    digitalWrite(MB_DIR,LOW); 
    analogWrite(MA_SPEED,speed-diff); mASpeed = speed-diff;
    analogWrite(MB_SPEED,speed); mBSpeed = speed;
  }

  if(dir == 'p'){
    digitalWrite(MA_DIR,LOW); 
    digitalWrite(MB_DIR,LOW); 
    analogWrite(MA_SPEED,0); mASpeed = 0;
    analogWrite(MB_SPEED,0); mBSpeed = 0;
  }
}


void apanhaObjeto(Adafruit_PWMServoDriver drv){
  armToInitPosition(drv);
  moveServo(drv, clawServo, clawOpenedAngle);
  moveServo(drv, horiServo, 170);
  moveServo(drv, clawServo, clawClosedAngle);
  moveServo(drv, horiServo, 90);
}



/**
* @TODO
* Função para corrigir a trajetória de acordo com os valores das variaveis left,center,rigth;
* Com algoritmo PID
*/
void corrigeTragetoria(){

  Serial.println("A corrigir trajetória");

}





/*********************************************************
* Função para imprimir informação dos sensores
*/
void printValues(bool full=false){

  if(full){
    Serial.println();
    Serial.println(l1);
    Serial.println(l2);
  }

  Serial.print("  "+String(servGarraLastPos)+"\t   "+String(servBaseLastPos)+"\t   "+String(servVertLastPos)+"\t   "+String(servHoriLastPos)+"\t\t");
  Serial.print(String(currDist)+"\t\t");
  Serial.print(String(left)+"\t"+String(center)+"\t"+String(rigth)+"\t");
  Serial.print(String(redValue)+"\t"+String(greenValue)+"\t"+String(blueValue)+"\t\t"+String(detectedColor)+"\t\t");
  Serial.print(String(mASpeed)+"\t"+String(mBSpeed));
  Serial.println();

  if(full){
    Serial.println();
  }

}



