#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using uchar = unsigned char;

inline uchar vigenere_encrypt_byte(uchar m, uchar k) {
    return static_cast<uchar>((m + k) & 0xFF);
}

inline uchar vigenere_decrypt_byte(uchar c, uchar k) {
    return static_cast<uchar>((c - k) & 0xFF);
}

bool processFile(const std::string& inFile, const std::string& outFile,
    const std::string& keyStr, bool encrypt) {
    std::ifstream fin(inFile, std::ios::binary);
    if (!fin) {
        std::cerr << "Ошибка: не удалось открыть входной файл: " << inFile << "\n";
        return false;
    }

    std::ofstream fout(outFile, std::ios::binary);
    if (!fout) {
        std::cerr << "Ошибка: не удалось создать выходной файл: " << outFile << "\n";
        return false;
    }

    std::vector<uchar> key;
    for (unsigned char c : keyStr) key.push_back(static_cast<uchar>(c));
    size_t keyLen = key.size();
    size_t keyPos = 0;

    const size_t BUF_SIZE = 1 << 16;
    std::vector<char> buffer(BUF_SIZE);

    while (fin) {
        fin.read(buffer.data(), static_cast<std::streamsize>(BUF_SIZE));
        std::streamsize readBytes = fin.gcount();
        if (readBytes <= 0) break;

        for (std::streamsize i = 0; i < readBytes; ++i) {
            uchar byte = static_cast<uchar>(buffer[i]);
            uchar k = key[keyPos % keyLen];
            uchar outByte = encrypt ? vigenere_encrypt_byte(byte, k)
                : vigenere_decrypt_byte(byte, k);
            buffer[i] = static_cast<char>(outByte);
            ++keyPos;
        }

        fout.write(buffer.data(), readBytes);
    }

    fin.close();
    fout.close();

    std::cout << (encrypt ? "Шифрование" : "Дешифровка") << " завершено успешно!\n";
    return true;
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::cout << "=========================================\n";
    std::cout << "      Шифр Виженера (по файлам)\n";
    std::cout << "=========================================\n";
    std::cout << "1 — Шифрование\n";
    std::cout << "2 — Дешифрование\n";
    std::cout << "Выберите режим (1/2): ";

    int mode;
    std::cin >> mode;
    std::cin.ignore(); 

    std::string inFile, outFile, key;
    std::cout << "Введите имя входного файла: ";
    std::getline(std::cin, inFile);

    std::cout << "Введите имя выходного файла: ";
    std::getline(std::cin, outFile);

    std::cout << "Введите ключ: ";
    std::getline(std::cin, key);

    if (key.empty()) {
        std::cerr << "Ошибка: ключ не может быть пустым!\n";
        return 1;
    }

    bool encrypt = (mode == 1);
    if (!processFile(inFile, outFile, key, encrypt)) {
        std::cerr << "Процесс завершился с ошибкой.\n";
        return 2;
    }

    std::cout << "Файл сохранён: " << outFile << "\n";
    std::cout << "=========================================\n";
    std::cout << "Нажмите любую клавишу для выхода...";
    std::cin.get();
    return 0;
}
