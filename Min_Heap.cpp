#include <cstdio>
#include <cstdlib>
#include <string>

using std::string;

class Processo
{
	friend class Heap;

public:
	Processo();
	Processo(int p, char id);

private:
	int p;
	char id;
};

class Heap
{
public:
	Heap();
	Heap(const int n, const Processo dados[]);
	Heap(const Heap &outro);
	~Heap();
	Heap &operator=(const Heap &outro);
	void insere(int p, char id);

private:
	Processo *S;
	int n;
	int capacidade;
	static const int TAMANHO_INICIAL = 4;

	int pai(int i);
	int esquerdo(int i);
	int direito(int i);
	void troca(int i, int j);
	void desce(int i);
	void sobe(int i);
};

void min_HeapComFreq();

/*int main() {

	calculateFrequency();
	return 0;
}*/



void min_HeapComFreq() {
	//Função que calcula a frequência dos caracteres
	//dentro do arquivo a ser lido.

	//abrir arquivo para leitura
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
  		//printf("%d, %c\n", simbols[(int)temp], temp);
  	}
	fclose(arquivo); //fecahar arquivo
}

//Construtor de um nó com dados
Processo::Processo() {}

//Construtor de um nó com dados, com parâmetros
Processo::Processo(int p, char id) : p(p), id(id) {}

//Construtor da heap já com atributos definidos
Heap::Heap() : n(0), capacidade(TAMANHO_INICIAL) {
	S = new Processo[TAMANHO_INICIAL];
}

Heap::~Heap() {
	// Destrutor da Heap
	delete[] S;
}

Heap::Heap(const int n, const Processo dados[]) : capacidade(n) {
	// Construtor da min-heap com um vetor pronto de dados
	S = new Processo[n];
	for (int i = 0; i < n; ++i)
		S[i] = dados[i];
	this->n = capacidade; // esse é o 'n' da classe!

	int i;
	for (i = n / 2 - 1; i >= 0; i--)
		desce(i);
}


void Heap::desce(int i) {
	// Função que desce os elementos com prioridade maior
	// para garantir a propriedade da min-heap
	int e, d, menor;
	e = esquerdo(i);
	d = direito(i);
	if (e < n && S[e].p < S[i].p)
		menor = e;
	else
		menor = i;
	if (d < n && S[d].p < S[menor].p)
		menor = d;
	if (menor != i)
	{
		troca(i, menor);
		desce(menor);
	}
}


void Heap::sobe(int i) {
	// Função que sobe elementos com prioridade menor
	// para manter a propriedade da min-heap
	while (S[pai(i)].p > S[i].p) {
		troca(i, pai(i));
		i = pai(i);
	}
}
//Acessa o nó pai
int Heap::pai(int i) { return (i - 1) / 2; }

//Acessa o filho esquerdo.
int Heap::esquerdo(int i) { return 2 * (i + 1) - 1; }

//Acessa o filho direito
int Heap::direito(int i) { return 2 * (i + 1); }

//Função que realiza a troca entre elementos
void Heap::troca(int i, int j) {
	Processo aux = S[i];
	S[i] = S[j];
	S[j] = aux;
}

Heap &Heap::operator=(const Heap &outro) {
	//Realiza cópia de uma heap, deletando a anterior
	this->n = outro.n;
	this->capacidade = outro.n;
	delete[] S;

	S = new Processo[capacidade];
	for (int i = 0; i < n; i++)
	{
		this->S[i] = outro.S[i];
	}

	return *this;
}


void Heap::insere(int p, char id)
{ // Função de inserção de elementos na min-heap
	if (n == capacidade) {
		capacidade = capacidade * 2;
		Processo *outroS = new Processo[capacidade];

		for (int i = 0; i < n; ++i) {
			outroS[i] = S[i];
		}
		delete[] S;
		S = outroS;
	}
	S[n].p = p;
	S[n].id = id; // n é o último alocado, é uma posicao depois
	
	++n;
	sobe(n - 1);
}

//Função retorna uma cópia da min-heap
Heap::Heap(const Heap &outro) : Heap(outro.n, outro.S) {}