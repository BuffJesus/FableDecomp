"""Link GFIntToCharString through the real CCharString::Sprintf provider."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import check_one
from gate_game_component_abi import Coff

ROOT = Path(__file__).resolve().parents[2]
SOURCES = [
    ROOT / 'rebuild/src/compiled/00/99/Global_GFIntToCharString_0099f830.cpp',
    ROOT / 'rebuild/src/compiled/00/99/CCharString_Sprintf_0099f1f0.cpp',
]
TEST = ROOT / 'rebuild/tests/integration/GFIntToCharString_Composed_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/gfint-composed.json'
SYMBOLS = ['?GFIntToCharString@@YI?AVCCharString@@J@Z',
           '?Sprintf@CCharString@@QAAXPBDZZ']


def main():
    work = Path(tempfile.mkdtemp(prefix='gfint-composed-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in SOURCES + [TEST]:
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    for i, symbol in enumerate(SYMBOLS):
        assert symbol in Coff(objects[i]).symbols
        assert symbol not in Coff(objects[-1]).symbols
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    for omitted in range(2):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'provider omission linked: ' + str(omitted)
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'GFINT_COMPOSED_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(compiled_cases=5, missing_provider_checks=2,
                                      source_sha256=[hashlib.sha256(p.read_bytes()).hexdigest() for p in SOURCES],
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('GFINT_COMPOSED_PASS compiled=5 omitted=2')


if __name__ == '__main__':
    main()
