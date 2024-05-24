#include <stdio.h>
#include <cstdint>
#include <vector>
#include <cmath>

using std::vector;
using namespace std;

/*
Cabeçalho:
    > 2 bytes, numFolhas, K, ;
    > 4 bytes, T
    > Código de cada caractere;
    > Sequência de bits do arquivo compactado.
    
*/

class Descompactador {
    private:
        FILE *fileRead;
        FILE *fileWrite;
        uint16_t K;
        uint32_t T;
        vector<uint8_t> caracteres;
        uint8_t *bitsTree;

    public:
        Descompactador(FILE *fileRead, FILE *fileWrite);
        void readHeader(FILE *fileRead);
        void createTree(uint8_t *bitsTree);
};

/*Nó para reconstrução da árvore de Huffman, sem as frequências*/
class Node {
private:
  uint8_t c; // Code
  Node *l;   // Left child
  Node *r;   // Right child

public:
  Node(uint8_t c, Node *l = nullptr, Node *r = nullptr); // Construtor                                            
  uint8_t code() const; // Devolve o código do caractere ;)
  Node *getterLeft(); // Devolve o filho esquerdo  ;)
  Node *getterRight(); // Devolve o filho direito    ;)
  bool leaf() const; // Devolve true se é folha e false caso contrário ;)
};

class Bits {
private:
    FILE *file;   // Ponteiro para o arquivo sendo lido/escrito
    uint8_t b[8]; // Buffer com bits de um byte
    uint8_t n;    // Quantidade de posições ocupadas no vetor acima

public:
    uint8_t livres();   // Devolve quantos bits ainda podem ser adicionados ao buffer do byte
    Bits(FILE *file);   // Construtor padrão com o arquivo que será lido ou escrito

    // Funções do modo escrita
    void adiciona_bit(uint8_t bit); // Adiciona um bit 0 ou 1 ao buffer (escreve
                                    // byte no arquivo se encher)
    void descarrega();              // Força a escrita do byte no buffer (completa com 0s, caso
                                    // necessário)

    // Funções do modo leitura
    uint8_t obtem_bit(); // Obtém o próximo bit do buffer (lê um byte do arquivo
                         // se estiver vazio)
};

Descompactador::Descompactador(FILE *fR, FILE *fW) : fileRead(fR), fileWrite(fW){};

// FUNÇÕES DA CLASSE NODE
Node *Node::getterRight() {
  return this->r;
}

Node *Node::getterLeft() {
  return this->l;
}

bool Node::leaf() const {
  if (this->l == nullptr && this->r == nullptr)
    return true;
  else
    return false;
}

uint8_t Node::code() const {
  return this->c;
}

Node::Node(uint8_t c, Node *l = nullptr, Node *r = nullptr) : c(c), l(l), r(r) {}

//FUNÇÕES DA CLASSE BITS
Bits::Bits(FILE *file) : file(file), n(0){}

void Bits::adiciona_bit(uint8_t bit) {
    b[n++] = bit != 0; // para usar 0/1 mesmo se bit > 1

    if (n >= 8)
        descarrega();
}

void Bits::descarrega()
{
    if (n == 0)
        return;
    // Deve completar o byte com 0s para preencher os 8 bits, caso n < 8

    while (n < 8) {// preenche com 0 os valores restantes do vetor(byte) caso esteja incompleto, até encher o byte
        b[n++] = 0;
    }

    uint8_t aux = 0; // variavel auxiliar

    for (uint8_t i = 0; i < 8; i++)
    { // transforma os numeros contidos no vetor b (byte em bytes), em um número de apenas uma variavel (byte unido)

        if (b[i] == 1) {
            aux += pow(2, (7 - i));
        }
    }

    fwrite(&aux, 1, 1, file); // escreve o byte (aux) no arquivo

    n = 0;
}

uint8_t Bits::obtem_bit() {
    // Caso n == 0, deve ler 1 byte do arquivo e colocar no buffer,
    //ou devolver 2 caso não haja mais bytes para serem lidos do arquivo
    if (n == 0) {
        uint8_t byte; // cria a variavel byte, onde havera um numero binário

        if (fread(&byte, 1, 1, file) == 0) { // fread faz com que a variavel byte receba o byte do arquivo, 
        //o if compara para ver se o arquivo é valido, caso não for valido, retorna 2 (indicando erro na main)
            return 2;
        }
        //Para preencher o vetor
        for (uint8_t i = 0; i < 8; i++) {
            b[(7 - i)] = (byte % 2);

            byte = byte / 2;
        }
        n = 8;
    }
    // Devolve um bit (0 ou 1) e decrementar n
    uint8_t bit = b[livres()];
    n--;
    return bit;
}

uint8_t Bits::livres() {
    return 8 - n;
}