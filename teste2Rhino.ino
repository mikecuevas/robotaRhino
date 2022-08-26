#include <Encoder.h>

#define IN1 12
#define IN2 11
#define EN1 10

//constantes
float Kp = 0.3;
float Ki = 0.0001;
float Kd = 0.01;

//valores de tempo
int ref_voltas = 5;
unsigned long temp_anterior=0, temp_atual, t;

//valores do pid
float P, I, D;
double erroPassado, erro, erro_anterior = 0, erroSomado = 0; 
double PID;

long oldPosition  = 0;
long posAtual;
double angle;

Encoder myEnc(2, 3);

void setup() {
  Serial.begin(115200);
  pinMode(EN1, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
}

void loop() {
  position();
  calculoErro();
  calculoPID();
  motor();
  /*if(angle>=4*360){
    analogWrite(EN1, 0);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,LOW);
  }
  else{
    analogWrite(EN1, 255);
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
  }*/
}

void position() {
  posAtual = myEnc.read();
  angle = posAtual*360/1610;
  Serial.println(posAtual);
}

void calculoErro(){
  erro = ref_voltas*360 - angle;
}

void calculoPID(){
  temp_atual = millis();
  t = temp_atual - temp_anterior;
  
  P = Kp * (erro);
  I = Ki * (erroSomado + erro)*t;
  D = Kd * (erro - erroPassado)/t;
  PID = P + I + D;
  if(PID>=12){
    PID = 12;
  }
  else if(PID<=-12){
    PID = -12;
  }
  else{
    erroSomado += erro;
  }

  if(erro<=5&&erro>=-5){
    PID = 0;
    erroSomado -=erro;
  }

  Serial.print(P);
  Serial.print(" - ");
  Serial.print(I);
  Serial.print(" - ");
  Serial.print(D);
  Serial.print(" - ");
  Serial.print(PID);
  Serial.print(" - ");
  Serial.print(erro);
  Serial.print(" - ");
  erroPassado = erro;
  temp_anterior = temp_atual; 
}

void motor(){
  if(PID>0){
    analogWrite(EN1, PID*255/12);
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
  }
  else if(PID<0){
    analogWrite(EN1, -PID*255/12);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
  }
  Serial.println(PID*255/12);
}
