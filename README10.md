# ОТЧЕТ №10
**Тема:** Улучшение UX консольного интерфейса (CLI) с помощью библиотеки `rich`, структурирование вывода и перехват системных логов ядра.  
**Выполнила:** студентка 2 курса Бойченко Вероника Алексеевна, группа ИКС-531  
**Дата:** 22.11.2026

---

# Модернизация визуального вывода CLI (UX терминала)

## Что было сделано и изучено за неделю

За отчетную неделю я провела рефакторинг визуального слоя консольного интерфейса (`sim_cli.py`) в рамках улучшения пользовательского опыта (UX) и завершения работ по визуализации перед финальной интеграцией с бэкендом:

* **Внедрила библиотеку `rich` для форматирования терминального вывода:** Установила пакет `python3-rich` в рабочее окружение WSL и заменила разрозненные текстовые логи (`print`) на структурированные визуальные компоненты:
  * Добавила стартовый баннер (`Panel`) с информацией о модуле.
  * Сгруппировала входные параметры запуска (сценарий, длительность, планировщик, количество UE) в наглядную таблицу (`Table`) с цветовой дифференциацией режимов (`simple` / `qos`).
* **Обновила индикацию прогресса и статусов выполнения:** Заменила статичный текстовый вывод процесса валидации на анимированный индикатор (`console.status`), а прогресс-бар `tqdm` перевела на компонент `rich.progress.Progress` с отображением спиннера, цветовой шкалы прогресса и таймера выполнения.
* **Реализовала перехват и стилизацию внутреннего вывода ядра (`redirect_stdout`):** Поскольку заглушка `API_stub_2` (и будущий модуль ядра) выводит служебную информацию напрямую в стандартный поток `stdout` без форматирования, я реализовала буферизацию через `io.StringIO` и `contextlib.redirect_stdout` на время вызова `api.run(spec)`. Перехваченный текст очищается от лишних разделителей и выводится в отдельной стилизованной панели «Лог ядра / Stub Runner» без изменения исходного кода самого API.
* **Добавила подсветку синтаксиса для итогового JSON-отчета:** С помощью компонента `rich.syntax.Syntax` (тема `monokai`) реализовала цветовое выделение ключей, строк и числовых метрик в финальном выводе статистики (`jain_fairness`, `avg_cell_throughput_kbps`, `avg_prb_utilization_pct` и др.).

---

## Реализованный код интерфейса (`sim_cli.py`)

Ниже представлен обновленный код скрипта с поддержкой расширенного визуального вывода:

```python
import argparse
import sys
import json
import io
from contextlib import redirect_stdout
from rich.console import Console
from rich.panel import Panel
from rich.table import Table
from rich.syntax import Syntax
from rich.progress import Progress, SpinnerColumn, TextColumn, BarColumn, TimeElapsedColumn
from rich import box

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

console = Console()


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
            console.print("[bold red]✖ Ошибка валидации параметров консоли.[/bold red]")
            sys.exit(2)

    if args.command == 'run':
        console.print(Panel.fit(
            "[bold cyan]PyScheduler CLI[/bold cyan] • [dim]Модуль симуляции сети[/dim]",
            border_style="cyan"
        ))

        selected_scenario = "qos" if args.qos else "simple"
        selected_algorithm = args.scheduler

        if args.qos:
            if selected_algorithm == "RoundRobin":
                selected_algorithm = "BasicQosAware"
            elif selected_algorithm == "ProportionalFair":
                selected_algorithm = "QosAware"
            console.print(f"[yellow]⚡ Активирован QoS-режим.[/yellow] Алгоритм адаптирован на: [bold magenta]{selected_algorithm}[/bold magenta]\n")

        config_table = Table(title="Параметры запуска", box=box.ROUNDED, border_style="blue", show_header=False)
        config_table.add_column("Параметр", style="bold cyan")
        config_table.add_column("Значение", style="white")

        config_table.add_row("Сценарий", f"[bold green]{selected_scenario.upper()}[/bold green]" if args.qos else selected_scenario)
        config_table.add_row("Длительность", f"{args.duration:,} TTI".replace(",", " "))
        config_table.add_row("Планировщик", f"[bold magenta]{selected_algorithm}[/bold magenta]")
        config_table.add_row("Пользователей (UE)", str(args.ue_count))

        console.print(config_table)
        console.print()

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
            console.print(f"[bold red]✖ Ошибка инициализации параметров API:[/bold red] {e}")
            sys.exit(1)

        api = SimulationAPI()

        with console.status("[bold cyan]Валидация параметров симуляции...[/bold cyan]", spinner="dots"):
            errors = api.validate(spec)

        if errors:
            console.print("[bold red]✖ Ошибка валидации параметров:[/bold red]")
            for err in errors:
                console.print(f"  [red]•[/red] {err}")
            sys.exit(1)

        console.print("[bold green]✔ Конфигурация успешно прошла проверку.[/bold green]\n")

        stub_output = io.StringIO()

        with Progress(
            SpinnerColumn(),
            TextColumn("[progress.description]{task.description}"),
            BarColumn(bar_width=40, complete_style="green", finished_style="bold green"),
            TextColumn("[progress.percentage]{task.percentage:>3.0f}%"),
            TimeElapsedColumn(),
            console=console
        ) as progress:
            task = progress.add_task("[cyan]Симуляция ядра (TTI)...", total=spec.duration)

            with redirect_stdout(stub_output):
                result = api.run(spec)

            progress.update(task, completed=spec.duration)

        raw_lines = [line for line in stub_output.getvalue().splitlines() if not line.startswith("===")]
        if raw_lines:
            cleaned_text = "\n".join(raw_lines)
            console.print()
            console.print(Panel(
                f"[cyan]{cleaned_text}[/cyan]",
                title="[bold yellow]Лог ядра / Stub Runner[/bold yellow]",
                border_style="yellow",
                expand=False
            ))

        console.print("\n[bold green]🎉 Симуляция успешно завершена![/bold green]\n")

        json_str = json.dumps(result.to_json(), indent=4, ensure_ascii=False)
        syntax = Syntax(json_str, "json", theme="monokai", line_numbers=False)

        console.print(Panel(
            syntax,
            title="[bold white]Результаты симуляции (JSON)[/bold white]",
            border_style="green",
            expand=False
        ))


if __name__ == '__main__':
    main()
```

---

## Примеры работы интерфейса в консоли (Тест-кейсы)

### Сценарий 1: Запуск в режиме QoS-трафика (`scenario='qos'`)
Проверка отображения таблицы конфигурации с выделением QoS-режима, перехвата лога заглушки и подсветки синтаксиса итогового JSON.

**Команда для терминала:**
```bash
python3 sim_cli.py run --duration 5000 --scheduler ProportionalFair --ue-count 3 --qos
```

**Вывод программы в консоль:**
```text
╭──────────────────────────────────────────╮
│ PyScheduler CLI • Модуль симуляции сети  │
╰──────────────────────────────────────────╯
⚡ Активирован QoS-режим. Алгоритм адаптирован на: QosAware

             Параметры запуска              
╭────────────────────┬─────────────────────╮
│ Сценарий           │ QOS                 │
│ Длительность       │ 5 000 TTI           │
│ Планировщик        │ QosAware            │
│ Пользователей (UE) │ 3                   │
╰────────────────────┴─────────────────────╯

✔ Конфигурация успешно прошла проверку.

⠋ Симуляция ядра (TTI)... ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 100% 0:00:00

╭─────────── Лог ядра / Stub Runner ───────────╮
│ [STUB RUNNER] Сборка сценария 'qos':         │
│  - Длительность: 5000 TTI                    │
│  - Пользователей: 3 (random)                 │
│  - Трафик: Poisson @ 5000.0 pkt/s            │
│  - Алгоритм: QosAware                        │
│  - Модель канала: UMa                        │
╰──────────────────────────────────────────────╯

🎉 Симуляция успешно завершена!

╭────────────────── Результаты симуляции (JSON) ───────────────────╮
│ {                                                                │
│     "duration_tti": 5000,                                        │
│     "algorithm": "QosAware",                                     │
│     "num_ues": 3,                                                │
│     "jain_fairness": 0.9713,                                     │
│     "avg_cell_throughput_kbps": 16631.62,                        │
│     "avg_spectral_efficiency": 3.37,                             │
│     "avg_prb_utilization_pct": 86.36,                            │
│     "files": {                                                   │
│         "main": "sim_stats.csv",                                 │
│         "detailed": "sim_stats_detailed.csv"                     │
│     }                                                            │
│ }                                                                │
╰──────────────────────────────────────────────────────────────────╯
```

### Сценарий 2: Стандартный запуск симуляции (`scenario='simple'`)
Проверка базового запуска с параметрами по умолчанию для планировщика `RoundRobin`.

**Команда для терминала:**
```bash
python3 sim_cli.py run --duration 1000 --ue-count 2
```

**Вывод программы в консоль:**
```text
╭──────────────────────────────────────────╮
│ PyScheduler CLI • Модуль симуляции сети  │
╰──────────────────────────────────────────╯
             Параметры запуска              
╭────────────────────┬─────────────────────╮
│ Сценарий           │ simple              │
│ Длительность       │ 1 000 TTI           │
│ Планировщик        │ RoundRobin          │
│ Пользователей (UE) │ 2                   │
╰────────────────────┴─────────────────────╯

✔ Конфигурация успешно прошла проверку.

⠋ Симуляция ядра (TTI)... ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━ 100% 0:00:00

╭─────────── Лог ядра / Stub Runner ───────────╮
│ [STUB RUNNER] Сборка сценария 'simple':      │
│  - Длительность: 1000 TTI                    │
│  - Пользователей: 2 (random)                 │
│  - Трафик: Poisson @ 5000.0 pkt/s            │
│  - Алгоритм: RoundRobin                      │
│  - Модель канала: UMa                        │
╰──────────────────────────────────────────────╯

🎉 Симуляция успешно завершена!

╭────────────────── Результаты симуляции (JSON) ───────────────────╮
│ {                                                                │
│     "duration_tti": 1000,                                        │
│     "algorithm": "RoundRobin",                                   │
│     "num_ues": 2,                                                │
│     "jain_fairness": 0.925,                                      │
│     "avg_cell_throughput_kbps": 19184.41,                        │
│     "avg_spectral_efficiency": 2.02,                             │
│     "avg_prb_utilization_pct": 89.56,                            │
│     "files": {                                                   │
│         "main": "sim_stats.csv",                                 │
│         "detailed": "sim_stats_detailed.csv"                     │
│     }                                                            │
│ }                                                                │
╰──────────────────────────────────────────────────────────────────╯
```

---

## План на следующую неделю

1. **Интеграция с боевым модулем API:** Подключение реального ядра симулятора (`API_MODULE`) вместо заглушки `API_stub_2` и проведение сквозного тестирования всех сценариев запуска (`simple` и `qos`).
2. **Оформление Pull Request (PR):** Подготовка ветки, актуализация зависимостей (`requirements.txt` с добавлением `rich`) и документации в репозитории, создание Pull Request для слияния финальной версии CLI в основную ветку проекта.
