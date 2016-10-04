/* Tiago Queiroz e Alexandre Villares
   Matriz 8x8 animada com Arduino
   https://github.com/belimawr/WolframCellularAutomataArduino
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
int regrasLegais[8][8] {    // Algumas regras de que gostamos
  {1, 0, 1, 1, 1, 1, 1, 0}, // Regra 125
  {1, 0, 0, 1, 0, 1, 0, 1}, // Regra 169
  {0, 1, 1, 1, 1, 0, 0, 0}, // Regra 30
  {0, 1, 1, 1, 0, 1, 1, 0}, // Regra 110
  {1, 0, 0, 0, 1, 1, 1, 0}, // degraus diagonais
  {1, 1, 0, 0, 1, 0, 0, 1}, // triângulos pequenos
  {1, 0, 0, 1, 0, 0, 1, 0}, // colchetes e pontos
  {1, 0, 0, 0, 0, 1, 0, 1}  // triângulos invertidos
};

void setup() {
  // Serial.begin(9600);  // Comunicação serial para debug
  matrixLED.begin(0x70);  // Avisa o endereço da matriz LED
  randomSeed(analogRead(5)); // Configura o gerador de aleatórios
  sorteiaRegra(); // Escolhe uma das 8 regras eleitas
  inicializa();   // Inicializa a matriz do Autômato Celular
}

void loop() {
  matrixLED.clear();
  mostra();       // Mostra na matriz de LEDs
  aplicaRegra();  // Calcula a próxima linha/geração
  delay(500);     // Pausa de meio segundo
  if (generation == 128) { // Recomeça a cada 128 gerações
    sorteiaRegra();
    inicializa();
  }
}

//Sorteia uma das 'regras' escolhidas pelos autores
void sorteiaRegra() {
  int sorteio = int(random(8));
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
  matrix[cols / 2][0] = 1; // Começa com "1" no meio da 1ª linha
  mostra();
  delay(500);
}

// Calcula a próxima geração/tempo
void aplicaRegra() {
  // Para cada célula, determine o próximo estado, baseado no atual
  // e nas vizinhas imediatas, ignorando a das bordas
  for (int i = 0; i < cols; i++) {
    // Estado da vizinha esquerda
    int esq  = matrix[(i + cols - 1) % cols][generation % rows];
    // Estado atual da célula
    int eu    = matrix[i][generation % rows];
    // Estado da vizinha direita
    int dir = matrix[(i + 1) % cols][generation % rows];
    // Calcula usando 'a regra'
    matrix[i][(generation + 1) % rows] = rules(esq, eu, dir);
  }
  generation++;
}

// Desenha a matriz de autômatos na matriz de LEDs
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
  matrixLED.writeDisplay();
}

// Aplicação de regra dos autômatos celulares de Wolfram numa célula
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

