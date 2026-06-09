#pragma once
#include <ctime>


enum class TransactionType { DEPOSIT, WITHDRAWAL };

// Структура транзакции
struct Transaction {
    TransactionType type; // Тип операции
    double amount;        // Сумма
    std::time_t timestamp;// Метка времени (POSIX-время из ctime)
};
