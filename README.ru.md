# NUN Launcher — Android-интерфейс для NUN

NUN Launcher — настоящее Android-приложение категории HOME: чёрно-жёлтый интерфейс, анимированные плитки, поиск приложений, системная панель и native bridge к runtime NUN.

## Используемый код экосистемы

Полное дерево NUN подключено в core/NUN. Внутри него закреплены все семь проектов:
- SAMBU — язык, интерпретатор, CLI, IDE и инструменты разработчика.
- ToolNG — нативное ядро политик и ограниченных workflow.
- ReaperCripple — bootstrap, rollback, BSP и телеметрия.
- Kloz — платформенный ABI, арена памяти и ресурсные лимиты.
- CO — детерминированный lifecycle runtime.
- SaintSI — наблюдение за health и целостностью.
- Klipot — реестр и планировщик интеграции.

Android NDK собирает нативный runtime NUN в libnun_android_bridge.so. SAMBU и Klipot остаются host-инструментами и полностью доступны в подключённом исходном дереве.

## Сборка

Требуются JDK 17, Gradle 9.6.0+, Android SDK Platform 36, Android NDK 28.2.13676358 и CMake 3.22.1.

Клонируйте репозиторий с recursive submodules и запустите:
- Windows: one.cmd
- Linux/macOS: python3 one.py

APK будет создан в app/build/outputs/apk/debug/app-debug.apk.

## Использование

Установите APK и выберите NUN Launcher в настройках домашнего экрана Android. Приложение показывает программы со стандартным launcher intent и запускает их без запроса QUERY_ALL_PACKAGES или привилегированного доступа к устройству.

## Статус

Это Android-лаунчер и host-интеграция runtime, а не самостоятельный Android OS image и не bare-metal bootloader. Для загрузочного ядра NUN отдельно нужны архитектурный boot path, memory manager, interrupt layer, драйверы и проверка на эмуляторе/оборудовании.
