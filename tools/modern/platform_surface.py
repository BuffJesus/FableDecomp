"""Inventory the retail platform surface: every DLL import in Fable.exe and the functions that use it.

Evidence for docs/modern/PLATFORM_SURFACE.md. An import is "referenced" wherever its absolute IAT slot
address appears in an executable section (call [iat], jmp [iat], mov reg,[iat]; call reg). Each
reference is attributed to the nearest preceding function start in rebuild/manifest/functions.tsv.

    python tools/modern/platform_surface.py            # writes docs/modern/PLATFORM_SURFACE.md
    python tools/modern/platform_surface.py --stdout
"""
from __future__ import annotations

import argparse
import bisect
import csv
import struct
import sys
from collections import defaultdict
from pathlib import Path

import pefile

ROOT = Path(__file__).resolve().parents[2]
EXE = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
MANIFEST = ROOT / "rebuild" / "manifest" / "functions.tsv"
OUT = ROOT / "docs" / "modern" / "PLATFORM_SURFACE.md"

# Planned replacement per DLL, keyed by lower-case DLL name. Unlisted DLLs print as "review".
PLAN = {
    "d3d9.dll": ("Graphics", "RHI: D3D12 + Vulkan backends (hand-written); D3D9 backend kept as reference"),
    "d3dx9_25.dll": ("Graphics", "Own math/texture/shader-compile code; shaders re-authored in HLSL, DXC -> DXIL + SPIR-V"),
    "dinput8.dll": ("Input", "Platform input service: raw input + XInput/GameInput on Windows, evdev later"),
    "xinput1_3.dll": ("Input", "Platform input service"),
    "ole32.dll": ("Video/COM", "Drops with DirectShow; nothing else should need COM"),
    "oleaut32.dll": ("Video/COM", "Drops with DirectShow"),
    "winmm.dll": ("Timing/Audio", "Platform clock (QueryPerformanceCounter / clock_gettime)"),
    "advapi32.dll": ("Config", "Registry -> config file in the user profile directory"),
    "user32.dll": ("Window", "Platform window/message service"),
    "gdi32.dll": ("Window", "Platform window service (GDI use is cursor/font fallback; verify)"),
    "kernel32.dll": ("OS", "Platform file/thread/sync/time layer (Win32 now, POSIX later)"),
    "shell32.dll": ("OS", "Platform paths (known folders / XDG)"),
    "shfolder.dll": ("OS", "Platform paths (known folders / XDG)"),
    "msvcr71.dll": ("CRT", "Modern CRT (MSVC / glibc); CRT-shaped retail code becomes ordinary C++ standard library use"),
    "msvcp71.dll": ("CRT", "Modern C++ standard library"),
    "version.dll": ("OS", "Drop (driver/version probing)"),
    "wsock32.dll": ("Network", "Drop or platform sockets (cut co-op only)"),
    "ws2_32.dll": ("Network", "Drop or platform sockets (cut co-op only)"),
    "imm32.dll": ("Input/IME", "Platform text-input service"),
}


# Platform libraries the exe loads at runtime (LoadLibrary/GetProcAddress or COM), so they never show in
# the import table. Each is confirmed by the string evidence listed; the tool reports which strings exist.
DYNAMIC = [
    ("Audio", "OpenAL (Creative `wrap_oal` DirectSound3D wrapper)", ["OpenAL32.dll", "wrap_oal.dll", "alBufferData", "alDistanceModel"],
     "OpenAL Soft (same API, x64 + Linux); `wrap_oal`/DirectSound3D path drops"),
    ("Video", "DirectShow via COM", ["CoCreateInstance", ".wmv"],
     "ffmpeg (libavformat/libavcodec) decoding the retail .wmv files"),
    ("Config", "ConfigDetect.dll (hardware tier probe)", ["ConfigDetect.dll"],
     "Own capability probe on the RHI device"),
    ("Text", "strings.dll (localised launcher/error strings)", ["strings.dll"],
     "Plain data file"),
]


def load_functions() -> tuple[list[int], list[str]]:
    starts: list[tuple[int, str]] = []
    with MANIFEST.open(newline="", encoding="utf-8") as fh:
        for row in csv.DictReader(fh, delimiter="\t"):
            try:
                starts.append((int(row["address"], 16), row["name"]))
            except (KeyError, ValueError):
                continue
    starts.sort()
    return [a for a, _ in starts], [n for _, n in starts]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--exe", type=Path, default=EXE)
    ap.add_argument("--stdout", action="store_true")
    args = ap.parse_args()

    pe = pefile.PE(str(args.exe), fast_load=False)
    base = pe.OPTIONAL_HEADER.ImageBase

    iat: dict[int, tuple[str, str]] = {}
    for entry in getattr(pe, "DIRECTORY_ENTRY_IMPORT", []):
        dll = entry.dll.decode(errors="replace").lower()
        for imp in entry.imports:
            name = imp.name.decode(errors="replace") if imp.name else f"#{imp.ordinal}"
            iat[imp.address] = (dll, name)

    starts, names = load_functions()
    refs: dict[int, list[int]] = defaultdict(list)
    for sec in pe.sections:
        if not sec.Characteristics & 0x20000000:  # IMAGE_SCN_MEM_EXECUTE
            continue
        data = sec.get_data()
        va0 = base + sec.VirtualAddress
        for off in range(len(data) - 3):
            val = struct.unpack_from("<I", data, off)[0]
            if val in iat:
                refs[val].append(va0 + off)

    by_dll: dict[str, dict] = defaultdict(lambda: {"imports": [], "refs": 0, "funcs": set()})
    for slot, (dll, name) in iat.items():
        callers = set()
        for site in refs.get(slot, []):
            i = bisect.bisect_right(starts, site) - 1
            if i >= 0:
                callers.add(f"{names[i]} @ 0x{starts[i]:08X}")
        d = by_dll[dll]
        d["imports"].append((name, len(refs.get(slot, [])), len(callers)))
        d["refs"] += len(refs.get(slot, []))
        d["funcs"] |= callers

    lines = [
        "# Retail platform surface",
        "",
        "Generated by `tools/modern/platform_surface.py` from the retail `Fable.exe` import table. Do not",
        "hand-edit; re-run the tool. Plan and context: [MODERNIZATION](MODERNIZATION.md).",
        "",
        "A reference is an absolute IAT-slot address found in an executable section; it is attributed to",
        "the nearest preceding function start in `rebuild/manifest/functions.tsv`. Every function listed",
        "here must sit behind a platform service before the x64 step (see MODERNIZATION, step 3).",
        "",
        "COM APIs undercount here: `d3d9.dll` shows one import (`Direct3DCreate9`) and `dinput8.dll` one",
        "(`DirectInput8Create`) because everything after creation is a vtable call on the returned",
        "interface. The real D3D9 surface is the `IDirect3DDevice9` call-site census (open task in",
        "MODERNIZATION, step 4).",
        "",
        "## Summary",
        "",
        "| DLL | Area | Imports | Code refs | Functions | Modern replacement |",
        "|---|---|---:|---:|---:|---|",
    ]
    order = sorted(by_dll, key=lambda k: (-by_dll[k]["refs"], k))
    for dll in order:
        d = by_dll[dll]
        area, repl = PLAN.get(dll, ("review", "review"))
        lines.append(f"| `{dll}` | {area} | {len(d['imports'])} | {d['refs']} | {len(d['funcs'])} | {repl} |")

    blob = pe.__data__[:]
    lines += [
        "",
        "## Loaded at runtime (not in the import table)",
        "",
        "| Area | Library | String evidence found in Fable.exe | Modern replacement |",
        "|---|---|---|---|",
    ]
    for area, lib, needles, repl in DYNAMIC:
        found = [n for n in needles if n.encode() in blob]
        missing = [n for n in needles if n not in found]
        ev = ", ".join(f"`{n}`" for n in found) + (f" (missing: {', '.join(missing)})" if missing else "")
        lines.append(f"| {area} | {lib} | {ev} | {repl} |")

    lines += ["", "## Imports by DLL", ""]
    for dll in order:
        d = by_dll[dll]
        lines.append(f"### `{dll}`")
        lines.append("")
        lines.append("| Import | Code refs | Functions |")
        lines.append("|---|---:|---:|")
        for name, nref, nfn in sorted(d["imports"], key=lambda t: (-t[1], t[0])):
            lines.append(f"| `{name}` | {nref} | {nfn} |")
        lines.append("")

    text = "\n".join(lines) + "\n"
    if args.stdout:
        sys.stdout.write(text)
    else:
        OUT.parent.mkdir(parents=True, exist_ok=True)
        OUT.write_text(text, encoding="utf-8")
        print(f"wrote {OUT.relative_to(ROOT)}: {len(by_dll)} DLLs, {len(iat)} imports")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
