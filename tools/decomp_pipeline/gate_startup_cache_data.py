"""Validate the seven startup cache pointers as retail zero-filled data."""
import argparse
import hashlib
import json
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / 'tools/decomp_pipeline'))
import check_one
import pefile
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE

ROOT = check_one.ROOT
NAMES = ['GraphicBankManager', 'RenderState', 'RenderManager2D', 'InputManager',
         'FontManager', 'DisplayManager', 'SoundManager']
ADDRESSES = [0x013B837C + i * 4 for i in range(7)]
SYMBOLS = [f'?g_Fable{n}_{a:08X}@@3' +
           ('PAXA' if n == 'RenderState' else 'PAVC' + ('ASoundManager' if n == 'SoundManager' else n) + '@@A')
           for n, a in zip(NAMES, ADDRESSES)]

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/01/3b/StartupCacheData_013b837c.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/StartupCacheData_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/startup-cache-data.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='startup-cache-data-'))
    env = check_one.env()
    objs = []
    for source in (args.source, args.test):
        obj = work / (source.stem + '.obj')
        r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source.resolve())], env, work)
        assert not r.returncode, r.stdout + r.stderr
        objs.append(obj)
    data = Coff(objs[0])
    fixture = Coff(objs[1])
    with pefile.PE(str(EXE)) as pe:
        for address, symbol in zip(ADDRESSES, SYMBOLS):
            assert symbol in data.symbols and symbol not in fixture.symbols, (address, symbol)
            assert coff_data(data, symbol, 4) == bytes(4)
            rva = address - pe.OPTIONAL_HEADER.ImageBase
            section = next(s for s in pe.sections if s.VirtualAddress <= rva and rva + 4 <= s.VirtualAddress + s.Misc_VirtualSize)
            assert section.Name.rstrip(b'\0') == b'.data'
            assert rva - section.VirtualAddress >= section.SizeOfRawData
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(cmd + [str(objs[1])], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(o) for o in objs], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'STARTUP_CACHE_DATA_PASS cases=16' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(addresses=[f'{a:08x}' for a in ADDRESSES], data_bytes=28,
                  compiled_cases=16, missing_provider_checks=1, behavior=r.stdout.strip(),
                  coverage_claim='data only; no new reconstructed functions',
                  source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('STARTUP_CACHE_DATA_PASS bytes=28 compiled=16 omitted=1')

if __name__ == '__main__':
    main()
