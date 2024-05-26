#include <iostream>
#include <fstream>
#include <bitset>

void printFileBits(const std::string& fileName) {
    // Abrir o arquivo em modo binário
    std::ifstream file(fileName, std::ios::binary);
    
    if (!file) {
        std::cerr << "Erro ao abrir o arquivo!" << std::endl;
        return;
    }

    char byte;
    // Ler o arquivo byte a byte
    while (file.read(&byte, 1)) {
        // Converter o byte em bits e imprimir
        std::bitset<8> bits(byte);
        std::cout << bits << ' ';
    }

    file.close();
}

int main() {
    std::string fileName = "compactado_original.txt";
    printFileBits(fileName);

    return 0;
}
