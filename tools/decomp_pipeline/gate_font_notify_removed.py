"""Validate both font-bank entry-removal routines against retail and linked behavior."""
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
from pe_oracle import EXE

ROOT = check_one.ROOT
SPECS = [
    ('00a635f0', '00/a6/CFontDataBank_NotifyEntryRemoved_00a635f0.cpp',
     '?NotifyEntryRemoved@CFontDataBank@@MAEXK@Z'),
    ('00a5f0b0', '00/a5/CStreamingFontDataBank_NotifyEntryRemoved_00a5f0b0.cpp',
     '?NotifyEntryRemoved@CStreamingFontDataBank@@MAEXK@Z'),
]

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source-dir', type=Path, default=ROOT / 'rebuild/src/compiled')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/00/a6/font_notify_removed_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/font-notify-removed.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='font-notify-removed-'))
    env = check_one.env()
    objs = []
    proof = []
    with pefile.PE(str(EXE)) as pe:
        for address, relative, symbol in SPECS:
            source = (args.source_dir / relative).resolve()
            obj = work / (address + '.obj')
            r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
            assert not r.returncode, r.stdout + r.stderr
            coff = Coff(obj)
            section, offset = coff.symbols[symbol]
            code, reloc = coff.sections[section]
            retail = pe.get_data(int(address, 16) - pe.OPTIONAL_HEADER.ImageBase, 63)
            assert offset == 0 and len(code) == 63 and not reloc and code == retail, (address, code.hex(), retail.hex(), reloc)
            objs.append(obj)
            proof.append(dict(address=address, symbol=symbol, bytes=63,
                              grade='MATCH', source_sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    fixture = work / 'fixture.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(fixture), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(cmd + [str(fixture)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    for omitted in objs:
        r = subprocess.run(cmd + [str(fixture)] + [str(o) for o in objs if o != omitted], env=env, capture_output=True, text=True)
        assert r.returncode, (omitted, 'provider omission linked')
    r = subprocess.run(cmd + [str(fixture)] + [str(o) for o in objs], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'FONT_NOTIFY_REMOVED_PASS cases=16' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(providers=proof, compiled_cases=32, missing_provider_checks=3,
                  behavior=r.stdout.strip(), fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('FONT_NOTIFY_REMOVED_PASS providers=2 bytes=126 compiled=32 omitted=3')

if __name__ == '__main__':
    main()
