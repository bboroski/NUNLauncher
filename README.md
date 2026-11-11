<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-SPECIFICATION%20ONLY-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/IMPLEMENTATION-DOCUMENTATION%20SCAFFOLD-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/LICENSE-NOT%20DECLARED-FFD600?style=for-the-badge&labelColor=111111"></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>


<p align="center"><sub><a href="design/DUCK_UI_SYSTEM.md">Duck Black / Yellow UI standard</a> · <a href="assets/duck-ui/duck-ui-preview.svg">Vector UI preview</a></sub></p>

**NUNLauncher описує майбутню вузьку межу між firmware/bootloader та мінімальним ядром NUN.** Його задача — перевірити образ, сформувати контекст і передати керування через визначений entry point.

## Цільові функції

- Перевірка формату та версії kernel image.
- Валідація memory map, alignment і допустимих діапазонів адрес.
- Перевірка потрібних platform capabilities.
- Формування версійованого boot-information block.
- Передача керування через architecture entry point.
- Явна фіксація помилки до handoff без прихованого переходу в невідомий режим.

## Поточний стан

<badge>SPECIFICATION ONLY</badge>

У репозиторії наразі немає launcher-коду, bootloader, конфігурації образу, build pipeline, тестів або bootable binary. Підтримка жодної архітектури не підтверджена. Elbrus/E2K залишається напрямом архітектурної роботи, а не перевіреною платформою.

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

Пов'язаний kernel-проєкт: [NUN](https://github.com/bboroski/NUN). Інженерні компоненти екосистеми: [Kloz](https://github.com/bboroski/Kloz), [CO](https://github.com/bboroski/CO), [SaintSI](https://github.com/bboroski/SaintSI) і [Klipot](https://github.com/bboroski/Klipot).

Файл <code>LICENSE</code> відсутній; ліцензію не оголошено. Визначте умови до публічного розповсюдження.

---

**Створено з любов'ю до системної інженерії та людей.**