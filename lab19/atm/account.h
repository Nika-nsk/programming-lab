#pragma once
#include "transaction.h"
#include <vector>

class Account {
private:
    double balance;                     // Текущий баланс
    std::vector<Transaction> history;   // Динамический массив (вектор) транзакций в ОЗУ

public:
    Account(double initialBalance = 0.0);
    
    double getBalance() const;            // Возвращает баланс
    void setBalance(double amount);       // Принудительно устанавливает баланс (для восстановления)
    void deposit(double amount);          // Пополнение баланса
    bool withdraw(double amount);         // Списание баланса
    void addTransaction(const Transaction& tx); // Добавление структуры в вектор истории
    std::vector<Transaction> getLastTransactions(size_t count) const; // Срез последних N транзакций
};
