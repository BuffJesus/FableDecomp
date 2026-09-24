#!/usr/bin/env python3
"""Compare readable primitive insertion with complete retail AddToList."""
import argparse
import hashlib
import itertools
import json
import random
import struct

from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--require-byte-match", action="store_true")
    args = parser.parse_args()
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != "41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10":
        raise RuntimeError("Retail oracle executable changed")
    machine = Uc(UC_ARCH_X86, UC_MODE_32)
    machine.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image):
        machine.mem_write(0x400000 + va, image[raw:raw + size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, head, nodes = 0x20000000, 0x20008000, 0x20010000, 0x20011000
    cases = [list(masks) for count in range(7)
             for masks in itertools.product((0, 1, 0xFFFFFFFF), repeat=count)]
    rng = random.Random(0xB8FDF0)
    for count in (8, 16, 32, 64):
        for _ in range(32):
            cases.append([rng.choice((0, 1, 2, 0x80000000, 0xFFFFFFFF,
                                      rng.getrandbits(32))) for _ in range(count)])
    directory = ROOT / "work/primitive_list_check"
    directory.mkdir(parents=True, exist_ok=True)
    inputs = directory / "cases.txt"
    inputs.write_text("\n".join(" ".join(map(str, [len(case), *case])) for case in cases) + "\n")
    env = parity.env()
    source = ROOT / "rebuild/src/compiled/00/b8/CEngineInternalPrimitiveBase_AddToList_00b8fdf0.cpp"
    fixture = ROOT / "rebuild/tests/integration/PrimitiveList_test.cpp"
    objects = []
    for name, path in (("primitive", source), ("behavior", fixture)):
        obj = directory / (name + ".obj")
        run([parity.CL_EXE, "/nologo", "/c", "/W3", "/MT", "/GS", "/O2", "/Oy",
             "/I" + str(ROOT / "rebuild/include"), "/Fo" + str(obj), path], env)
        objects.append(obj)
    exe = directory / "behavior.exe"
    run([parity.VC / "bin/link.exe", "/nologo", "/subsystem:console",
         "/out:" + str(exe), *objects, "kernel32.lib"], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases):
        raise RuntimeError("Incomplete primitive-list results")
    body, section, symbol = parity.obj_text(objects[0], "FablePrimitiveAddToList")
    offset = pe_oracle.va_to_off(pe_oracle.pe_sections(image), 0xB8FDF0)
    retail = image[offset:offset + 106]
    byte_status = "MATCH" if body == retail else "DIFFER"
    (directory / "primitive.asm").write_text(run([parity.OBJDUMP, "-dr", objects[0]], env))

    def normalized(pointer):
        if pointer == 0: return -1
        if pointer == head: return -2
        return pointer - nodes

    errors = []
    for index, (case, line) in enumerate(zip(cases, lines)):
        machine.mem_write(head, b"\0" * 4)
        machine.mem_write(nodes, b"\xA5" * (64*0x48))
        for slot, mask in enumerate(case):
            node = nodes + slot*0x48
            machine.mem_write(node + 0x30, struct.pack("<I", mask))
            machine.mem_write(stack, struct.pack("<II", stop, head))
            machine.reg_write(UC_X86_REG_ESP, stack)
            machine.reg_write(UC_X86_REG_ECX, node)
            machine.emu_start(0xB8FDF0, stop, count=10000)
            if machine.reg_read(UC_X86_REG_EIP) != stop:
                raise RuntimeError("Retail insertion did not return")
        expected = [normalized(struct.unpack("<I", machine.mem_read(head, 4))[0])]
        for slot in range(len(case)):
            links = struct.unpack("<IIII", machine.mem_read(nodes + slot*0x48 + 0x38, 16))
            expected.extend(normalized(link) for link in links)
        actual = list(map(int, line.split()))
        if actual != expected:
            errors.append({"index": index, "masks": case, "actual": actual, "retail": expected})
    accepted = not errors and (byte_status == "MATCH" or not args.require_byte_match)
    report = {"accepted": accepted, "cases": len(cases), "errors": errors,
              "scope": "all four links and head after full retail insertion; no dependency doubles",
              "byte_parity": byte_status, "compiled_bytes": len(body), "retail_bytes": len(retail),
              "source_sha256": hashlib.sha256(source.read_bytes()).hexdigest(),
              "require_byte_match": args.require_byte_match}
    report_name = "strict-report.json" if args.require_byte_match else "report.json"
    (directory / report_name).write_text(json.dumps(report, indent=2) + "\n")
    print(f"PRIMITIVE_LIST {'PASS' if not errors else 'FAIL'} cases={len(cases)} failures={len(errors)}")
    print(f"PRIMITIVE_LIST {byte_status} bytes={len(body)}/{len(retail)}")
    return int(not accepted)


if __name__ == "__main__":
    raise SystemExit(main())
