
#include <Servo.h>
 
#define SERVO 9
#define PINRELE 13

Servo servo_medidor;
int posicao_local;

// Flow sensor
volatile double waterFlow;
double constant = 1.0;

void setup(){
  Serial.begin(9600);
  Serial.println("Start");

  //servo
  servo_medidor.attach(SERVO); //informa que tem um servo motor ligado no arduino
  servo_medidor.write(180); // Inicia o servo motor escrevendo a posicao "0"

  // rele
  pinMode(PINRELE, OUTPUT);
  digitalWrite(PINRELE, HIGH);

  // flow
  waterFlow = 0;
  //digital 2 do Uno, quando mudar para nivel alto
  attachInterrupt(0, pulse, RISING);
}

//########### Funcao para Mover o Medidor ##########################

void gira_direita(){
  for(posicao_local = 180; posicao_local >= 30; posicao_local--){
    servo_medidor.write(posicao_local);// vai escrever a posicao no medidor
    delay(40); // Intervalo de tempo a passada de cada posicao, atrasado de 40 
  }
}

void gira_esquerda(){
  for(posicao_local = 30; posicao_local <= 180; posicao_local++){
    servo_medidor.write(posicao_local);
    delay(40);
  }
}

void movimento_medidor(){
  gira_direita();
  delay(20000);
  gira_esquerda();
}
//########### FIM Funcao para Mover o Medidor ############

void pulse(){ 
    //waterFlow += 1.0 / 5880.0;
    waterFlow += 1.0;
}

void loop(){
  if (waterFlow > 20){
    digitalWrite(PINRELE, LOW);
    movimento_medidor();
  }
  Serial.println(waterFlow);
}
