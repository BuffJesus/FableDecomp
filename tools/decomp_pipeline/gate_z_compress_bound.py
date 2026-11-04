"""Validate the retail zlib 1.2.1 compression bound helper."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/c0/z_compressBound_00c03eb0.cpp'
TEST = ROOT / 'rebuild/tests/00/c0/z_compressBound_00c03eb0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/z-compress-bound.json'

def main():
    work = Path(tempfile.mkdtemp(prefix='z-compress-bound-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    coff = Coff(objects[0])
    section, offset = coff.symbols['_z_compressBound']
    body, relocs = coff.sections[section]
    assert offset == 0 and len(body) == 21 and not relocs
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x00C03EB0 - pe.OPTIONAL_HEADER.ImageBase, 21)
    assert body == retail
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'Z_COMPRESS_BOUND_PASS cases=11' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='00c03eb0', grade='MATCH', bytes=21,
                                      compiled_cases=11, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('Z_COMPRESS_BOUND_PASS bytes=21 compiled=11 omitted=1')

if __name__ == '__main__':
    main()
