"""Check timer static storage against retail and its real constructor/GetInfo users."""
import argparse
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SPECS = [
    ('?GPUBlocks@CVsyncCallbackTimer@@0RCUCGPUInfoBlock@1@C', 0x013CB4B0, 48),
    ('?CPUTimeStamp@CVsyncCallbackTimer@@0NA', 0x013CB4E0, 8),
    ('?CPUPrevTimeStamp@CVsyncCallbackTimer@@0NA', 0x013CB4E8, 8),
    ('?CPUFrameCount@CVsyncCallbackTimer@@0JA', 0x013CB4F0, 4),
    ('?LatestBlockIndex@CVsyncCallbackTimer@@0JC', 0x013CB4F4, 4),
]
CALLERS = [
    ('CVsyncCallbackTimer_Constructor_009f70e0.cpp', '/Od', ['/Ob1']),
    ('CVsyncCallbackTimer_Destructor_009f7330.cpp', '/Od', ['/Ob1']),
    ('CVsyncCallbackTimer_GetInfo_009f7050.cpp', '/O2', []),
]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT/'rebuild/src/compiled/01/3c/CVsyncCallbackTimer_StaticData_013cb4b0.cpp')
    ap.add_argument('--test', type=Path, default=ROOT/'rebuild/tests/integration/TimerStaticData_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT/'rebuild/compile-gate/timer-static-data.json')
    args = ap.parse_args()
    env = check_one.env()
    work = Path(tempfile.mkdtemp(prefix='timer-static-data-'))
    objects, providers = [], []
    sources = [(args.source, '/O2', [])] + [
        (ROOT/'rebuild/src/compiled/00/9f'/name, opt, flags)
        for name, opt, flags in CALLERS] + [(args.test, '/O2', [])]
    for i, (source, opt, flags) in enumerate(sources):
        obj = work/('source%d.obj' % i)
        result = check_one.cl(['/nologo', '/c', opt, *flags, '/MT', '/Fo'+str(obj), str(source.resolve())], env, work)
        assert result.returncode == 0, result.stdout+result.stderr
        objects.append(obj)
        providers.append(dict(source=str(source), sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    data, fixture = Coff(objects[0]), Coff(objects[-1])
    assert not any(symbol in fixture.symbols for symbol, _, _ in SPECS)
    checked = []
    with pefile.PE(str(EXE)) as pe:
        for symbol, address, size in SPECS:
            rva = address-pe.OPTIONAL_HEADER.ImageBase
            section = next(s for s in pe.sections if s.VirtualAddress <= rva and rva+size <= s.VirtualAddress+s.Misc_VirtualSize)
            assert section.Name.rstrip(b'\0') == b'.data'
            assert rva >= section.VirtualAddress+section.SizeOfRawData, symbol
            assert coff_data(data, symbol, size) == bytes(size), symbol
            checked.append(dict(symbol=symbol, address='%08x' % address, size=size, value='zero-filled .data tail'))
    assert len(set(data.symbols) & {s for s, _, _ in SPECS}) == len(SPECS)
    exe = work/'fixture.exe'
    link = [str(check_one.VC/'bin/link.exe'), '/nologo', '/out:'+str(exe), 'kernel32.lib']
    for omit in range(len(objects)-1):
        result = subprocess.run(link+[str(o) for i, o in enumerate(objects) if i != omit], env=env, capture_output=True, text=True)
        assert result.returncode, ('provider omission linked', sources[omit][0])
        assert 'LNK2001' in result.stdout or 'LNK2019' in result.stdout, result.stdout+result.stderr
    result = subprocess.run(link+list(map(str, objects)), env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout+result.stderr
    result = subprocess.run([str(exe)], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'TIMER_STATIC_DATA_BEHAVIOR_PASS scenarios=512' in result.stdout, result.stdout+result.stderr
    report = dict(data_symbols=checked, data_bytes=sum(n for _, _, n in SPECS), behavior=result.stdout.strip(),
                  providers=providers, missing_provider_checks=len(objects)-1, coverage_claim='data only; no new reconstructed functions')
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2)+'\n')
    print('TIMER_STATIC_DATA_PASS symbols=5 bytes=72 compiled=512')


if __name__ == '__main__':
    main()
