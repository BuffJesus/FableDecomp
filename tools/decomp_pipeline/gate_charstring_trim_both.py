"""Verify the retail whitespace wrapper and linked call order."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from near_match import compare, retail_relocations
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/00/99/CCharString_RemoveWhiteSpaceBeginEnd_0099f560.cpp'
TEST = ROOT / 'rebuild/tests/00/99/CCharString_RemoveWhiteSpaceBeginEnd_0099f560_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/charstring-trim-both.json'
SYMBOL = '?RemoveWhiteSpaceBeginEnd@CCharString@@QAEXXZ'
ADDRESS = 0x0099F560


def main():
    work = Path(tempfile.mkdtemp(prefix='charstring-trim-both-'))
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
    assert offset == 0 and len(body) == 16 and len(relocs) == 2
    assert SYMBOL not in fixture.symbols
    assert {symbol for symbol, _ in relocs.values()} == {
        '?RemoveWhiteSpaceBegin@CCharString@@QAEXXZ',
        '?RemoveWhiteSpaceEnd@CCharString@@QAEXXZ',
    }
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 16)
    similarity = compare(body, retail, relocs.keys(), retail_relocations('0099f560', 16))
    assert similarity.near_match and similarity.opcode_similarity == 1.0, similarity
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CHARSTRING_TRIM_BOTH_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='0099f560', grade='RELOCATION_MATCH',
                                      candidate_bytes=16, retail_bytes=16,
                                      similarity=similarity.to_dict(), compiled_cases=1,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CHARSTRING_TRIM_BOTH_PASS exact=16/16 order=1 omitted=1')


if __name__ == '__main__':
    main()
