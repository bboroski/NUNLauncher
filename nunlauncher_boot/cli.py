"""Command-line interface for NUNLauncher boot-image verification."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

from .manifest import VerificationError
from .e2k import verify_e2k_runtime
from .verify import verify_image


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(prog="nunlauncher-boot")
    subparsers = parser.add_subparsers(dest="command", required=True)
    verify = subparsers.add_parser("verify", help="validate a NUN ISO before boot handoff")
    verify.add_argument("--image", required=True, type=Path)
    verify.add_argument("--manifest", required=True, type=Path)
    verify.add_argument("--architecture", default="x86_64")
    verify.add_argument("--require-signature", action="store_true")
    verify.add_argument("--json", action="store_true", help="print a machine-readable result")
    verify_e2k = subparsers.add_parser("verify-e2k", help="validate a native NUN E2K Linux runtime")
    verify_e2k.add_argument("--binary", required=True, type=Path)
    verify_e2k.add_argument("--manifest", required=True, type=Path)
    verify_e2k.add_argument("--require-signature", action="store_true")
    verify_e2k.add_argument("--json", action="store_true", help="print a machine-readable result")

    args = parser.parse_args(argv)

    try:
        if args.command == "verify-e2k":
            result = verify_e2k_runtime(
                args.binary,
                args.manifest,
                require_signature=args.require_signature,
            )
            if args.json:
                print(json.dumps(result, indent=2, sort_keys=True))
            else:
                print("NUNLauncher: E2K ELF STRUCTURE ACCEPTED")
                print(f"Binary:       {result['binary']}")
                print(f"SHA-256:      {result['sha256']}")
                print("Target:       E2K / Linux userspace")
                print(f"ELF machine:  {result['elf_machine']} (EM_MCST_ELBRUS)")
                print(f"Entry point:  {result['entry_point']}")
                print("Boot status:  not tested by preflight")
                print("Trust:        unsigned; digest is not publisher authentication")
            return 0

        result = verify_image(
            args.image,
            args.manifest,
            architecture=args.architecture,
            require_signature=args.require_signature,
        )
    except VerificationError as exc:
        print(f"NUNLauncher: REJECTED: {exc}", file=sys.stderr)
        return 2

    if args.json:
        print(json.dumps(result, indent=2, sort_keys=True))
    else:
        print("NUNLauncher: IMAGE ACCEPTED FOR THE VERIFIED x86_64 PROFILE")
        print(f"Image:       {result['image']}")
        print(f"SHA-256:     {result['sha256']}")
        print("Firmware:    BIOS + x86_64 UEFI")
        print(f"Bootloader:  Limine {result['limine_version']}")
        print(f"Kernel entry:{result['kernel_entry_point']}")
        print("Trust:       unsigned; digest is not publisher authentication")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
