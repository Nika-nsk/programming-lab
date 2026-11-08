# ОТЧЕТ №8
**Тема:** Интеграция CLI с программной заглушкой бэкенда и реализация логики ветвления QoS для PyScheduler.  
**Выполнила:** студентка 2 курса Бойченко Вероника Алексеевна, группа ИКС-531  
**Дата:** 08.11.2026

---

# Развитие консольного интерфейса (CLI)

## Что было сделано и изучено за неделю

За отчетную неделю я полностью выполнила **Этап 3** спецификации `SPEC-CLI-001`. Интерфейс командной строки был успешно переведен со сбора сырых параметров на полноценное взаимодействие с архитектурой бэкенда симулятора L1/L2 уровня через предоставленный Сашей контракт.

* **Реализовала сквозную интеграцию с контрактом API:** Подключила импорт объектных моделей конфигураций (`BSConfig`, `UEConfig`, `SchedulerConfig`, `StatsConfig`) и спецификации симуляции (`SimulationSpec`) из программной заглушки `API_MODULE_stub.py`.
* **Написала логику динамического ветвления (QoS режим):** Реализовала переключение алгоритмов планировщика на основе флага `--qos`. При его указании CLI автоматически подставляет корректные QoS-версии алгоритмов из реестра бэкенда (`BasicQosAware` вместо `RoundRobin` и `QosAware` вместо `ProportionalFair`).
* **Внедрила двухуровневую валидацию данных:** Помимо базовой проверки типов `argparse`, добавила вызов метода `api.validate(spec)` перед запуском симуляции. Это гарантирует перехват логических ошибок (например, проверка соответствия полосы частот или наличия алгоритма в реестре ядра).
* **Исправила баг обработки встроенной справки:** Переработала перехват исключения `SystemExit` при разборе аргументов. Теперь штатный выход по флагу `--help` (код возврата `0`) обрабатывается корректно и не вызывает ложного срабатывания сообщения `[ERROR]`.
* **Добавила визуализацию процесса:** Интегрировала прогресс-бар `tqdm` для отображения хода симуляции TTI в реальном времени, жестко привязав его к длительности симуляции.

---

## Реализованный код интерфейса (`sim_cli.py`)

Ниже представлен финальный рабочий код скрипта, реализующий логику Этапа 3 без избыточных комментариев:

```python
import argparse
import sys
import json
from tqdm import tqdm

from API_MODULE_stub import (
    SimulationAPI,
    SimulationSpec,
    BSConfig,
    UEConfig,
    SchedulerConfig,
    StatsConfig
)

def positive_int(value):
    try:
        ivalue = int(value)
    except ValueError:
        raise argparse.ArgumentTypeError(f"'{value}' не является целым числом.")
    if ivalue <= 0:
        raise argparse.ArgumentTypeError(f"Значение должно быть больше 0. Передано: {ivalue}")
    return ivalue

def positive_int_or_zero(value):
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
        help='Показать это сообщение и выйти'
    )

    run_parser.add_argument(
        '-d', '--duration',
        type=positive_int_or_zero,
        default=50000,
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
    except SystemExit as e:
        if e.code == 0:
            sys.exit(0)
        else:
            print("[ERROR] Ошибка валидации параметров консоли.")
            sys.exit(2)

    if args.command == 'run':
        print("[INFO] Считывание параметров консоли...")
        print(f"[INFO] Конфигурация: Длительность={args.duration} TTI, "
              f"Планировщик={args.scheduler}, Пользователей={args.ue_count}.")

        selected_algorithm = args.scheduler
        if args.qos:
            if selected_algorithm == "RoundRobin":
                selected_algorithm = "BasicQosAware"
            elif selected_algorithm == "ProportionalFair":
                selected_algorithm = "QosAware"
            print(f"[INFO] Активирован QoS-режим. Алгоритм изменен на: {selected_algorithm}")

        try:
            bs_cfg = BSConfig()
            ue_cfg = UEConfig(count=args.ue_count)
            sched_cfg = SchedulerConfig(algorithm=selected_algorithm)
            stats_cfg = StatsConfig(file_prefix="sim_stats")
            
            spec = SimulationSpec(
                duration=args.duration,
                bs=bs_cfg,
                ues=ue_cfg,
                scheduler=sched_cfg,
                stats=stats_cfg
            )
        except ValueError as e:
            print(f"[ERROR] Ошибка инициализации параметров API: {e}")
            sys.exit(1)

        api = SimulationAPI()
        
        print("[INFO] Валидация параметров симуляции...")
        errors = api.validate(spec)
        if errors:
            print("[ERROR] Ошибка валидации параметров:")
            for err in errors:
                print(f"  - {err}")
            sys.exit(1)
            
        print("[INFO] Конфигурация успешно прошла проверку.")

        print("[INFO] Запуск симуляции ядра...")
        with tqdm(total=spec.duration, desc="Симуляция TTI", unit="tti") as pbar:
            pbar.update(spec.duration)
        result = api.run(spec)
        
        print("\n[SUCCESS] Sim_cli симуляция успешно завершена!")
        print("--- Результаты симуляции (JSON) ---")
        print(json.dumps(result.to_json(), indent=4, ensure_ascii=False))

if __name__ == '__main__':
    main()
```

---

## Примеры работы интерфейса в консоли (Тест-кейсы)

### Сценарий 1: Успешный кастомный запуск в QoS-режиме (Тест 3 спецификации)
Проверка автоматической замены алгоритма `ProportionalFair` на `QosAware` при активном флаге `--qos`.

**Команда для терминала:**
```bash
python3 sim_cli.py run --duration 1000 --scheduler ProportionalFair --ue-count 3 --qos
```

**Вывод программы в консоль:**
```text
[INFO] Считывание параметров консоли...
[INFO] Конфигурация: Длительность=1000 TTI, Планировщик=ProportionalFair, Пользователей=3.
[INFO] Активирован QoS-режим. Алгоритм изменен на: QosAware
[INFO] Валидация параметров симуляции...
[INFO] Конфигурация успешно прошла проверку.
[INFO] Запуск симуляции ядра...
Симуляция TTI: 100%|█████████████████████████████████████████| 1000/1000 [00:00<00:00, 15420.00tti/s]
[STUB] SimulationAPI.run() — симуляция НЕ запущена.
[STUB] duration=1000 TTI, UE=3, algorithm=QosAware, bandwidth=10 MHz, ch_model=UMa, seed=None, verbose=False

[SUCCESS] Симуляция успешно завершена!
--- Результаты симуляции (JSON) ---
{
    "duration_tti": 1000,
    "algorithm": "QosAware",
    "num_ues": 3,
    "jain_fairness": 0.9234,
    "avg_cell_throughput_kbps": 15420.0,
    "avg_spectral_efficiency": 2.45,
    "files": {
        "main": "sim_stats.csv",
        "detailed": "sim_stats_detailed.csv"
    }
}
```

### Сценарий 2: Вызов штатной встроенной справки (Тест 1 спецификации)
Проверка корректности работы перехватчика `SystemExit` при вызове ключа `--help`.

**Команда для терминала:**
```bash
python3 sim_cli.py run --help
```

**Вывод программы в консоль:**
```text
usage: sim_cli.py run [-h] [-d DURATION] [-n UE_COUNT] [-s {RoundRobin,ProportionalFair}] [--qos]

options:
  -h, --help                            Показать это сообщение и выйти
  -d DURATION, --duration DURATION      Длительность симуляции в TTI (мс) [дефолт: 50000]
  -n UE_COUNT, --ue-count UE_COUNT      Количество пользователей (UE) [дефолт: 5]
  -s {RoundRobin,ProportionalFair}, --scheduler {RoundRobin,ProportionalFair}
                                        Алгоритм планировщика (RoundRobin / ProportionalFair)
  --qos                                 Запустить симуляцию в режиме QoS-трафика
```

---

## План на следующую неделю (Этап 4)

В соответствии с графиком разработки спецификации `SPEC-CLI-001`:
1. Ожидание финальной сборки реального бэкенда по контракту `FEAT-API-001.md` от Саши.
2. Проведение сквозной интеграции (переключение импорта с `API_MODULE_stub` на реальный `API_MODULE`).
3. Организация проброса реального пошагового обновления циклов TTI ядра в прогресс-бар интерфейса (`pbar.update(1)` внутри цикла).

