<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-SPECIFICATION%20ONLY-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/IMPLEMENTATION-DOCUMENTATION%20SCAFFOLD-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/LICENSE-NOT%20DECLARED-FFD600?style=for-the-badge&labelColor=111111"></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>

---

**NUNLauncher определяет будущую узкую границу между firmware/bootloader и минимальным ядром NUN.** Его задача — проверить образ, сформировать контекст загрузки и передать управление через заданную точку входа.

## Предполагаемые функции

- Проверка формата и версии kernel image.
- Проверка memory map, alignment и допустимых диапазонов адресов.
- Проверка необходимых platform capabilities.
- Формирование версионированного boot-information block.
- Передача управления через architecture entry point.
- Явная фиксация ошибок до handoff без скрытого перехода в неизвестный режим.

## Текущий статус

<badge>SPECIFICATION ONLY</badge>

В репозитории пока нет launcher-кода, bootloader, конфигурации образа, build pipeline, тестов или bootable binary. Ни одна архитектура не подтверждена как поддерживаемая. Elbrus/E2K остаётся архитектурным направлением, а не квалифицированной платформой.

## Контракт NUN

[NUN](https://github.com/bboroski/NUN) должен самостоятельно проверить handoff-контекст до инициализации. NUNLauncher отвечает только за проверку образа, формирование boot-information block и контролируемую передачу управления. Управление памятью, планирование и драйверы относятся к ядру.

## План реализации

| Этап | Доказательство готовности |
|---|---|
| Image contract | Определены формат, версия, checksum и совместимость |
| Memory validation | Проходят boundary, alignment и address-range тесты |
| Capability gate | Несовместимая платформа отклоняется до handoff |
| Boot information | Handoff-структура версионирована и проверяется ядром |
| Handoff | Entry point проверен в эмуляторе и на целевом оборудовании |

Ошибки до handoff должны фиксироваться явно. Неизвестный режим нельзя запускать скрытно. Host-тесты не доказывают реальную загрузку на процессоре или плате.

## Экосистема и лицензия

Связанный kernel-проект: [NUN](https://github.com/bboroski/NUN). Инженерные компоненты экосистемы: [Kloz](https://github.com/bboroski/Kloz), [CO](https://github.com/bboroski/CO), [SaintSI](https://github.com/bboroski/SaintSI) и [Klipot](https://github.com/bboroski/Klipot).

Файл <code>LICENSE</code> отсутствует; лицензия не объявлена. Определите условия до публичного распространения.

---

**Создано с любовью к системной инженерии и людям.**