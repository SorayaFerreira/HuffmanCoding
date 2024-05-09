#include <cstdint>
#include <cstdio>
#include <cmath>
/* Byte com buffer para leitura ou escrita bit-a-bit em arquivo. No
modo escrita, quanto o buffer completa 8 bits, escreve 1 byte no
arquivo. No modo leitura, quando o buffer fica vazio, lê 1 byte do
arquivo e preenche o buffer. Utilize cada objeto desta classe apenas
para ler ou apenas para escrever, nunca os dois (seu comportamento
seria indefinido) */

class Bits {
private:
FILE *file; // Ponteiro para o arquiv-o sendo lido/escrito
uint8_t b[8]; // Buffer com bits de um byte
uint8_t n; // Quantidade de posições ocupadas no vetor acima

public:
Bits(FILE *file); // Construtor padrão com o arquivo que será lido ou escrito
uint8_t ocupados(); // Devolve quantos bits estão ocupados no buffer do byte
uint8_t livres(); // Devolve quantos bits ainda podem ser adicionados ao buffer do byte

// Funções do modo escrita
void adiciona_bit(uint8_t bit); // Adiciona um bit 0 ou 1 ao buffer (escreve byte no arquivo se encher)
void descarrega(); // Força a escrita do byte no buffer (completa com 0s, caso necessário)

// Funções do modo leitura
uint8_t obtem_bit(); // Obtém o próximo bit do buffer (lê um byte do arquivo se estiver vazio)
};

int main()
{
FILE *original, *copia;
original = fopen("original.txt", "rb");
copia = fopen("copia.txt", "wb");

Bits in(original), out(copia);
uint8_t bit;

while ((bit = in.obtem_bit()) != 2) {
putchar('0' + bit);
fflush(stdout);
out.adiciona_bit(bit);
}
putchar('\n');
out.descarrega();

fclose(original);
fclose(copia);

return 0;
}

Bits::Bits(FILE *file) :
file(file),
n(0)
{ }

void Bits::adiciona_bit(uint8_t bit)
{
b[n++] = bit != 0; // para usar 0/1 mesmo se bit > 1

if (n >= 8)
descarrega();
}

void Bits::descarrega()
{
if (n == 0)
return;
// Deve completar o byte com 0s para preencher os 8 bits, caso n < 8

while (n < 8) // preenche com 0 os valores restantes do vetor(byte) caso esteja incompleto, até encher o byte
{
b[n++] = 0;
}

uint8_t aux = 0; // variavel auxiliar

for(uint8_t i = 0; i < 8; i++){ //transforma os numeros contidos no vetor b (byte em bytes), em um número de apenas uma variavel (byte unido)

if(b[i] == 1){
aux += pow(2,(7-i));
}

}

// Leia sobre a função fwrite
fwrite(&aux,1,1, file); // escreve o byte (aux) no arquivo

// No final, não esqueça de fazer n receber 0

n = 0;
}

uint8_t Bits::obtem_bit()
{
// Implementar
// Caso n == 0, deve ler 1 byte do arquivo e colocar no buffer, ou devolver 2 caso não haja mais bytes para serem lidos do arquivo
if (n == 0)
{
uint8_t byte; // cria a variavel byte, onde havera um numero binário

// Leia sobre a função fread

if(fread(&byte,1,1,file) == 0) // fread faz com que a variavel byte receba o byte do arquivo, o if compara para ver se o arquivo é valido, caso não for valido, retorna 2 (indicando erro na main)
{
return 2;
}

// prencher o vetor

for(uint8_t i = 0; i < 8; i++)
{
b[(7-i)]= (byte % 2);

byte = byte / 2;
}

n = 8;
}

// No final, devolver um bit (0 ou 1) e decrementar n

uint8_t bit = b[livres()];

n--;

return bit;
}

uint8_t Bits::ocupados()
{
return n;
}

uint8_t Bits::livres()
{
return 8 - n;
}

