#pragma once
#include "card.h"
#include "account.h"
#include "journal.h"

class ATMEngine {
private:
    Card currentCard;          // Активная сессионная карта
    Account currentAccount;    // Активный сессионный счет
    Journal journal;           // Объект журнала логов
    bool sessionActive;        // Флаг авторизации сессии
    double dailyWithdrawn;     // Счетчик суточного лимита снятия наличных

public:
    ATMEngine();
    bool startSession(const std::string& cardNum, const std::string& pin); // Авторизация
    void run();            // Запуск цикла меню
    void checkBalance();   // Консольный вывод баланса
    void depositCash();    // Консольный ввод и обработка пополнения
    void withdrawCash();   // Обработка снятия с учетом лимитов и комиссий
    void printHistory();   // Вывод 5 последних операций
};
