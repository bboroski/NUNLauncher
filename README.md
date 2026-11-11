<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-PORTABLE%20C11%20HOST%20VALIDATOR%20%C2%B7%20NO%20FIRMWARE%20HANDOFF-FFD600?style=for-the-badge&labelColor=111111" alt="Portable C11 host validator · no firmware handoff"></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>

> **NUNLauncher** — NUNLauncher визначає вузьку межу між firmware/bootloader та ядром NUN і перевіряє kernel-image metadata перед майбутнім handoff.

## Поточні можливості
- Host-side validator 48-byte kernel-image header.
- Перевірка architecture, entry offset і меж завантаження в caller-provided memory map.
- Portable CMake build, host tests та GCC/Clang CI.
- Versioned boot-information contract і capability-gate design.
- Окремі readiness gates для emulator та hardware handoff.

## Швидкий старт і перевірка
```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build --output-on-failure
python one.py check
```

## Архітектура, межі та достовірність
Це ще не bootloader і не bootable binary: firmware entry, linker scripts, architecture handoff, secure signature chain та реальний emulator/hardware boot не реалізовані. Host validation не доводить запуск ОС.

## Інженерна якість
- Перевіряйте збірку й тести саме для цільової платформи; host CI не замінює hardware qualification.
- Статуси **implemented**, **partial**, **planned** мають підтверджуватися кодом, тестами та документацією.
- Не оголошуйте функцію production-ready лише на підставі макета, специфікації чи відкритого Pull Request.
- Зовнішні інтеграції мають бути явними, версіонованими, ізольованими й перевірюваними.

## NUN Mesh та пов'язані репозиторії
[SAMBU](https://github.com/bboroski/SAMBU) · [ToolNG](https://github.com/bboroski/ToolNG) · [ReaperCripple](https://github.com/bboroski/ReaperCripple) · [Kloz](https://github.com/bboroski/Kloz) · [CO](https://github.com/bboroski/CO) · [SaintSI](https://github.com/bboroski/SaintSI) · [Klipot](https://github.com/bboroski/Klipot) · [NUN](https://github.com/bboroski/NUN) · [NUNLauncher](https://github.com/bboroski/NUNLauncher) · [Fragile-Baby](https://github.com/bboroski/Fragile-Baby) · [Luci](https://github.com/bboroski/Luci)

Репозиторії зберігають власні назви, код, API, цикли випуску та межі відповідальності. Інтеграція означає узгоджені контракти, а не автоматичне злиття незалежних runtime.

## Дизайн і документація
Базовий інтерфейсний напрям екосистеми — **Duck Black / Yellow**: чорні поверхні, жовтий акцент для дій і станів, читабельна типографіка, доступний контраст, клавіатурна навігація та підтримка reduced motion. Деталі дивіться в [design/DUCK_UI_SYSTEM.md](design/DUCK_UI_SYSTEM.md), якщо файл присутній у цьому репозиторії.

## Ліцензія
Ліцензію не оголошено; файл LICENSE відсутній.

---
**Створено з любов'ю до інженерії, системності та людей.**


## Майбутній GUI та внесок

NUNLauncher поки описує bootstrap/handoff-контракти, а не завершений GUI або перевірену реалізацію bootloader. Вимоги до майбутнього host-інструмента та межі доказів: [GUI Design Standard](docs/gui/GUI_DESIGN_STANDARD.md). Макетні статуси не є результатом boot-перевірки. Шаблон PR: [.github/PULL_REQUEST_TEMPLATE.md](.github/PULL_REQUEST_TEMPLATE.md).
