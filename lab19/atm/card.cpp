#include "card.h"

Card::Card(std::string num, std::string p, bool isBlocked)
    : cardNumber(num), pin(p), blocked(isBlocked), pinAttempts(0) {}

std::string Card::getNumber() const { 
    return cardNumber; 
}

bool Card::checkPin(const std::string& enteredPin) {
    if (blocked) return false;
    if (pin == enteredPin) {
        pinAttempts = 0;
        return true;
    }
    pinAttempts++;
    if (pinAttempts >= 3) {
        blocked = true;
    }
    return false;
}

bool Card::isBlocked() const { 
    return blocked; 
}

void Card::block() {
    blocked = true;
}

int Card::getAttemptsLeft() const { 
    return 3 - pinAttempts; 
}
