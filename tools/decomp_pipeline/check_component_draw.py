#!/usr/bin/env python3
"""Compare readable component traversal with retail, including callback order."""
import hashlib
import json
import random
import struct

from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != "41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10":
        raise RuntimeError("Retail oracle executable changed")
    machine = Uc(UC_ARCH_X86, UC_MODE_32)
    machine.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image):
        machine.mem_write(0x400000 + va, image[raw:raw+size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, parent, vtable, nodes = 0x20000000, 0x20008000, 0x20010000, 0x20011000, 0x20012000
    live, retiring, index_pointer, draw = 0x20014000, 0x20014100, 0x20014200, 0x20015000
    queries = {0x52E7C0: 0, 0x52F180: 1, 0x52F190: 2, 0x52F1C0: 3, 0x52F1D0: 4}
    for slot, target in ((8, draw), (0xD0, 0x52E7C0), (0x190, 0x52F180),
                         (0x194, 0x52F190), (0x1A0, 0x52F1C0), (0x1A4, 0x52F1D0)):
        machine.mem_write(vtable+slot, struct.pack("<I", target))
    machine.mem_write(draw, b"\xC2\x14\x00")
    events, calls, active_mode = [], {}, 0

    def word(address): return struct.unpack("<I", machine.mem_read(address, 4))[0]
    def write_word(address, value): machine.mem_write(address, struct.pack("<I", value))

    def callback(machine, address, size, data):
        if address not in queries and address != draw: return
        receiver = machine.reg_read(UC_X86_REG_ECX)
        identity = word(receiver+0x130)
        if address in queries:
            kind = queries[address]
            events.append(f"{kind}:{identity}")
            if kind == 0 and identity == 1 and active_mode == 2:
                write_word(live, nodes+6*0x200)
            if kind == 4:
                calls[identity] = calls.get(identity, 0)+1
                if identity == 1 and active_mode == 3 and calls[identity] == 2:
                    machine.mem_write(receiver+0x12E, b"\x01")
        else:
            sp = machine.reg_read(UC_X86_REG_ESP)
            engine, handle, layer, counter, owner = struct.unpack("<IIiII", machine.mem_read(sp+4, 20))
            index = word(counter)
            events.append(f"5:{identity}:{layer}:{int(engine == 0x11111111)}:{int(handle == 0x22222222)}:{index}:{int(owner == parent)}")
            write_word(counter, index+1)
            if identity == 1 and active_mode == 1: write_word(parent+0xB4, live)
    machine.hook_add(UC_HOOK_CODE, callback)
    rng = random.Random(0x530260)
    cases = []
    for _ in range(512):
        header = [rng.choice((0, 0x20, 0x40, 0x60)), rng.choice((-128, -3, 0, 7, 127)),
                  rng.choice((-8, 0, 3, 13)), rng.randrange(4), rng.randrange(4), 0]
        children = [[rng.choice((0, 0x80)), rng.randrange(2), rng.randrange(2)] for _ in range(7)]
        cases.append((header, children))
    for mode in (1, 2, 3):
        cases.append(([0, 7, 3, 3, 3, mode], [[0, 0, 1] for _ in range(7)]))
    directory = ROOT / "work/component_draw_check"
    directory.mkdir(parents=True, exist_ok=True)
    inputs = directory / "cases.txt"
    inputs.write_text("\n".join(" ".join(map(str, [*header, *[n for child in children for n in child]]))
                                 for header, children in cases)+"\n")
    env, objects = parity.env(), []
    for name, source in (("draw", "rebuild/src/compiled/00/53/CComponent_Draw_00530260.cpp"),
                         ("behavior", "rebuild/tests/integration/ComponentDraw_test.cpp")):
        obj = directory/(name+".obj")
        run([parity.CL_EXE, "/nologo", "/c", "/W3", "/MT", "/GS", "/O2", "/Oy",
             "/I"+str(ROOT/"rebuild/include"), "/Fo"+str(obj), ROOT/source], env)
        objects.append(obj)
    exe = directory/"behavior.exe"
    run([parity.VC/"bin/link.exe", "/nologo", "/subsystem:console", "/out:"+str(exe), *objects], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases): raise RuntimeError("Incomplete traversal traces")
    errors = []
    for case_index, ((header, children), actual) in enumerate(zip(cases, lines)):
        flags, layer, parent_layer, live_count, retiring_count, active_mode = header
        events.clear(); calls.clear()
        for i, (child_flags, hidden, owns) in enumerate([[flags, 0, 0], *children]):
            receiver = parent if i == 0 else nodes+(i-1)*0x200
            machine.mem_write(receiver, b"\0"*0x180)
            write_word(receiver, vtable); write_word(receiver+0x130, i)
            write_word(receiver+0xC8, parent if owns else 0)
            machine.mem_write(receiver+0x12C, bytes((child_flags, 0, hidden, layer & 255 if i == 0 else 0)))
        machine.mem_write(live, b"".join(struct.pack("<II", nodes+i*0x200, 0) for i in range(3)))
        machine.mem_write(retiring, b"".join(struct.pack("<II", nodes+(i+3)*0x200, 0) for i in range(3)))
        machine.mem_write(parent+0xB0, struct.pack("<IIIIII", live, live+live_count*8, live+24,
                                                retiring, retiring+retiring_count*8, retiring+24))
        write_word(index_pointer, 9)
        machine.mem_write(stack, struct.pack("<IIIiII", stop, 0x11111111, 0x22222222, parent_layer, index_pointer, 0))
        machine.reg_write(UC_X86_REG_ESP, stack); machine.reg_write(UC_X86_REG_ECX, parent)
        machine.emu_start(0x530260, stop, count=10000)
        if machine.reg_read(UC_X86_REG_EIP) != stop: raise RuntimeError("Retail traversal did not return")
        expected = "TRACE " + " ".join(events) + f" END:{word(index_pointer)}"
        if actual != expected: errors.append({"case": case_index, "actual": actual, "retail": expected})
    (directory/"draw.asm").write_text(run([parity.OBJDUMP, "-dr", objects[0]], env))
    report = {"accepted": not errors, "cases": len(cases), "errors": errors,
              "scope": "native Draw and query bodies; Draw callbacks doubled, three explicit mutation scenarios",
              "byte_parity": "not claimed; traversal helper factored into readable C++"}
    (directory/"report.json").write_text(json.dumps(report, indent=2)+"\n")
    print(f"COMPONENT_DRAW {'PASS' if not errors else 'FAIL'} cases={len(cases)} failures={len(errors)}")
    return int(bool(errors))


if __name__ == "__main__":
    raise SystemExit(main())
