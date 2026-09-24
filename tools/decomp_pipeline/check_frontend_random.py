#!/usr/bin/env python3
"""Differential check of frontend random selection against retail x86 code."""
import hashlib
import json
import struct
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_ECX, UC_X86_REG_EDX, UC_X86_REG_EAX, UC_X86_REG_EIP
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
    seed_address, stack, stop = 0x20010000, 0x20008000, 0x20000000
    directory = ROOT / "work/frontend_random_check"
    directory.mkdir(parents=True, exist_ok=True)
    env = parity.env()
    options = ["/nologo", "/c", "/W3", "/MT", "/GS", "/O2", "/Oy"]
    obj, exe = directory / "behavior.obj", directory / "behavior.exe"
    fixture = ROOT / "rebuild/tests/integration/FrontendRandom_test.cpp"
    run([parity.CL_EXE, *options, "/Fo" + str(obj), fixture], env)
    run([parity.VC / "bin/link.exe", "/nologo", "/subsystem:console",
         "/out:" + str(exe), obj, "kernel32.lib"], env)
    cases, mismatches, frontend_sequence = 0, [], {3: [], 4: []}
    for line in run([exe], env).splitlines():
        count, previous, before, actual, actual_seed = map(int, line.split())
        emulator.mem_write(seed_address, struct.pack("<I", before))
        emulator.mem_write(stack, struct.pack("<II", stop, seed_address))
        emulator.reg_write(UC_X86_REG_ESP, stack)
        emulator.reg_write(UC_X86_REG_ECX, count)
        emulator.reg_write(UC_X86_REG_EDX, previous)
        emulator.emu_start(0x5472A0, stop, count=20000)
        if emulator.reg_read(UC_X86_REG_EIP) != stop:
            raise RuntimeError("Retail random function did not return")
        expected = emulator.reg_read(UC_X86_REG_EAX)
        expected_seed = struct.unpack("<I", emulator.mem_read(seed_address, 4))[0]
        if (actual, actual_seed) != (expected, expected_seed):
            mismatches.append([count, previous, before, actual, actual_seed, expected, expected_seed])
        if count in frontend_sequence and len(frontend_sequence[count]) < 16:
            frontend_sequence[count].append(actual)
        cases += 1
    if cases != 4480:
        raise RuntimeError("Incomplete random fixture sample grid")
    report = {"accepted": not mismatches, "cases": cases, "mismatches": mismatches,
              "frontend_sequences_from_seed_13": frontend_sequence,
              "scope": "random selection and seed evolution; retry-exhaustion diagnostic not exercised"}
    (directory / "report.json").write_text(json.dumps(report, indent=2) + "\n")
    print(f"FRONTEND_RANDOM {'PASS' if not mismatches else 'FAIL'} cases={cases} mismatches={len(mismatches)}")
    print(json.dumps(frontend_sequence))
    return 1 if mismatches else 0


if __name__ == "__main__":
    raise SystemExit(main())
