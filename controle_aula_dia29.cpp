#include <SoftwareSerial.h>
#include <SoftPWM.h>
#include <Servo.h>

SoftwareSerial bt(11, 12); 
Servo atacante;

////vel controla velocidade e dir controla o sentido do motor, 1 e 2 sao os motores esquerdo e direito respectivamente
const int velMot1 = 9; 
const int dirMot1 = 10; 
const int velMot2 = 7;
const int dirMot2 = 8;  
const int pinoServo = 3;

int velocidadepadrao = 70; 
bool autonomaConcluida = false;

bool pausa(unsigned long ms);

void setup() {
  atacante.attach(pinoServo);   ///o servo começa recolhido
  atacante.write(0);
  pinMode(dirMot1, OUTPUT);
  pinMode(dirMot2, OUTPUT);
  
  Serial.begin(9600);
  bt.begin(9600);
  SoftPWMBegin();
  parar();
  
  Serial.println(" Sistema Pronto");
}

void autonoma_direita() {
  Serial.println("Saindo da base");
  
  frente();   
  if (pausa(3000)) return;  // curva a esquerda
  
  esquerda();    
  if (pausa(100)) return; // avanca ate o W e, depois, ate a bola oval
  
  frente();     
  if (pausa(8000)) return;  // Pausa rápida para estabilizar
  
  parar();   
  if (pausa(500)) return;  // fica de frente para a primeira bola
  Serial.println("Fim da fase autonoma"); ///acaba a fase autonoma idependente de ter feito gol
}

void frente() {
  digitalWrite(dirMot1, HIGH); 
  digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, velocidadepadrao); 
  SoftPWMSetPercent(velMot2, velocidadepadrao);
}

void tras() {
  digitalWrite(dirMot1, LOW); 
  digitalWrite(dirMot2, LOW);
  SoftPWMSetPercent(velMot1, velocidadepadrao); 
  SoftPWMSetPercent(velMot2, velocidadepadrao);
}

void esquerda() {
  digitalWrite(dirMot1, HIGH); 
  digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, velocidadepadrao); 
  SoftPWMSetPercent(velMot2, 0); 
}

void direita() {
  digitalWrite(dirMot1, HIGH); 
  digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, 0); 
  SoftPWMSetPercent(velMot2, velocidadepadrao);
}

void parar() {
  SoftPWMSetPercent(velMot1, 0); 
  SoftPWMSetPercent(velMot2, 0);
}

bool chutar() {
  atacante.write(90); 
  if (pausa(2000)) return true;        
  
  atacante.write(0); 
  if (pausa(500)) return true;       
  
  return false;
}


void loop() {
  if (autonomaConcluida == false) {
    autonoma();
    autonomaConcluida = true; 
    }
  else { 
    // Controle Bluetooth 
    if (bt.available() > 0) {
      char comando = (char)bt.read();
      if (comando == 'F' || comando == 'B' || comando == 'L' || comando == 'R' || comando == 'S' || comando == 'C') {
        if (comando == 'F') frente();
        else if (comando == 'B') tras();
        else if (comando == 'L') esquerda();
        else if (comando == 'R') direita();
        else if (comando == 'S') parar();
        else if (comando == 'C') chutar(); 
      }
    }
  }
}

bool pausa(unsigned long ms) {
  unsigned long inicio = millis();
  while (millis() - inicio < ms) {
    if (bt.available() > 0) {
      char c = (char)bt.read();
      if (c == 'T') {
        parar(); 
        autonomaConcluida = true; 
        Serial.println("Autonoma interrompida (T)");
        return true; 
      }
    }
  }
  return false; 
}
