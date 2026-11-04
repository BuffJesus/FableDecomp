"""Validate genuine CFontDataBank prepare hook and counted-pointer cleanup."""
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
ADDRESS = 0x00A63420
SYMBOL = '?PrepareDataForInclusion@CFontDataBank@@MAEXKABV?$CArray@VCCharString@@@@KV?$CCountedPointer@VCBankStateBlock@@@@AAVCDataOutputStream@@2@Z'
DELETE = '??3@YAXPAX@Z'

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/a6/CFontDataBank_PrepareDataForInclusion_00a63420.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/00/a6/CFontDataBank_PrepareDataForInclusion_00a63420_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/font-prepare-empty.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='font-prepare-empty-'))
    env = check_one.env()
    obj = work / 'provider.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    code, reloc = coff.sections[section]
    assert offset == 0 and len(code) == 32 and reloc == {21: (DELETE, 20)}, (offset, code.hex(), reloc)
    patched = bytearray(code)
    patched[21:25] = struct.pack('<I', (0x00BFE9BC - ADDRESS - 25) & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 32)
    assert patched == retail, (patched.hex(), retail.hex())
    fixture = work / 'fixture.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(fixture), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(cmd + [str(fixture)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(obj), str(fixture)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'FONT_PREPARE_EMPTY_PASS cases=16' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address='00a63420', symbol=SYMBOL, bytes=32, grade='RELOCATION_MATCH',
                  compiled_cases=16, missing_provider_checks=1, behavior=r.stdout.strip(),
                  source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('FONT_PREPARE_EMPTY_PASS bytes=32 compiled=16 omitted=1')

if __name__ == '__main__':
    main()
