<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="Duck mark" width="104" /></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Duck Black/Yellow · Android Home Application</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-ANDROID%20APP%20FOUNDATION-FFD600?style=for-the-badge&labelColor=111111" /><img src="https://img.shields.io/badge/PLATFORM-ANDROID%2026%2B-FFD600?style=for-the-badge&labelColor=111111" /></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>
<hr />

**NUNLauncher — Android Home Launcher і користувацька оболонка екосистеми NUN.**

## Призначення

NUNLauncher — звичайний Android-додаток, який користувач може вибрати як Home за замовчуванням. Він показує доступні Android-застосунки, дозволяє шукати й запускати їх, а згодом отримає системні панелі та інтеграцію з NUN OS.

Це **не** UEFI-завантажувач, fastboot/recovery, root-інструмент або механізм прошивки. Звичайний Android-додаток не може самостійно замінити boot chain чи записувати захищені розділи.

## Що вже реалізовано в основі застосунку

- Android Gradle project: Java 17, SDK 26+, Gradle 9.9.0 та Android Gradle Plugin 9.4.1.
- HOME/DEFAULT і LAUNCHER intent filters.
- Локальне виявлення Activity з категорією LAUNCHER.
- Пошук за назвою застосунку та package name.
- Запуск вибраного застосунку через явний ComponentName.
- Кнопка переходу до Android Settings.
- Duck Black/Yellow palette, графітові панелі та жовті акценти.
- Unit-тести локальної фільтрації та GitHub Actions workflow для збирання APK.

Список пакетів обмежений видимістю Android launcher-активностей. Дозволи `QUERY_ALL_PACKAGES`, root, firmware-write і мережеві дозволи не запитуються.

## Швидкий запуск

Потрібні JDK 17, Android SDK Platform 36 і Gradle 9.9.0:

```bash
gradle :app:testDebugUnitTest :app:assembleDebug
```

Debug APK після успішної збірки з’явиться тут:

```text
app/build/outputs/apk/debug/app-debug.apk
```

Збірку й запуск на фізичному пристрої ще мають підтвердити CI та device smoke tests. Доданий workflow запускає JVM unit tests і збирає debug APK.

## Архітектурні межі

- **NUN OS** — самостійна bare-metal ОС, ядро й системний ABI.
- **NUNLauncher** — Android Home-додаток.
- **NUN System Bridge** — майбутній обмежений Binder-клієнт для NUN Android distribution; наразі це контракт, а не робочий системний сервіс.

NUNLauncher має залишатися повноцінним Android Home і без NUN-служби. За відсутності bridge системні NUN-функції мають показувати стан «недоступно», а не імітувати системний доступ.

## Наступні напрями

Адаптивні робочі простори, dock, папки, віджети, швидкі налаштування, доступність, клавіатурна/D-pad-навігація та безпечна інтеграція з NUN-сервісами додаватимуться окремими етапами з тестами.

## Документація

- [Android Home architecture](docs/ANDROID_HOME_ARCHITECTURE.md)
- [NUN system bridge contract](docs/NUN_SYSTEM_BRIDGE.md)
- [Duck UI system](docs/DUCK_UI_SYSTEM.md)
- [NUN OS repository](https://github.com/bboroski/NUN)
