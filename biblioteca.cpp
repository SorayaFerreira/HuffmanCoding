#include "biblioteca.h"

// Métodos para obter e definir a frequência
int  No::getFrequencia() { return this->frequencia;}
void No::setFrequencia(int freq) {this->frequencia += freq;}

// Métodos para obter e definir o filho esquerdo
No * No::getFilho_esquerdo() {return this->esq;}
void No::setFilho_esquerdo(No * &filho, No * &novo_pai) { this->esq = filho; esq->pai = novo_pai;}

// Métodos para obter e definir o filho direito
No * No::getFilho_direito() {return this->dir;}
void No::setFilho_direito(No * &filho, No * &novo_pai) {this->dir = filho; dir->pai = novo_pai;}

// Métodos para obter e definir o valor compactado
uint8_t No::getCompacto() {return this->compacto;}
void No::setCompacto(uint8_t bin) {this->compacto = bin;}

// Método para verificar se é folha
bool No::getFolha() {return this->folha;}
void No::setFolha() {this->folha = true;}

void No::imprime_No(int byte){
    printf("Byte: %d\n", byte);
    printf("Frequencia: %d\n", this->frequencia);
    printf("É folha : %s\n", this->folha ? "Sim" : "Nao");
}

Compactador::Compactador(FILE * file): leitor(file), vetor_frequencia(256,nullptr)
{
    this->distribui_Byte();
    this->cria_Arvore();
    imprime_arvore(this->indice[0]);

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

    while (((byte = this->obtem_Byte()) >= 0) && (byte < 256)){ //percorrendo os bytes do arquivo

            if(this->vetor_frequencia[byte] == nullptr){ //caso não houver um objeto da classe "No" com determinado byte, criar.
                this->vetor_frequencia[byte] = new No(); // criando objeto "No" em novo byte
                this->vetor_frequencia[byte]->setFolha(); // ele será uma folha (usado mais para frente)
                this->vetor_frequencia[byte]->setFrequencia(1); // sua frequencia passa a ser "1" (0+1)

                this->indice.push_back(this->vetor_frequencia[byte]);

                printf("Nó %d incluso com sucesso\n", byte);
            }
            else
            {
                this->vetor_frequencia[byte]->setFrequencia(1); // Caso contrário, apenas aumenta em 1 sua frequencia

                printf("Nó %d incrementado com sucesso\n", byte);
            }
        }

    printf("\n -=-=-=-=-=-=-=- Bytes distribuídos com sucesso -=-=-=-=--=--==-=-\n\n");   
}

void Compactador::cria_Arvore()
{
    min_heap(this->indice);

    while (this->indice.size()>1)
    {
        No * novo = new No();

        novo->setFilho_esquerdo(this->indice[0],novo);
        retira_Minimo(this->indice);

        novo->setFilho_direito(this->indice[0],novo);
        retira_Minimo(this->indice);

        novo->setFrequencia((novo->getFilho_direito()->getFrequencia()) + (novo->getFilho_esquerdo()->getFrequencia()));
        
        this->indice.push_back(novo);

        sobe(this->indice,this->indice.size() -1);
    }
    
}

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