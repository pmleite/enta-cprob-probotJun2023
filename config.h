#define minPulseValue 500
#define maxPulseValue 2500
#define midPulseValue (minPulseValue+maxPulseValue)/2
#define armSpeed 15   //Higher is slow MAX 15

#define DEBUG                     false
#define AUTO_MODE                 true  // false se tiver implementado o controlo remoto PS2
#define COLOR_READ_DELAY          50    // Não deve ser inferior a 50 nem superior a 60 
#define CALIBRATE_COLOR_DETECTOR  false // True para entrar no modo de calibração do sensor
#define PWM_FREQ                  60

#define maxX 127//159 
#define maxY 63 //127

#define baseServo 0
#define clawServo 1
#define vertServo 2
#define horiServo 3

#define  LEFT_IR_SENS  A2 
#define  MIDD_IR_SENS  8 
#define  RIGHT_IR_SENS 9 

#define MA_DIR    2
#define MA_SPEED  3
#define MB_DIR    4
#define MB_SPEED  5

#define COLOR_SENSOR_S2   6
#define COLOR_SENSOR_S3   7
#define COLOR_SENSOR_READ A3

#define ULTRA_S_PING     A1
#define ULTRA_S_ECHO     A0

#define clawClosedAngle 25
#define clawOpenedAngle 100
#define clawMinPulse (minPulseValue+maxPulseValue)/2


/*******************************************
* Variaveis de controlo de velocidade do motor
*/
int mASpeed, mBSpeed;


/*******************************************
* Variaveis de controlo de detetores de linha
*/
int left,center,rigth;


/********************************************
* Variavel de controlo de distância
*/
int currDist = 0;



/********************************************
* Variaveis de calibração.
*/
int redMin    = 24;     // Pulso mínimo (vermelho)
int redMax    = 25;     // Pulso máximo (vermelho)

int greenMin  = 83;     // Pulso mínimo (Verde)
int greenMax  = 85;     // Pulso máximo (Verde)

int blueMin   = 53;     // Pulso mínimo (Azul)
int blueMax   = 54;     // Pulso máximo (Azul)

int whiteMin  = 6;      // Pulso mínimo (Branco)
int whiteMax  = 7;      // Pulso máximo (Branco)

// Variaveis que estão constantemente a serem atualizadas com os valores da leitura das cores
int redValue;
int greenValue;
int blueValue;
int whiteValue;

// Variaveis para a escala da função map
int scaleLowVal  = 0;
int scaleHighVal = 255;

// Variavel onde é guardada a cor detetada sem sempre que há uma atualização das cores
int detectedColor;


/********************************************
* Variavel de controlo de servos
*/

//Variaveis de controlo da última posição dos servos
int servGarraLastPos =   0;
int servBaseLastPos  =   0;
int servVertLastPos  =   0;
int servHoriLastPos  =   0;


/*******************************************
* Print control
*/
String l1   = " ______Servos POS (deg)________          (cm)          ______IRSensors_____     __________________COLOR_______________        ______SPEED______";
String l2   = "  Garra    Base    Vert    Hori        Distance        Left   Middle  Right     RED     GREEN    BLUE         DETColor        M1Speed   M2Speed";


