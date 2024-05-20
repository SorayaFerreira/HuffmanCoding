#ifndef NO_H
#define No_H

#include <cstdint>
#include <cstdio>
#include <vector>

using namespace std;

class Bytes{ // classe para percorrer os Bytes do arquivo
    private:
    FILE * file;

    public:
    Bytes(FILE * file): file(file){}
    int obtem_byte();
};

class No {
    private:

    int frequencia; // - quantas vezes o byte aparece
    No * esq; // - filho esquerdo
    No * dir; // - filho direito
    No * pai; // pai do no 
    uint8_t compacto; // - o código gerado para ser o byte no arquivo compactado
    bool folha; // - indicador se o nó é galho da arvore ou uma folha

    public:

    No() : frequencia(0), esq(nullptr), dir(nullptr), pai(nullptr), compacto(0), folha(false) {}

    // Métodos para obter e definir a frequência
    int getFrequencia();
    void setFrequencia(int freq);

    // Métodos para obter e definir o filho esquerdo
    No * getFilho_esquerdo();
    void setFilho_esquerdo(No * &filho, No * &novo_pai);

    // Métodos para obter e definir o filho direito
    No * getFilho_direito();
    void setFilho_direito(No * &filho, No * &novo_pai);

    // Métodos para obter e definir o valor compactado
    uint8_t getCompacto();
    void setCompacto(uint8_t bin);

     // Método para verificar se é folha
    bool getFolha();
    void setFolha();

    void imprime_No(int byte);
};

int pai_Heap(int i);

int direito(int i);

int esquerdo(int i);

void troca(No* & ponteiro_1,No* & ponteiro_2);

void imprime_arvore(No * raiz);

void desce(vector<No*>& vetor, int pai, int tamanho);

void imprime_heap(vector<No * > &vetor);

void min_heap(vector<No*> & vetor);

void sobe(vector<No*>& vetor, int i);

void cria_arvore(vector<No*> & vetor);

void distribui_Byte(vector<No*> &vetor_frequencia,vector<No*> &indice, Bytes &entrada);

void retira_Minimo(vector<No*> &vetor);



#endif //NO_H