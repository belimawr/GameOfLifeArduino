/* Tiago Queiroz e Alexandre Villares
   Matriz 8x8 animada com Arduino
   Objeto apresentado em outubro de 2016
   em Zonas de Compensação / IA-UNESP / São Paulo
   Baseado nos Autômatos celulares de Wolfram
   e código de Daniel Shiffman http://natureofcode.com
*/

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_8x8matrix matrixLED = Adafruit_8x8matrix();

int generation;  // Contador de gerações
int matrix[8][8];// Matriz com histórico de gerações
int cols = 8;
int rows = 8 ;
int ruleset[8];  // Array de 1s e 0s de uma 'regra'
int regrasLegais[4][8] {    // Algumas regras de que gostamos
  {0, 1, 1, 1, 1, 0, 1, 1}, // Regra
  {0, 1, 1, 1, 1, 1, 0, 1}, // Regra
  {0, 1, 1, 1, 1, 0, 0, 0}, // Regra
  {0, 1, 1, 1, 0, 1, 1, 0}  // Regra
};

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(5));
  matrixLED.begin(0x70);  // Avisa o endereço da matriz LED
  inicializa();  // Inicializa a matriz do Autômato Celular
  sorteiaRegra();
}

void loop() {
  matrixLED.clear();
  mostra();   // Manda para a matriz de LEDs
  aplicaRegra();
  delay(500);
  if (generation == 128) { //e Es
    inicializa();
    sorteiaRegra();
  }
}

//Sorteia uma das 'regras' escolhidas pelos autores
void sorteiaRegra() {
  int sorteio = int(random(4));
  Serial.println(sorteio);
  for (int i = 0; i < 8; i++) {
    ruleset[i] = regrasLegais[sorteio][i];
  }
}

// Restaura matriz para geração (ou tempo) zero
void inicializa() {
  generation = 0; // Zera contador de geração/tempo
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      matrix[i][j] = 0;  // Zera células
    }
  }
  matrix[cols / 2][0] = 1; // Começa com "1" no meio da primeira linha
  mostra();
  delay(500);
}

// Calcula a próxima geração/tempo
void aplicaRegra() {
  // Para cada célula, determine o próximo estado, baseado no estado atual
  // e nas vizinhas imediatas, ignorando a das bordas
  for (int i = 0; i < cols; i++) {
    int esq  = matrix[(i + cols - 1) % cols][generation % rows]; // Estado da vizinha esquerda
    int eu    = matrix[i][generation % rows];            // Estado atual da célula
    int dir = matrix[(i + 1) % cols][generation % rows]; // Estado da vizinha direita
    matrix[i][(generation + 1) % rows] = rules(esq, eu, dir); // Calcula usando 'a regra'
  }
  generation++;

}

// Desenha a matriz no autômato na matriz de LEDs
void mostra() {
  int offset = generation % rows;
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      int y = j - offset;
      if (y <= 0) {
        y = rows + y;
      }
      if (matrix[i][j] == 1) {  // Se a célula i,j está viva/ativa/1
        matrixLED.drawPixel(i, y - 1, 0); // Desligar LED
      } else {                            // Senão
        matrixLED.drawPixel(i, y - 1, 1); // Ligar LED
      }
    }
  }
  matrixLED.writeDisplay(); /
}

// Aplicação de regra do autômato celular de Wolfram numa célula
int rules (int esquerda, int centro, int direita) {
  if (esquerda == 1 && centro == 1 && direita == 1) return ruleset[7];
  if (esquerda == 1 && centro == 1 && direita == 0) return ruleset[6];
  if (esquerda == 1 && centro == 0 && direita == 1) return ruleset[5];
  if (esquerda == 1 && centro == 0 && direita == 0) return ruleset[4];
  if (esquerda == 0 && centro == 1 && direita == 1) return ruleset[3];
  if (esquerda == 0 && centro == 1 && direita == 0) return ruleset[2];
  if (esquerda == 0 && centro == 0 && direita == 1) return ruleset[1];
  if (esquerda == 0 && centro == 0 && direita == 0) return ruleset[0];
  return 0;
};
