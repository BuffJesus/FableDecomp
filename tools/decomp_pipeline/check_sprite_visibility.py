#!/usr/bin/env python3
"""Compare sprite submission/release decisions to retail Draw's prefix."""
import hashlib
import itertools
import json
import struct

from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP, UC_X86_REG_FPCW
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != "41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10":
        raise RuntimeError("Retail oracle executable changed")
    machine = Uc(UC_ARCH_X86, UC_MODE_32)
    machine.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image):
        machine.mem_write(0x400000 + va, image[raw:raw + size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, receiver, submit = 0x20000000, 0x20008000, 0x20010000, 0x41B065
    def boundary(machine, address, size, data):
        if address in (stop, submit):
            machine.emu_stop()
    machine.hook_add(UC_HOOK_CODE, boundary)
    # Zero, signed zero, negative/positive finite, infinities, quiet NaN.
    zooms = (0, 0x80000000, 0xBF800000, 0x3F800000, 0x3F000000,
             0x7F800000, 0xFF800000, 0x7FC00000)
    cases = list(itertools.product((0, 1, 2, 255), (0, 1, 127, 255), zooms, zooms))
    directory = ROOT / "work/sprite_visibility_check"
    directory.mkdir(parents=True, exist_ok=True)
    inputs = directory / "cases.txt"
    inputs.write_text("\n".join(" ".join(map(str, row)) for row in cases) + "\n")
    env = parity.env()
    obj, exe = directory / "behavior.obj", directory / "behavior.exe"
    run([parity.CL_EXE, "/nologo", "/c", "/W3", "/MT", "/GS", "/O2", "/Oy",
         "/Fo" + str(obj), ROOT / "rebuild/tests/integration/SpriteVisibility_test.cpp"], env)
    run([parity.VC / "bin/link.exe", "/nologo", "/subsystem:console", "/out:" + str(exe), obj], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases): raise RuntimeError("Incomplete sprite sample grid")
    errors = []
    for index, (case, line) in enumerate(zip(cases, lines)):
        pending, alpha, xbits, ybits = case
        data = bytearray(0x200)
        struct.pack_into("<II", data, 0x7C, xbits, ybits)
        data[0x97], data[0x170] = alpha, pending
        machine.mem_write(receiver, bytes(data))
        machine.mem_write(stack, struct.pack("<IIIIII", stop, 0, 0, 0, 0, 0))
        machine.reg_write(UC_X86_REG_ESP, stack)
        machine.reg_write(UC_X86_REG_ECX, receiver)
        machine.reg_write(UC_X86_REG_FPCW, 0x37F)
        machine.emu_start(0x41AFA0, stop, count=1000)
        endpoint = machine.reg_read(UC_X86_REG_EIP)
        if endpoint not in (submit, stop): raise RuntimeError("Retail decision did not finish")
        expected = [int(endpoint == stop), machine.mem_read(receiver + 0x170, 1)[0]]
        actual = list(map(int, line.split()))
        if actual != expected:
            errors.append({"index": index, "case": case, "actual": actual, "retail": expected})
    report = {"accepted": not errors, "cases": len(cases), "errors": errors,
              "scope": "Draw prefix to submission boundary or return; empty persistent handle, no doubles",
              "excludes": "geometry submission and non-null handle release"}
    (directory / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(f"SPRITE_VISIBILITY {'PASS' if not errors else 'FAIL'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__ == "__main__":
    raise SystemExit(main())
