<!-- Title: NUNLauncher — C11 kernel image boot contract validator -->
<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="Логотип NUNLauncher" width="76"></p>

# NUNLauncher

> **Visual identity:** Duck Black / Yellow — accent `#FFD600`, primary ink `#111111`.

[![CI](https://github.com/bboroski/NUNLauncher/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/bboroski/NUNLauncher/actions/workflows/ci.yml)

**NUNLauncher** — невелика portable C11-бібліотека для перевірки метаданих kernel image та caller-provided memory map перед майбутньою інтеграцією boot handoff із ядром NUN.

<sub>Оновлено: 2026-11-11 · Версія CMake: 0.2.0 · Мова: C11</sub>

> **TL;DR:** зберіть контрактну бібліотеку через CMake та запустіть CTest. NUNLauncher зараз є host-side validator, а не bootloader і не firmware, що запускає ядро.

[Українська](README.md) · [English](README.en.md) · [Русский](README.ru.md)

## Зміст

- [Призначення](#призначення)
- [Можливості](#можливості)
- [Вимоги та швидкий старт](#вимоги-та-швидкий-старт)
- [Контракт образу](#контракт-образу)
- [Перевірка та обмеження](#перевірка-та-обмеження)
- [Документація та пов'язані проєкти](#документація-та-повязані-проєкти)
- [FAQ](#faq)
- [Підтримка та ліцензія](#підтримка-та-ліцензія)

## Призначення

NUNLauncher формалізує одну вузьку межу: чи відповідають поля kernel-image header заданому контракту та чи вкладається описаний image у memory map, який передав викликач.

## Можливості

- Host-side перевірка 48-byte kernel-image header.
- Перевірка architecture identifier та entry offset.
- Перевірка меж завантаження у caller-provided memory map.
- Versioned boot-information contract.
- Окремі readiness gates для майбутнього emulator та hardware handoff.

## Вимоги та швидкий старт

Потрібні CMake **3.20+** і C11-компілятор GCC, Clang або MSVC.

~~~bash
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build --config Release
ctest --test-dir build --build-config Release --output-on-failure
~~~

## Контракт образу

Специфікація полів, розмірів і перевірок описана в [BOOT_IMAGE_CONTRACT.md](docs/BOOT_IMAGE_CONTRACT.md). Реалізація контракту міститься в [src](src), публічні заголовки — в [include](include), тести — в [tests/test_boot_contract.c](tests/test_boot_contract.c).

Значення architecture, alignment, entry offset та меж пам'яті потрібно брати з відповідної версії контракту, а не виводити з властивостей іншої ISA.

## Перевірка та обмеження

- CTest перевіряє host-side contract library.
- Проєкт не містить підтвердженого firmware entry point або повного bootloader.
- Не заявлено завершений secure signature chain, architecture-specific handoff або успішне завантаження NUN на emulator/physical target.
- Host validation не доводить, що kernel image можна безпечно виконати на реальній системі.

Перевірки автоматизації: [CI workflow](.github/workflows/ci.yml), [Duck quality gate](.github/workflows/duck-quality-gate.yml) та [E2K workflow](.github/workflows/e2k-native.yml).

## Документація та пов'язані проєкти

- [Boot image contract](docs/BOOT_IMAGE_CONTRACT.md)
- [Ecosystem integration](docs/ECOSYSTEM_INTEGRATION.md)
- [Engineering governance](docs/ENGINEERING_GOVERNANCE.md)
- [NUN](https://github.com/bboroski/NUN) — системні архітектурні контракти та host runtime composition.

## FAQ

**Чи запускає NUNLauncher ядро?** Ні. Поточний код перевіряє метадані образу; реального firmware handoff немає.

**Чи достатньо успішного CTest для boot qualification?** Ні. Потрібні architecture-specific firmware, emulator та hardware докази.

## Підтримка та ліцензія

Проблеми: [Issue tracker](https://github.com/bboroski/NUNLauncher/issues) · [bug report](https://github.com/bboroski/NUNLauncher/issues/new). У PR додавайте тест на boundary/error case та посилання на змінену версію boot contract. Профіль супроводу: [bboroski](https://github.com/bboroski).

**Ліцензія не оголошена:** файл LICENSE відсутній. Узгодьте умови використання з правовласником.
