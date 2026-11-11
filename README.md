<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-HOST%20VALIDATOR-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/IMPLEMENTATION-NO%20FIRMWARE%20HANDOFF-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/LICENSE-NOT%20DECLARED-FFD600?style=for-the-badge&labelColor=111111"></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>

---

**NUNLauncher визначає майбутню вузьку межу між firmware/bootloader та мінімальним ядром NUN.** Його задача — перевірити образ, сформувати контекст і передати керування через визначений entry point.

## Цільові функції

- Перевірка формату та версії kernel image.
- Валідація memory map, alignment і допустимих діапазонів адрес.
- Перевірка потрібних platform capabilities.
- Формування версійованого boot-information block.
- Передача керування через architecture entry point.
- Явна фіксація помилки до handoff без прихованого переходу в невідомий режим.

## Поточний стан

У репозиторії тепер є переносимий C11 host-side validator для 48-байтового заголовка kernel image, перевірки архітектури, entry offset і меж завантаження в caller-provided memory map. Є CMake build, тести та GCC/Clang CI. Це **ще не bootloader і не bootable binary**: firmware entry, linker scripts, architecture handoff, secure signature chain та emulator/hardware boot tests ще не реалізовані. Ручний E2K workflow лише підготовлений і потребує self-hosted runner з LCC; він не є доказом кваліфікованої підтримки.

## Контракт із NUN

[NUN](https://github.com/bboroski/NUN) має самостійно перевірити handoff-контекст перед ініціалізацією. NUNLauncher відповідає лише за валідацію образу, формування boot-information block і контроль переходу. Менеджер пам'яті, scheduler і драйвери належать ядру.

## План реалізації

| Етап | Доказ готовності |
|---|---|
| Image contract | Формат, версія, checksum і сумісність визначені |
| Memory validation | Boundary, alignment та address-range тести проходять |
| Capability gate | Невідповідна платформа відхиляється до handoff |
| Boot information | Handoff-структура версіонована й перевіряється ядром |
| Handoff | Entry point перевірений в емуляторі та на цільовій платформі |

Помилка до handoff має бути явною. Невідомий режим не повинен запускатися приховано. Host-тести не є доказом реального boot на процесорі або платі.

## Екосистема та ліцензія

Основний kernel-проєкт: [NUN](https://github.com/bboroski/NUN). Суміжні компоненти: [Kloz](https://github.com/bboroski/Kloz), [CO](https://github.com/bboroski/CO), [SaintSI](https://github.com/bboroski/SaintSI) та [Klipot](https://github.com/bboroski/Klipot).

Файл <code>LICENSE</code> відсутній; ліцензію не оголошено. Визначте умови до публічного розповсюдження.

---

**Створено з любов'ю до системної інженерії та людей.**