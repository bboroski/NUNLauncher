# NUNLauncher — Kernel Bootstrap Supervisor

**Специфікаційний каркас для контрольованого запуску системного ядра.**

Поточний стан: **заготовка документації**. У репозиторії поки немає launcher-коду, bootloader, конфігурації образу, build pipeline чи тестів. NUNLauncher описує майбутній вузький контракт між firmware/bootloader і мінімальним ядром MKS.

[Українська](README.md) · [English](README.en.md) · [Русский](README.ru.md) · [Репозиторій](https://github.com/bboroski/NUNLauncher)

## Призначення

Майбутній bootstrap supervisor має:
- перевіряти формат і версію kernel image;
- валідувати memory map, alignment і допустимі діапазони адрес;
- перевіряти необхідні platform capabilities;
- формувати версійований boot-information block;
- передавати керування через визначений architecture entry point;
- фіксувати помилку до handoff без прихованого fallback у невідомий режим.

## Контракт із ядром

NUNLauncher не повинен дублювати kernel memory manager, scheduler чи device drivers. Він відповідає лише за валідацію образу, формування handoff-даних і контроль переходу. Ядро самостійно перевіряє отриманий контекст перед ініціалізацією.

## Поточна перевірка

Реалізації, збірки та тестів немає. До появи коду не існує bootable binary або перевіреної підтримки архітектур.

## Технічна ідентичність

**NUNLauncher** — канонічне ім'я майбутнього bootstrap supervisor. GitHub-адреса збережена для сумісності посилань.

## Статус

Не використовувати як завантажувач: реалізованого bootloader/launcher тут ще немає.


## UI foundation

The repository includes a standalone, mobile-first NUNLauncher preview in `ui/`. Open `ui/index.html` in a modern browser. The mobile experience uses its own midnight indigo, lavender, atmospheric cyan and soft coral palette with an app library and quick-control surfaces. It is deliberately distinct from the NUN engineering desktop.

This remains a web UI prototype, not a native Android APK or bootloader. Shortcut previews and quick-control switches do not launch installed apps or change actual device settings.
