/* 
   Tiago Queiroz e Alexandre Villares
   Matriz 8x8 animada com Arduino
   Objeto apresentado em outubro de 2016
   em Zonas de Compensação / IA-UNESP / São Paulo
   Baseado nos Autômatos celulares de Wolfram
   e no código de Daniel Shiffman http://natureofcode.com
*/

int generation; // Contador de gerações
int w = 10;
int matrix[8][8];  // matriz com histórico de gerações
int cols = 8;
int rows = 8 ;

//ruleset é o Array com a regra {0,1,1,0,1,1,0,1}
//int ruleset[] = {0,1,1,1,1,0,1,1};           // Rule 222
//int ruleset[] = {0,1,1,1,1,1,0,1};           // Rule 190
int ruleset[] = {0, 1, 1, 1, 1, 0, 0, 0};    // Rule 30
//int ruleset[] = {0,1,1,1,0,1,1,0};             // Rule 110

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  inicializa();  // Inicializa a matriz do Autômato Celular
}

void loop() {
  mostra();   // Manda para a matriz de LEDs
  regenera();
  delay(2000);
}

// Escolhedor de 'regra' aleatória
void randomiza() {
  for (int i = 0; i < 8; i++) {
    ruleset[i] = int(random(2));
  }
}

// Reset para geração/tempo 0
void inicializa() {
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      matrix[i][j] = 0;
    }
  }
  matrix[cols / 2][0] = 1; // Tudo começa com "1"(vivo/ligado) no meio da primeira linha
  generation = 0;         //na geração/tempo zero
}

// O método que calcula a próxima geração/tempo
void regenera() {
  // Para cada célula, determine o próximo estado, baseado no estado atual
  // e nos vizinhos imediatos, ignorando o das bordas
  for (int i = 0; i < cols; i++) {
    int esq  = matrix[(i + cols - 1) % cols][generation % rows]; // Estado do vizinho à esquerda
    int eu    = matrix[i][generation % rows];            // Estado atual da célula
    int dir = matrix[(i + 1) % cols][generation % rows]; // Estado do vizinho à direita
    matrix[i][(generation + 1) % rows] = rules(esq, eu, dir); // Calcula usando a 'regra'
  }
  generation++;
}

// Método que desenha a matriz
void mostra() {
  int offset = generation % rows;
  Serial.println("--------------------------------");
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      int y = j - offset;

      if (y <= 0) {
        y = rows + y;
      }

      if (matrix[i][i] == 1) {
        Serial.print(0);
      }
      else {
        Serial.print(1);
      }
    }
    Serial.println("");
  }

  Serial.println("\n\n");
}

// Implementação das regras de Wolfram
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
