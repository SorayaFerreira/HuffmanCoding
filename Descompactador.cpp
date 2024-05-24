#include <stdio.h>
#include <cstdint>
#include <cmath>
//#include "compactador.cpp"

/*
Formato do cabeçalho:
    1º. 1 byte (K), numFolhas;
    2º. 4 bytes (T), nº total de caracteres no arquivo.
    3º. Código de cada caractere;
    4º. Bits percurso pré-ordem;
    5º. Sequência de bits do arquivo compactado.

ANOTAÇÕES:
    > Momento de reconstruir a árvore:
        - As letras que aparacer no 3º estão na ordem
        do percurso pré-ordem, então os 1s, são as mesmas letras
        na mesma ordem.
*/

class Descompactador {
    private:
        FILE *file;
        uint8_t T;
        uint8_t K;

    public:
        uint8_t getterT();
};

class Bits {
private:
    FILE *file;   // Ponteiro para o arquiv-o sendo lido/escrito
    uint8_t b[8]; // Buffer com bits de um byte
    uint8_t n;    // Quantidade de posições ocupadas no vetor acima

public:
    Bits(FILE *file);   // Construtor padrão com o arquivo que será lido ou escrito
    uint8_t ocupados(); // Devolve quantos bits estão ocupados no buffer do byte
    uint8_t livres();   // Devolve quantos bits ainda podem ser adicionados ao buffer do byte

    // Funções do modo escrita
    void adiciona_bit(uint8_t bit); // Adiciona um bit 0 ou 1 ao buffer (escreve byte no arquivo se encher)
    void descarrega();              // Força a escrita do byte no buffer (completa com 0s, caso necessário)

    // Funções do modo leitura
    uint8_t obtem_bit(); // Obtém o próximo bit do buffer (lê um byte do arquivo se estiver vazio)
};

Bits::Bits(FILE *file) : file(file), n(0) {}

void Bits::adiciona_bit(uint8_t bit) {
    b[n++] = bit != 0; // para usar 0/1 mesmo se bit > 1

    if (n >= 8)
        descarrega();
}

void Bits::descarrega()
{
    if (n == 0)
        return;
    // Deve completar o byte com 0s para preencher os 8 bits, caso n < 8

    while (n < 8) // preenche com 0 os valores restantes do vetor(byte) caso esteja incompleto, até encher o byte
    {
        b[n++] = 0;
    }

    uint8_t aux = 0; // variavel auxiliar

    for (uint8_t i = 0; i < 8; i++)
    { // transforma os numeros contidos no vetor b (byte em bytes), em um número de apenas uma variavel (byte unido)

        if (b[i] == 1)
        {
            aux += pow(2, (7 - i));
        }
    }

    // Leia sobre a função fwrite
    fwrite(&aux, 1, 1, file); // escreve o byte (aux) no arquivo

    // No final, não esqueça de fazer n receber 0

    n = 0;
}

uint8_t Bits::obtem_bit()
{
    // Implementar
    // Caso n == 0, deve ler 1 byte do arquivo e colocar no buffer, ou devolver 2 caso não haja mais bytes para serem lidos do arquivo
    if (n == 0)
    {
        uint8_t byte; // cria a variavel byte, onde havera um numero binário

        // Leia sobre a função fread

        if (fread(&byte, 1, 1, file) == 0) // fread faz com que a variavel byte receba o byte do arquivo, o if compara para ver se o arquivo é valido, caso não for valido, retorna 2 (indicando erro na main)
        {
            return 2;
        }

        // prencher o vetor

        for (uint8_t i = 0; i < 8; i++)
        {
            b[(7 - i)] = (byte % 2);

            byte = byte / 2;
        }

        n = 8;
    }

    // No final, devolver um bit (0 ou 1) e decrementar n

    uint8_t bit = b[livres()];

    n--;

    return bit;
}

uint8_t Bits::ocupados()
{
    return n;
}

uint8_t Bits::livres()
{
    return 8 - n;
}