#include <cstdint>
#include <cstdio>

// PERGUNTAS: o que fazer com os caracteres nulos ao final do arquivo?
// Por que no copia.txt aparece uma coisa esquisita?
//

/* Byte com buffer para leitura ou escrita bit-a-bit em arquivo. No
   modo escrita, quando o buffer completa 8 bits, escreve 1 byte no
   arquivo. No modo leitura, quando o buffer fica vazio, lê 1 byte do
   arquivo e preenche o buffer. Utilize cada objeto desta classe apenas
   para ler ou apenas para escrever, nunca os dois (seu comportamento
   seria indefinido) */

class Bits {
private:
  FILE *file;   // Ponteiro para o arquivo sendo lido/escrito
  uint8_t b[8]; // Buffer com bits de um byte
  uint8_t n;    // Quantidade de posições ocupadas no vetor acima

public:
  Bits(FILE *file); // Construtor padrão com o arquivo que será lido ou escrito
  uint8_t ocupados(); // Devolve quantos bits estão ocupados no buffer do byte
  uint8_t livres();   // Devolve quantos bits ainda podem ser adicionados ao
                      // buffer do byte

  // Funções do modo escrita
  void adiciona_bit(uint8_t bit); // Adiciona um bit 0 ou 1 ao buffer (escreve
                                  // byte no arquivo se encher)
  void descarrega(); // Força a escrita do byte no buffer (completa com 0s, caso
                     // necessário)

  // Funções do modo leitura
  uint8_t obtem_bit(); // Obtém o próximo bit do buffer (lê um byte do arquivo
                       // se estiver vazio)
};

int main() {
  FILE *original, *copia;
  original = fopen("original.txt", "rb");
  copia = fopen("copia.txt", "wb");

  Bits in(original), out(copia);
  uint8_t bit;

  while ((bit = in.obtem_bit()) != 2) {
    // putchar('0' + bit);
    out.adiciona_bit(bit);
  }
  putchar('\n');
  out.descarrega();

  fclose(original);
  fclose(copia);

  return 0;
}

Bits::Bits(FILE *file) : file(file), n(0) {}

void Bits::adiciona_bit(uint8_t bit) {
  b[n++] = bit != 0; // para usar 0/1 mesmo se bit > 1

  if (n == 8)
    descarrega();
}

void Bits::descarrega() {
  if (n == 0)
    return;

  if (n < 8) {
    for (int i = n; i < 8; i++) {
      b[i] = 0;
      n++;
    }
  }

  uint8_t buffer = 0;
  for (int i = 0; i < 8; i++) {
    buffer = buffer + b[i];
    if (i < 7) {
      buffer = buffer << 1;
    }
  }
  printf("%d;\n", buffer);
  fwrite(&buffer, sizeof(uint8_t), 1, file);
  n = 0;
  // Deve completar o byte com 0s para preencher os 8 bits, caso n < 8
  // Leia sobre a função fwrite
  // No final, não esqueça de fazer n receber 0
}

uint8_t Bits::obtem_bit() {
  /*se o arquivo acaba, retorna algo
  diferente de 0*/
  if (feof(file)) {
    return 2;
  }

  if (n == 0) {
    uint8_t buffer;
    uint8_t temp = fread(&buffer, sizeof(uint8_t), 1, file);
    if (!temp)
      printf("");

    for (int i = 7; i >= 0; i--) {
      b[i] = buffer % 2;
      buffer = buffer >> 1;
    }
    n = 8;
  }
  n--;
  // printf("%d,\n", b[7-n]);
  return b[7 - n];

  // Caso n == 0, deve ler 1 byte do arquivo e colocar no buffer, ou devolver 2
  // caso não haja mais bytes para serem lidos do arquivo Leia sobre a função
  // fread No final, devolver um bit (0 ou 1) e decrementar n
}

uint8_t Bits::ocupados() { return n; }
uint8_t Bits::livres() { return 8 - n; }
