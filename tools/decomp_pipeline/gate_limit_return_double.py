"""Check the typed double limit helper against retail and linked edge cases."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from near_match import compare
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/00/41/global_GFLimitReturndouble_0041919c.cpp'
FIXTURE = ROOT / 'rebuild/tests/00/41/global_GFLimitReturndouble_0041919c_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/limit-return-double.json'
SYMBOL = '??$GFLimitReturn@N@@YIABNABN00@Z'


def main():
    work = Path(tempfile.mkdtemp(prefix='limit-return-double-'))
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
    assert offset == 0 and len(body) == 37 and not relocs
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x0041919C - pe.OPTIONAL_HEADER.ImageBase, 37)
    similarity = compare(body, retail, (), min_similarity=0.875)
    assert similarity.near_match, similarity

    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'limit-return-double.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'fixture linked without the real provider'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'limit-return-double.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'LIMIT_RETURN_DOUBLE_PASS' in result.stdout, (
        result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='NEAR_MATCH', address='0041919c',
                                      candidate_bytes=len(body), retail_bytes=len(retail),
                                      similarity=similarity.to_dict(), missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(FIXTURE.read_bytes()).hexdigest()), indent=2) + '\n')
    print('LIMIT_RETURN_DOUBLE_PASS 37/37 omitted=1')


if __name__ == '__main__':
    main()
