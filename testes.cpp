#include <SoftwareSerial.h>
#include <SoftPWM.h>

SoftwareSerial bt(11, 12); 

const int velMot1 = 9; 
const int dirMot1 = 10; 

const int velMot2 = 7;
const int dirMot2 = 8;  

int velocidadePadrao = 70; 
char comando;

void setup() {
  pinMode(dirMot1, OUTPUT);
  pinMode(dirMot2, OUTPUT);
  
  Serial.begin(9600);
  bt.begin(9600);
  
  SoftPWMBegin();

  parar();
  
  Serial.println("--- Sistema Pronto: Controle Bluetooth PACA ---");
}

void loop() {
  if (bt.available() > 0) {
    comando = (char)bt.read();
    Serial.print("Comando via Bluetooth: ");
    Serial.println(comando);
    
    if (comando == 'F') {
      frente();
    } 
    else if (comando == 'B') {
      tras();
    } 
    else if (comando == 'L') {
      esquerda();
    } 
    else if (comando == 'R') {
      direita();
    } 
    else if (comando == 'S') {
      parar();
    }
  }

  if (Serial.available() > 0) {
    bt.write(Serial.read());
  }
}


void frente() {
  digitalWrite(dirMot1, HIGH);
  digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, velocidadePadrao);
  SoftPWMSetPercent(velMot2, velocidadePadrao);
}

void tras() {
  digitalWrite(dirMot1, LOW);
  digitalWrite(dirMot2, LOW);
  SoftPWMSetPercent(velMot1, velocidadePadrao);
  SoftPWMSetPercent(velMot2, velocidadePadrao);
}

void esquerda() {
  digitalWrite(dirMot1, HIGH);
  digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, velocidadePadrao);
  SoftPWMSetPercent(velMot2, 0); 
}

void direita() {
  digitalWrite(dirMot1, HIGH);
  digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, 0);
  SoftPWMSetPercent(velMot2, velocidadePadrao);
}

void parar() {
  SoftPWMSetPercent(velMot1, 0);
  SoftPWMSetPercent(velMot2, 0);
}