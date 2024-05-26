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
class Bits;
class Node;

class Descompactador {

    private:
        FILE *fileRead;            // arquivo compactado a ser lido
        FILE *fileWrite;           // arquivo descompactado a ser gerado
        uint16_t K;                // nº de folhas na árvore
        uint32_t T;                // nº de caracteres no arquivo
        vector<uint8_t> caracters; // lista de bytes dos caracteres que
                                // aparecem no arquivo (ordem do percurso)
        vector<uint8_t> bitsTree;  // vetor com os bits da árvore, p reconstrução
        vector<Node *> newTree;    // vetor com a árvore reconstruída
        int tamanho;               // número de bits do percurso pré-ordem no cabeçalho
        Node *noRaiz; //nó raiz da árvore reconstruída
        Bits *lerBits;

    public : Descompactador(FILE *fileRead, FILE *fileWrite); // Construtor
        Node *createTree(int &iBitsTree, int &iLetter, int tam); // Cria a árvore a partir dos bits do percurso
        void readHeader(FILE *fileRead, Bits *lerBits);                           // lê o cabeçalho e armazena nos atributos privados acima
        void writeDecode(Node *noh, Bits *obj);         // escreve no arquivo os bytes descompactados
        uint16_t getterK();                                        // para acessar o K
        void manageNewTree(); //para auxiliar a criação da árvore
        void percursoPreOrdem(Node *no);
};

/*Nó para reconstrução da árvore de Huffman, sem as frequências*/
class Node {
    friend class Descompactador;

    private:
        uint8_t c; // Code
        Node *l;   // Left child
        Node *r;   // Right child
        bool leaf; // folha (pode ser sim, ou não)

    public:
        Node(uint8_t c = 0, Node *l = nullptr, Node *r = nullptr, bool leaf = (false)); // Construtor folhas
        // Node(Node *l = nullptr, Node *r = nullptr); // construtor nós internos

        uint8_t getCode();                                             // Devolve o código do caractere ;)
        void setCode(uint8_t cod);                                     // atribui um código ao nó

        Node *getterLeft(); // Devolve o filho esquerdo  ;)
        void setterLeft(Node *no);

        Node *getterRight(); // Devolve o filho direito    ;)
        void setterRight(Node *no);

        

        // bool leaf() const; // Devolve true se é folha e false caso contrário ;)
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



//MAIN***************************************************************************************************
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
    
        Descompactador decod(arquivo1, arquivo2);
        printf("\n\tO arquivo descompactado foi gerado.\n");
     }

    fclose(arquivo1);
    fclose(arquivo2);
    return 0;
}

// FUNÇÕES DA CLASSE DESCOMPACTADOR****************************************************************************************

Descompactador::Descompactador(FILE *fR, FILE *fW) : fileRead(fR), fileWrite(fW){
    lerBits = new Bits(fileRead);// objeto para ler a sequência de bits
    // para reconstrução da árvore e armazenar no vetor bitsTree.

    this->readHeader(fR, lerBits);
    this->manageNewTree();
    uint8_t bitFromFile;
    this->writeDecode(this->noRaiz, this->lerBits);

};

void Descompactador::readHeader(FILE *fileRead, Bits *lerBits) {
//Lê o arquivo e inicializa os atributos para usar em outras funções.

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

    
    printf("\n");
    uint8_t bit;
    for (int j = 0; umLidos < this->K; j++)
    {
        bit = lerBits->obtem_bit();
        bitsTree.push_back(bit);
        printf(" %d", bitsTree[j]);

        if(bit)
            ++umLidos;
    }
    this->tamanho = bitsTree.size();
    printf("\n");
}


void Descompactador::manageNewTree() {
/*Auxilia a criação da nova árvore de Huffman, chamando 
a função createTree com seus devidos parâmetros e armazenando
os ponteiros da árvore no vetor newTree.*/

    int i = 0, j = 0;
    this->noRaiz = createTree(i, j, this->tamanho);
    printf("\tnoRaiz: %d\n", noRaiz->getCode());

    Node *aux = noRaiz;
    this->percursoPreOrdem(aux);

    for (int i = 0; i < tamanho; ++i) {
        printf(" %d ", newTree[i]->c);
    }
    printf("\n");
}

void Descompactador::percursoPreOrdem(Node *no)
{ // Armazena os ponteiros da nova árvore no vetor newTree
// na sequência de pré-ordem.
    if (no)
    {
        // visita a raiz
        printf("\tNo: %c\n", no->c);
        newTree.push_back(no);
        // desce para o filho esquerdo, se ele existir
        percursoPreOrdem(no->l);
        // desce para o filho direito, se ele existir
        percursoPreOrdem(no->r);
    }
    else
    {
        return;
    }
}

Node * Descompactador::createTree(int &iBitsTree, int &iLetter, int tam)
{
    /* iBitsTree recebe o índice do vetor de bits do percuro pré-ordem
    usado para reconstrução da árvore;
    iLetter recebe o índice do vetor de letras presentes no arquivo (folhas da tree)
    'tam' é o comprimento do vetor bitsTree, isto é, o número de nós da árvore*/

    if (iBitsTree >= tam) // Para evitar segmentation fault
        return nullptr;

    if (bitsTree[iBitsTree] == 1)
    {//se for nó folha, então
        ++iBitsTree; 
        return new Node(caracters[iLetter++], nullptr, nullptr, true); //retorna o nó folha criado
    }
    else
    { // se não for, é nó interno
        Node *noh = new Node(0, nullptr, nullptr, false);
        ++iBitsTree;
        noh->setterLeft(createTree(iBitsTree, iLetter, tam));
        noh->setterRight(createTree(iBitsTree, iLetter, tam));
        return noh;
    }
}


void Descompactador::writeDecode(Node *noh, Bits *obj)
{
    /*'count' conta o número de folhas que já foram visitadas
    'bitFromFile' recebe um bit do arquivo compactado
    'noh' recebe o nó da árvore que será visitado ou não
    'obj' é uma instância da classe Bits para ler o arquivo, obtendo os bits*/
    // na hora de chamar a função, já terá o primeiro obtem_bit
    uint8_t bitFromFile;
    Node *aux = noh;

    for (uint8_t count = 0; count < this->T;)
    {
        bitFromFile = obj->obtem_bit();

        if (aux->leaf) {
            uint8_t byte = aux->c;
            count = count + 1;
            fwrite(&byte, sizeof(uint8_t), 1, fileWrite);
            //fprintf(this->fileWrite, "%c", byte);
            aux = noh;
            
        } else {
            if (bitFromFile == 0) {
                aux = aux->l;
            } else {
                aux = aux->r;
            }
        } 
    }
}

uint16_t Descompactador::getterK() {
    return this->K;
}

// FUNÇÕES DA CLASSE NODE *************************************************************************************

//construtor de um node
Node::Node(uint8_t c, Node *l, Node *r, bool leaf) : c(c), l(l), r(r), leaf(leaf) {}
//Node::Node(Node *l, Node *r) : l(l), r(r) {}

//retorna ptr do filho direito
Node *Node::getterRight() {return this->r;}
void Node::setterRight(Node *no) {this->r = no;}

//retorna ptr do filho esquerdo
Node *Node::getterLeft() {return this->l;}
void Node::setterLeft(Node *no) {this->l = no;}

//Retorna o caractere (em byte), presente numa folha
uint8_t Node::getCode() {return this->c;}

void Node::setCode(uint8_t cod) {this->c = cod;}

// FUNÇÕES DA CLASSE BITS*************************************************************************************************
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

uint8_t Bits::livres() {return 8 - n;}

uint8_t *Bits::getterBzinho() {return b;}