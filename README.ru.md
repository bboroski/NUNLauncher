# NUNLauncher — супервизор начальной загрузки ядра

NUNLauncher — слой допуска образов NUN перед передачей управления ядру.
Он проверяет манифест, точный размер и SHA-256, профиль x86_64, BIOS/UEFI
El Torito entries, ELF64 entry point ядра и EFI fallback loader в формате
PE/COFF AMD64.

## Реализовано

- Python CLI и строгая fail-closed проверка манифеста.
- Проверка ISO в режиме чтения через xorriso.
- Проверка ELF64/LE/EM_X86_64/ET_DYN и исполняемого PT_LOAD entry point.
- Проверка PE/COFF AMD64 для EFI/BOOT/BOOTX64.EFI.
- Позитивные и негативные host unit-тесты.
- CI: сборка NUN, проверка образа и запуск в QEMU через BIOS и x86_64 UEFI.

Команды:

    make test
    make verify IMAGE=../NUN/build/NUN.iso MANIFEST=../NUN/build/NUN.image.json

Firmware запускает Limine из гибридного ISO; Limine загружает ядро NUN и
выполняет передачу управления по своему протоколу. NUNLauncher — супервизор
проверки образа, а не замена firmware или второй kernel loader.

Текущий квалифицированный профиль: **только x86_64 с BIOS и x86_64 UEFI**.
Поддержка AArch64, RISC-V, E2K/Эльбрус, Apple Silicon и других архитектур
не заявляется.

Образы пока unsigned. SHA-256 выявляет несовпадение файла и манифеста, но не
подтверждает издателя. Secure Boot trust store, подпись манифеста, ротация
ключей и TPM attestation ещё не реализованы; `--require-signature`
поэтому отклоняет образ.

Подробности: [BOOT_SUPERVISOR.md](docs/BOOT_SUPERVISOR.md).
