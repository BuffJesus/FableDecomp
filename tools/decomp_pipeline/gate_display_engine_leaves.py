"""Validate display initialization and resolution forwarders with real native providers."""
import argparse
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from unicorn import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import *

ROOT = Path(__file__).resolve().parents[2]
SPECS = [
    dict(address='00419270', file='CDisplayEngineInit_CDisplayEngineInit_00419270.cpp', symbol='??0CDisplayEngineInit@@QAE@XZ', size=35, argc=0),
    dict(address='00434a10', file='CDisplayEngine_PreChangeResolution_00434a10.cpp', symbol='?PreChangeResolution@CDisplayEngine@@QAEXXZ', size=8, argc=0),
    dict(address='00434a20', file='CDisplayEngine_PostChangeResolution_00434a20.cpp', symbol='?PostChangeResolution@CDisplayEngine@@QAEXKKK@Z', size=8, argc=3),
    dict(address='00434dc0', file='CDisplayEngine_ChangeTextureColourDepth_00434dc0.cpp', symbol='?ChangeTextureColourDepth@CDisplayEngine@@QAEXJ@Z', size=3, argc=1),
]


def execute(code, spec, pattern, result):
    address = int(spec['address'], 16)
    obj, engine, table, stack, stop, callback = 0x2000010, 0x2100010, 0x2200000, 0x3008000, 0x4000000, 0x5000000
    u = Uc(UC_ARCH_X86, UC_MODE_32)
    for base, size in [(address & ~4095, 4096), (obj & ~4095, 4096), (engine & ~4095, 4096), (table, 4096), (stack & ~65535, 65536), (stop, 4096), (callback, 4096)]:
        u.mem_map(base, size)
    u.mem_write(address, bytes(code))
    initial = bytearray(bytes([pattern])*0x110)
    struct.pack_into('<I', initial, 0x10+0x2c, engine)
    u.mem_write(obj-0x10, bytes(initial))
    u.mem_write(engine, struct.pack('<III', table, 0xabc, 0xdef))
    slot = 16 if spec['argc'] == 3 else 15
    u.mem_write(table+slot*4, struct.pack('<I', callback))
    args = [pattern*0x01010101, (~(pattern*0x01010101)) & 0xffffffff, pattern][:spec['argc']]
    u.mem_write(stack, struct.pack('<'+'I'*(len(args)+1), stop, *args))
    u.reg_write(UC_X86_REG_ECX, obj)
    u.reg_write(UC_X86_REG_ESP, stack)
    saved = [UC_X86_REG_EBX, UC_X86_REG_ESI, UC_X86_REG_EDI, UC_X86_REG_EBP]
    for i, reg in enumerate(saved):
        u.reg_write(reg, 0x12345000+i)
    trace = []

    def hook(uc, pc, size, data):
        if pc != callback:
            return
        sp = uc.reg_read(UC_X86_REG_ESP)
        trace.append((uc.reg_read(UC_X86_REG_ECX), list(struct.unpack('<'+'I'*len(args), uc.mem_read(sp+4, 4*len(args))))))
        uc.reg_write(UC_X86_REG_EAX, result)
        uc.reg_write(UC_X86_REG_EIP, struct.unpack('<I', uc.mem_read(sp, 4))[0])
        uc.reg_write(UC_X86_REG_ESP, sp+4+4*len(args))

    u.hook_add(UC_HOOK_CODE, hook)
    u.emu_start(address, stop, count=100)
    assert u.reg_read(UC_X86_REG_ESP) == stack+4+4*len(args)
    assert all(u.reg_read(reg) == 0x12345000+i for i, reg in enumerate(saved))
    expected = initial
    if spec['address'] == '00419270':
        struct.pack_into('<9I', expected, 0x10, 0, 0, 0, 0, 0, 0, 0, 0xffffffff, 0)
        assert u.reg_read(UC_X86_REG_EAX) == obj
    elif spec['address'] in ('00434a10', '00434a20'):
        assert trace == [(engine, args)]
    else:
        assert not trace
    memory = bytes(u.mem_read(obj-0x10, 0x110))
    assert memory == expected
    assert bytes(u.mem_read(engine, 12)) == struct.pack('<III', table, 0xabc, 0xdef)
    return memory, trace


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--stage', type=Path)
    ap.add_argument('--test', type=Path, default=ROOT/'rebuild/tests/integration/DisplayEngineLeaves_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT/'rebuild/compile-gate/display-engine-leaves.json')
    args = ap.parse_args()
    env, work = check_one.env(), Path(tempfile.mkdtemp(prefix='display-engine-leaves-'))
    objects, providers, entries = [], [], []
    comparisons = 0
    with pefile.PE(str(EXE)) as pe:
        for i, spec in enumerate(SPECS):
            source = args.stage/spec['file'] if args.stage else ROOT/'rebuild/src/compiled'/spec['address'][:2]/spec['address'][2:4]/spec['file']
            obj = work/('source%d.obj' % i)
            result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo'+str(obj), str(source.resolve())], env, work)
            assert result.returncode == 0, result.stdout+result.stderr
            objects.append(obj)
            providers.append(dict(source=str(source), sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
            coff = Coff(obj)
            sec, offset = coff.symbols[spec['symbol']]
            built, relocs = coff.sections[sec]
            assert offset == 0 and not relocs and len(built) == spec['size']
            native = pe.get_data(int(spec['address'], 16)-pe.OPTIONAL_HEADER.ImageBase, len(built))
            assert built == native, spec
            for pattern in range(256):
                for returned in range(2 if spec['address'] in ('00434a10', '00434a20') else 1):
                    assert execute(built, spec, pattern, returned) == execute(native, spec, pattern, returned)
                    comparisons += 1
            entries.append(dict(spec, grade='MATCH'))
    fixture = work/'test.obj'
    result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo'+str(fixture), str(args.test.resolve())], env, work)
    assert result.returncode == 0, result.stdout+result.stderr
    assert not any(s['symbol'] in Coff(fixture).symbols for s in SPECS)
    objects.append(fixture)
    exe = work/'fixture.exe'
    link = [str(check_one.VC/'bin/link.exe'), '/nologo', '/out:'+str(exe)]
    for omit in range(len(SPECS)):
        result = subprocess.run(link+[str(o) for i, o in enumerate(objects) if i != omit], env=env, capture_output=True, text=True)
        assert result.returncode and ('LNK2001' in result.stdout or 'LNK2019' in result.stdout), result.stdout+result.stderr
    result = subprocess.run(link+list(map(str, objects)), env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout+result.stderr
    result = subprocess.run([str(exe)], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'DISPLAY_ENGINE_LEAVES_BEHAVIOR_PASS scenarios=768' in result.stdout, result.stdout+result.stderr
    report = dict(entries=entries, providers=providers, comparative_scenarios=comparisons, behavior=result.stdout.strip(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(), missing_provider_checks=len(SPECS))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    print('DISPLAY_ENGINE_LEAVES_PASS providers=4 bytes=54 compiled=768 comparative=%d' % comparisons)


if __name__ == '__main__':
    main()
