<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="Duck mark" width="104" /></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-C11%20CONTRACT-FFD600?style=for-the-badge&labelColor=111111" /><img src="https://img.shields.io/badge/IMPLEMENTATION-HOST%20METADATA%20SLICE-FFD600?style=for-the-badge&labelColor=111111" /></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>
<hr />

**Специфікаційний каркас для контрольованого запуску ядра та передачі перевіреного контексту.**

## Продуктова роль

NUNLauncher описує майбутню вузьку межу між firmware/bootloader і мінімальним ядром NUN. Завдання шару — перевірити вхідні дані та передати керування передбачуваним способом, не дублюючи внутрішні підсистеми ядра.

## Передбачені обов'язки

- Перевірка формату й версії kernel image.
- Валідація memory map, alignment і допустимих діапазонів адрес.
- Перевірка необхідних platform capabilities.
- Формування версійованого boot-information block.
- Передача керування через визначений architecture entry point.
- Фіксація помилки до handoff без прихованого fallback у невідомий режим.

## Контракт із ядром

NUNLauncher не повинен дублювати kernel memory manager, scheduler чи device drivers. Він відповідає за валідацію образу, формування handoff-даних і контроль переходу. [NUN](https://github.com/bboroski/NUN) має самостійно перевірити отриманий контекст перед ініціалізацією.

## Поточний стан

**Поточний стан: додано host-контракт перевірки образу та handoff-метаданих; фактичного завантаження поки немає.**

C11 API перевіряє magic/version, розмір структури, вибрану архітектуру, діапазон образу, entry offset, карту пам'яті та відповідність образу handoff-блоку. Є CMake і host-тести (python one.py check). nunlauncher_handoff_prepare готує перевірену копію метаданих, але не виконує CPU jump, не перемикає privilege mode і не створює bootable binary. Android home-screen profile відокремлений і залишається специфікацією.

## Управління розробкою та readiness gates

| Етап | Необхідний доказ |
|---|---|
| Image contract | Формат, версія, checksum і правила сумісності визначені |
| Memory validation | Boundary, alignment та address-range тести проходять |
| Capability gate | Невідповідна платформа відхиляється до handoff |
| Boot information | Структура handoff версіонована й перевіряється ядром |
| Handoff | Архітектурний entry point перевірений в емуляторі та на цільовій платформі |

ID архітектури в метаданных не подтверждает её поддержку. Для настоящего handoff нужны architecture-specific assembly entry, platform startup, linker integration, emulator smoke tests и target qualification. Ошибка до передачи управления остаётся явной.

## Межі відповідальності

NUNLauncher не керує пам'яттю ядра після передачі керування, не планує задачі та не реалізує драйвери. Після handoff відповідальність переходить до NUN.

## Екосистема

[SAMBU](https://github.com/bboroski/SAMBU) · [ToolNG](https://github.com/bboroski/ToolNG) · [ReaperCripple](https://github.com/bboroski/ReaperCripple) · [Kloz](https://github.com/bboroski/Kloz) · [CO](https://github.com/bboroski/CO) · [SaintSI](https://github.com/bboroski/SaintSI) · [Klipot](https://github.com/bboroski/Klipot) · [NUN](https://github.com/bboroski/NUN).


## Reaper-inspired engineering integration

- [Project-specific architecture mapping](docs/REAPER_DOMAIN_INTEGRATION.md)
- [Canonical 50-pattern catalogue in SAMBU](https://github.com/bboroski/SAMBU/blob/main/data/reaper/engineering_patterns_50.tsv)
