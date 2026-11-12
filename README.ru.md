# NUNLauncher
**Kernel Bootstrap Supervisor**

[Українська](README.md) · [English](README.en.md) · [Русский](README.ru.md)

Host-side supervisor для проверки метаданных kernel image и boot-information contracts перед возможной передачей управления.

## Возможности
- Host validator 48-байтового заголовка kernel image.
- Проверка architecture, entry offset и границ memory map вызывающей стороны.
- Переносимая CMake-сборка, host-тесты и GCC/Clang CI.
- Версионированный boot-information contract и capability-gate design.

## Запуск и проверка
```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build --output-on-failure
python one.py check
make test
```

## Границы
Supervisor не записывает образ на диск и не передаёт управление ядру. QEMU CI охватывает только проверенные профили эмулятора; E2K ELF validation не является bare-metal загрузкой E2K.

## Документация проекта
У этой host-side утилиты нет отдельного GUI. Результаты тестов и CI logs — подходящие наглядные доказательства.

## Связанные проекты
[NUN](https://github.com/bboroski/NUN) · [Fragile-Baby](https://github.com/bboroski/Fragile-Baby)

## Лицензия
Корневой LICENSE не объявлен; перед повторным использованием проверьте условия репозитория.

---
**两心相印，白首不离。**
*Два сердца понимают друг друга и остаются вместе до старости.*
— Традиционное китайское выражение
