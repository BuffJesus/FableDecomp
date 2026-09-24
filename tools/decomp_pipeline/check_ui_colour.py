#!/usr/bin/env python3
"""Check colour retargeting and full update against retail, including inheritance."""
import hashlib
import json
import random
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
    receiver, stack, stop, vtable, query, argument = (0x20010000, 0x20008000,
        0x20000000, 0x20011000, 0x20012000, 0x20013000)
    # Only the virtual independence query is a double. The complete colour
    # update body, setter and CRT conversion instructions execute unchanged.
    emulator.mem_write(vtable + 0x194, struct.pack("<I", query))
    emulator.mem_write(query, bytes.fromhex("8b81f0010000c3"))
    directory = ROOT / "work/ui_colour_check"
    directory.mkdir(parents=True, exist_ok=True)
    rng = random.Random(0x52F900)
    cases = []
    for i in range(1200):
        mode = i % 3 == 0
        colour = [rng.getrandbits(32) for _ in range(6)]
        duration = (0.0, 2.0, 8.0)[i % 3]
        elapsed = (0.0, 0.5, 1.5, 2.0, 7.5, 8.0, 12.0)[i % 7]
        time = (-1.0, 0.0, 0.125, 0.5, 1.0, 2.0, 8.0, 16.0)[i % 8]
        cases.append([0 if mode else 1, *colour[:5], elapsed, duration,
                      i % 2, colour[5], time])
    # Black/white alpha ramps and inherited-alpha endpoint boundaries.
    for alpha in (0, 1, 63, 127, 128, 191, 254, 255):
        for parent in (0, 1, 127, 128, 254, 255):
            for independent in (0, 1):
                colour = 0xFFFFFF | (alpha << 24)
                cases.append([1, colour, colour, colour, 0xFFFFFF | (parent << 24),
                              0, 0.0, 0.0, independent, 0, 0.125])
    inputs = directory / "cases.txt"
    inputs.write_text("\n".join(" ".join(map(str, row)) for row in cases) + "\n")
    env = parity.env()
    obj, exe = directory / "behavior.obj", directory / "behavior.exe"
    run([parity.CL_EXE, "/nologo", "/c", "/W3", "/MT", "/GS", "/O2", "/Oy",
         "/Fo" + str(obj), ROOT / "rebuild/tests/integration/UiColour_test.cpp"], env)
    run([parity.VC / "bin/link.exe", "/nologo", "/subsystem:console",
         "/out:" + str(exe), obj], env)
    actual_lines = run([exe, inputs], env).splitlines()
    if len(actual_lines) != len(cases):
        raise RuntimeError("Incomplete colour sample grid")
    errors, worst = [], 0
    for index, (case, line) in enumerate(zip(cases, actual_lines)):
        mode, current, target, initial, parent, render, elapsed, duration, independent, arg, time = case
        data = bytearray(0x200)
        struct.pack_into("<I", data, 0, vtable)
        struct.pack_into("<IIIII", data, 0x84, current, target, initial, parent, render)
        struct.pack_into("<ff", data, 0xA8, elapsed, duration)
        struct.pack_into("<I", data, 0x1F0, independent)
        emulator.mem_write(receiver, bytes(data))
        emulator.mem_write(argument, struct.pack("<I", arg))
        args = struct.pack("<IIfI", stop, argument, time, 0) if mode == 0 else struct.pack("<If", stop, time)
        emulator.mem_write(stack, args)
        emulator.reg_write(UC_X86_REG_ESP, stack)
        emulator.reg_write(UC_X86_REG_ECX, receiver)
        emulator.reg_write(UC_X86_REG_FPCW, 0x37F)
        emulator.emu_start(0x52EC60 if mode == 0 else 0x52F900, stop, count=30000)
        if emulator.reg_read(UC_X86_REG_EIP) != stop:
            raise RuntimeError("Retail colour function did not return")
        expected = [struct.unpack("<I", emulator.mem_read(receiver + offset, 4))[0]
                    for offset in (0x84, 0x88, 0x8C, 0x94)]
        actual = list(map(int, line.split()[:4]))
        error = max(abs((a >> shift & 255) - (b >> shift & 255))
                    for a, b in zip(actual, expected) for shift in (0, 8, 16, 24))
        expected_time = struct.unpack("<ff", emulator.mem_read(receiver + 0xA8, 8))
        time_ok = all(abs(a - b) < 0.000001 for a, b in zip(map(float, line.split()[4:]), expected_time))
        worst = max(worst, error)
        if error > 1 or not time_ok:
            errors.append({"index": index, "case": case, "retail": expected,
                           "compiled": actual, "channel_error": error, "time_ok": time_ok})
    report = {"accepted": not errors, "cases": len(cases), "max_channel_error": worst,
              "errors": errors, "scope": "complete retail colour functions; independence query double"}
    (directory / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(f"UI_COLOUR {'PASS' if not errors else 'FAIL'} cases={len(cases)} max_channel_error={worst} failures={len(errors)}")
    return 1 if errors else 0


if __name__ == "__main__":
    raise SystemExit(main())
