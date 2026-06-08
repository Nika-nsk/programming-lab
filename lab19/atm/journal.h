#pragma once
#include "transaction.h"
#include <string>

class Journal {
private:
    std::string filename; // Путь к файлу логирования

public:
    Journal(std::string file = "journal.bin");
    void appendLog(TransactionType type, double amount); // Запись операции в конец файла
    std::string getFilename() const;                    // Возвращает имя файла
};
