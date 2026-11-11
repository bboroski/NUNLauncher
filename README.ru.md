<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-HOST%20VALIDATOR-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/IMPLEMENTATION-IMAGE%20CHECKER-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/LICENSE-NOT%20DECLARED-FFD600?style=for-the-badge&labelColor=111111"></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>

---

**NUNLauncher определяет будущую узкую границу между firmware/bootloader и минимальным ядром NUN.** Его задача — проверить образ, сформировать boot-контекст и передать управление через заданную точку входа.

## Предполагаемые функции

- Проверка формата и версии kernel image.
- Проверка memory map, alignment и допустимых диапазонов адресов.
- Проверка необходимых platform capabilities.
- Формирование версионированного boot-information block.
- Передача управления через architecture entry point.
- Явная фиксация ошибок до handoff без скрытого перехода в неизвестный режим.

## Текущий статус

В репозитории появился host-side validator (`tools/validate_boot_image.py`) и unit tests для проверки JSON-manifest, SHA-256, безопасного пути к образу, границ памяти и выравнивания entry point. Это **не bootloader**: architecture-specific startup/loader, linker script, полный boot pipeline и bootable binary пока отсутствуют. Ни одна архитектура не квалифицирована; E2K допускается только как экспериментальная цель проверки метаданных.

## Host-side проверка образа

```sh
python tools/validate_boot_image.py path/to/kernel.manifest.json --target x86_64
python -m unittest discover -s tests -p 'test_*.py' -v
```

Validator не загружает и не исполняет код. Проверка manifest/digest не заменяет bootloader, проверку подписи образа, тестирование в эмуляторе или квалификацию на физическом оборудовании.

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

Основной kernel-проект: [NUN](https://github.com/bboroski/NUN). Связанные компоненты: [Kloz](https://github.com/bboroski/Kloz), [CO](https://github.com/bboroski/CO), [SaintSI](https://github.com/bboroski/SaintSI) и [Klipot](https://github.com/bboroski/Klipot).

Файл <code>LICENSE</code> отсутствует; лицензия не объявлена. Определите условия до публичного распространения.

---

**Создано с любовью к системной инженерии и людям.**