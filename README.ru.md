<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><strong>Допуск образа перед загрузкой · unsigned</strong></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>
<sub>Обновлено: 2026-11-11</sub>


## Навигация

- [Текущие возможности](#текущие-возможности)
- [Быстрый старт и проверка](#быстрый-старт-и-проверка)
- [Архитектура и подтверждённые границы](#архитектура-и-подтверждённые-границы)
- [Интерфейс и визуальные материалы](#интерфейс-и-визуальные-материалы)
- [Инженерное качество](#инженерное-качество)
- [Карта документации](#карта-документации)
- [NUN Mesh и связанные репозитории](#nun-mesh-и-связанные-репозитории)
- [Дизайн и документация](#дизайн-и-документация)
- [Лицензия](#лицензия)
- [Палитра Duck Black / Yellow](#палитра-duck-black-yellow)

> **NUNLauncher** — NUNLauncher определяет границу между firmware/bootloader и ядром NUN и проверяет метаданные kernel image перед будущей передачей управления.

## Текущие возможности
- Host-side validator 48-байтового заголовка kernel image.
- Проверка architecture, entry offset и границ caller-provided memory map.
- Переносимая CMake-сборка, host-тесты и GCC/Clang CI.
- Версионированный boot-information contract и capability-gate design.

## Быстрый старт и проверка
```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build --output-on-failure
python one.py check
make test
```

Проверка собранного NUN ISO: `make verify IMAGE=../NUN/build/NUN.iso MANIFEST=../NUN/build/NUN.image.json`.

## Архитектура и подтверждённые границы
NUNLauncher — host-side слой допуска образа перед запуском: он проверяет собранный NUN ISO, но не заменяет firmware или Limine, не передаёт управление ядру и не выполняет код образа. CI отдельно запускает тот же образ в QEMU через BIOS и x86_64 UEFI. Host preflight сам по себе не доказывает совместимость с произвольным физическим оборудованием.

Secure Boot, подпись издателя, ротация ключей и TPM attestation не реализованы. Текущий manifest явно отмечает образ как unsigned; SHA-256 подтверждает соответствие manifest, а не личность издателя.

## Интерфейс и визуальные материалы

NUNLauncher — host-side валидатор метаданных загрузки на C11, а не графическое приложение. Скриншотов GUI нет; изображение ниже показывает только общий стиль документации Duck Black / Yellow.

## Проверка образа на host

`tools/validate_boot_image.py` проверяет JSON-manifest, SHA-256, безопасный путь к образу, границы памяти и выравнивание entry point. Это проверка метаданных, а не bootloader и не подтверждение поддержки архитектуры.

```sh
python tools/validate_boot_image.py path/to/kernel.manifest.json --target x86_64
python -m unittest discover -s tests -p 'test_*.py' -v
```

Validator не загружает и не исполняет код. Проверка подписи и квалификация на физическом оборудовании остаются отдельными этапами.

## NUN boot-image supervisor

Команда `make verify IMAGE=../NUN/build/NUN.iso MANIFEST=../NUN/build/NUN.image.json` запускает полный pre-handoff процесс: manifest/schema, имя и размер ISO, SHA-256, BIOS/UEFI El Torito entries, ELF64 x86_64 kernel и AMD64 EFI fallback loader. Проверка выполняется только на чтение; запись на диск и передача управления ядру не происходят.

`make test` запускает host-тесты supervisor. CI собирает NUN из закреплённого commit и запускает тот же ISO в QEMU через BIOS и x86_64 UEFI. Это подтверждает начальный boot path в этих эмуляторных профилях, а не готовность полноценной ОС или физическую аппаратную квалификацию.

Отдельная команда `verify-e2k` проверяет структуру E2K Linux userspace ELF (`EM_MCST_ELBRUS=175`) и manifest. Это не bare-metal E2K bootloader/kernel. Нативный запуск выделен в отдельный ручной workflow на self-hosted E2K runner.

<p align="center"><img src="assets/duck-ui/duck-ui-preview.svg" alt="Общая дизайн-система Duck Black Yellow; не интерфейс NUNLauncher" width="960"></p>

## Инженерное качество
- Запускайте проверки для целевой платформы; host CI не заменяет аппаратную квалификацию.
- Статусы implemented, partial и planned должны подтверждаться кодом, тестами и доказательствами.
- Макет, спецификация или открытый Pull Request не доказывают production-ready реализацию.
- Внешние интеграции должны быть явными, версионированными, изолированными и тестируемыми.

## Карта документации

- [Контракт образа ядра](docs/BOOT_IMAGE_CONTRACT.md)
- [Boot-image supervisor и модель доверия](docs/BOOT_SUPERVISOR.md)
- [E2K Linux userspace admission](docs/E2K_RUNTIME.md)
- [Интеграция экосистемы](docs/ECOSYSTEM_INTEGRATION.md)
- [Инженерное управление](docs/ENGINEERING_GOVERNANCE.md)

## NUN Mesh и связанные репозитории
[SAMBU](https://github.com/bboroski/SAMBU) · [ToolNG](https://github.com/bboroski/ToolNG) · [ReaperCripple](https://github.com/bboroski/ReaperCripple) · [Kloz](https://github.com/bboroski/Kloz) · [CO](https://github.com/bboroski/CO) · [SaintSI](https://github.com/bboroski/SaintSI) · [Klipot](https://github.com/bboroski/Klipot) · [NUN](https://github.com/bboroski/NUN) · [NUNLauncher](https://github.com/bboroski/NUNLauncher)

Каждый репозиторий сохраняет каноническое имя, собственный код, API, цикл выпуска и границы ответственности. Интеграция означает версионированные контракты, а не автоматическое слияние независимых runtime.

## Дизайн и документация
Общее направление интерфейсов — **Duck Black / Yellow**: тёмные поверхности, жёлтые акценты для действий и состояний, читаемая типографика, доступный контраст, клавиатурная навигация и поддержка reduced motion. Подробности — в [design/DUCK_UI_SYSTEM.md](design/DUCK_UI_SYSTEM.md), если файл присутствует.

## Лицензия
Лицензия не объявлена; файл LICENSE отсутствует.

---
**Создано с любовью к инженерии, системности и людям.**


## Палитра Duck Black / Yellow
Совместимые цвета прежних badges и marks: `#FFD600` (жёлтый) и `#111111` (почти чёрный). Текущий фон интерфейса — `#0D0D0D`. Полная палитра из шести цветов, четырнадцати функциональных оттенков и значения RGB/HSL находятся в `design/duck-ui.tokens.json` (schema 3.0.0). Жёлтый зарезервирован для основных действий, фокуса и выделения; красный и голубой обозначают семантические ошибки и информационные состояния.
