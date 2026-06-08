#pragma once
#include <string>

class Card {
private:
    std::string cardNumber;
    std::string pin;
    bool blocked;
    int pinAttempts;

public:
    Card(std::string num = "", std::string p = "", bool isBlocked = false);
    std::string getNumber() const;
    bool checkPin(const std::string& enteredPin);
    bool isBlocked() const;
    void block();
    int getAttemptsLeft() const;
};
