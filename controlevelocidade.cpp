#include <SoftwareSerial.h>
#include <SoftPWM.h>
#include <Servo.h>

SoftwareSerial bt(11, 12); 
Servo atacante;

const int velMot1 = 9; 
const int dirMot1 = 10; 
const int velMot2 = 7;
const int dirMot2 = 8;  
const int pinoServo = 3;

int velocidadepadrao = 70; // Começa em 70% por segurança
bool autonomaConcluida = false;

bool pausa(unsigned long ms);

void setup() {
  atacante.attach(pinoServo);
  atacante.write(0);
  pinMode(dirMot1, OUTPUT);
  pinMode(dirMot2, OUTPUT);
  
  Serial.begin(9600);
  bt.begin(9600);
  
  // CRÍTICO: Define um tempo de espera baixo para ler o slider sem dar lag no robô
  bt.setTimeout(50); 
  
  SoftPWMBegin();
  parar();
  
  Serial.println(" Sistema Pronto");
}

void autonoma_direita() {
  Serial.println("Saindo da base");
  frente();   
  if (pausa(3000)) return;
  esquerda();    
  if (pausa(100)) return;
  frente();     
  if (pausa(8000)) return;
  parar();   
  if (pausa(500)) return;
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
      if (comando == 'A') { 
         autonoma_direita(); 
         autonomaConcluida = true; 
      }
    }
  }
  else { 
    // Controle Bluetooth 
    if (bt.available() > 0) {
      char comando = (char)bt.read();
      
      // NOVA FUNÇÃO: Se o comando for 'V', lê o valor do slider
      if (comando == 'V') {
        int novaVelocidade = bt.parseInt(); // Lê o número que vem depois do 'V'
        
        // Garante que o valor fique estritamente entre 0 e 100%
        velocidadepadrao = constrain(novaVelocidade, 0, 100); 
        
        Serial.print("Nova velocidade definida: ");
        Serial.println(velocidadepadrao);
      }
      
      // Comandos de movimentação manual
      else if (comando == 'F' || comando == 'B' || comando == 'L' || comando == 'R' || comando == 'S' || comando == 'C') {
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