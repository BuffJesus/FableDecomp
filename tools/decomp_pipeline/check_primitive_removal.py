#!/usr/bin/env python3
"""Differential native primitive unlinking, group promotion and grid ownership."""
import hashlib
import itertools
import json
import random
import struct

from unicorn import Uc, UC_ARCH_X86, UC_MODE_32
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP
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
    stop, stack, head, nodes, grid = 0x20000000, 0x20008000, 0x20010000, 0x20011000, 0x20014000
    cases = []
    for count in range(1, 6):
        for masks in itertools.product((0, 1, 0xFFFFFFFF), repeat=count):
            for victim in range(count):
                cases.append((masks, [victim], 0, 1))
    rng = random.Random(0xB8FE60)
    for count, flags, with_grid in itertools.product((1, 3, 8, 16, 64), (0, 2, 8, 255), (0, 1)):
        masks = [rng.choice((0, 1, 2, 0xFFFFFFFF)) for _ in range(count)]
        removals = list(range(count))
        rng.shuffle(removals)
        # Repeated removal checks cleared backlinks and persistent grid counts.
        removals.insert(1, removals[0])
        cases.append((masks, removals, flags, with_grid))
    directory = ROOT / "work/primitive_removal_check"
    directory.mkdir(parents=True, exist_ok=True)
    inputs = directory / "cases.txt"
    inputs.write_text("\n".join(" ".join(map(str, [len(masks), len(removals), flags, with_grid,
        *masks, *removals])) for masks, removals, flags, with_grid in cases) + "\n")
    env, objects = parity.env(), []
    for name, source in (
        ("add", "rebuild/src/compiled/00/b8/CEngineInternalPrimitiveBase_AddToList_00b8fdf0.cpp"),
        ("remove", "rebuild/src/compiled/00/b8/CEngineInternalPrimitiveBase_RemoveFromList_00b8fe60.cpp"),
        ("behavior", "rebuild/tests/integration/PrimitiveRemoval_test.cpp")):
        obj = directory / (name + ".obj")
        run([parity.CL_EXE, "/nologo", "/c", "/W3", "/MT", "/GS", "/O2", "/Oy",
             "/I" + str(ROOT / "rebuild/include"), "/Fo" + str(obj), ROOT / source], env)
        objects.append(obj)
    exe = directory / "behavior.exe"
    run([parity.VC / "bin/link.exe", "/nologo", "/subsystem:console", "/out:" + str(exe), *objects], env)
    lines = run([exe, inputs], env).splitlines()
    expected_count = sum(len(case[1]) for case in cases)
    if len(lines) != expected_count: raise RuntimeError("Incomplete removal samples")

    def invoke(address, node, *args):
        machine.mem_write(stack, struct.pack("<" + "I" * (len(args)+1), stop, *args))
        machine.reg_write(UC_X86_REG_ESP, stack)
        machine.reg_write(UC_X86_REG_ECX, node)
        machine.emu_start(address, stop, count=10000)
        if machine.reg_read(UC_X86_REG_EIP) != stop: raise RuntimeError("Retail call did not return")

    def word(address): return struct.unpack("<I", machine.mem_read(address, 4))[0]
    def link(pointer): return -1 if not pointer else (-2 if pointer == head else pointer-nodes)

    errors, sample = [], 0
    for case_index, (masks, removals, flags, with_grid) in enumerate(cases):
        machine.mem_write(head, b"\0"*4)
        machine.mem_write(nodes, b"\0"*(64*0x48))
        machine.mem_write(grid, b"\0"*0x48)
        machine.mem_write(grid+0x34, struct.pack("<I", 100))
        for i, mask in enumerate(masks):
            node = nodes+i*0x48
            machine.mem_write(node+0x20, struct.pack("<I", grid if with_grid else 0))
            machine.mem_write(node+0x30, struct.pack("<IB", mask, flags))
            invoke(0xB8FDF0, node, head)
        for victim in removals:
            invoke(0xB8FE60, nodes+victim*0x48)
            expected = [link(word(head)), word(grid+0x34)]
            for i in range(len(masks)):
                node = nodes+i*0x48
                expected.extend(link(word(node+offset)) for offset in (0x38, 0x3C, 0x40, 0x44))
                expected.append(int(bool(word(node+0x20))))
            actual = list(map(int, lines[sample].split()))
            if actual != expected:
                errors.append({"case": case_index, "victim": victim, "actual": actual, "retail": expected})
            sample += 1
    body, section, symbol = parity.obj_text(objects[1], "FablePrimitiveRemoveFromList")
    offset = pe_oracle.va_to_off(pe_oracle.pe_sections(image), 0xB8FE60)
    retail = image[offset:offset+116]
    status = "MATCH" if body == retail else "DIFFER"
    (directory / "remove.asm").write_text(run([parity.OBJDUMP, "-dr", objects[1]], env))
    accepted = not errors and status == "MATCH"
    report = {"accepted": accepted, "cases": len(cases), "removal_snapshots": sample,
              "errors": errors, "byte_parity": status, "compiled_bytes": len(body),
              "retail_bytes": len(retail), "scope": "complete retail add/remove; no doubles"}
    (directory / "report.json").write_text(json.dumps(report, indent=2)+"\n")
    print(f"PRIMITIVE_REMOVAL {'PASS' if not errors else 'FAIL'} cases={len(cases)} snapshots={sample} failures={len(errors)}")
    print(f"PRIMITIVE_REMOVAL {status} bytes={len(body)}/{len(retail)}")
    return int(not accepted)


if __name__ == "__main__":
    raise SystemExit(main())
