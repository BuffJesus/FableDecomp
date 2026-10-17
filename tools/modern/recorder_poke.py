"""Switch on retail Fable.exe's compiled-in event-package / input recorder in a running game.

Usage (game sitting at the frontend, before a world is started):
  python tools/modern/recorder_poke.py <pid> --save-events C:\\abs\\run.evp [--save-inputs C:\\abs\\run.inp]
  python tools/modern/recorder_poke.py <pid> --load-events C:\\abs\\run.evp [--load-inputs ...]
  python tools/modern/recorder_poke.py <pid> --status

Retail reads four .bss flag bytes that nothing writes (docs/modern/FRAME_TIMING.md):
  0x13B85F7 event-package saving, 0x13B85F8 event-package loading  (CMainGameComponent::InitWorld)
  0x13B85F6 input saving,         0x13B85F5 input loading          (CMainGameComponent::Run)
File names are the CWideString globals 0x13B8660 (GEventPackageFileName) and 0x13B865C (inputs).
Retail CWideStringData is 16 bytes: a std::vector<wchar_t> {first, last, end} and NoRefs at +0xC
(not the 28-byte wstring Ego_r.pdb shows). The readers only take operator const wchar_t*
(0x99AF00 = [[g]]) and copy it, so a block we allocate in the target with a huge refcount is enough.
"""
from __future__ import annotations

import argparse
import ctypes
import struct
import sys

FLAG_INPUT_LOAD, FLAG_INPUT_SAVE = 0x13B85F5, 0x13B85F6
FLAG_EVENT_SAVE, FLAG_EVENT_LOAD = 0x13B85F7, 0x13B85F8
NAME_INPUT, NAME_EVENT = 0x13B865C, 0x13B8660

k32 = ctypes.WinDLL("kernel32", use_last_error=True)
k32.OpenProcess.restype = ctypes.c_void_p
k32.VirtualAllocEx.restype = ctypes.c_void_p
k32.VirtualAllocEx.argtypes = [ctypes.c_void_p, ctypes.c_void_p, ctypes.c_size_t, ctypes.c_uint32, ctypes.c_uint32]


class Proc:
    def __init__(self, pid: int) -> None:
        # VM_OPERATION | VM_READ | VM_WRITE | QUERY_INFORMATION
        self.h = k32.OpenProcess(0x8 | 0x10 | 0x20 | 0x400, False, pid)
        if not self.h:
            raise SystemExit(f"OpenProcess({pid}) failed: {ctypes.get_last_error()}")

    def read(self, a: int, n: int) -> bytes:
        b = ctypes.create_string_buffer(n)
        got = ctypes.c_size_t()
        if not k32.ReadProcessMemory(ctypes.c_void_p(self.h), ctypes.c_void_p(a), b, n, ctypes.byref(got)):
            raise OSError(f"read {a:#x}: {ctypes.get_last_error()}")
        return b.raw

    def write(self, a: int, data: bytes) -> None:
        got = ctypes.c_size_t()
        if not k32.WriteProcessMemory(ctypes.c_void_p(self.h), ctypes.c_void_p(a), data, len(data), ctypes.byref(got)):
            raise OSError(f"write {a:#x}: {ctypes.get_last_error()}")

    def u32(self, a: int) -> int:
        return struct.unpack("<I", self.read(a, 4))[0]

    def alloc(self, n: int) -> int:
        a = k32.VirtualAllocEx(ctypes.c_void_p(self.h), None, n, 0x3000, 0x04)  # COMMIT|RESERVE, RW
        if not a:
            raise OSError(f"VirtualAllocEx: {ctypes.get_last_error()}")
        return a


def wide_string(p: Proc, text: str) -> int:
    """Allocate a retail CWideStringData for `text` in the target; return its address."""
    chars = (text + "\0").encode("utf-16-le")
    blk = p.alloc(16 + len(chars))
    buf = blk + 16
    last = buf + len(chars) - 2          # vector holds the terminator? harmless either way: readers use [0]
    p.write(blk, struct.pack("<IIII", buf, last, buf + len(chars), 0x40000000) + chars)
    return blk


def string_at(p: Proc, g: int) -> str:
    d = p.u32(g)
    if not d:
        return "<empty>"
    s = p.u32(d)
    raw = p.read(s, 520)
    return raw.decode("utf-16-le", "replace").split("\0")[0]


def status(p: Proc) -> None:
    for name, a in (("input load", FLAG_INPUT_LOAD), ("input save", FLAG_INPUT_SAVE),
                    ("event save", FLAG_EVENT_SAVE), ("event load", FLAG_EVENT_LOAD)):
        print(f"{name:10} [{a:#x}] = {p.read(a, 1)[0]}")
    print(f"input file  [{NAME_INPUT:#x}] = {string_at(p, NAME_INPUT)}")
    print(f"event file  [{NAME_EVENT:#x}] = {string_at(p, NAME_EVENT)}")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("pid", type=int)
    g = ap.add_mutually_exclusive_group()
    g.add_argument("--save-events")
    g.add_argument("--load-events")
    h = ap.add_mutually_exclusive_group()
    h.add_argument("--save-inputs")
    h.add_argument("--load-inputs")
    ap.add_argument("--status", action="store_true")
    ap.add_argument("--clear", action="store_true", help="zero all four flags")
    a = ap.parse_args()
    p = Proc(a.pid)
    if a.clear:
        for f in (FLAG_INPUT_LOAD, FLAG_INPUT_SAVE, FLAG_EVENT_SAVE, FLAG_EVENT_LOAD):
            p.write(f, b"\0")
    for path, flag, name in ((a.save_events, FLAG_EVENT_SAVE, NAME_EVENT), (a.load_events, FLAG_EVENT_LOAD, NAME_EVENT),
                             (a.save_inputs, FLAG_INPUT_SAVE, NAME_INPUT), (a.load_inputs, FLAG_INPUT_LOAD, NAME_INPUT)):
        if path:
            p.write(name, struct.pack("<I", wide_string(p, path)))
            p.write(flag, b"\1")
    status(p)


if __name__ == "__main__":
    sys.exit(main())
