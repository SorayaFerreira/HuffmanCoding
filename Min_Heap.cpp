#include <cstdio>
#include <string>
#include <cstdint>
#include "HuffmanTree.cpp"

/*Aqui tem a função que calcula a frequência dos caracteres.
coloca num vetor e depois chama as funções de Node e MinHeap 
para criar a min-heap com os valores do vetor.*/


using std::string;


void CalcFreq(FILE *arquivo1);

/*int main() {

	FILE *arquivo1;
	arquivo1 = fopen(original.txt, "rb");
	
	if(!arquivo1) {
		printf("Error reading from file :(\n");
		return;
	}

	min_HeapComFreq();

	fclose(arquivo1);
	return 0;
}*/



void CalcFreq(FILE *arquivo1) {
//Função que calcula a frequência dos caracteres
//dentro do arquivo a ser lido e os insere na MinHeap.


	int *simbols; simbols = new int[256];
	// o new inicializa todo mundo com 0.

  	while(!feof(arquivo1)) { //retorna 0 se não estiver no fim
  		uint8_t temp;
  		fread(&temp, sizeof(uint8_t), 1, arquivo1);

  		simbols[(int)temp]++;
  		//printf("%d, %c\n", simbols[(int)temp], temp);
  	}

	MinHeap heap;
	for(int i = 0; i < 256; ++i) {

		if(simbols[i] > 0) {
			uint8_t code = i;
			Node novo(simbols[i], code);
			heap.insert(&novo);
		}
	}

}
