"""Validate CConsoleCommandBase's retail type-string return and relocations."""
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
ADDRESS = 0x00414f70
STRING_ADDRESS = 0x0122E650
CTOR_ADDRESS = 0x0099EBF0
SYMBOL = '?GetTypeAsString@CConsoleCommandBase@@MBE?AVCCharString@@XZ'
STRING_SYMBOL = '??_C@_07KMMNMMFF@command?$AA@'
CTOR_SYMBOL = '??0CCharString@@QAE@PBDJ@Z'

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/41/CConsoleCommandBase_GetTypeAsString_00414f70.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/00/41/CConsoleCommandBase_GetTypeAsString_00414f70_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/console-command-type-string.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='console-command-type-string-'))
    env = check_one.env()
    obj = work / 'provider.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    code, reloc = coff.sections[section]
    assert offset == 0 and len(code) == 25 and reloc == {
        8: (STRING_SYMBOL, 6), 15: (CTOR_SYMBOL, 20)}, (offset, code.hex(), reloc)
    literal_section, literal_offset = coff.symbols[STRING_SYMBOL]
    literal = coff.sections[literal_section][0][literal_offset:literal_offset + 8]
    assert literal == b'command\0'
    patched = bytearray(code)
    patched[8:12] = struct.pack('<I', STRING_ADDRESS)
    patched[15:19] = struct.pack('<I', (CTOR_ADDRESS - ADDRESS - 19) & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 25)
        retail_literal = pe.get_data(STRING_ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 8)
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
    assert not r.returncode and 'CONSOLE_COMMAND_TYPE_STRING_PASS cases=16' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address='00414f70', symbol=SYMBOL, bytes=25, grade='RELOCATION_MATCH',
                  compiled_cases=16, missing_provider_checks=1, behavior=r.stdout.strip(),
                  source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('CONSOLE_COMMAND_TYPE_STRING_PASS bytes=25 compiled=16 omitted=1')

if __name__ == '__main__':
    main()
