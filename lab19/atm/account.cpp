#include "account.h"

Account::Account(double initialBalance) : balance(initialBalance) {}

double Account::getBalance() const { return balance; }

void Account::setBalance(double amount) { balance = amount; }

void Account::deposit(double amount) {
    balance += amount;
   
    history.push_back({TransactionType::DEPOSIT, amount, std::time(nullptr)});
}

bool Account::withdraw(double amount) {
    if (balance >= amount) {
        balance -= amount;
        history.push_back({TransactionType::WITHDRAWAL, amount, std::time(nullptr)});
        return true;
    }
    return false;
}

void Account::addTransaction(const Transaction& tx) {
    history.push_back(tx);
}

std::vector<Transaction> Account::getLastTransactions(size_t count) const {
    std::vector<Transaction> lastTx;
   
    size_t start = (history.size() > count) ? history.size() - count : 0;
    for (size_t i = start; i < history.size(); ++i) {
        lastTx.push_back(history[i]);
    }
    return lastTx;
}
