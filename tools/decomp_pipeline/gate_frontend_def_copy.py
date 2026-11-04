"""Validate CFrontEndDef's out-of-line assignment forwarder."""
import argparse
import hashlib
import json
import struct
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
ADDRESS = 0x00432E50
CALLEE = 0x00432E60
SYMBOL = '?Copy@CFrontEndDef@@UAEXPBVCDefClassBase@@@Z'
ASSIGN = '??4CFrontEndDef@@QAEAAV0@ABV0@@Z'

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/43/CFrontEndDef_Copy_00432e50.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/00/43/CFrontEndDef_Copy_00432e50_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/frontend-def-copy.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='frontend-def-copy-'))
    env = check_one.env()
    obj = work / 'provider.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    code, reloc = coff.sections[section]
    assert offset == 0 and len(code) == 5 and reloc == {1: (ASSIGN, 20)}, (offset, code.hex(), reloc)
    assert ASSIGN not in coff.symbols, 'assignment should remain an external provider'
    patched = bytearray(code)
    patched[1:5] = struct.pack('<I', (CALLEE - ADDRESS - 5) & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 5)
    assert patched == retail, (patched.hex(), retail.hex())
    fixture = work / 'fixture.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(fixture), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    assert ASSIGN in Coff(fixture).symbols
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(cmd + [str(fixture)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(obj), str(fixture)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'FRONTEND_DEF_COPY_PASS cases=16' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address='00432e50', symbol=SYMBOL, bytes=5, grade='RELOCATION_MATCH',
                  compiled_cases=16, missing_provider_checks=1, behavior=r.stdout.strip(),
                  source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),
                  header_sha256=hashlib.sha256((args.source.parent / 'fable_frontend_def.h').read_bytes()).hexdigest() if (args.source.parent / 'fable_frontend_def.h').exists() else hashlib.sha256((ROOT / 'rebuild/include/fable_frontend_def.h').read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('FRONTEND_DEF_COPY_PASS bytes=5 compiled=16 omitted=1')

if __name__ == '__main__':
    main()
