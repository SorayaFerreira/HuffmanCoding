#include <cstdint>
#include <cstdio>
#include <vector>
#include <cmath>

using namespace std;

// CABEÇALHO

class Compactador;

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

    // Métodos para obter e definir a frequência
    int getFrequencia();
    void setFrequencia(int freq);

    // Métodos para obter e definir o filho esquerdo
    No * getFilho_esquerdo();
    void setFilho_esquerdo(No * &filho);

    // Métodos para obter e definir o filho direito
    No * getFilho_direito();
    void setFilho_direito(No * &filho);

    // Métodos para obter e definir o valor compactado
    vector<uint8_t> getCodigo();
    void setCodigo(vector<uint8_t> &bin);

     // Método para verificar se é folha
    bool getFolha();
    void setFolha();

    uint8_t getByte_letra();
    void setByte_letra(uint8_t x);

    void imprime_No(int byte);

    void percorre_Arvore(No * raiz, Compactador & compact);
};

class Compactador
{
   private:
   vector<No*> vetor_frequencia;//{256, nullptr}; // vetor de ponteiros para No (objetos)
   vector<No*> indice;

   FILE * leitor;
   FILE * escritor;

   uint8_t alfabeto_k; // número de letras no alfabeto
   int letras_t; // número de letras impressas 

   vector<uint8_t> codigo_Arvore;
   vector<uint8_t> codigo_No;

   uint8_t buffer[8];
   uint8_t buffer_elementos;

   public:
   Compactador(FILE * leitor, FILE * escritor);
   int obtem_Byte();
   void distribui_Byte();
   void cria_Arvore();

   void adiciona_codigo_Arvore(uint8_t x);
   void adiciona_codigo_No(uint8_t x);
   void remove_codigo_No();
   void escreve_letras_t();
   vector<uint8_t> devolve_Codigo_No();
   vector<uint8_t> devolve_Codigo_Arvore();
   void escreve_Byte(uint8_t byte);
   void descarrega();
   void escreve_Vector(vector<uint8_t> vector);
   void escreve_Arquivo_Compactado();
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

int main(){

    FILE * arquivo_original = fopen("original.txt", "rb"); //ponteiro de arquivo apontando para arquivo desejado
    FILE * arquivo_compactado = fopen("compactado.clink", "wb");


    printf("Leitor inicializado\n");

    if (!arquivo_original) { // verifica se o arquivo foi aberto corretamente, caso contrário, imprime a mensagem de erro.
        fprintf(stderr, "Erro ao abrir o arquivo.\n"); 
        return 1;
    }

    printf("leitor sem erros\n");

    Compactador clinkCompacta(arquivo_original, arquivo_compactado);

    fclose(arquivo_original); //fecha o leitor
    fclose(arquivo_compactado);

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
void No::setFilho_esquerdo(No * &filho) { this->esq = filho;}

// Métodos para obter e definir o filho direito
No * No::getFilho_direito() {return this->dir;}
void No::setFilho_direito(No * &filho) {this->dir = filho;}

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

// metodos da classe Compactador

void Compactador::escreve_Arquivo_Compactado()
{
    this->escreve_Vector(this->codigo_Arvore); // escreve o código da arvore

    int byte; // variável byte

    while (((byte = this->obtem_Byte()) >= 0) && (byte < 256))
    {
       this->escreve_Vector(this->vetor_frequencia[byte]->getCodigo()); 
    }

}

void Compactador::escreve_Vector(vector<uint8_t> vector)
{

    for(uint8_t i; i < vector.size(); i++)
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

Compactador::Compactador(FILE * leitor, FILE * escritor): leitor(leitor), escritor(escritor), vetor_frequencia(256,nullptr), alfabeto_k(0), letras_t(0), buffer_elementos(0)
{
    for(uint8_t i = 0; i < 8; i++){this->buffer[i]= 0;} // inicializa o buffer com 0 em seus elementos

    this->distribui_Byte();
    this->cria_Arvore();
    this->indice[0]->percorre_Arvore(this->indice[0], *this);
    //imprime_arvore(this->indice[0]);
    this->escreve_Arquivo_Compactado();
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

    fwrite(&this->alfabeto_k,1,1, this->escritor); //escreve a quantidade K no cabeçalho do arquivo.

    this->escreve_letras_t();// escreve a quantidade de T letras (bytes) do arquivo

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


void Compactador::escreve_letras_t()
{
    int quociente = this->letras_t;
    int resto;
    int casa = 0;

    bool vetor[32] = {0}; // numero total de letras que o arquivo pode ter (4bytes)

    while (quociente != 0) //transforma o numero inteiro de letras T em um numero binario no vetor
    {
        resto = quociente % 2;
        vetor[31-casa] = resto;
        quociente = quociente / 2;
        casa ++;
    }

    int byte = 0; 
    int potencia = 8;

    for(int i = 0 ; i < 32 ; i++) // percorre o vetor anterior, dividindo-o em 4, a cada 8 casas percorridas se transforma em byte e é escrito
    { 
        potencia --;

        if(vetor[i])
        {
            byte += pow(2,potencia);
        }

        if(potencia == 0)
        {
            fwrite(&byte,1,1,this->escritor);
            potencia = 8;
        }

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

void imprime_heap(vector<No * > &vetor)
{
    for(int i = 0; i < vetor.size(); i++)
    {
        printf("\nposicao: %d \nfrequencia: %d",i, vetor[i]->getFrequencia());
    }
}