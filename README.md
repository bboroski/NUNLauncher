# NUNLauncher
**Перевірка kernel image та контрольований boot handoff.**

Host-side supervisor для перевірки образів ядра й versioned boot-information contract перед передачею керування.

## Можливості
- Перевірка 48-byte kernel-image header.
- Перевірка architecture, entry offset і меж caller-provided memory map.
- Portable CMake build, host tests та GCC/Clang CI.
- Versioned boot-information contract і capability-gate design.
- Окремі readiness gates для emulator та hardware handoff.

## Запуск і перевірка
```bash
cmake -S . -B build
cmake --build build --config Release
ctest --test-dir build --output-on-failure
python one.py check
make test
```

## Додатково
Перевірка NUN image:

```bash
make verify IMAGE=../NUN/build/NUN.iso MANIFEST=../NUN/build/NUN.image.json
```

Read-only перевірка manifest/schema, розміру й SHA-256 ISO, BIOS/UEFI entries та структури kernel/EFI loader.

Це host-side інструмент без окремого GUI; результати перевірки та CI logs є основними наочними доказами.

## Межі
Supervisor не записує образ на диск і не передає керування ядру. QEMU CI підтверджує лише перевірені emulator-профілі, а не фізичну апаратну кваліфікацію. E2K ELF-перевірка не означає bare-metal E2K boot.

## Пов'язані проєкти
[NUN](https://github.com/bboroski/NUN) · [Fragile-Baby](https://github.com/bboroski/Fragile-Baby)

## Ліцензія
Ліцензію не оголошено.

---
**「两心相印，白首不离。」**
*«Два серця розуміють одне одного й не розлучаються до сивини».*
— Традиційний китайський вислів