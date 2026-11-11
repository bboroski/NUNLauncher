<p align="center"><img src="assets/duck-ui/duck-mark.svg" alt="Duck mark" width="104" /></p>
<h1 align="center">NUNLauncher</h1>
<p align="center"><strong>Kernel Bootstrap Supervisor</strong></p>
<p align="center"><img src="https://img.shields.io/badge/STATUS-BOOT%20SUPERVISOR%20IMPLEMENTED-FFD600?style=for-the-badge&labelColor=111111" /></p>
<p align="center"><a href="README.md">🇺🇦 Українська</a> · <a href="README.en.md">🇬🇧 English</a> · <a href="README.ru.md">🇷🇺 Русский</a></p>
<hr />

**NUNLauncher — контрольний шар допуску завантажувальних образів NUN до boot handoff.**

## Реалізовано

- CLI-перевірка маніфесту, імені файлу, розміру та SHA-256.
- Fail-closed перевірка цільової архітектури x86_64 та Limine base revision 6.
- Перевірка BIOS та UEFI El Torito boot entries через xorriso.
- Перевірка kernel ELF64/LE/EM_X86_64/ET_DYN, меж program headers та executable entry point.
- Перевірка EFI fallback loader як PE/COFF AMD64.
- Unit-тести позитивних та негативних сценаріїв.
- CI, який збирає NUN ISO, запускає валідатор і перевіряє boot у QEMU через BIOS та x86_64 UEFI.

## Команда

    make test
    make verify IMAGE=../NUN/build/NUN.iso MANIFEST=../NUN/build/NUN.image.json

Або:

    PYTHONPATH=. python3 -m nunlauncher_boot.cli verify \
      --image ../NUN/build/NUN.iso \
      --manifest ../NUN/build/NUN.image.json

Для машинозчитуваного результату додайте `--json`.

## Архітектурна межа

Firmware запускає Limine із гібридного ISO. Limine завантажує ELF64 PIE-ядро NUN і передає йому керування за протоколом Limine. NUNLauncher виконує попередню перевірку образу; він не дублює firmware, не є Android HOME activity і не підміняє власний kernel-entry код.

Підтримка, перевірена QEMU CI-профілем: **x86_64 + BIOS та x86_64 UEFI**. Додано окремий preflight-контракт для E2K Linux userspace: `verify-e2k` перевіряє ELF64 little-endian з `EM_MCST_ELBRUS=175`, але не виконує binary. Нативний E2K runtime NUN запускається через Elbrus Linux після PNS; окрема E2K-native workflow перевіряє виконання на runner `linux,e2k`. AArch64, RISC-V та Apple Silicon не заявлені.

## Модель довіри

Поточні образи позначаються як **unsigned**. SHA-256 виявляє невідповідність файлу маніфесту, але не доводить особу видавця. Secure Boot, довірений ключ видавця, підпис маніфесту, ротація ключів і TPM attestation ще не реалізовані. Параметр `--require-signature` тому завершується відмовою, а не послабленням перевірки.

## Документація

- [Boot supervisor: pipeline, format checks and trust model](docs/BOOT_SUPERVISOR.md)
- [Accepted NUN image manifest schema](spec/nun-image-manifest.schema.json)
- [NUN E2K runtime manifest schema](spec/nun-e2k-runtime-manifest.schema.json)
- [E2K runtime admission](docs/E2K_RUNTIME.md)
- [NUN kernel and boot image](https://github.com/bboroski/NUN)
- [Limine bootloader](https://github.com/Limine-Bootloader/Limine)

NUNLauncher не реалізує планувальник, драйвери чи пам'ять ядра після handoff. Ці обов'язки належать NUN.


## Екосистема та ліцензія

Основний kernel-проєкт: [NUN](https://github.com/bboroski/NUN). Суміжні компоненти: [Kloz](https://github.com/bboroski/Kloz), [CO](https://github.com/bboroski/CO), [SaintSI](https://github.com/bboroski/SaintSI) та [Klipot](https://github.com/bboroski/Klipot).

Файл `LICENSE` відсутній; ліцензію не оголошено. Визначте умови до публічного розповсюдження.
