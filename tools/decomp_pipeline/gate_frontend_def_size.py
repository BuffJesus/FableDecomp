"""Validate the real CFrontEndDef size virtual method."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/43/CFrontEndDef_GetSizeofClass_00431e30.cpp'
TEST = ROOT / 'rebuild/tests/00/43/CFrontEndDef_GetSizeofClass_00431e30_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/frontend-def-size.json'
SYMBOL = '?GetSizeofClass@CFrontEndDef@@UBEKXZ'
ADDRESS = 0x00431E30

def main():
    work = Path(tempfile.mkdtemp(prefix='frontend-def-size-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    provider, fixture = Coff(objects[0]), Coff(objects[1])
    section, offset = provider.symbols[SYMBOL]
    body, relocs = provider.sections[section]
    assert offset == 0 and len(body) == 6 and not relocs
    assert SYMBOL not in fixture.symbols
    with pefile.PE(str(EXE)) as pe:
        assert body == pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 6)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'PASS_00431e30' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='00431e30', grade='MATCH', bytes=6,
                                      compiled_cases=1, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('FRONTEND_DEF_SIZE_PASS bytes=6 compiled=1 omitted=1')

if __name__ == '__main__':
    main()
