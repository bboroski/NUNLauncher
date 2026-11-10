<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="Duck mark" width="104" /></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-C11%20CONTRACT-FFD600?style=for-the-badge&labelColor=111111" /><img src="https://img.shields.io/badge/IMPLEMENTATION-HOST%20METADATA%20SLICE-FFD600?style=for-the-badge&labelColor=111111" /></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>
<hr />

**Спецификационный каркас для контролируемого запуска ядра и передачи проверенного контекста выполнения.**

## Позиционирование

NUNLauncher описывает будущую узкую границу между firmware/bootloader и минимальным ядром NUN. Его задача — проверить входные данные и предсказуемо передать управление, не дублируя подсистемы ядра.

## Предполагаемые обязанности

- Проверка формата и версии kernel image.
- Валидация memory map, alignment и допустимых диапазонов адресов.
- Проверка необходимых platform capabilities.
- Формирование версионируемого boot-information block.
- Передача управления через определённый architecture entry point.
- Фиксация ошибки до handoff без скрытого перехода в неизвестный режим.

## Контракт с ядром

NUNLauncher не должен дублировать kernel memory manager, scheduler или device drivers. Он отвечает за проверку образа, создание handoff-данных и контроль перехода. [NUN](https://github.com/bboroski/NUN) должен самостоятельно проверить полученный контекст до инициализации.

## Текущий статус

**Текущий статус: добавлен host-контракт проверки образа и handoff-метаданных; фактической загрузки пока нет.**

C11 API проверяет magic/version, размер структуры, выбранную архитектуру, диапазон образа, entry offset, карту памяти и согласованность дескриптора образа с handoff-блоком. Добавлены CMake и host-тесты (python one.py check). nunlauncher_handoff_prepare готовит проверенную копию метаданных, но не выполняет CPU jump, не переключает privilege mode и не создаёт bootable binary. Android home-screen profile отделён и остаётся спецификацией.

## Управление проектом и readiness gates

| Этап | Требуемое доказательство |
|---|---|
| Image contract | Формат, версия, checksum и правила совместимости определены |
| Memory validation | Тесты boundary, alignment и address-range проходят |
| Capability gate | Несовместимая платформа отклоняется до handoff |
| Boot information | Handoff-структура версионируется и проверяется ядром |
| Handoff | Architecture entry point проверен в эмуляторе и на целевом устройстве |

ID архитектуры в метаданных не подтверждает её поддержку. Для настоящего handoff нужны architecture-specific assembly entry, platform startup, linker integration, тесты в эмуляторе и квалификация целевой платформы.

## Границы ответственности

NUNLauncher не управляет памятью ядра после передачи управления, не планирует задачи и не реализует драйверы. После handoff ответственность переходит к NUN.

## Экосистема

[SAMBU](https://github.com/bboroski/SAMBU) · [ToolNG](https://github.com/bboroski/ToolNG) · [ReaperCripple](https://github.com/bboroski/ReaperCripple) · [Kloz](https://github.com/bboroski/Kloz) · [CO](https://github.com/bboroski/CO) · [SaintSI](https://github.com/bboroski/SaintSI) · [Klipot](https://github.com/bboroski/Klipot) · [NUN](https://github.com/bboroski/NUN).
