#include <cstdio>
//#include <climits>
#include <string>
#include <algorithm>

using std::string;

class Processo
{
    friend class Heap;

public:
    Processo();
    Processo(int p, int id);
    void escreve(const char *sep = "");

private:
    int p;
    int id;
};

class Heap
{
public:
    Heap();
    Heap(const int n, const Processo dados[]);
    Heap(const Heap &outro);
    ~Heap();
    Heap &operator=(const Heap &outro);
    void escreve_niveis();
    void escreve(const string &prefixo = "", int i = 0);
    void insere(int p, int id);
    Processo consulta_maxima();
    Processo extrai_maxima();
    void altera_prioridade(int i, int p);

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

int main(void)
{
    Heap h; // construtor Heap()

    for (int i = 1; i <= 10; i++)
        h.insere(i, (7 * i * i) % 17);
    printf("h:\n");
    h.escreve();

    h.extrai_maxima();
    h.altera_prioridade(0, -3);
    printf("h:\n");
    h.escreve();

    Processo v[] = {Processo(1, 23),
                    Processo(2, 78),
                    Processo(3, 17),
                    Processo(4, 8),
                    Processo(5, 34)};

    Heap h2(5, v); // construtor Heap(int n, int dados[])
    h2.insere(15, 64);
    printf("h2:\n");
    h2.escreve();

    Heap h3(h2); // construtor de cópia padrão
    h2.insere(30, 12);
    printf("h3:\n");
    h3.escreve();

    Heap h4 = h2; // construtor de cópia padrão
    h2.insere(40, 53);
    printf("h4:\n");
    h4.escreve();

    h = h2; // operador de atribuição, sem constructor ou constructor de cópia
    h.insere(100, 85);
    printf("h2:\n");
    h2.escreve();
    printf("h:\n");
    h.escreve();

    h = Heap(5, v); // construtor Heap(int n, Processo dados[]), seguido de atribuição (de cópia, não transferência (move))
    printf("h:\n");
    h.escreve();

    h.extrai_maxima();
    h.altera_prioridade(0, -2);
    printf("h:\n");
    h.escreve();

    return 0;
}

Processo::Processo() {}

Processo::Processo(int p, int id) : p(p), id(id) {}

Heap::Heap() : n(0), capacidade(TAMANHO_INICIAL) {
    S = new Processo[TAMANHO_INICIAL];
}

Heap::~Heap() {
    delete[] S;
}

Heap::Heap(const int n, const Processo dados[]) : capacidade(n) {
    S = new Processo[n];
    for (int i = 0; i < n; ++i)
        S[i] = dados[i];
    this->n = capacidade; // esse é o 'n' da classe!

    int i;
    for (i = n / 2 - 1; i >= 0; i--)
        desce(i);
}

void Heap::escreve_niveis() {
    int escritos = 0, fim_nivel = 1;

    for (int i = 0; i < n; i++)
    {
        // Equivalente a for (unsigned i = 0; i < S.size(); i++) { printf("%d ",
        // S[i]);
        printf("%d ", S[i]);
        if (++escritos == fim_nivel)
        {
            putchar('\n');
            fim_nivel *= 2;
            escritos = 0;
        }
    }
    putchar('\n');
}

void Heap::escreve(const string &prefixo, int i) {
    if (i < n)
    {
        bool ehEsquerdo = i % 2 != 0;
        bool temIrmao = i < n - 1;

        printf("%s", prefixo.c_str());
        printf(ehEsquerdo and temIrmao ? "├──" : "└──");

        printf("%d\n", S[i]);

        escreve(prefixo + (ehEsquerdo ? "│   " : "    "), esquerdo(i));
        escreve(prefixo + (ehEsquerdo ? "│   " : "    "), direito(i));
    }
}

int Heap::pai(int i) { return (i - 1) / 2; }

int Heap::esquerdo(int i) { return 2 * (i + 1) - 1; }

int Heap::direito(int i) { return 2 * (i + 1); }

void Heap::troca(int i, int j) {
    Processo aux = S[i];
    S[i] = S[j];
    S[j] = aux;
}

void Heap::desce(int i)
{
    int e, d, maior;
    e = esquerdo(i);
    d = direito(i);
    if (e < n && S[e].p > S[i].p)
        maior = e;
    else
        maior = i;
    if (d < n && S[d].p > S[maior].p)
        maior = d;
    if (maior != i)
    {
        troca(i, maior);
        desce(maior);
    }
}

void Heap::sobe(int i)
{
    while (S[pai(i)].p < S[i].p)
    {
        troca(i, pai(i));
        i = pai(i);
    }
}
Processo Heap::consulta_maxima() { return S[0]; }

Processo Heap::extrai_maxima() {
    Processo maior;
    if (n > 0) {
        maior = S[0];
        S[0] = S[--n];
        desce(0);
        return maior;
    } else
        return;
}

void Heap::altera_prioridade(int i, int p) {

    Processo anterior = S[i];
    S[i].p = p;
    if (p < anterior.p) {
        desce(i);
    } else {
        sobe(i);
    }
}

Heap &Heap::operator=(const Heap &outro) {
    
    this->n = outro.n;
    this->capacidade = outro.n;
    delete[] S;

    S = new Processo[capacidade];
    for (int i = 0; i < n; i++) {
        this->S[i] = outro.S[i];
    }

    return *this;
}

void Heap::insere(int p, int id) {
    if (n == capacidade) { 
        capacidade = capacidade * 2;
        Processo *outroS = new Processo[capacidade];

        for (int i = 0; i < n; ++i)
        {
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

Heap::Heap(const Heap &outro) : Heap(outro.n, outro.S) {}