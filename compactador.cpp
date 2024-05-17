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
    int getFrequencia() { return this->frequencia;}
    void setFrequencia(int freq) {this->frequencia += freq;}

    // Métodos para obter e definir o filho esquerdo
    No * getFilho_esquerdo() {return this->esq;}
    void setFilho_esquerdo(No * &filho, No * &novo_pai) { this->esq = filho; esq->pai = novo_pai;}

    // Métodos para obter e definir o filho direito
    No * getFilho_direito() {return this->dir;}
    void setFilho_direito(No * &filho, No * &novo_pai) {this->dir = filho; dir->pai = novo_pai;}

    // Métodos para obter e definir o valor compactado
    uint8_t getCompacto() {return this->compacto;}
    void setCompacto(uint8_t bin) {this->compacto = bin;}

     // Método para verificar se é folha
    bool getFolha() {return this->folha;}
    void setFolha() {this->folha = true;}

    void imprime_No(int byte);
};

int main(){

    vector<No*> vetor_frequencia(256, nullptr); // vetor de ponteiros para No (objetos)
    vector<No*> indice;

    printf("vetor inicializado\n");

    // ponteiro de arquivo 
    FILE * leitor;
    leitor = fopen("original.txt", "rb"); //ponteiro de arquivo apontando para arquivo desejado

    printf("Leitor inicializado\n");

    if (!leitor) { // verifica se o arquivo foi aberto corretamente, caso contrário, imprime a mensagem de erro.
        fprintf(stderr, "Erro ao abrir o arquivo.\n"); 
        return 1;
    }

    printf("leitor sem erros\n");

    int byte; // variável byte;

    Bytes entrada(leitor); // Objeto Bytes chamado "entrada" inicializando com o ponteiro "leitor"

    while (((byte = entrada.obtem_byte()) >= 0) && (byte < 256)){ //percorrendo os bytes do arquivo

        if(vetor_frequencia[byte] == nullptr){ //caso não houver um objeto da classe "No" com determinado byte, criar.
            vetor_frequencia[byte] = new No(); // criando objeto "No" em novo byte
            vetor_frequencia[byte]->setFolha(); // ele será uma folha (usado mais para frente)
            vetor_frequencia[byte]->setFrequencia(1); // sua frequencia passa a ser "1" (0+1)

            indice.push_back(vetor_frequencia[byte]);

            printf("Nó %d incluso com sucesso\n", byte);
        }
        else
        {
            vetor_frequencia[byte]->setFrequencia(1); // Caso contrário, apenas aumenta em 1 sua frequencia

            printf("Nó %d incrementado com sucesso\n", byte);
        }
    }



    printf("\n -=-=-=-=-=-=-=- Bytes distribuídos com sucesso -=-=-=-=--=--==-=-\n\n");

    for (int i = 0; i < 256; i++) {
        if(vetor_frequencia[i]!=nullptr) // caso o objeto No exista, ele é impresso
        {
            vetor_frequencia[i]->imprime_No(i); 
        }

        delete vetor_frequencia[i]; // libera espaço da memória
    }



    fclose(leitor); //fecha o leitor

    return 0;
}

int Bytes::obtem_byte(){ // Método usado para extrair os Bytes do arquivo

    uint8_t byte;

    if(fread(&byte,1,1,file) == 0){return -1;} // caso não houver mais bytes, retorna "-1", quebrando o "while" da main.
    return byte;
}

void No::imprime_No(int byte){
    printf("Byte: %d\n", byte);
    printf("Frequencia: %d\n", this->frequencia);
    printf("É folha : %s\n\n", this->folha ? "Sim" : "Não");

}

void min_heap(vector<No*> &vetor){

    int n = vetor.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        desce(vetor, i, n);
    }

    for (int i = n - 1; i > 0; i--) {
        troca(vetor[0], vetor[i]);
        desce(vetor, 0, i);
    }

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

void sobe(vector<No*>& vetor, int i)
{
while (vetor[pai_Heap(i)]->getFrequencia() > vetor[i]->getFrequencia()) {
troca(vetor[i], vetor[pai_Heap(i)]);
i = pai_Heap(i);
}
}


void troca(No* & ponteiro_1,No* & ponteiro_2){
    No* aux;
    aux = ponteiro_1;
    ponteiro_1 = ponteiro_2;
    ponteiro_2 = aux;
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
