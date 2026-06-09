#include "atm_engine.h"
#include "file_storage.h"
#include "recovery.h"
#include <iostream>

ATMEngine::ATMEngine() : sessionActive(false), dailyWithdrawn(0.0) {}

bool ATMEngine::startSession(const std::string& cardNum, const std::string& pin) {
    if (!FileStorage::loadCardData(cardNum, currentCard)) {
        currentCard = Card(cardNum, "1234", false);
        FileStorage::saveCardData(currentCard);
    }

    if (currentCard.isBlocked()) {
        std::cout << "Карта заблокирована!\n";
        return false;
    }

    if (FileStorage::loadAccountData(cardNum, currentAccount)) {
        Recovery rec;
        rec.restoreState(currentAccount, journal);
    } else {
        currentAccount = Account(5000.0);
        FileStorage::saveAccountData(cardNum, currentAccount);
    }

    if (currentCard.checkPin(pin)) {
        sessionActive = true;
        std::cout << "Успешный вход!\n";
        return true;
    } else {
        FileStorage::saveCardData(currentCard); 
        return false;
    }
}

void ATMEngine::checkBalance() {
    std::cout << "Текущий баланс: " << currentAccount.getBalance() << " руб.\n";
}

void ATMEngine::depositCash() {
    double amount;
    std::cout << "Введите сумму пополнения: ";
    std::cin >> amount;
    if (amount <= 0) return;

    currentAccount.deposit(amount);
    journal.appendLog(TransactionType::DEPOSIT, amount);
    FileStorage::saveAccountData(currentCard.getNumber(), currentAccount);
    std::cout << "Успешно пополнено.\n";
}

void ATMEngine::withdrawCash() {
    double amount;
    std::cout << "Введите сумму для снятия: ";
    std::cin >> amount;

    if (amount <= 0) return;
    if (amount > 10000) {
        std::cout << "Превышен лимит на разовое снятие (макс. 10000 руб).\n";
        return;
    }
    if (dailyWithdrawn + amount > 50000) {
        std::cout << "Превышен суточный лимит снятия (макс. 50000 руб).\n";
        return;
    }

    double fee = amount * 0.01;
    if (fee < 30.0) fee = 30.0;
    double totalDeduction = amount + fee;

    if (currentAccount.getBalance() < totalDeduction) {
        std::cout << "Недостаточно средств. Требуется списать (с комиссией): " << totalDeduction << " руб.\n";
        return;
    }

    currentAccount.withdraw(totalDeduction);
    dailyWithdrawn += amount;
    journal.appendLog(TransactionType::WITHDRAWAL, totalDeduction);
    FileStorage::saveAccountData(currentCard.getNumber(), currentAccount);
    std::cout << "Успешно снято. Комиссия: " << fee << " руб.\n";
}

void ATMEngine::printHistory() {
    auto txs = currentAccount.getLastTransactions(5);
    std::cout << "--- Последние 5 операций ---\n";
    for (const auto& tx : txs) {
        std::string typeStr = (tx.type == TransactionType::DEPOSIT) ? "Пополнение" : "Снятие";
        std::cout << typeStr << ": " << tx.amount << " руб.\n";
    }
}

void ATMEngine::run() {
    std::string cardNum, pin;
    std::cout << "Введите номер карты: ";
    std::cin >> cardNum;
    
    int attemptsLeft = 3;
    
    while (!sessionActive && attemptsLeft > 0) {
        std::cout << "Введите PIN: ";
        std::cin >> pin;
        
        if (startSession(cardNum, pin)) {
            break;
        }
        
        attemptsLeft--;
        
        if (attemptsLeft > 0) {
            std::cout << "Неверный PIN. Осталось попыток: " << attemptsLeft << "\n";
        } else {
            std::cout << "Карта заблокирована из-за превышения попыток ввода PIN!\n";
            currentCard.block();
            FileStorage::saveCardData(currentCard);
        }
    }

    if (!sessionActive) return;

    int choice;
    do {
        std::cout << "\n1.Баланс 2.Пополнить 3.Снять 4.История 5.Выход\nВыбор: ";
        std::cin >> choice;
        if (choice == 1) checkBalance();
        else if (choice == 2) depositCash();
        else if (choice == 3) withdrawCash();
        else if (choice == 4) printHistory();
    } while (choice != 5);
}
