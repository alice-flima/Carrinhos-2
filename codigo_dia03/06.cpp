#include <SoftwareSerial.h>
#include <SoftPWM.h>
#include <Servo.h>

SoftwareSerial bt(11, 12); 
Servo atacante3;
Servo atacante2;

const int velMot1 = 9; 
const int dirMot1 = 10; 
const int velMot2 = 7;
const int dirMot2 = 8;  
const int pinoServo3 = 3;
const int pinoServo2 = 2;

int velocidadepadrao = 100; 
bool autonomaConcluida = false;

void frente();
void tras();
void esquerda();
void direita();
void parar();
void chutar();
void pausa(unsigned long tempo);
void autonoma();

void setup() {
  atacante3.attach(pinoServo3);   
  atacante3.write(0);
  atacante2.attach(pinoServo2);   
  atacante2.write(180);
  
  pinMode(dirMot1, OUTPUT);
  pinMode(dirMot2, OUTPUT);
  
  Serial.begin(9600);
  bt.begin(9600);
  SoftPWMBegin();
  parar();
  
  Serial.println(" Sistema Pronto");
}

void loop() {
  if (bt.available() > 0) {
    char comando = (char)bt.read();
    
    if (autonomaConcluida == false && comando == 'E') {
      autonoma();
      autonomaConcluida = true; 
    }
    else {
      if (comando == 'F') frente();
      else if (comando == 'B') tras();
      else if (comando == 'L') esquerda();
      else if (comando == 'R') direita();
      else if (comando == 'S') parar();
      else if (comando == 'C') chutar(); 
    }
  }
}

void pausa(unsigned long tempo) {
  unsigned long tempoInicial = millis();
  while (millis() - tempoInicial < tempo) {
  }
}

void autonoma() {
  Serial.println("Saindo da base");
  frente(); pausa(200);
  esquerda(); pausa(200);   
  frente(); pausa(5000); 
  parar(); pausa(200);   
  direita(); pausa(500);   
  frente(); pausa(2000); 
  direita(); pausa(300);   
  parar(); pausa(200);   
  Serial.println("Fim da fase autonoma"); 
}

void frente() {
  digitalWrite(dirMot1, HIGH); digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, velocidadepadrao); SoftPWMSetPercent(velMot2, velocidadepadrao);
}

void tras() {
  digitalWrite(dirMot1, LOW); digitalWrite(dirMot2, LOW);
  SoftPWMSetPercent(velMot1, velocidadepadrao); SoftPWMSetPercent(velMot2, velocidadepadrao);
}

void esquerda() {
  digitalWrite(dirMot1, HIGH); digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, velocidadepadrao); SoftPWMSetPercent(velMot2, 0); 
}

void direita() {
  digitalWrite(dirMot1, HIGH); digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, 0); SoftPWMSetPercent(velMot2, velocidadepadrao);
}

void parar() {
  SoftPWMSetPercent(velMot1, 0); SoftPWMSetPercent(velMot2, 0);
}

void chutar() {
  atacante3.write(180); 
  atacante2.write(0); 
  pausa(2000);        
  atacante3.write(0); 
  atacante2.write(180); 
  pausa(500);         
}