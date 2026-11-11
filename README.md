<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="NUNLauncher mark" width="92"></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Android HOME Launcher · NUN Boot-Chain Design</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-ANDROID%20STARTER-FFD600?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/BUILD-VALIDATION%20PENDING-FFB000?style=for-the-badge&labelColor=111111"><img src="https://img.shields.io/badge/LANG-UK%20%7C%20EN%20%7C%20RU-FFD600?style=for-the-badge&labelColor=111111"></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>
<p align="center"><sub><a href="design/DUCK_UI_SYSTEM.md">Duck Black / Yellow UI standard</a> · <a href="assets/duck-ui/duck-ui-preview.svg">Vector UI preview</a></sub></p>

**NUNLauncher — Android HOME-застосунок екосистеми NUN.** Він формує домашній екран, знаходить доступні програми, запускає їх і зберігає обране. Майбутній firmware boot supervisor — окрема ціль збірки, а не Android activity.

## Реалізований стартовий шар

- Android application module на Kotlin з applicationId org.nun.launcher.
- MAIN + HOME + DEFAULT intent filter для ролі домашнього екрана Android.
- Окремий MAIN + LAUNCHER filter для запуску NUNLauncher як звичайного застосунку.
- Виявлення доступних launchable activities через LauncherApps і видимі Android-профілі.
- Локальний пошук, сітка застосунків, запуск програм і локальне обране (довге натискання).
- Чорно-жовта Duck UI палітра, векторна adaptive icon і рядки українською, англійською та російською.
- CI workflow для складання debug APK.

## Межі відповідальності

Android HOME — звичайний застосунок під керуванням Android. Він не замінює Android kernel, firmware, recovery, системний образ або bootloader. Вибір NUNLauncher як HOME виконується користувачем через Android Settings. Підключення до NUN core не імітується: статус явно показує, що ядро не підключено.

## Збірка

Потрібні JDK 17, Android SDK Platform 36 та Gradle 9.6.0. Команда: <code>gradle :app:assembleDebug</code>.

Очікуваний шлях до APK: <code>app/build/outputs/apk/debug/app-debug.apk</code>. Саме існування цього шляху не заявляється, доки збірка не виконана й артефакт не перевірений.

## Перший запуск

1. Зібрати та встановити debug APK на Android 8.0 або новіший.
2. Відкрити Settings → Apps → Default apps → Home app (назви пунктів залежать від виробника).
3. Обрати NUNLauncher.
4. Натиснути Home та перевірити результат системного резолвера.

## Майбутні можливості

Dock, папки, віджети, shortcuts, gestures, notification dots, wallpaper controls, accessibility settings, локальний NUN device dashboard і синхронізація сесій плануються окремо. Кожна функція має використовувати публічні Android API, мінімальні дозволи, версіоноване сховище й зрозумілий offline-стан.

## Окремий firmware boot chain

Специфікація майбутнього завантажувача NUN зберігається окремо в [NUN_BOOT_CHAIN.md](docs/NUN_BOOT_CHAIN.md). Наявність Android HOME-коду не означає наявності UEFI loader, bootable image чи перевіреної архітектури. Ядро NUN незалежно перевірятиме handoff-контекст.

## Документація

- [Android HOME Launcher](docs/ANDROID_HOME_LAUNCHER.md)
- [NUN boot chain separation](docs/NUN_BOOT_CHAIN.md)
- [OS foundation in NUN](https://github.com/bboroski/NUN/blob/main/docs/OS_FOUNDATION.md)

## Ліцензія

У NUNLauncher відсутній файл LICENSE, ліцензія не оголошена. Перед публічним розповсюдженням потрібно визначити умови використання.

---

**NUNLauncher — створено з любов'ю до простих, швидких і зрозумілих інтерфейсів.**
