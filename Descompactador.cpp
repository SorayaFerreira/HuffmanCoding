#include <stdio.h>
#include <cstdint>
#include <vector>
/*
Cabeçalho:
    > 2 bytes, numFolhas, K;
    > 4 bytes, T
    > Código de cada caractere;
    > Sequência de bits do arquivo compactado.
    Tipos inteiros não sinalizados de 2 e 4 bytes são definidos em <cstdint> como uint16_t
e uint32_t, respectivamente.
*/

class Descompactador {
    private:
        FILE *fileRead;
        FILE *fileWrite;

    public:
        Descompactador(FILE *fileRead, FILE *fileWrite);


};

class Node {
private:
  uint8_t c; // Code
  Node *l;   // Left child
  Node *r;   // Right child

public:
  Node(int f, uint8_t c, Node *l = nullptr, Node *r = nullptr); // Construtor
  Node(int f = 0, Node *l = nullptr, Node *r = nullptr);        // Construtor
  int freq() const;                                             // Retorna a frequência do caractere ;)
  uint8_t code() const;                                         // Devolve o código do caractere ;)
  Node *left();                                                 // Devolve o filho esquerdo  ;)
  Node *right();                                                // Devolve o filho direito    ;)
  bool leaf() const;
  uint8_t obtem_bit();                                            // Devolve true se é folha e false caso contrário ;)
};

Descompactador::Descompactador(FILE *fR, FILE *fW) : fileRead(fR), fileWrite(fW){};

Node *Node::right()
{
  return this->r;
}

Node *Node::left()
{
  return this->l;
}

bool Node::leaf() const
{
  if (this->l == nullptr && this->r == nullptr)
    return true;
  else
    return false;
}

uint8_t Node::code() const {
  return this->c;
}

Node::Node(int f, uint8_t c, Node *l = nullptr, Node *r = nullptr) : c(c), l(l), r(r) {}

Node::Node(int f = 0, Node *l = nullptr, Node *r = nullptr) : l(l), r(r) {}
