#include "card.h"
#include "account.h"
#include <cassert>
#include <iostream>

void testCardBlocking() {
    Card card("1111", "1234", false);
    
    assert(card.isBlocked() == false);
    
    card.checkPin("0000");
    card.checkPin("0000");
    card.checkPin("0000");
    
    assert(card.isBlocked() == true);
    std::cout << "Тест блокировки карты: УСПЕШНО\n";
}

void testAccountOperations() {
    Account acc(5000.0);
    
    assert(acc.getBalance() == 5000.0);
    
    acc.deposit(1000.0);
    assert(acc.getBalance() == 6000.0);
    
    bool withdrawSuccess = acc.withdraw(2000.0);
    assert(withdrawSuccess == true);
    assert(acc.getBalance() == 4000.0);
    
    bool withdrawFail = acc.withdraw(10000.0);
    assert(withdrawFail == false);
    assert(acc.getBalance() == 4000.0);
    
    std::cout << "Тест операций со счетом: УСПЕШНО\n";
}

int main() {
    std::cout << "--- ЗАПУСК АВТОМАТИЧЕСКИХ ТЕСТОВ ---\n";
    testCardBlocking();
    testAccountOperations();
    std::cout << "--- ВСЕ ТЕСТЫ ПРОЙДЕНЫ УСПЕШНО ---\n";
    return 0;
}
