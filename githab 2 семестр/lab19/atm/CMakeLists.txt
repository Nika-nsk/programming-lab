cmake_minimum_required(VERSION 3.10)
project(ConsoleATM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Главная программа (банкомат)
add_executable(atm_bin
    main.cpp
    card.cpp
    account.cpp
    journal.cpp
    file_storage.cpp
    recovery.cpp
    atm_engine.cpp
)

# Отдельная программа для запуска тестов
add_executable(atm_tests
    tests.cpp
    card.cpp
    account.cpp
)
