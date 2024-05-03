//

#include <cstdio>
#include <cstdlib>

void calculateFrequency();


int main() {

	calculateFrequency();
	return 0;
}


void calculateFrequency() {

	FILE *arquivo = fopen("original.txt", "r+");
	if(!arquivo) {
		printf("Error of reading the file :(\n");
		return;
	}

	int *simbols; simbols = new int[256];
	// o new inicializa todo mundo com 0.

  	while(!feof(arquivo)) { //retorna 0 se n estiver no fim
  		char temp;
  		fread(&temp, sizeof(char), 1, arquivo);

  		simbols[(int)temp]++;

  		printf("%d, %c\n", simbols[(int)temp], temp);

  	}
	

	fclose(arquivo);
}