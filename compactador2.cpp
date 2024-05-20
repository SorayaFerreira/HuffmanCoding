#include "biblioteca.h"

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


    Bytes entrada(leitor); // Objeto Bytes chamado "entrada" inicializando com o ponteiro "leitor"

    distribui_Byte(vetor_frequencia, indice, entrada);

    printf("\n -=-=-=-=-=-=-=- Bytes distribuídos com sucesso -=-=-=-=--=--==-=-\n\n");

    cria_arvore(indice);

    imprime_arvore(indice[0]);


    /*for (int i = 0; i < 256; i++) {
        if(vetor_frequencia[i]!=nullptr) // caso o objeto No exista, ele é impresso
        {
            vetor_frequencia[i]->imprime_No(i); 
        }

        delete vetor_frequencia[i]; // libera espaço da memória
    }*/

    fclose(leitor); //fecha o leitor

    return 0;
}

