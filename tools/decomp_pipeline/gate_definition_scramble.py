"""Check the retail no-op CDefinitionManager::ScrambleDefs body."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/9a/CDefinitionManager_ScrambleDefs_009ac9e0.cpp'
FIXTURE = ROOT / 'rebuild/tests/00/9a/CDefinitionManager_ScrambleDefs_009ac9e0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/definition-scramble.json'
SYMBOL = '?ScrambleDefs@CDefinitionManager@@QAEX_N@Z'


def main():
    work = Path(tempfile.mkdtemp(prefix='definition-scramble-'))
    env = check_one.env()
    objects = []
    for source in (SOURCE, FIXTURE):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    provider = Coff(objects[0])
    section, offset = provider.symbols[SYMBOL]
    body, relocs = provider.sections[section]
    assert offset == 0 and body == b'\xc2\x04\x00' and not relocs
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x009AC9E0 - pe.OPTIONAL_HEADER.ImageBase, 3)
    assert body == retail
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'definition-scramble.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'fixture linked without ScrambleDefs provider'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'definition-scramble.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'DEFINITION_SCRAMBLE_PASS' in result.stdout, (
        result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='MATCH', address='009ac9e0', bytes=3,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(FIXTURE.read_bytes()).hexdigest()), indent=2) + '\n')
    print('DEFINITION_SCRAMBLE_PASS 3/3 omitted=1')


if __name__ == '__main__':
    main()
