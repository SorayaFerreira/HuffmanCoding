#include "biblioteca.h"

int main(){

    FILE * arquivo = fopen("original.txt", "rb"); //ponteiro de arquivo apontando para arquivo desejado

    printf("Leitor inicializado\n");

    if (!arquivo) { // verifica se o arquivo foi aberto corretamente, caso contrário, imprime a mensagem de erro.
        fprintf(stderr, "Erro ao abrir o arquivo.\n"); 
        return 1;
    }

    printf("leitor sem erros\n");

    Compactador clinkCompacta(arquivo);

    fclose(arquivo); //fecha o leitor

    return 0;
}
