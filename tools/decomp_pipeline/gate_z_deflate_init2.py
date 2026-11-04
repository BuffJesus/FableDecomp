"""Validate the zlib 1.2.1 deflate initializer against retail bytes and allocation behavior."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/c0/z_deflateInit2__00c01f90.cpp'
TEST = ROOT / 'rebuild/tests/00/c0/z_deflateInit2__00c01f90_test.cpp'
HEADER = ROOT / 'rebuild/include/third_party/zlib121/deflate.h'
REPORT = ROOT / 'rebuild/compile-gate/z-deflate-init2.json'
TARGETS = {'_zcalloc': 0x00C06030, '_zcfree': 0x00C06050,
           '_z_deflateReset': 0x00C01B60, '_z_errmsg': 0x012B1780,
           '_z_deflateEnd': 0x00C00AE0}

def main():
    work = Path(tempfile.mkdtemp(prefix='z-deflate-init2-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    coff, fixture = Coff(objects[0]), Coff(objects[1])
    section, offset = coff.symbols['_z_deflateInit2_']
    body, relocs = coff.sections[section]
    assert '_z_deflateInit2_' not in fixture.symbols
    assert offset == 0 and len(body) == 558 and len(relocs) == 5
    candidate = bytearray(body)
    for pos, (symbol, kind) in relocs.items():
        assert symbol in TARGETS and kind in (6, 20)
        addend = struct.unpack_from('<I', body, pos)[0]
        target = TARGETS[symbol] + addend
        struct.pack_into('<I', candidate, pos,
                         (target - 0x00C01F90 - pos - 4 if kind == 20 else target) & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x00C01F90 - pe.OPTIONAL_HEADER.ImageBase, 558)
    assert candidate == retail
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'Z_DEFLATE_INIT2_PASS cases=24' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='00c01f90', grade='RELOCATION_MATCH', bytes=558,
                                      relocations=len(relocs), compiled_cases=24, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      header_sha256=hashlib.sha256(HEADER.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('Z_DEFLATE_INIT2_PASS bytes=558 relocations=5 compiled=24 omitted=1')

if __name__ == '__main__':
    main()
