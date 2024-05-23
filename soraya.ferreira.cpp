/**************************************************
*
* Soraya Dias Ferreira
* Trabalho de Estruturas de Dados
* Professor(a): Diego Padilha Rubert 
*
*/
#include <stdio.h>
//#include "HuffmanTree.h"

//   ./programa c texto.txt texto.huff
//   ./programa d texto.huff texto-descompactado.txt

int main(int argc, char *argv[]) {
	// argv[0] = nomeDoPrograma
	// argv[argc] = NULL;

	FILE *arquivo1, *arquivo2;
	arquivo1 = fopen(argv[2], "rb");
	arquivo2 = fopen(argv[3], "wb");

	if(*argv[1] == 'c') {
		compacta(arquivo1, arquivo2);
		
	} else if(*argv[1] == 'd') {
		descompacta(arquivo1, arquivo2);
	}
	return 0;
}

