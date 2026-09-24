#!/usr/bin/env python3
"""Compare the scaffold fade adapter with the actual retail x87 instructions.

Emulates only 0052F900..0052FE38, before parent-colour/vtable work. No game launch.
This validates the interpolation extraction, not the entire GUI update method.
"""
import hashlib
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_ECX, UC_X86_REG_FPCW
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
    receiver, stack = 0x20010000, 0x20008000
    directory = ROOT / "work/frontend_fade_check"
    directory.mkdir(parents=True, exist_ok=True)
    env = parity.env()
    options = ["/nologo", "/c", "/W3", "/MT", "/GS", "/O2", "/Oy"]
    obj, exe = directory / "behavior.obj", directory / "behavior.exe"
    fixture = ROOT / "rebuild/tests/integration/FrontendFade_test.cpp"
    run([parity.CL_EXE, *options, "/Fo" + str(obj), fixture], env)
    run([parity.VC / "bin/link.exe", "/nologo", "/subsystem:console",
         "/out:" + str(exe), obj], env)
    rows = []
    for line in run([exe], env).splitlines():
        duration_index, incoming, step, actual = map(int, line.split())
        duration = (2.0, 8.0)[duration_index]
        elapsed = duration * step / 64
        start, target = (0, 255) if incoming else (255, 0)
        data = bytearray(0x200)
        data[0x84:0x88] = bytes([255, 255, 255, start])
        data[0x88:0x8c] = bytes([255, 255, 255, target])
        data[0x8c:0x90] = bytes([255, 255, 255, start])
        struct.pack_into("<ff", data, 0xA8, 0, duration)
        emulator.mem_write(receiver, bytes(data))
        emulator.mem_write(stack, struct.pack("<If", 0, elapsed))
        emulator.reg_write(UC_X86_REG_ESP, stack)
        emulator.reg_write(UC_X86_REG_ECX, receiver)
        emulator.reg_write(UC_X86_REG_FPCW, 0x37F)
        # The real CRT __ftol2 runs too; no math or clock doubles.
        emulator.emu_start(0x52F900, 0x52FE38, count=20000)
        expected = emulator.mem_read(receiver + 0x87, 1)[0]
        rows.append({"duration": duration, "incoming": bool(incoming),
                     "elapsed": elapsed, "retail": expected, "compiled": actual})
    if len(rows) != 260:
        raise RuntimeError("Incomplete fixture sample grid")
    error = max(abs(row["retail"] - row["compiled"]) for row in rows)
    # Simplified polynomial evaluation can move truncation by one alpha unit.
    accepted = error <= 1
    presenter = ROOT / "rebuild/integration/visual_boot_d3d9.cpp"
    run([parity.CL_EXE, *options, "/I" + str(ROOT / "rebuild/integration"),
         "/Fo" + str(directory / "presenter.obj"), presenter], env)
    report = {"accepted": accepted, "max_alpha_error": error, "samples": rows,
              "scope": "retail colour interpolation prefix; presenter compile only",
              "presenter_sha256": hashlib.sha256(presenter.read_bytes()).hexdigest()}
    (directory / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(f"FRONTEND_FADE {'PASS' if accepted else 'FAIL'} samples={len(rows)} max_alpha_error={error} presenter=COMPILE_PASS")
    return 0 if accepted else 1


if __name__ == "__main__":
    raise SystemExit(main())
