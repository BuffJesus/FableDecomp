#!/usr/bin/env python3
"""Compare completion and completion-edge queries against retail instructions."""
import hashlib
import itertools
import json
import struct

from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_EAX, UC_X86_REG_ECX, UC_X86_REG_EIP, UC_X86_REG_ESP
from check_cgame_play import ROOT, parity, pe_oracle, run


def main():
    image = pe_oracle.EXE.read_bytes()
    if hashlib.sha256(image).hexdigest() != "41dc91090ae853715ac06d2e9fc96e5d545381d197ed55d624c642f34509ac10":
        raise RuntimeError("Retail oracle executable changed")
    machine = Uc(UC_ARCH_X86, UC_MODE_32)
    machine.mem_map(0x400000, 0x1100000)
    for _, va, _, raw, size in pe_oracle.pe_sections(image):
        machine.mem_write(0x400000+va, image[raw:raw+size])
    machine.mem_map(0x20000000, 0x20000)
    stop, stack, receiver = 0x20000000, 0x20008000, 0x20010000
    head, task, vtable, query = 0x20011000, 0x20011020, 0x20012000, 0x20013000
    machine.mem_write(query, b"\xc3")
    calls, sequence = 0, 0

    def hook(machine, address, size, data):
        nonlocal calls
        if address == stop:
            machine.emu_stop()
        elif address == query:
            calls += 1
            machine.reg_write(UC_X86_REG_EAX, (sequence >> (calls-1)) & 1)
        elif address == 0x52C8F0:
            calls += 1
    machine.hook_add(UC_HOOK_CODE, hook)

    def invoke(address):
        machine.mem_write(stack, struct.pack("<I", stop))
        machine.reg_write(UC_X86_REG_ESP, stack)
        machine.reg_write(UC_X86_REG_ECX, receiver)
        machine.reg_write(UC_X86_REG_EAX, 0xDEADBEEF)
        machine.emu_start(address, stop, count=1000)
        if machine.reg_read(UC_X86_REG_EIP) != stop:
            raise RuntimeError("Retail query did not return")
        return machine.reg_read(UC_X86_REG_EAX) & 255

    masks = (0, 1, 2, 3, 4, 7, 0x80000000, 0xFFFFFFFF)
    cases = list(itertools.product(masks, masks, (0, 1), (0, 1, 2, 255), range(5)))
    directory = ROOT / "work/state_progress_check"
    directory.mkdir(parents=True, exist_ok=True)
    inputs = directory / "cases.txt"
    inputs.write_text("\n".join(" ".join(map(str, case)) for case in cases)+"\n")
    sources = ["rebuild/src/compiled/00/52/CChangingStateComponent_HasCompletedStateChange_0052c8f0.cpp",
               "rebuild/src/compiled/00/41/CChangingStateComponent_ChangedStateLastUpdate_0041c5e0.cpp",
               "rebuild/tests/integration/StateProgress_test.cpp"]
    env, objects = parity.env(), []
    for i, source in enumerate(sources):
        obj = directory / f"part{i}.obj"
        run([parity.CL_EXE, "/nologo", "/c", "/W3", "/MT", "/GS", "/O2", "/Oy",
             "/I"+str(ROOT/"rebuild/include"), "/Fo"+str(obj), ROOT/source], env)
        objects.append(obj)
    exe = directory / "behavior.exe"
    run([parity.VC/"bin/link.exe", "/nologo", "/subsystem:console", "/out:"+str(exe), *objects], env)
    lines = run([exe, inputs], env).splitlines()
    if len(lines) != len(cases): raise RuntimeError("Incomplete progress-query results")
    errors = []
    for index, ((pending, done, queued, before, sequence), line) in enumerate(zip(cases, lines)):
        data = bytearray(0x15C)
        struct.pack_into("<I", data, 0, vtable)
        struct.pack_into("<III", data, 0x134, pending, done, head)
        data[0x154] = before
        machine.mem_write(receiver, bytes(data))
        machine.mem_write(head, struct.pack("<I", task if queued else head))
        machine.mem_write(vtable+0xC4, struct.pack("<I", 0x52C8F0 if sequence == 4 else query))
        complete = invoke(0x52C8F0)
        calls = 0
        changed = invoke(0x41C5E0)
        expected = [complete, changed, calls]
        actual = list(map(int, line.split()))
        if expected != actual:
            errors.append({"case": index, "retail": expected, "actual": actual})
    for i in range(2):
        (directory/f"part{i}.asm").write_text(run([parity.OBJDUMP, "-dr", objects[i]], env))
    body, _, _ = parity.obj_text(objects[0], "?FableUiHasCompletedStateChange@@")
    offset = pe_oracle.va_to_off(pe_oracle.pe_sections(image), 0x52C8F0)
    completion_match = body == image[offset:offset+37]
    accepted = not errors and completion_match
    report = {"accepted": accepted, "cases": len(cases), "errors": errors,
              "completion_query_parity": "MATCH" if completion_match else "DIFFER",
              "completion_edge_parity": "DIFFER; functional acceptance",
              "scope": "complete retail queries; real completion query plus controlled virtual query sequences",
              "excludes": "task queue mutation, component Update and state transition processing"}
    (directory/"report.json").write_text(json.dumps(report, indent=2)+"\n")
    print(f"STATE_PROGRESS {'PASS' if not errors else 'FAIL'} cases={len(cases)} failures={len(errors)}")
    print(f"STATE_COMPLETION_QUERY {'MATCH' if completion_match else 'DIFFER'} bytes={len(body)}/37")
    return int(not accepted)


if __name__ == "__main__":
    raise SystemExit(main())
