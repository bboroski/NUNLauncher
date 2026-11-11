"""Read-only ISO inspection using the xorriso command line tool."""

from __future__ import annotations

import re
import shutil
import subprocess
import tempfile
from pathlib import Path

from .manifest import VerificationError


class XorrisoInspector:
    def __init__(self, executable: str = "xorriso", timeout: int = 30) -> None:
        resolved = shutil.which(executable)
        if resolved is None:
            raise VerificationError(f"required tool not found in PATH: {executable}")
        self.executable = resolved
        self.timeout = timeout

    def boot_modes(self, image: Path) -> set[str]:
        try:
            result = subprocess.run(
                [self.executable, "-indev", str(image), "-report_el_torito", "plain"],
                check=True,
                capture_output=True,
                text=True,
                timeout=self.timeout,
            )
        except (OSError, subprocess.SubprocessError) as exc:
            raise VerificationError(f"xorriso could not inspect the image: {exc}") from exc

        lines = [
            line for line in (result.stdout + "\n" + result.stderr).splitlines()
            if "El Torito boot img" in line
        ]
        modes: set[str] = set()
        for line in lines:
            if re.search(r"\bBIOS\b", line, re.IGNORECASE):
                modes.add("bios")
            if re.search(r"\bUEFI\b", line, re.IGNORECASE):
                modes.add("uefi")
        return modes

    def extract(self, image: Path, iso_path: str) -> bytes:
        # Callers accept only fixed, schema-validated paths; extraction is read-only.
        if iso_path not in {"boot/NUN/kernel.elf", "EFI/BOOT/BOOTX64.EFI"}:
            raise VerificationError(f"refusing to extract an unapproved path: {iso_path}")
        with tempfile.TemporaryDirectory(prefix="nunlauncher-") as directory:
            destination = Path(directory) / Path(iso_path).name
            try:
                subprocess.run(
                    [
                        self.executable,
                        "-osirrox", "on",
                        "-indev", str(image),
                        "-extract", f"/{iso_path}", str(destination),
                    ],
                    check=True,
                    capture_output=True,
                    text=True,
                    timeout=self.timeout,
                )
            except (OSError, subprocess.SubprocessError) as exc:
                raise VerificationError(f"xorriso could not extract {iso_path}: {exc}") from exc
            try:
                return destination.read_bytes()
            except OSError as exc:
                raise VerificationError(f"required image member is missing: {iso_path}") from exc
