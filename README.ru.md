<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="Duck mark" width="104" /></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Duck Black/Yellow · Android Home Application</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-ANDROID%20APP%20FOUNDATION-FFD600?style=for-the-badge&labelColor=111111" /><img src="https://img.shields.io/badge/PLATFORM-ANDROID%2026%2B-FFD600?style=for-the-badge&labelColor=111111" /></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>
<hr />

**NUNLauncher — домашний экран Android и пользовательская оболочка экосистемы NUN.**

## Назначение

NUNLauncher — обычное Android-приложение, которое пользователь может выбрать в качестве Home-приложения по умолчанию. Оно показывает доступные Android-приложения, позволяет искать и запускать их, а затем будет расширяться системными панелями и интеграцией с NUN OS.

Это **не** UEFI-загрузчик, не fastboot/recovery, не root-инструмент и не механизм прошивки. На обычном Android устройстве приложение не может самостоятельно заменить boot chain или записывать защищённые разделы.

## Что уже реализовано в исходной основе

- Android Gradle project: Java 17, SDK 26+, Gradle 9.9.0 и Android Gradle Plugin 9.4.1.
- HOME/DEFAULT и LAUNCHER intent filters.
- Локальное обнаружение Activity с категорией LAUNCHER.
- Поиск по названию приложения и package name.
- Запуск выбранного приложения через явный ComponentName.
- Кнопка перехода в Android Settings.
- Duck Black/Yellow palette, графитовые панели и жёлтые акценты.
- Unit-тесты локальной фильтрации и GitHub Actions workflow для сборки APK.

Список пакетов ограничен видимостью Android launcher-активностей. Не запрашиваются `QUERY_ALL_PACKAGES`, root, firmware-write или сетевые разрешения.

## Быстрый запуск

Требуются JDK 17, Android SDK Platform 36 и Gradle 9.9.0:

```bash
gradle :app:testDebugUnitTest :app:assembleDebug
```

Debug APK после успешной сборки появится в:

```text
app/build/outputs/apk/debug/app-debug.apk
```

Сборку и запуск на физическом устройстве ещё необходимо подтвердить CI и device smoke tests. Добавленный workflow выполняет JVM unit tests и собирает debug APK.

## Архитектурные границы

- **NUN OS** — самостоятельная bare-metal ОС, ядро и системный ABI.
- **NUNLauncher** — Android Home-приложение.
- **NUN System Bridge** — будущий ограниченный Binder-клиент для NUN Android distribution; пока это только контракт, не работающий системный сервис.

NUNLauncher остаётся полноценным Android Home даже без NUN-службы. При отсутствии моста любые NUN-специфичные элементы должны показывать состояние «недоступно», а не имитировать системный доступ.

## Дальнейшие направления

Адаптивные рабочие пространства, dock, папки, виджеты, быстрые настройки, доступность, расширенная навигация с клавиатуры/D-pad и безопасная интеграция с NUN сервисами будут добавляться отдельными этапами с тестами.

## Документация

- [Android Home architecture](docs/ANDROID_HOME_ARCHITECTURE.md)
- [NUN system bridge contract](docs/NUN_SYSTEM_BRIDGE.md)
- [Duck UI system](docs/DUCK_UI_SYSTEM.md)
- [NUN OS repository](https://github.com/bboroski/NUN)
