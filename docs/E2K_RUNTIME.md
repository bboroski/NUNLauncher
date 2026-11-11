# NUNLauncher E2K runtime admission

NUNLauncher has a separate `verify-e2k` path for the NUN native E2K Linux
userspace runtime. It checks the exact file size and SHA-256 digest, ELF64
little-endian class, `EM_MCST_ELBRUS=175`, executable entry-point range,
compiler metadata and the explicit PNS-to-Elbrus-Linux userspace deployment
contract.

Build the runtime in NUN with the vendor LCC toolchain, then run:

    PYTHONPATH=. python3 -m nunlauncher_boot.cli verify-e2k \
      --binary ../NUN/build/e2k/NUN-E2K-host \
      --manifest ../NUN/build/e2k/NUN-E2K-host.json

This is **preflight validation of an E2K Linux process**, not a firmware
bootloader, system image or bare-metal kernel handoff. The command does not
execute the binary. Native execution proof comes from the manually dispatched
E2K runner workflow (or a documented physical-hardware run).

The original `verify` command remains the x86_64 hybrid ISO / Limine verifier.
It is not reused or relabelled for E2K.
