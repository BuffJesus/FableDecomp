#!/usr/bin/env python3
"""Compare readable swap decisions with retail; child/base update is a double."""
import hashlib
import itertools
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, pe_oracle, parity, run


def main():
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != "41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10":
        raise RuntimeError("Retail oracle executable changed")
    emulator = Uc(UC_ARCH_X86, UC_MODE_32)
    emulator.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image):
        emulator.mem_write(0x400000 + va, image[raw:raw + size])
    emulator.mem_map(0x20000000, 0x20000)
    receiver, stack, stop, vtable, query, entries = (0x20010000, 0x20008000,
        0x20000000, 0x20011000, 0x20012000, 0x20015000)
    # Explicit dependencies: the base update has already produced the supplied
    # current/target/completion state; ChangeState records the requested ID only.
    emulator.mem_write(0x52C7E0, bytes.fromhex("c20400"))
    for slot, offset, code in [(0x224, 0, "8b81e0010000c3"),
                               (0x238, 16, "8b81e4010000c3"),
                               (0xC0, 32, "8b4424048981e8010000ff81ec010000c20400")]:
        emulator.mem_write(vtable + slot, struct.pack("<I", query + offset))
        emulator.mem_write(query + offset, bytes.fromhex(code))
    tables = [[], [(0, 0.0)], [(0, 0.0), (1, 0.5)],
              [(i, 0.0) for i in range(4)], [(10, 1.0), (20, 2.0), (30, 0.5)],
              [(0, 2.0), (0, 0.0), (1, 0.0)]]
    cases = []
    for table in tables:
        ids = sorted(set([0, 99] + [entry[0] for entry in table]))
        for current, target, changed, random, time in itertools.product(ids, ids, (0, 1), (0, 1), (9.0, 10.0, 10.5, 12.0, 500.0)):
            cases.append((table, current, target, 13, 10.0, time, changed, random))
    directory = ROOT / "work/ui_swapping_check"
    directory.mkdir(parents=True, exist_ok=True)
    inputs = directory / "cases.txt"
    inputs.write_text("\n".join(" ".join(map(str, [len(table), *rest,
        *[value for entry in table for value in entry]])) for table, *rest in cases) + "\n")
    env = parity.env()
    obj, exe = directory / "behavior.obj", directory / "behavior.exe"
    run([parity.CL_EXE, "/nologo", "/c", "/W3", "/MT", "/GS", "/O2", "/Oy",
         "/Fo" + str(obj), ROOT / "rebuild/tests/integration/UiSwapping_test.cpp"], env)
    run([parity.VC / "bin/link.exe", "/nologo", "/subsystem:console",
         "/out:" + str(exe), obj, "kernel32.lib"], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases): raise RuntimeError("Incomplete swap sample grid")
    errors = []
    for index, (case, line) in enumerate(zip(cases, lines)):
        table, current, target, seed, last, time, changed, random = case
        data = bytearray(0x200)
        struct.pack_into("<I", data, 0, vtable)
        struct.pack_into("<f", data, 0x30, time)
        struct.pack_into("<II", data, 0x144, current, target)
        struct.pack_into("<III", data, 0x15C, entries, entries + len(table)*8, entries + len(table)*8)
        struct.pack_into("<fI", data, 0x168, last, seed)
        struct.pack_into("<IIII", data, 0x1E0, changed, random, 0xDEADBEEF, 0)
        emulator.mem_write(receiver, bytes(data))
        if table:
            emulator.mem_write(entries, b"".join(struct.pack("<If", *entry) for entry in table))
        emulator.mem_write(stack, struct.pack("<If", stop, 0.0))
        emulator.reg_write(UC_X86_REG_ESP, stack)
        emulator.reg_write(UC_X86_REG_ECX, receiver)
        emulator.reg_write(UC_X86_REG_FPCW, 0x37F)
        emulator.emu_start(0x547380, stop, count=30000)
        if emulator.reg_read(UC_X86_REG_EIP) != stop: raise RuntimeError("Retail update did not return")
        expected_last, expected_seed = struct.unpack("<fI", emulator.mem_read(receiver + 0x168, 8))
        requested, count = struct.unpack("<II", emulator.mem_read(receiver + 0x1E8, 8))
        actual = line.split()
        expected = [count, requested, expected_seed]
        if list(map(int, actual[:3])) != expected or abs(float(actual[3]) - expected_last) > 0.000001:
            errors.append({"index": index, "case": case, "actual": actual,
                           "retail": [*expected, expected_last]})
    report = {"accepted": not errors, "cases": len(cases), "errors": errors,
              "scope": "retail swap update with explicit base/query/ChangeState doubles"}
    (directory / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(f"UI_SWAPPING {'PASS' if not errors else 'FAIL'} cases={len(cases)} failures={len(errors)}")
    return 1 if errors else 0


if __name__ == "__main__":
    raise SystemExit(main())
