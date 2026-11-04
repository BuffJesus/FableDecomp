#!/usr/bin/env python3
"""Check component vtable slots and their retail targets, beyond masked text parity.

Compiles the three recovered constructors with VC7.1. Reads COFF symbols and
relocations directly, so missing slots, inherited methods, or a misplaced
secondary-interface virtual cannot hide behind a RELOCATION_MATCH.
"""
import argparse
import struct
import subprocess
import tempfile
from pathlib import Path

import verify_and_land as V

ROOT = Path(__file__).resolve().parents[2]
DEFAULT_RETAIL = Path(r"C:\Programs\Steam\steamapps\common\Fable The Lost Chapters\Fable.exe")
# Addresses from retail vtables, names from RTTI/FableWin plus call-site recovery.
TABLES = {
    "CGameComponent": (0x122ECB0, 0x122ECA4,
        [0x415940, 0x415920, 0xBFEA08, 0xBFEA08, 0x415930],
        [0xBFEA08, 0xBFEA08], "00/51/CGameComponent_CGameComponent_0051d8e0.cpp"),
    "CMainGameComponent": (0x122F180, 0x122F174,
        [0x4197B3, 0x4184BD, 0x4189C2, 0x41799B, 0x4197A0,
         0x418289, 0x416E78, 0x417001, 0x416953, 0x4197B0],
        [0x415E83, 0x4175DA], "00/41/CMainGameComponent_CMainGameComponent_00418dca.cpp"),
    "CNewFrontendGameComponent": (0x1230CA0, 0x1230C94,
        [0x4330D2, 0x42F75E, 0x42EC7C, 0x42DC5C, 0x430340],
        [0x42DC5F, 0x42DC60], "00/42/CNewFrontendGameComponent_CNewFrontendGameComponent_0042ea8f.cpp"),
}


def retail_bytes(data, address, size):
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    image_base = struct.unpack_from("<I", data, pe + 52)[0]
    sections = pe + 24 + struct.unpack_from("<H", data, pe + 20)[0]
    for i in range(struct.unpack_from("<H", data, pe + 6)[0]):
        virtual_size, rva, raw_size, raw = struct.unpack_from("<IIII", data, sections + 40*i + 8)
        offset = address - image_base - rva
        if 0 <= offset and offset + size <= raw_size:
            return data[raw + offset:raw + offset + size]
    raise ValueError(f"unmapped retail address {address:08x}")


class Coff:
    def __init__(self, path):
        self.data = path.read_bytes()
        machine, count, _, symptr, symcount, optional, _ = struct.unpack_from("<HHIIIHH", self.data)
        if machine != 0x14C or optional:
            raise ValueError("expected an i386 COFF object")
        strings = symptr + 18*symcount
        self.symbols = {}
        self.indices = {}
        i = 0
        while i < symcount:
            at = symptr + 18*i
            rawname, value, section, typ, storage, aux = struct.unpack_from("<8sIhHBB", self.data, at)
            if rawname[:4] == bytes(4):
                start = strings + struct.unpack_from("<I", rawname, 4)[0]
                name = self.data[start:self.data.index(0, start)].decode("ascii")
            else:
                name = rawname.rstrip(bytes(1)).decode("ascii")
            self.indices[i] = name
            if section > 0:
                self.symbols[name] = (section, value)
            i += 1 + aux
        self.sections = {}
        self.section_metadata = {}
        for i in range(count):
            at = 20 + 40*i
            size, raw, rel = struct.unpack_from("<III", self.data, at + 16)
            nrel = struct.unpack_from("<H", self.data, at + 32)[0]
            relocations = {}
            for j in range(nrel):
                offset, symbol, kind = struct.unpack_from("<IIH", self.data, rel + 10*j)
                relocations[offset] = (self.indices[symbol], kind)
            # PointerToRawData == 0 means no initialized file payload (e.g.
            # .bss), not bytes starting at the COFF header or fabricated zeros.
            self.sections[i+1] = (self.data[raw:raw+size] if raw else b"", relocations)
            self.section_metadata[i+1] = {
                "raw_size": size, "raw_pointer": raw,
                "virtual_size": struct.unpack_from("<I", self.data, at + 8)[0],
                "characteristics": struct.unpack_from("<I", self.data, at + 36)[0],
            }

    def table(self, symbol, expected):
        section, start = self.symbols[symbol]
        data, relocs = self.sections[section]
        # No RTTI locator is emitted under these build flags. Check the full
        # contribution, not a prefix that could conceal extra virtual slots.
        if start != 0 or len(data) != 4*len(expected):
            raise AssertionError(f"{symbol}: unexpected size/start {len(data)}/{start}")
        actual = []
        for offset in range(0, len(data), 4):
            name, kind = relocs[offset]
            if kind != 6 or data[offset:offset+4] != bytes(4):
                raise AssertionError(f"{symbol}: invalid absolute pointer relocation")
            actual.append(name)
        if actual != expected:
            raise AssertionError(f"{symbol}:\nexpected {expected}\nactual   {actual}")


def expected_slots(owner):
    primary = [f"??_E{owner}@@UAEPAXI@Z", f"?Init@{owner}@@UAEXXZ",
        f"?Run@{owner}@@UAE_NPAPAVCGameComponent@@@Z",
        f"?ChangeTextureColourDepth@{owner}@@UAEXJ@Z", f"?SetQuit@{owner}@@UAEXXZ"]
    secondary = [f"?OnPreDeviceReset@{owner}@@UAEXXZ", f"?OnPostDeviceReset@{owner}@@UAE_NXZ"]
    if owner == "CGameComponent":
        primary[2:4] = ["__purecall", "__purecall"]
        secondary = ["__purecall", "__purecall"]
    if owner == "CMainGameComponent":
        primary += [f"?Update@{owner}@@MAEXXZ", f"?GetInputs@{owner}@@MAEXXZ",
                    f"?Render@{owner}@@MAEXXZ", f"?PostInit@{owner}@@MAEXXZ",
                    f"?IsEditorActive@{owner}@@UBE_NXZ"]
    return primary, secondary


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--retail", type=Path, default=DEFAULT_RETAIL)
    args = ap.parse_args()
    data = args.retail.read_bytes()
    with tempfile.TemporaryDirectory(prefix="fable_component_abi_") as tmp:
        for owner, (primary, secondary, targets, callbacks, source) in TABLES.items():
            obj = Path(tmp) / (owner + ".obj")
            run = V.cl(["/nologo", "/c", "/O2", "/Oy", "/MT", "/GS", f"/Fo{obj}",
                str(ROOT / "rebuild/src/compiled" / source)], V.env())
            if run.returncode:
                raise RuntimeError(run.stdout + run.stderr)
            coff = Coff(obj)
            for base, va, addresses, symbols in zip(
                ["CBaseClassNonCopyable", "CDeviceResetCallback"],
                [primary, secondary], [targets, callbacks], expected_slots(owner)):
                retail = list(struct.unpack("<" + "I"*len(addresses), retail_bytes(data, va, 4*len(addresses))))
                if retail != addresses:
                    raise AssertionError(f"retail {owner}/{base} targets differ: {retail}")
                coff.table(f"??_7{owner}@@6B{base}@@@", symbols)
            print(f"{owner} ABI PASS primary={len(targets)} secondary={len(callbacks)} "
                  f"retail-targets={len(targets) + len(callbacks)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
