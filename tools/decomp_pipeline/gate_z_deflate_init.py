"""Validate the retail zlib deflate-init forwarding wrapper."""
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

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/00/c0/z_deflateInit__00c021c0.cpp'
TEST = ROOT / 'rebuild/tests/00/c0/z_deflateInit__00c021c0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/z-deflate-init.json'

def main():
    work = Path(tempfile.mkdtemp(prefix='z-deflate-init-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    coff, fixture = Coff(objects[0]), Coff(objects[1])
    section, offset = coff.symbols['_z_deflateInit_']
    body, relocs = coff.sections[section]
    assert '_z_deflateInit_' not in fixture.symbols
    assert offset == 0 and len(body) == 37 and len(relocs) == 1
    patched = bytearray(body)
    for pos, (symbol, kind) in relocs.items():
        assert symbol == '_z_deflateInit2_' and kind == 20
        struct.pack_into('<I', patched, pos, (0x00C01F90 - 0x00C021C0 - pos - 4) & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x00C021C0 - pe.OPTIONAL_HEADER.ImageBase, 37)
    assert patched == retail
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'Z_DEFLATE_INIT_PASS cases=11' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='00c021c0', grade='RELOCATION_MATCH', bytes=37,
                                      compiled_cases=11, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('Z_DEFLATE_INIT_PASS bytes=37 compiled=11 omitted=1')

if __name__ == '__main__':
    main()
