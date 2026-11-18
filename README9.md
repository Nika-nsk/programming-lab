# ОТЧЕТ №9
**Тема:** Адаптация CLI под контракт API v2, поддержка сценариев симуляции и подготовка к сквозной интеграции с ядром PyScheduler.  
**Выполнила:** студентка 2 курса Бойченко Вероника Алексеевна, группа ИКС-531  
**Дата:** 15.11.2026

---

# Развитие консольного интерфейса (CLI)

## Что было сделано и изучено за неделю

За отчетную неделю я провела рефакторинг и интеграционное тестирование консольного интерфейса в рамках перехода на **Этап 4** спецификации `SPEC-CLI-001`. После получения от бэкендера Саши обновленной версии контракта API (v2) с поддержкой сценариев (`simple` и `qos`) я адаптировала `sim_cli.py` под новую архитектуру и подготовила интерфейс к бесшовному переключению на боевое ядро:

* **Адаптировала формирование спецификации под контракт API v2:** Обновила сборку объекта `SimulationSpec` для передачи параметра `scenario`. Теперь при активации флага `--qos` CLI не только подменяет алгоритм планировщика (`BasicQosAware` / `QosAware`), но и явно переключает режим работы симулятора на `scenario="qos"` (в штатном режиме передается `scenario="simple"`).
* **Реализовала механизм автоматического переключения импортов (Fallback):** Внедрила конструкцию `try / except ImportError` при подключении модулей API. При наличии в проекте боевого `API_MODULE` интерфейс автоматически подключается к реальному ядру, а при его отсутствии — откатывается на актуальную заглушку `API_stub_2` без необходимости ручной правки исходного кода.
* **Усилила предварительную валидацию длительности симуляции:** Привела проверку флага `-d / --duration` в соответствие с ограничениями `SimulationSpec.__post_init__` (требование `duration > 0`). Заменила валидатор на строгий `positive_int`, чтобы отсекать нулевые и отрицательные значения на уровне консоли до инициализации объектов API.
* **Синхронизировала порядок отрисовки прогресс-бара `tqdm`:** Перенесла обновление индикатора выполнения (`pbar.update`) строго после завершения вызова `api.run(spec)`, устранив преждевременное заполнение шкалы прогресса до старта расчетов.
* **Провела серию интеграционных тестов на заглушке v2 (`API_stub_2`):** Проверила корректность сборки сценариев `'simple'` и `'qos'`, валидацию входных параметров и формирование итогового JSON-отчета со статистикой (`avg_prb_utilization_pct`, `jain_fairness`, пути к CSV-файлам).

---

## Реализованный код интерфейса (`sim_cli.py`)

Ниже представлен актуальный рабочий код скрипта, синхронизированный с контрактом API v2:

```python
import argparse
import sys
import json
from tqdm import tqdm

try:
    from API_MODULE import (
        SimulationAPI,
        SimulationSpec,
        BSConfig,
        UEConfig,
        SchedulerConfig,
        StatsConfig
    )

except ImportError:
    from API_stub_2 import (
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


def main():
    parser = argparse.ArgumentParser(
        description="CLI для управления модулем PyScheduler",
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
        type=positive_int,
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

        selected_scenario = "qos" if args.qos else "simple"
        selected_algorithm = args.scheduler
        if args.qos:
            if selected_algorithm == "RoundRobin":
                selected_algorithm = "BasicQosAware"
            elif selected_algorithm == "ProportionalFair":
                selected_algorithm = "QosAware"
            print(f"[INFO] Активирован QoS-режим. Алгоритм изменен на: {selected_algorithm}")
        
        print(f"[INFO] Конфигурация: Длительность={args.duration} TTI, "
              f"Планировщик={selected_algorithm}, Пользователей={args.ue_count}.")
        try:
            bs_cfg = BSConfig()
            ue_cfg = UEConfig(count=args.ue_count)
            sched_cfg = SchedulerConfig(algorithm=selected_algorithm)
            stats_cfg = StatsConfig(file_prefix="sim_stats")
            
            spec = SimulationSpec(
                duration=args.duration,
                scenario=selected_scenario,
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
            result = api.run(spec)
            pbar.update(spec.duration)
        print("\n[SUCCESS] Симуляция успешно завершена!")
        print("--- Результаты симуляции (JSON) ---")
        print(json.dumps(result.to_json(), indent=4, ensure_ascii=False))

if __name__ == '__main__':
    main()
```

---

## Примеры работы интерфейса в консоли (Тест-кейсы)

### Сценарий 1: Запуск в режиме QoS-трафика (`scenario='qos'`)
Проверка одновременного переключения сценария на `'qos'` и подмены алгоритма `ProportionalFair` на `QosAware` при указании флага `--qos`.

**Команда для терминала:**
```bash
python3 sim_cli.py run --duration 5000 --scheduler ProportionalFair --ue-count 3 --qos
```

**Вывод программы в консоль:**
```text
[INFO] Считывание параметров консоли...
[INFO] Активирован QoS-режим. Алгоритм изменен на: QosAware
[INFO] Конфигурация: Длительность=5000 TTI, Планировщик=QosAware, Пользователей=3.
[INFO] Валидация параметров симуляции...
[INFO] Конфигурация успешно прошла проверку.
[INFO] Запуск симуляции ядра...
Симуляция TTI:   0%|                                                                 | 0/5000 [00:00<?, ?tti/s]
==================================================
[STUB RUNNER] Сборка сценария 'qos':
 - Длительность: 5000 TTI
 - Пользователей: 3 (random)
 - Трафик: Poisson @ 5000.0 pkt/s
 - Алгоритм: QosAware
 - Модель канала: UMa
==================================================

Симуляция TTI: 100%|██████████████████████████████████████████████████████████| 5000/5000 [00:00<00:00, 9977.95tti/s]

[SUCCESS] Симуляция успешно завершена!
--- Результаты симуляции (JSON) ---
{
    "duration_tti": 5000,
    "algorithm": "QosAware",
    "num_ues": 3,
    "jain_fairness": 0.9713,
    "avg_cell_throughput_kbps": 16631.62,
    "avg_spectral_efficiency": 3.37,
    "avg_prb_utilization_pct": 86.36,
    "files": {
        "main": "sim_stats.csv",
        "detailed": "sim_stats_detailed.csv"
    }
}
```

### Сценарий 2: Стандартный запуск симуляции (`scenario='simple'`)
Проверка базового сценария работы планировщика `RoundRobin` с кастомной длительностью и количеством пользователей.

**Команда для терминала:**
```bash
python3 sim_cli.py run --duration 1000 --ue-count 2
```

**Вывод программы в консоль:**
```text
[INFO] Считывание параметров консоли...
[INFO] Конфигурация: Длительность=1000 TTI, Планировщик=RoundRobin, Пользователей=2.
[INFO] Валидация параметров симуляции...
[INFO] Конфигурация успешно прошла проверку.
[INFO] Запуск симуляции ядра...
Симуляция TTI:   0%|                                                                 | 0/1000 [00:00<?, ?tti/s]
==================================================
[STUB RUNNER] Сборка сценария 'simple':
 - Длительность: 1000 TTI
 - Пользователей: 2 (random)
 - Трафик: Poisson @ 5000.0 pkt/s
 - Алгоритм: RoundRobin
 - Модель канала: UMa
==================================================

Симуляция TTI: 100%|██████████████████████████████████████████████████████████| 1000/1000 [00:00<00:00, 1988.41tti/s]

[SUCCESS] Симуляция успешно завершена!
--- Результаты симуляции (JSON) ---
{
    "duration_tti": 1000,
    "algorithm": "RoundRobin",
    "num_ues": 2,
    "jain_fairness": 0.925,
    "avg_cell_throughput_kbps": 19184.41,
    "avg_spectral_efficiency": 2.02,
    "avg_prb_utilization_pct": 89.56,
    "files": {
        "main": "sim_stats.csv",
        "detailed": "sim_stats_detailed.csv"
    }
}
```

---

## План на следующую неделю (Завершение Этапа 4)

В соответствии с графиком разработки спецификации `SPEC-CLI-001`:
1. Проведение финального сквозного тестирования после слияния реального модуля `API_MODULE.py` от бэкенда в общий репозиторий.
2. Согласование с Сашей механизма отображения прогресса работы ядра (использование `spec.verbose = True` или проброс колбэка в `tqdm` во избежание дублирования прогресс-баров в терминале).
3. Актуализация документации `SPEC-CLI-001.md` (обновление архитектурной схемы и импортов под финальный контракт v2) и закрытие критериев готовности (Definition of Done).
4. Улучшение форматирования вывода в консоли (UX терминала).