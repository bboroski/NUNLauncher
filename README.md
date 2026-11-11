<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-PORTABLE%20C11%20HOST%20VALIDATOR%20%C2%B7%20NO%20FIRMWARE%20HANDOFF-FFD600?style=for-the-badge&labelColor=111111" alt="Portable C11 host validator · no firmware handoff"></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>
<sub>Оновлено: 2026-11-11</sub>


## Навігація

- [Поточні можливості](#поточні-можливості)
- [Швидкий старт і перевірка](#швидкий-старт-і-перевірка)
- [Архітектура, межі та достовірність](#архітектура-межі-та-достовірність)
- [Інтерфейс і візуальні матеріали](#інтерфейс-і-візуальні-матеріали)
- [Інженерна якість](#інженерна-якість)
- [Карта документації](#карта-документації)
- [NUN Mesh та пов'язані репозиторії](#nun-mesh-та-повязані-репозиторії)
- [Дизайн і документація](#дизайн-і-документація)
- [Ліцензія](#ліцензія)

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

## Інтерфейс і візуальні матеріали

NUNLauncher — host-side C11 validator для boot metadata, а не графічний застосунок. Скриншотів GUI немає; зображення нижче показує лише спільний стиль документації Duck Black / Yellow.

## Перевірка образу на host

`tools/validate_boot_image.py` перевіряє JSON-manifest, SHA-256, безпечний шлях до образу, межі пам’яті та вирівнювання entry point. Це перевірка метаданих, а не bootloader і не підтвердження підтримки архітектури.

```sh
python tools/validate_boot_image.py path/to/kernel.manifest.json --target x86_64
python -m unittest discover -s tests -p 'test_*.py' -v
```

Validator не завантажує й не виконує код. Перевірка підпису, запуск в емуляторі та кваліфікація на фізичному обладнанні залишаються окремими етапами.

<p align="center"><img src="assets/duck-ui/duck-ui-preview.svg" alt="Спільний дизайн Duck Black Yellow; не інтерфейс NUNLauncher" width="960"></p>

## Інженерна якість
- Перевіряйте збірку й тести саме для цільової платформи; host CI не замінює hardware qualification.
- Статуси **implemented**, **partial**, **planned** мають підтверджуватися кодом, тестами та документацією.
- Не оголошуйте функцію production-ready лише на підставі макета, специфікації чи відкритого Pull Request.
- Зовнішні інтеграції мають бути явними, версіонованими, ізольованими й перевірюваними.

## Карта документації

- [Контракт образу ядра](docs/BOOT_IMAGE_CONTRACT.md)
- [Інтеграція екосистеми](docs/ECOSYSTEM_INTEGRATION.md)
- [Інженерне врядування](docs/ENGINEERING_GOVERNANCE.md)

## NUN Mesh та пов'язані репозиторії
[SAMBU](https://github.com/bboroski/SAMBU) · [ToolNG](https://github.com/bboroski/ToolNG) · [ReaperCripple](https://github.com/bboroski/ReaperCripple) · [Kloz](https://github.com/bboroski/Kloz) · [CO](https://github.com/bboroski/CO) · [SaintSI](https://github.com/bboroski/SaintSI) · [Klipot](https://github.com/bboroski/Klipot) · [NUN](https://github.com/bboroski/NUN) · [NUNLauncher](https://github.com/bboroski/NUNLauncher)

Репозиторії зберігають власні назви, код, API, цикли випуску та межі відповідальності. Інтеграція означає узгоджені контракти, а не автоматичне злиття незалежних runtime.

## Дизайн і документація
Базовий інтерфейсний напрям екосистеми — **Duck Black / Yellow**: чорні поверхні, жовтий акцент для дій і станів, читабельна типографіка, доступний контраст, клавіатурна навігація та підтримка reduced motion. Деталі дивіться в [design/DUCK_UI_SYSTEM.md](design/DUCK_UI_SYSTEM.md), якщо файл присутній у цьому репозиторії.

## Ліцензія
Ліцензію не оголошено; файл LICENSE відсутній.

---
**Створено з любов'ю до інженерії, системності та людей.**

## Федерація Hellduck

Цей репозиторій входить до федерації [Hellduck](https://github.com/bboroski/Hellduck) — спільного каталогу проєктів, контрактних зв’язків і станів інтеграції. Вихідний код, API, тести та цикл випуску залишаються відповідальністю цього репозиторію; Hellduck індексує зв’язки й не завантажує та не виконує код учасників. Практичні сценарії взаємодії, типи адаптерів і межі сумісності: [Integration Playbook](https://github.com/bboroski/Hellduck/blob/main/docs/INTEGRATION_PLAYBOOK.md).
