"""Validate the genuine zlib deflate-end cleanup body and ownership order."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/00/c0/z_deflateEnd_00c00ae0.cpp'
TEST = ROOT / 'rebuild/tests/00/c0/z_deflateEnd_00c00ae0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/z-deflate-end.json'

def main():
    work = Path(tempfile.mkdtemp(prefix='z-deflate-end-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    coff = Coff(objects[0])
    fixture = Coff(objects[1])
    section, offset = coff.symbols['_z_deflateEnd']
    body, relocs = coff.sections[section]
    assert '_z_deflateEnd' not in fixture.symbols
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x00C00AE0 - pe.OPTIONAL_HEADER.ImageBase, 178)
    assert offset == 0 and len(body) == 178 and not relocs and body == retail
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'Z_DEFLATE_END_PASS cases=66' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='00c00ae0', grade='MATCH', bytes=178,
                                      compiled_cases=66, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('Z_DEFLATE_END_PASS bytes=178 compiled=66 omitted=1')

if __name__ == '__main__':
    main()
