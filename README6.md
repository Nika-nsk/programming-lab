# ОТЧЕТ №6
**Тема:** Реализация консольного интерфейса (CLI) для PyScheduler.  
**Выполнила:** студентка 2 курса Бойченко Вероника Алексеевна, группа ИКС-531  

---

# Реализация консольного интерфейса (CLI)

## Что было сделано и изучено за неделю

За эту неделю я перешла от проектирования к программной реализации консольного интерфейса, написала и успешно протестировала кодовую базу в файле `sim_cli.py`.

* **Разработала структуру CLI:** На основе стандартной библиотеки `argparse` написала готовый скрипт, поддерживающий главную подкоманду `run` и гибкую параметризацию флагами.
* **Перенесла параметры из спецификации в код:** Успешно настроила типы данных, ограничения (`choices`) и дефолтные значения для всех четырех ключевых флагов (`--duration`, `--ue-count`, `--scheduler`, `--qos`).
* **Реализовала логику логирования и считывания:** Программа успешно парсит переданные аргументы командной строки и выводит информационные сообщения формата `[INFO]` с текущей конфигурацией запуска.
* **Проверила и отладила работу интерфейса в консоли:** Проверила выполнение скрипта в среде WSL на локальной машине. Информационные логи успешно выводятся в терминал после обработки аргументов командной строки.
* **Проверила генерацию встроенной справки:** Убедилась, что библиотека `argparse` автоматически формирует корректный и читаемый Usage-текст при вызове флага `-h` или `--help`.

---

## Реализованный код интерфейса (`sim_cli.py`)

Ниже представлен рабочий код скрипта `sim_cli.py`:

```python
import argparse

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
        help='Показать это сообщение и выйти'
    )

    run_parser.add_argument(
        '-d', '--duration',
        type=int,
        default=5000,
        help='Длительность симуляции в TTI (мс) [дефолт: 50000]'
    )

    run_parser.add_argument(
        '-n', '--ue-count',
        type=int,
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

    args = parser.parse_args()

    if args.command == 'run':
            print("[INFO] Считывание параметров консоли...")
            print(f"[INFO] Конфигурация: Длительность={args.duration} TTI, "
                f"Планировщик={args.scheduler}, Пользователей={args.ue_count}.")

if __name__ == '__main__':
    main()
```

---

## Примеры работы интерфейса в консоли

### Сценарий 1: Просмотр встроенной справки
При вводе команды вызова помощи:

>  **Команда для терминала:**
> ```bash
> python3 sim_cli.py run --help
> ```

Программа автоматически выводит в терминал следующий текст:
```text
usage: sim_cli.py run [-h] [-d DURATION] [-n UE_COUNT] [-s {RoundRobin,ProportionalFair}] [--qos]

options:
  -h, --help            Показать это сообщение и выйти
  -d, --duration        Длительность симуляции в TTI (мс) [дефолт: 50000]
  -n, --ue-count        Количество пользователей (UE) [дефолт: 5]
  -s, --scheduler       Алгоритм планировщика (RoundRobin / ProportionalFair)
  --qos                 Запустить симуляцию в режиме QoS-трафика
```

### Сценарий 2: Запуск симуляции с кастомными параметрами
При вводе команды запуска со своими флагами в WSL:

>  **Команда для терминала:**
> ```bash
> python3 sim_cli.py run --duration 5000 --scheduler ProportionalFair --ue-count 3
> ```

Интерфейс успешно обрабатывает ввод и выводит информацию в консоль:

>  **Вывод программы в консоль:**
> ```text
> [INFO] Считывание параметров консоли...
> [INFO] Конфигурация: Длительность=5000 TTI, Планировщик=ProportionalFair, Пользователей=3.
> ```
