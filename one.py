#!/usr/bin/env python3
"""Build NUN Launcher and the native NUN runtime through one command."""

from __future__ import annotations
import os
import shutil
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent


def run(command: list[str]) -> None:
    print("+", " ".join(command), flush=True)
    subprocess.run(command, cwd=ROOT, check=True)


def main() -> int:
    git = shutil.which("git")
    if git is None:
        raise SystemExit("Git is required for the NUN source submodule.")
    if not (ROOT / "core" / "NUN" / "CMakeLists.txt").is_file():
        run([git, "submodule", "update", "--init", "--recursive"])
    if not (ROOT / "core" / "NUN" / "modules" / "SAMBU" / "sambu.module.json").is_file():
        raise SystemExit("Nested NUN modules are missing. Run git submodule update --init --recursive.")

    if not (os.environ.get("ANDROID_HOME") or os.environ.get("ANDROID_SDK_ROOT")):
        raise SystemExit("Set ANDROID_HOME or ANDROID_SDK_ROOT to your Android SDK directory.")
    gradle = shutil.which("gradle")
    if gradle is None:
        raise SystemExit("Gradle 9.6.0+ is required. Android Gradle Plugin 9.4 requires Gradle 9.6.")
    run([gradle, "--no-daemon", "--stacktrace", ":app:assembleDebug"])

    apk = ROOT / "app" / "build" / "outputs" / "apk" / "debug" / "app-debug.apk"
    if not apk.is_file():
        raise SystemExit("Gradle completed but the debug APK was not found.")
    print(f"APK: {apk}")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except subprocess.CalledProcessError as exc:
        raise SystemExit(exc.returncode)
