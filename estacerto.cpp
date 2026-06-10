#include <SoftwareSerial.h>
#include <SoftPWM.h>
#include <Servo.h>

SoftwareSerial bt(11, 12); 
Servo atacante3;
Servo atacante2;

////vel controla velocidade e dir controla o sentido do motor, 1 e 2 sao os motores esquerdo e direito respectivamente
const int velMot1 = 9; 
const int dirMot1 = 10; 
const int velMot2 = 7;
const int dirMot2 = 8;  
const int pinoServo3 = 3;
const int pinoServo2 = 2;

int velocidadepadrao = 100; 
int velocidademenor = 75;
bool autonomaConcluida = false;

bool pausa(unsigned long ms);

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

void autonoma_direita() {
  Serial.println("Saindo da base");
  direita();   
  if (pausa(90)) return;
  frente();   
  if (pausa(4000)) return;
  esquerda();    
  if (pausa(1200)) return; 
  frente();     
  if (pausa(300)) return; 
  direita();   
  if (pausa(2000)) return; 
  frente();
  if (pausa(1300)) return; 
  parar();   
  if (pausa(700)) return;  
  chutar();
  Serial.println("Fim da fase autonoma"); ///acaba a fase autonoma idependente de ter feito gol
}

void autonoma_esquerda() {
  Serial.println("Saindo da base");
  frente();   
  if (pausa(1500)) return;  
  esquerda();    
  if (pausa(1400)) return;
  frente();   
  if (pausa(2100)) return; 
  direita();
  if (pausa(1700)) return; 
  frente();   
  if (pausa(2000)) return; 
  esquerda();     
  if (pausa(3900)) return;   
  frente();   
  if (pausa(2000)) return; 
  frente();   
  if (pausa(1600)) return; 
  parar();
  chutar();
  if (pausa(500)) return; 
  tras();   
  if (pausa(1000)) return; 
  esquerda();   
  if (pausa(2500)) return; 
  frente();   
  if (pausa(3000)) return; 
  parar();
  Serial.println("Fim da fase autonoma");
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

void direita() {
  digitalWrite(dirMot1, HIGH); 
  digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, velocidadepadrao); 
  SoftPWMSetPercent(velMot2, 0); 
}

void esquerda() {
  digitalWrite(dirMot1, HIGH); 
  digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, 0); 
  SoftPWMSetPercent(velMot2, velocidadepadrao);
}

void noroeste() {
  digitalWrite(dirMot1, HIGH); 
  digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, velocidademenor); 
  SoftPWMSetPercent(velMot2, velocidadepadrao); 
}

void nordeste() {
  digitalWrite(dirMot1, HIGH); 
  digitalWrite(dirMot2, HIGH);
  SoftPWMSetPercent(velMot1, velocidadepadrao); 
  SoftPWMSetPercent(velMot2, velocidademenor); 
}

void sudoeste() {
  digitalWrite(dirMot1, LOW); 
  digitalWrite(dirMot2, LOW);
  SoftPWMSetPercent(velMot1, velocidademenor); 
  SoftPWMSetPercent(velMot2, velocidadepadrao); 
}

void sudeste() {
  digitalWrite(dirMot1, LOW); 
  digitalWrite(dirMot2, LOW);
  SoftPWMSetPercent(velMot1, velocidadepadrao); 
  SoftPWMSetPercent(velMot2, velocidademenor); 
}

void parar() {
  SoftPWMSetPercent(velMot1, 0); 
  SoftPWMSetPercent(velMot2, 0);
}

void chutar() {
  atacante3.write(180); 
  atacante2.write(0); 
  pausa(2000);        
  atacante3.write(0); 
  atacante2.write(180); 
  pausa(500);         
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

void loop() {
  if (autonomaConcluida == false) {
    if (bt.available() > 0) {
      char comando = (char)bt.read();
        if (comando == 'D') { 
         autonoma_direita(); 
         autonomaConcluida = true; 
      }
      else if (comando == 'E') { 
         autonoma_esquerda(); 
         autonomaConcluida = true; 
      }
    }
  }
  else { 
    // Controle Bluetooth 
    if (bt.available() > 0) {
      char comando = (char)bt.read();
        if (comando == 'F') frente();
        else if (comando == 'B') tras();
        else if (comando == 'L') esquerda();
        else if (comando == 'R') direita();
        else if (comando == 'S') parar();
        else if (comando == 'C') chutar(); 
        else if (comando == 'a') noroeste(); 
        else if (comando == 'b') nordeste(); 
        else if (comando == 'c') sudoeste(); 
        else if (comando == 'd') sudeste(); 
        else if (comando == 'E') autonoma_esquerda();  ////tirar 
        else if (comando == 'D') autonoma_direita();  ////tirar
      }
    }
  }

