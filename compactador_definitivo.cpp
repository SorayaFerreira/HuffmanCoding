#include <cstdint>
#include <cstdio>
#include <vector>
#include <cmath>

using namespace std;

// CABEÇALHO

class Compactador;
class Descompactador;

class No {
    private:

    int frequencia; // - quantas vezes o byte aparece
    No * esq; // - filho esquerdo
    No * dir; // - filho direito
    vector<uint8_t> codigo; // - o código gerado para ser o byte no arquivo compactado
    bool folha; // - indicador se o nó é galho da arvore ou uma folha
    uint8_t byte_letra; // letra a qual o nó representa em bytes

    public:

    No() : frequencia(0), esq(nullptr), dir(nullptr), codigo(0), folha(false) {}
    ~No();

    // Métodos para obter e definir a frequência
    int getFrequencia();
    void setFrequencia(int freq);

    // Métodos para obter e definir o filho esquerdo
    No * getFilho_esquerdo();
    void setFilho_esquerdo(No * filho);

    // Métodos para obter e definir o filho direito
    No * getFilho_direito();
    void setFilho_direito(No * filho);

    // Métodos para obter e definir o valor compactado
    vector<uint8_t> getCodigo();
    void setCodigo(vector<uint8_t> &bin);

     // Método para verificar se é folha
    bool getFolha();
    void setFolha();

    uint8_t getByte_letra(); //retorna o Byte (letra) que o nó representa
    void setByte_letra(uint8_t x);//determina o Byte (letra) que o nó representa

    void imprime_No(int byte);// imprime o nó (para testes)

    void percorre_Arvore(No * raiz, Compactador & compact); //Percorre a arvore a partir do No raiz, estabelecendo seus dados
    void Destroi_arvore(No * raiz);
    void Destroi_Arvore_Raiz(No * raiz);
};

class Compactador
{
   private:

   FILE * leitor; // ponteiro leitor
   FILE * escritor; // ponteiro que escreverá o arquivo compactado

   vector<No*> vetor_frequencia;//{256, nullptr}; // vetor de ponteiros para No (objetos)
   vector<No*> indice; // vetor de indice, receberá os nós ultilizados na compactacao do arquivo

   uint16_t alfabeto_k; // número de letras no alfabeto
   uint32_t letras_t; // número de letras impressas 

   vector<uint8_t> codigo_Arvore; // vetor que guardará o codigo da arvore em binario
   vector<uint8_t> codigo_No; // vetor que guardara o codigo que o no compactado representa
   
   uint8_t buffer_elementos; // quantidade de elementos inseridos no buffer
   uint8_t buffer[8]; // vetor de buffer para auxiliar na escrita do arquivo compactado
  

   public:
   Compactador(FILE * leitor, FILE * escritor); // construtor
   ~Compactador(); // destrutor
   int obtem_Byte(); // obtem byte do arquivo lido
   void distribui_Byte(); // cria os devidos Nos
   void cria_Arvore(); // cria a arvore de huffman

   void adiciona_codigo_Arvore(uint8_t x); // usado ao percorrer a arvore, para settar os numeros
   void adiciona_codigo_No(uint8_t x); // cria o codigo do No
   void remove_codigo_No(); // remove o ultimo elemento do No ao retornar da recursao
   vector<uint8_t> devolve_Codigo_No(); // devolve o vector de codigo do no
   vector<uint8_t> devolve_Codigo_Arvore(); // retorne o vector de codigo da arvore
   void escreve_Byte(uint8_t byte); // escreve byte no arquivo compactado
   void descarrega(); // escreve os elementos do buffer no arquivo compactado
   void escreve_Vector(vector<uint8_t> vector); // escreve os elementos do vecto no buffer
   void escreve_Arquivo_Compactado(); // escreve o codigo da arvore e em seguida as letras codificadas no arquivo compactado
};

class Descompactador
{
    private:
    vector<No*> indice;

    FILE * leitor;
    FILE * escritor;

    uint16_t alfabeto_k;
    uint32_t letras_t;

    vector<uint8_t> codigo_Arvore;

    uint8_t buffer[8];
    uint8_t buffer_elementos;

    public:
    Descompactador(FILE * leitor, FILE * escritor);
    ~Descompactador();
    //~Descompactador();

    void Obtem_Cabecalho();
    void Obtem_Letras();
    void Obtem_Codigo_Arvore();
    uint8_t Obtem_Bit();
    No * Cria_Arvore();
    void Descompacta_bits();
    void Percorre_Arvore(No * raiz);
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

void retira_Minimo(vector<No*> &vetor);

// CODIGO MAIN

int main(int argc, char *argv[]){

    FILE * arquivo_lido = fopen(argv[2], "rb"); //ponteiro de arquivo apontando para arquivo desejado
    FILE * arquivo_escrito = fopen(argv[3], "wb");

    char modo = argv[1][0];

    printf("Leitor inicializado\n");

    if (!arquivo_lido) { // verifica se o arquivo foi aberto corretamente, caso contrário, imprime a mensagem de erro.
        fprintf(stderr, "Erro ao abrir o arquivo.\n"); 
        return 1;
    }

    printf("leitor sem erros\n");

    if(modo == 'c')
    {
        Compactador compacta(arquivo_lido, arquivo_escrito);  
        // objeto destruido automaticamente ao final do escopo
    }
    else if (modo == 'd')
    {
        Descompactador descompacta(arquivo_lido, arquivo_escrito);
        // objeto destruido automaticamente ao final do escopo
    }
    else
    {
        printf("\n -=-=-=-=-=-=-=- nenhuma instrução passada -=-=-=-=-=-=--=\n \n -=-=-=-=-=-=--=-=-=-=- Programa finalizado -=-=-=-=-=-==-=\n");
    }
    
    fclose(arquivo_lido); //fecha o leitor
    fclose(arquivo_escrito);// fecha o escritor

    return 0;
}

// DECLARACOES DE METODOS E FUNCOES

// Metodos da classe No:

uint8_t No::getByte_letra()
{
    return this->byte_letra;
}

void No::setByte_letra(uint8_t x)
{
    this->byte_letra = x;
}

// Métodos para obter e definir a frequência
int  No::getFrequencia() { return this->frequencia;}
void No::setFrequencia(int freq) {this->frequencia += freq;}

// Métodos para obter e definir o filho esquerdo
No * No::getFilho_esquerdo() {return this->esq;}
void No::setFilho_esquerdo(No * filho) { this->esq = filho;}

// Métodos para obter e definir o filho direito
No * No::getFilho_direito() {return this->dir;}
void No::setFilho_direito(No * filho) {this->dir = filho;}

// Métodos para obter e definir o valor compactado
vector<uint8_t> No::getCodigo() {return this->codigo;}
void No::setCodigo(vector<uint8_t> &bin) {this->codigo = bin;}

// Método para verificar se é folha
bool No::getFolha() {return this->folha;}
void No::setFolha() {this->folha = true;}

void No::imprime_No(int byte){
    printf("Byte: %d\n", byte);
    printf("Frequencia: %d\n", this->frequencia);
    printf("É folha : %s\n", this->folha ? "Sim" : "Nao");
}

void No::Destroi_Arvore_Raiz(No*raiz)
{
    if(raiz->esq != nullptr)
    {
        Destroi_arvore(raiz->esq);
    }

    if (raiz->dir != nullptr)
    {
        Destroi_arvore(raiz->dir);
    }
    
    delete raiz;
}

void No::Destroi_arvore(No*raiz)
{
    if (raiz->folha)
    {
        return;
    }

    if(raiz->esq != nullptr)
    {
        Destroi_arvore(raiz->esq);
    }

    if (raiz->dir != nullptr)
    {
        Destroi_arvore(raiz->dir);
    }
    
    delete raiz;
}

void No::percorre_Arvore(No * raiz, Compactador & compact)
{
  if(raiz->folha)
  {
    compact.adiciona_codigo_Arvore(1);

    raiz->codigo = compact.devolve_Codigo_No();
    
    compact.escreve_Byte(raiz->byte_letra);
    
    return;
  }
  else
  {
    compact.adiciona_codigo_Arvore(0);
  }

  if(raiz->esq != nullptr)
  {
    compact.adiciona_codigo_No(0);
    percorre_Arvore(raiz->esq, compact);
    compact.remove_codigo_No();
  }

  if(raiz->dir != nullptr)
  {
    compact.adiciona_codigo_No(1);
    percorre_Arvore(raiz->dir, compact);
    compact.remove_codigo_No();
  }
  
  return;
}

No::~No()
{   
    this->esq = nullptr;
    this->dir = nullptr;
}

// metodos da classe descompactador

Descompactador::~Descompactador()
{
     if (leitor) {
            fclose(leitor);
            leitor = nullptr;
        }
        if (escritor) {
            fclose(escritor);
            escritor = nullptr;
        }

        // Deletar todos os ponteiros no vetor 'indice'
        for (No* no : indice) {
            delete no;
        }

        // Limpar o vetor 'indice' para evitar ponteiros pendentes
        indice.clear();
}

void Descompactador::Descompacta_bits()
{
    No * raiz = this->Cria_Arvore();

    for(uint32_t i = 0; i < this->letras_t; i++)
    {
        this->Percorre_Arvore(raiz);
    }

    printf("\n Arquivo descompactado com sucesso\n");

    raiz->Destroi_Arvore_Raiz(raiz);

    return;
}

void Descompactador::Percorre_Arvore(No * raiz)
{   
    if(raiz->getFolha())
    {   
        uint8_t byte = raiz->getByte_letra();
        fwrite(&byte,1,1,this->escritor);
        return;
    }
    else
    {
        uint8_t bit = this->Obtem_Bit();

        if(bit == 2){printf("\nacabaram os bits durante a leitura do byte codificado\n"); return;}

        if (bit == 0)
        {
            this->Percorre_Arvore(raiz->getFilho_esquerdo());
        }
        else if (bit == 1)
        {
            this->Percorre_Arvore(raiz->getFilho_direito());
        }
        else{printf("\n encontrado outro valor de bit \n");}
        
        return;
    }    
}

No * Descompactador::Cria_Arvore()
{   

    if(this->codigo_Arvore.size()<1){return nullptr;} // se não houver codigo de arvore, para a funcao

    No * novo;

    if(this->codigo_Arvore[0] == 0) // verifica se o Nó será galho ou folha
    {
        novo = new No;

        this->codigo_Arvore.erase(this->codigo_Arvore.begin()); // apaga o primeiro elemento codigo arvore
    }
    else
    {
        novo = this->indice[0]; 

        this->indice.erase(this->indice.begin()); // tira o ponteiro do primeiro elemento do indice
        this->codigo_Arvore.erase(this->codigo_Arvore.begin()); // apaga o primeiro elemento codigo arvore

        return novo;
    }
    
    // indo para a esquerda
    
    if(novo->getFilho_esquerdo() == nullptr)
    {
        novo->setFilho_esquerdo(this->Cria_Arvore());
    }
    
    // indo para a direita
    if(novo->getFilho_direito() == nullptr)
    {
        novo->setFilho_direito(this->Cria_Arvore());
    }
    
    return novo;
}

uint8_t Descompactador::Obtem_Bit()
{
    if(this->buffer_elementos == 0)
    {
        uint8_t byte;

        // Leia sobre a função fread

        if(fread(&byte,1,1,this->leitor) == 0){return 2;} // fread faz com que a variavel byte receba o byte do arquivo, o if compara para ver se o arquivo é valido, caso não for valido, retorna 2 (indicando erro na main)
        
        for(uint8_t i = 0; i < 8; i++)
        {
            this->buffer[(7-i)] = (byte % 2);

            byte = byte / 2;
        }

        this->buffer_elementos = 8;
    }

    uint8_t bit = this->buffer[8 - this->buffer_elementos];

    this->buffer_elementos--;

    return bit;
}

void Descompactador::Obtem_Codigo_Arvore()
{
    uint16_t contador = 0;

    uint8_t bit;

    if(this->codigo_Arvore.empty()){printf("\n\n O Código arvore está vazio antes de obter os códgios da arvore \n");}

    while (contador < this->alfabeto_k)
    {
        bit = this->Obtem_Bit();
        this->codigo_Arvore.push_back(bit);

        //printf("\nObteve o bit : %d e adicionou ao final da arvore\n", &bit);

        if(bit == 1)
        {
            contador++;
        }

        if(bit == 2){printf("erro ao capturar bits da arvore"); return;}
    }
    
}

void Descompactador::Obtem_Letras()
{
    uint8_t byte;

    for(uint16_t i = 0; i < this->alfabeto_k; i++)
    {
        fread(&byte,1,1,this->leitor);

        No * novo = new No();

        novo->setFolha();
        novo->setByte_letra(byte);

        //printf("\n\n Novo nó folha criado, recebendo o byte : %d\n", &byte);

        this->indice.push_back(novo);
    }
}

void Descompactador::Obtem_Cabecalho()
{
    fread(&this->alfabeto_k,2,1,this->leitor);

    //printf("\n Alfabeto K de %d letras obtido \n", &this->alfabeto_k);

    fread(&this->letras_t,4,1,this->leitor);

    //printf("\n Quantidade de T letras %d Obtido\n", &this->letras_t);
}

Descompactador::Descompactador(FILE * leitor, FILE * escritor): leitor(leitor), escritor(escritor), alfabeto_k(0), letras_t(0), buffer_elementos(0)
{
    for(uint8_t i = 0; i < 8; i++){this->buffer[i]= 0;} // inicializa o buffer com 0 em seus elementos
    this->Obtem_Cabecalho();
    this->Obtem_Letras();
    this->Obtem_Codigo_Arvore();
    this->Descompacta_bits();
}

// metodos da classe Compactador

void Compactador::escreve_Arquivo_Compactado()
{
    this->escreve_Vector(this->codigo_Arvore); // escreve o código da arvore

    int byte; // variável byte

    while (((byte = this->obtem_Byte()) >= 0) && (byte < 256))
    {
       this->escreve_Vector(this->vetor_frequencia[byte]->getCodigo()); 
    }

    this->descarrega(); // ultima escrita no arquivo, caso ainda haja algum bit remanescente no buffer
}

void Compactador::escreve_Vector(vector<uint8_t> vector)
{

    for(uint8_t i = 0 ; i < vector.size(); i++)
    {
        this->buffer[this->buffer_elementos++] = vector[i];

        if(this->buffer_elementos == 8)
        {
            this->descarrega();
        }

    }
}

void Compactador::descarrega()
{
    if(this->buffer_elementos == 0){return;} // se não houver elementos no buffer, retorna sem escrever

    while(this->buffer_elementos < 8) // completa o buffer com 0 para escrever 
    {
        this->buffer[this->buffer_elementos++]= 0;
    }

    uint8_t aux = 0;

    for(uint8_t i = 0; i < 8; i++) //transforma os elementos do vetor em um numero inteiro para ser escrito como byte
    {
        if(this->buffer[i]!= 0)
        {
            aux += pow(2,(7-i));
        }
    } 

    fwrite(&aux,1,1, this->escritor); // escreve o numero inteiro como byte

    this->buffer_elementos = 0;
}

void Compactador::escreve_Byte(uint8_t byte)
{
    fwrite(&byte,1,1, this->escritor);
}

vector<uint8_t> Compactador::devolve_Codigo_No()
{
    return this->codigo_No;
}

vector<uint8_t> Compactador::devolve_Codigo_Arvore()
{
    return this->codigo_Arvore;
}

void Compactador::adiciona_codigo_Arvore(uint8_t x)
{
    this->codigo_Arvore.push_back(x);
}

void Compactador::adiciona_codigo_No(uint8_t x)
{
    this->codigo_No.push_back(x);
}

void Compactador::remove_codigo_No()
{
    this->codigo_No.pop_back();
}

Compactador::Compactador(FILE * leitor, FILE * escritor): leitor(leitor), escritor(escritor), vetor_frequencia(256,nullptr),indice(0,nullptr), alfabeto_k(0), letras_t(0), codigo_Arvore(0,0), codigo_No(0,0), buffer_elementos(0)
{
    for(uint8_t i = 0; i < 8; i++){this->buffer[i]= 0;} // inicializa o buffer com 0 em seus elementos

    this->distribui_Byte();
    this->cria_Arvore();
    this->indice[0]->percorre_Arvore(this->indice[0], *this);
    //imprime_arvore(this->indice[0]);
    this->escreve_Arquivo_Compactado();
    this->indice[0]->Destroi_arvore(this->indice[0]);
    printf("\n Arquivo Compactado com sucesso\n");
}

Compactador::~Compactador()
{
    for (unsigned int i = 0; i < 256; i++)
    {
        if(vetor_frequencia[i] != nullptr){delete vetor_frequencia[i];}
    }

    vetor_frequencia.clear();

     // Deletar todos os ponteiros no vetor 'indice'
    for (unsigned int i = 0; i < indice.size(); i++) 
    {
        if(indice[i] != nullptr){delete indice[i];}
    }

    // Limpar o vetor 'indice' para evitar ponteiros pendentes
    indice.clear();
}

int Compactador::obtem_Byte()
{
    uint8_t byte;

    if(fread(&byte,1,1,this->leitor) == 0){return -1;} // caso não houver mais bytes, retorna "-1", quebrando o "while" do distribuidor.
    return byte;
}

void Compactador::distribui_Byte()
{
    int byte; // variável byte

    while (((byte = this->obtem_Byte()) >= 0) && (byte < 256))
        { //percorrendo os bytes do arquivo

        if(this->vetor_frequencia[byte] == nullptr)
            { //caso não houver um objeto da classe "No" com determinado byte, criar.
                this->vetor_frequencia[byte] = new No(); // criando objeto "No" em novo byte
                this->vetor_frequencia[byte]->setFolha(); // ele será uma folha (usado mais para frente)
                this->vetor_frequencia[byte]->setFrequencia(1); // sua frequencia passa a ser "1" (0+1)
                this->vetor_frequencia[byte]->setByte_letra(byte);// atribui ao nó qual byte ele representa
                this->indice.push_back(this->vetor_frequencia[byte]);

                printf("Nó %d incluso com sucesso\n", byte);
            }
        else
            {
                this->vetor_frequencia[byte]->setFrequencia(1); // Caso contrário, apenas aumenta em 1 sua frequencia

                printf("Nó %d incrementado com sucesso\n", byte);
            }

            this->letras_t++; // adiciona 1 letra ao contador de letras T que será adicionada ao cabeçalho
        }

    this->alfabeto_k = this->indice.size(); // recebe o número de letras do qual o alfabeto é munido, será adicionado ao cabeçalho

    fwrite(&this->alfabeto_k,2,1, this->escritor); //escreve a quantidade K no cabeçalho do arquivo.

    fwrite(&this->letras_t,4,1,this->escritor); // escreve a quantidade de T letras (bytes) do arquivo

    printf("\n -=-=-=-=-=-=-=- Bytes distribuídos com sucesso -=-=-=-=--=--==-=-\n\n");   

    fseek(this->leitor,0,SEEK_SET); // faz com que o ponteiro leitor volte a apontar para o início
}

void Compactador::cria_Arvore()
{
    min_heap(this->indice);

    while (this->indice.size()>1)
    {
        No * novo = new No();

        novo->setFilho_esquerdo(this->indice[0]);
        retira_Minimo(this->indice);

        novo->setFilho_direito(this->indice[0]);
        retira_Minimo(this->indice);

        novo->setFrequencia((novo->getFilho_direito()->getFrequencia()) + (novo->getFilho_esquerdo()->getFrequencia()));
        
        this->indice.push_back(novo);

        sobe(this->indice,this->indice.size() -1);
    }
    
}


// FUNCOES/ METODOS GERAIS



int pai_Heap(int i)
{
return (i - 1) / 2;
}

int direito(int i)
{
return 2 * (i + 1);
}

int esquerdo(int i)
{
return 2 * (i + 1) - 1;
}

void troca(No* & ponteiro_1,No* & ponteiro_2){
    No* aux;
    aux = ponteiro_1;
    ponteiro_1 = ponteiro_2;
    ponteiro_2 = aux;
}

void imprime_arvore(No * raiz){
    if(raiz == nullptr){return;}

    raiz->imprime_No(0);

    printf("\n\nFilho Esquerdo:\n\n");
    imprime_arvore(raiz->getFilho_esquerdo());

    printf("\n\nFilho Direito:\n\n");
    imprime_arvore(raiz->getFilho_direito());

}

void desce(vector<No*>& vetor, int pai, int tamanho) {
    int filho_esq = esquerdo(pai);
    int filho_dir = direito(pai);
    int menor = pai;

    if (filho_esq < tamanho && vetor[filho_esq]->getFrequencia() < vetor[pai]->getFrequencia()) {
        menor = filho_esq;
    }

    if (filho_dir < tamanho && vetor[filho_dir]->getFrequencia() < vetor[menor]->getFrequencia()) {
        menor = filho_dir;
    }

    if (menor != pai) {
        troca(vetor[pai], vetor[menor]);
        desce(vetor, menor, tamanho);
    }
}

void min_heap(vector<No*> & vetor){

    int n = vetor.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        desce(vetor, i, n);
    }

}

void sobe(vector<No*>& vetor, int i)
{
while (vetor[pai_Heap(i)]->getFrequencia() > vetor[i]->getFrequencia()) {
troca(vetor[i], vetor[pai_Heap(i)]);
i = pai_Heap(i);
}
}

void retira_Minimo(vector<No*> &vetor)
{
    if(vetor.size() > 0)
    {
        vetor[0] = vetor[vetor.size() - 1];
        vetor.pop_back();
        desce(vetor, 0, vetor.size());

        return;
    }
    else
    {
        printf("Heap vazia");
        return;
    }
}

/*void imprime_heap(vector<No * > &vetor)
{
    for(uint8_t i = 0; i < vetor.size(); i++)
    {
        printf("\nposicao: %d \nfrequencia: %d",i, vetor[i]->getFrequencia());
    }
}*/