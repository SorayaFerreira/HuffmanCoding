#include <stdio.h>
#include <cstdint>
#include <vector>
#include <cmath>

using std::vector;
using namespace std;

/*
Cabeçalho:
    > 2 bytes, numFolhas, K, ;
    > 4 bytes, T, numero de vezes que vou encontrar uma folha,
    no momento da descompactação;
    > Código de cada caractere;
    > bits do percurso pré-ordem
    > Sequência de bits do arquivo compactado.

*/

class Node;

class Descompactador {
    private:
        FILE *fileRead; //arquivo compactado a ser lido
        FILE *fileWrite; //arquivo descompactado a ser gerado
        uint16_t K; //nº de folhas na árvore
        uint32_t T; //nº de caracteres no arquivo
        vector<uint8_t> caracters; // lista de bytes dos caracteres que
                                    //aparecem no arquivo (ordem do percurso)
        vector<uint8_t> bitsTree; // vetor com os bits da árvore, p reconstrução
        vector<Node *> newTree;

    public:
        Descompactador(FILE *fileRead, FILE *fileWrite);
        void readHeader(FILE *fileRead);
        void createTree(int iBitsTree = 0, int iLetter = 0);
        void writeDecode(vector<Node *> newTree, FILE *fileWrite);
        uint16_t getterK();
};

/*Nó para reconstrução da árvore de Huffman, sem as frequências*/
class Node {
private:
  uint8_t c; // Code
  Node *l;   // Left child
  Node *r;   // Right child

public:
  Node(uint8_t c, Node *l = nullptr, Node *r = nullptr); // Construtor folhas
  Node(Node *l = nullptr, Node *r = nullptr); // construtor nós internos
  
  uint8_t getCode(); // Devolve o código do caractere ;)
  void setCode(uint8_t cod); //atribui um código ao nó
  
  Node *getterLeft(); // Devolve o filho esquerdo  ;)
  void setterLeft(Node *no);

  Node *getterRight(); // Devolve o filho direito    ;)
  void setterRight(Node *no);

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
    uint8_t *getterBzinho();
};

int main(int argc, char *argv[]) {
    // argv[0] = nomeDoPrograma
    // argv[argc] = NULL;

    FILE *arquivo1, *arquivo2;
    arquivo1 = fopen(argv[2], "rb");
    arquivo2 = fopen(argv[3], "wb");

    if (!arquivo1) {
        printf("\tError reading from file :(\n");
        return -1;
    } else
        printf("\tArquivo lido ;)\n");

    if (*argv[1] == 'c') {
        // compactador(arquivo1, arquivo2);
    } else if (*argv[1] == 'd') {
        // descompactador(arquivo1, arquivo2);
        Descompactador decod(arquivo1, arquivo2);
        printf("\tO arquivo descompactado foi gerado.\n");

        decod.readHeader(arquivo1);
    }

    fclose(arquivo1);
    fclose(arquivo2);
    return 0;
}

// FUNÇÕES DA CLASSE DESCOMPACTADOR
Descompactador::Descompactador(FILE *fR, FILE *fW) : fileRead(fR), fileWrite(fW){};

//Lê o arquivo e inicializa os atributos para usar em outras funções.
void Descompactador::readHeader(FILE *fileRead) {
    fread(&(this->K), sizeof(uint16_t), 1, fileRead);
    printf("\tK: %d\n", this->K); //para teste
    
    fread(&(this->T), sizeof(uint32_t), 1, fileRead);
    printf("\tT: %d\n", this->T); //para teste

    printf("\tCaracteres presentes:");
    for (int i = 0; i < this->K; i++) {
        uint8_t car;
        fread(&car, sizeof(uint8_t), 1, fileRead);
        this->caracters.push_back(car);
        printf(" %c", this->caracters[i]);
    }

    int umLidos = 0; //número de 1s que já foram lidos
    Bits lerBits(fileRead); //objeto para ler a sequência de bits 
    //para reconstrução da árvore e armazenar no vetor bitsTree.
    
    printf("\n");
    for (int j = 0; umLidos < this->K; j++) {
        uint8_t bit = lerBits.obtem_bit();
        bitsTree.push_back(bit);
        printf(" %d", bitsTree[j]);

        if(bit)
            ++umLidos;
    }
}

void Descompactador::createTree(int iBitsTree, int iLetter) {


    
}


uint16_t Descompactador::getterK() {
    return this->K;
}

// FUNÇÕES DA CLASSE NODE
//construtor de um node
Node::Node(uint8_t c, Node *l, Node *r) : c(c), l(l), r(r) {}
Node::Node(Node *l, Node *r) : l(l), r(r) {}

//retorna ptr do filho direito
Node *Node::getterRight() {
  return this->r;
}

void Node::setterLeft(Node *no) {
    this->l = no;
}

//retorna ptr do filho esquerdo
Node *Node::getterLeft() {
  return this->l;
}

void Node::setterRight(Node *no) {
    this->r = no;
}

//Retorna true se é folha, e false se não
bool Node::leaf() const {
  if (this->l == nullptr && this->r == nullptr)
    return true;
  else
    return false;
}

//Retorna o caractere (em byte), presente numa folha
uint8_t Node::getCode() {
  return this->c;
}

void Node::setCode(uint8_t cod) {
    this->c = cod;
}

    // FUNÇÕES DA CLASSE BITS
Bits::Bits(FILE *file) : file(file), n(0){} // construtor

void Bits::adiciona_bit(uint8_t bit) {
    b[n++] = bit != 0; // para usar 0/1 mesmo se bit > 1

    if (n >= 8)
        descarrega();
}

void Bits::descarrega() {
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

uint8_t *Bits::getterBzinho() {
    return b;
}