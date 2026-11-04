"""Validate CConsoleVariable's formatted-name operator+ call."""
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
ADDRESS = 0x009EA490
STRING_ADDRESS = 0x01258C1C
CALLEE_ADDRESS = 0x0099F600
SYMBOL = '?GetFormattedName@CConsoleVariable@@EBE?AVCCharString@@XZ'
STRING_SYMBOL = '??_C@_01CLKCMJKC@?5?$AA@'
CALLEE_SYMBOL = '??H@YI?AVCCharString@@ABV0@PBD@Z'

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/9e/CConsoleVariable_GetFormattedName_009ea490.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/00/9e/CConsoleVariable_GetFormattedName_009ea490_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/console-variable-formatted-name.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='console-variable-formatted-name-'))
    env = check_one.env()
    obj = work / 'provider.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    code, reloc = coff.sections[section]
    assert offset == 0 and len(code) == 26 and reloc == {
        9: (STRING_SYMBOL, 6), 16: (CALLEE_SYMBOL, 20)}, (offset, code.hex(), reloc)
    literal_section, literal_offset = coff.symbols[STRING_SYMBOL]
    literal = coff.sections[literal_section][0][literal_offset:literal_offset + 2]
    assert literal == b' \0'
    patched = bytearray(code)
    patched[9:13] = struct.pack('<I', STRING_ADDRESS)
    patched[16:20] = struct.pack('<I', (CALLEE_ADDRESS - ADDRESS - 20) & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 26)
        retail_literal = pe.get_data(STRING_ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 2)
    assert patched == retail and literal == retail_literal, (patched.hex(), retail.hex())
    fixture = work / 'fixture.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(fixture), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    command = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(command + [str(fixture)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(command + [str(obj), str(fixture)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'CONSOLE_VARIABLE_FORMATTED_NAME_PASS cases=16' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address='009ea490', symbol=SYMBOL, bytes=26, grade='RELOCATION_MATCH',
                  compiled_cases=16, missing_provider_checks=1, behavior=r.stdout.strip(),
                  source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('CONSOLE_VARIABLE_FORMATTED_NAME_PASS bytes=26 compiled=16 omitted=1')

if __name__ == '__main__':
    main()
