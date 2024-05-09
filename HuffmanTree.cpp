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
  int freq() const;      // Retorna a frequência do caractere ;)
  uint8_t code() const; // Devolve o código do caractere ;)
  Node *left();         // Devolve o filho esquerdo  ;)
  Node *right();        // Devolve o filho direito    ;)
  bool leaf() const;    // Devolve true se é folha e false caso contrário ;)
};


/* Lista de min-prioridades (min-heap) com nós de uma árvore de
   Huffman, utilizando a frequência como chave */
class MinHeap {
private:
  vector<Node *> v;                    // Vetor de ponteiros para objectos Node ;)
  void up(unsigned int i);             // Sobe ;)
  void down(unsigned int i);           // Desce ;)
  unsigned int parent(unsigned int i); // Pai ;)
  unsigned int left(unsigned int i);   // Filho esquerdo ;)
  unsigned int right(unsigned int i);  // Filho direito ;)

public:
  MinHeap();            // Construtor padrão ;)
  ~MinHeap();           // Destrutor padrão ;)
  size_t size();        // Tamanho da heap ;)
  void insert(Node *n); // Inserir elemento ;)
  Node *extract();      // Remover (menor) elemento ;)
  void troca(unsigned int i, unsigned int j); // ;)
};

MinHeap::MinHeap(){}

MinHeap::~MinHeap(){
  int i = 0;
  while(i < v.size()) {
    delete [] v[i];
  }
}

size_t MinHeap::size() {
  return v.size();
}

unsigned int MinHeap::left(unsigned int i) {
  return 2 * (i + 1) - 1;
}

unsigned int MinHeap::right(unsigned int i) {
  return 2 * (i + 1);
}

unsigned int MinHeap::parent(unsigned int i)
{
  return (i - 1) / 2;
}

void MinHeap::troca(unsigned int i, unsigned int j) {
  Node aux = *v[i];
  *v[i] = *v[j];
  *v[j] = aux;
}

void MinHeap::up(unsigned int i) { //Função Sobe

  while(v[parent(i)]->freq() > v[i]->freq()) {
    troca(i, parent(i));
    i = parent(i); 
  }
}

Node * Node::right() {
  return this->r;
}

Node * Node::left() {
  return this->l;
}

void MinHeap::down(unsigned int i) {
//Garante as propriedades da min heap, descendo na árvore

  unsigned int e, d, menor;
  e = left(i);
  d = right(i);
  if(e < size() && v[e]->freq() < v[i]->freq())
    menor = e;
  else
    menor = i;
  if(d < size() && v[d]->freq() < v[menor]->freq())
    menor = d;
  if(menor != i) {
    troca(i, menor);
    down(menor);
  }
}

int Node::freq() const {
  return this->f;
}

//Retorna o ponteiro para o Node de menor prioridade
Node * MinHeap::extract() {
  return v[0];
}


bool Node::leaf() const {
  if(this->l == nullptr && this->r == nullptr) 
    return true;
  else 
    return false;
}

uint8_t Node::code() const {
  return this->c;
}

void MinHeap::insert(Node *n) {
  v.push_back(n);
  up(v.size()-1);

  //atribuir os filhos esquerdo e direito.
}

//Cria o nó para ser utilizado na min heap
Node::Node(int f, uint8_t c, Node *l = nullptr, Node *r = nullptr) : 
  f(f), c(c), l(l), r(r) {}

//Cria o nó intermediário utilizado na construção da árvore de Huffman
Node::Node(int f = 0, Node *l = nullptr, Node *r = nullptr) :
  f(f), l(l), r(r) {
    this->c = NULL;
  }