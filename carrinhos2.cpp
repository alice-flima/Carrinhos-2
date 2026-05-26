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

void loop() {
  if (autonomaConcluida == false) {
    autonoma();
    autonomaConcluida = true; 
  }
  // Controle Bluetooth 
  if (bt.available() > 0) {
    char comando = (char)bt.read();
    if (comando == 'F') frente();
    else if (comando == 'B') tras();
    else if (comando == 'L') esquerda();
    else if (comando == 'R') direita();
    else if (comando == 'S') parar();
    else if (comando == 'C') chutar(); 
  }
}

void autonoma() {
  Serial.println("Saindo da base");
  ///inicio: sai da base e segue caminho pre-definido ate alem do W
  ////coloquei ele para começar já andando pela diagonal para avançar mais rapido, diminuir o risco de colisao e deixar a etapa mais fluida
  esquerda();  delay(200);  // curva a esquerda
  frente();    delay(5000); // sai da base para a diagonal direto em direcao ao W
  ////depois de alcançar o w :
  parar();     delay(200);  // Pausa rápida para estabilizar
  direita();   delay(500);  // Vira para tras em direçao ao cesto
  frente();    delay(2000); // Vai em direção a área das bolas
  direita();   delay(300);  // Alinha em direção à bolas
  parar();     delay(200);  // Pausa rápida

  Serial.println("Alcançamos a área! Buscando alvo pelos sensores");
//// busca pelas bolas: alinhar o robo com a bola oval
  while (true) {
    int luz1 = analogRead(A0); // Sensor Esquerdo
    int luz2 = analogRead(A1); // Sensor Direito
    
    int diferenca = luz1 - luz2;
    // Condição buscada: se ambos lerem valores muito altos, o carrinho achou a bola e ela está centralizada o suficiente para chutar
    if (luz1 > 900 && luz2 > 900) { 
      Serial.println("Bola centralizada e perto");
      frente(); delay(500); // empurra para posicionar e aproximar do cesto
      parar();
      chutar();            
      break;                // Sai do loop e finaliza a autonoma
    } 
    
    // Correção de trajetória:
    if (abs(diferenca) <= 100) {
      frente(); // Se a luz estiver equilibrada, vai reto
    } 
    else if (diferenca > 100) {
      esquerda(); // Se o sensor 1 receber mais luz, corrige para a esquerda
    } 
    else if (diferenca < -100) {
      direita(); // Se o sensor 2 receber mais luz, corrige para a direita
    }
  } 
  Serial.println("Fim da fase autonoma"); ///acaba a fase autonoma idependente de ter feito gol
  parar();
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
  atacante.write(90); 
  delay(2000);        
  atacante.write(0); 
  delay(500);         
}