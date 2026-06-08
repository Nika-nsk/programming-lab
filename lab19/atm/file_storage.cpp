#include "file_storage.h"
#include <fstream>

bool FileStorage::loadCardData(const std::string& cardNum, Card& card) {
    std::ifstream in("card_" + cardNum + ".txt");
    if (!in.is_open()) return false;
    std::string num, pin;
    bool blocked;
    if (in >> num >> pin >> blocked) { // Чтение пробельных данных из файла через оператор >>
        card = Card(num, pin, blocked);
        return true;
    }
    return false;
}

bool FileStorage::saveCardData(const Card& card) {
    std::ofstream out("card_" + card.getNumber() + ".txt");
    if (!out.is_open()) return false;
    out << card.getNumber() << " 1234 " << card.isBlocked(); // Запись текстовых данных через <<
    return true;
}

bool FileStorage::loadAccountData(const std::string& cardNum, Account& account) {
    std::ifstream in("acc_" + cardNum + ".txt");
    if (!in.is_open()) return false;
    double balance;
    if (in >> balance) {
        account = Account(balance);
        return true;
    }
    return false;
}

bool FileStorage::saveAccountData(const std::string& cardNum, const Account& account) {
    std::ofstream out("acc_" + cardNum + ".txt");
    if (!out.is_open()) return false;
    out << account.getBalance();
    return true;
}
