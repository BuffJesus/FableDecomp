"""Verify STLport's wide-filename sort wrapper and its helper routing."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import pefile
import check_one
from gate_game_component_abi import Coff
from near_match import compare
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/00/9b/STL_Sort_WideString_009b8440.cpp'
TEST = ROOT / 'rebuild/tests/00/9b/STL_Sort_WideString_009b8440_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/sort-widestring.json'
SYMBOL = '??$sort@PAVCWideString@@U?$less@VCWideString@@@_STL@@@_STL@@YIXPAVCWideString@@0U?$less@VCWideString@@@0@@Z'
ADDRESS = 0x009B8440
SIZE = 117


def main():
    work = Path(tempfile.mkdtemp(prefix='sort-widestring-'))
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
    body, relocations = provider.sections[section]
    body = body[offset:]
    assert len(body) == 112 and len(relocations) == 4
    assert SYMBOL not in fixture.symbols
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, SIZE)
    similarity = compare(body, retail, relocations.keys())
    assert similarity.complete_decode and similarity.opcode_similarity >= 0.8, similarity
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'SORT_WIDESTRING_OK' in result.stdout, (
        result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='009b8440', grade='BEHAVIORAL',
                                      retail_bytes=SIZE, compiled_bytes=len(body),
                                      relocations=len(relocations), linked_cases=4,
                                      omitted_provider_checks=1,
                                      similarity=similarity.to_dict(),
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('SORT_WIDESTRING_PASS retail=117 compiled=112 cases=4 omitted=1')


if __name__ == '__main__':
    main()
