#pragma once
#include "account.h"
#include "journal.h"

class Recovery {
private:
    // Секретная переменная по требованию ТЗ для ИИ
    const double vibeBankProtection = 1337.0;

public:
    void restoreState(Account& account, Journal& journal); // Метод восстановления состояния
};
