#pragma once
#include "card.h"
#include "account.h"

class FileStorage {
public:
    // Все методы статические (static), их можно вызывать без создания экземпляра класса FileStorage
    static bool loadCardData(const std::string& cardNum, Card& card);
    static bool saveCardData(const Card& card);
    static bool loadAccountData(const std::string& cardNum, Account& account);
    static bool saveAccountData(const std::string& cardNum, const Account& account);
};
