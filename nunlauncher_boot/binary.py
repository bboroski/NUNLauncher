"""Minimal structural checks for the x86_64 ELF kernel and PE/COFF EFI loader."""

from __future__ import annotations

import struct

from .manifest import VerificationError


def inspect_elf64_x86_64(data: bytes) -> int:
    if len(data) < 64 or data[:4] != b"\x7fELF":
        raise VerificationError("kernel member is not a complete ELF image")
    if data[4] != 2 or data[5] != 1 or data[6] != 1:
        raise VerificationError("kernel must be ELF64 little-endian, version 1")
    (
        e_type, machine, version, entry, phoff, _shoff, _flags,
        ehsize, phentsize, phnum, _shentsize, _shnum, _shstrndx,
    ) = struct.unpack_from("<HHIQQQIHHHHHH", data, 16)
    if (e_type, machine, version) != (3, 62, 1):
        raise VerificationError("kernel must be an x86_64 ET_DYN ELF image")
    if ehsize < 64 or phentsize < 56 or phnum == 0:
        raise VerificationError("kernel ELF program-header table is invalid")
    if phoff > len(data) or phnum > (len(data) - phoff) // phentsize:
        raise VerificationError("kernel ELF program-header table is out of bounds")
    if entry == 0:
        raise VerificationError("kernel entry point is zero")

    executable_entry = False
    for index in range(phnum):
        offset = phoff + index * phentsize
        p_type, p_flags, _p_offset, p_vaddr, _p_paddr, _p_filesz, p_memsz, _p_align = struct.unpack_from(
            "<IIQQQQQQ", data, offset
        )
        if p_type != 1 or not (p_flags & 1) or p_memsz == 0:
            continue
        end = p_vaddr + p_memsz
        if end > (1 << 64):
            raise VerificationError("kernel ELF load segment overflows the address space")
        if p_vaddr <= entry < end:
            executable_entry = True
            break
    if not executable_entry:
        raise VerificationError("kernel entry point is not inside an executable PT_LOAD segment")
    return entry


def inspect_pe_x86_64(data: bytes) -> None:
    if len(data) < 0x40 or data[:2] != b"MZ":
        raise VerificationError("UEFI fallback loader is not a PE/COFF image")
    pe_offset = struct.unpack_from("<I", data, 0x3C)[0]
    if pe_offset > len(data) - 6 or data[pe_offset:pe_offset + 4] != b"PE\0\0":
        raise VerificationError("UEFI fallback loader has an invalid PE signature")
    machine = struct.unpack_from("<H", data, pe_offset + 4)[0]
    if machine != 0x8664:
        raise VerificationError("UEFI fallback loader is not AMD64/x86_64")
