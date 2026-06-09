#include "journal.h"
#include <fstream> // Работа с файловыми потоками

Journal::Journal(std::string file) : filename(file) {}

void Journal::appendLog(TransactionType type, double amount) {
    
    std::ofstream out(filename, std::ios::binary | std::ios::app);
    if (out.is_open()) {
        int tType = static_cast<int>(type);
        std::time_t now = std::time(nullptr);
        
        
        out.write(reinterpret_cast<const char*>(&tType), sizeof(tType));
        out.write(reinterpret_cast<const char*>(&amount), sizeof(amount));
        out.write(reinterpret_cast<const char*>(&now), sizeof(now));
    }
}

std::string Journal::getFilename() const { return filename; }
