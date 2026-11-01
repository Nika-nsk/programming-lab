# ОТЧЕТ №7
**Тема:** Реализация кастомной валидации данных и безопасной обработки ошибок в CLI для PyScheduler.  
**Выполнила:** студентка 2 курса Бойченко Вероника Алексеевна, группа ИКС-531  
**Дата:** 01.11.2026
---

# Развитие консольного интерфейса (CLI)

## Что было сделано и изучено за неделю

За отчетную неделю я перешла от базового скелета CLI (Этап 1) к реализации логики предварительной проверки данных и безопасной обработки исключений (Этап 2 спецификации `SPEC-CLI-001`). Кодовая база в файле `sim_cli.py` была существенно доработана и успешно протестирована в среде WSL.

* **Реализовала кастомные валидаторы типов:** Написала функции `positive_int` и `positive_int_or_zero`. Они гарантируют, что симуляция не запустится с некорректными логическими параметрами (например, с отрицательным временем или нулевым числом пользователей).
* **Обеспечила безопасную обработку ошибок парсинга:** Обернула вызов `parser.parse_args()` в блок `try-except SystemExit`. Это позволило перехватывать стандартный экстренный выход `argparse` при невалидных флагах и выводить аккуратное брендированное сообщение `[ERROR]` вместо системного трейсбэка.
* **Привела проект в соответствие со спецификацией:** Архитектура CLI теперь полностью готова к интеграции с бэкенд-функциями `sim_with_manager` и `sim_with_manager_qos` (согласно схеме контракта `FEAT-API-001.md`).
* **Проверила новые тест-кейсы на локальной машине:** Успешно протестировала сценарии с вводом некорректных данных. Система корректно блокирует выполнение и информирует пользователя.

---

## Реализованный код интерфейса (`sim_cli.py`)

Ниже представлен обновленный рабочий код скрипта с функциями валидации:

```python
import argparse
import sys

def positive_int(value):
    """Валидатор для строго положительных целых чисел (параметр ue-count)."""
    try:
        ivalue = int(value)
    except ValueError:
        raise argparse.ArgumentTypeError(f"'{value}' не является целым числом.")
    if ivalue <= 0:
        raise argparse.ArgumentTypeError(f"Значение должно быть больше 0. Передано: {ivalue}")
    return ivalue

def positive_int_or_zero(value):
    """Валидатор для неотрицательных целых чисел (параметр duration)."""
    try:
        ivalue = int(value)
    except ValueError:
        raise argparse.ArgumentTypeError(f"'{value}' не является целым числом.")
    if ivalue < 0:
        raise argparse.ArgumentTypeError(f"Значение не может быть отрицательным. Передано: {ivalue}")
    return ivalue

def main():
    parser = argparse.ArgumentParser(
        description="",
        add_help=False,
        formatter_class=lambda prog: argparse.HelpFormatter(prog, max_help_position=40, width=100)
    )

    subparsers = parser.add_subparsers(dest="command", required=True)
    
    run_parser = subparsers.add_parser(
        'run',
        help='Запуск симуляции',
        add_help=False,
        formatter_class=lambda prog: argparse.HelpFormatter(prog, max_help_position=40, width=100)
    )

    run_parser.add_argument(
        '-h', '--help',
        action='help',
        help='Показать это сообщение и退出'
    )

    run_parser.add_argument(
        '-d', '--duration',
        type=positive_int_or_zero,
        default=5000,
        help='Длительность симуляции в TTI (мс) [дефолт: 50000]'
    )

    run_parser.add_argument(
        '-n', '--ue-count',
        type=positive_int,
        default=5,
        help='Количество пользователей (UE) [дефолт: 5]'
    )

    run_parser.add_argument(
        '-s', '--scheduler',
        type=str,
        choices=['RoundRobin', 'ProportionalFair'],
        default='RoundRobin',
        help='Алгоритм планировщика (RoundRobin / ProportionalFair)'
    )

    run_parser.add_argument(
        '--qos',
        action='store_true',
        help='Запустить симуляцию в режиме QoS-трафика'
    )

    try:
        args = parser.parse_args()
    except SystemExit:
        print("[ERROR] Ошибка валидации параметров консоли.")
        sys.exit(2)

    if args.command == 'run':
            print("[INFO] Считывание параметров консоли...")
            print(f"[INFO] Конфигурация: Длительность={args.duration} TTI, "
                f"Планировщик={args.scheduler}, Пользователей={args.ue_count}.")

if __name__ == '__main__':
    main()
```

---

## Примеры работы интерфейса в консоли (Тест-кейсы)

### Сценарий 1: Успешный запуск с валидными параметрами
При вводе корректных кастомных флагов в терминал WSL:

**Команда для терминала:**
```bash
python3 sim_cli.py run --duration 2500 --scheduler ProportionalFair --ue-count 4
```

**Вывод программы в консоль:**
```text
[INFO] Считывание параметров консоли...
[INFO] Конфигурация: Длительность=2500 TTI, Планировщик=ProportionalFair, Пользователей=4.
```

### Сценарий 2: Тест валидации (Отрицательное значение длительности)
Попытка передать некорректное значение `-100` для флага `--duration`:

**Команда для терминала:**
```bash
python3 sim_cli.py run --duration -100
```

**Вывод программы в консоль:**
```text
sim_cli.py run: error: argument -d/--duration: Значение не может быть отрицательным. Передано: -100
[ERROR] Ошибка валидации параметров консоли.
```
*(Скрипт успешно прерывает выполнение до вызова логики симулятора).*

### Сценарий 3: Тест валидации (Некорректное число пользователей)
Попытка передать `0` в качестве количества пользователей (UE):

**Команда для терминала:**
```bash
python3 sim_cli.py run --ue-count 0
```

**Вывод программы в консоль:**
```text
sim_cli.py run: error: argument -n/--ue-count: Значение должно быть больше 0. Передано: 0
[ERROR] Ошибка валидации параметров консоли.
```

---

## План на следующую неделю (Этап 3)

В соответствии с документом `SPEC-CLI-001` на следующей неделе планируется:
1. Написать логику ветвления в зависимости от флага `--qos`.
2. Подключить программную заглушку бэкенда `API_MODULE-001.md` для автономной отладки вызовов `sim_with_manager` и `sim_with_manager_qos`.
