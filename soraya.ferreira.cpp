
/**************************************************
* Estudantes: 
*	Soraya Dias Ferreira
*	Christiyan ...
*
* Trabalho de Estruturas de Dados
* Professor(a): Diego Padilha Rubert 
*/


/*
Arquivo com início da função main geral.
*/


#include <stdio.h>

//   ./programa c texto.txt texto.huff
//   ./programa d texto.huff texto-descompactado.txt

int main(int argc, char *argv[]) {
	// argv[0] = nomeDoPrograma
	// argv[argc] = NULL;

	FILE *arquivo1, *arquivo2;
	arquivo1 = fopen(argv[2], "rb");
	arquivo2 = fopen(argv[3], "wb");

	if(!arquivo1) {
		printf("\tError reading from file :(\n");
		return;
	}

	if(*argv[1] == 'c') {
		//compactador(arquivo1, arquivo2);
		
	} else if(*argv[1] == 'd') {
		//descompactador(arquivo1, arquivo2);
	}
	return 0;

	fclose(arquivo1);
	fclose(arquivo2);
}

