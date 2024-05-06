#include <cstddef>
#include <cstdint>
#include <vector>

using std::vector;

/* Nó de uma árvore de Huffman */
class Node {
private:
  int f;     // Frequency
  uint8_t c; // Code
  Node *l;   // Left child
  Node *r;   // Right child
  
public:
  Node(int f, uint8_t c, Node *l = nullptr, Node *r = nullptr); // Construtor
  Node(int f = 0, Node *l = nullptr, Node *r = nullptr);        // Construtor
  int freq() const;     // Devolve a frequência do caractere
  uint8_t code() const; // Devolve o código do caractere
  Node *left();         // Devolve o filho esquerdo
  Node *right();        // Devolve o filho direito
  bool leaf() const;    // Devolve true se é folha e false caso contrário
};


/* Lista de min-prioridades (min-heap) com nós de uma árvore de
   Huffman, utilizando a frequência como chave */
class MinHeap {
private:
  vector<Node *> v;                    // Elementos
  void up(unsigned int i);             // Sobe
  void down(unsigned int i);           // Desce
  unsigned int parent(unsigned int i); // Pai
  unsigned int left(unsigned int i);   // Filho esquerdo
  unsigned int right(unsigned int i);  // Filho direito

public:
  MinHeap();            // Construtor padrão
  ~MinHeap();           // Destrutor padrão
  size_t size();        // Tamanho da heap
  void insert(Node *n); // Inserir elemento
  Node *extract();      // Remover (menor) elemento
};