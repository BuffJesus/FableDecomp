"""Check bank-info map iterator traversal and the two retail bodies."""
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
CASES = [
    ('009a9e20', 'CBankInfoMap_IteratorAdvance_009a9e20',
     '??Eiterator@CBankInfoMap@CBankFileManager@@QAEAAV012@XZ', 62, 61, 0.88),
    ('009aa320', 'CBankInfoMap_IteratorRetreat_009aa320',
     '??Fiterator@CBankInfoMap@CBankFileManager@@QAEAAV012@XZ', 79, 79, 1.0),
]
FIXTURE = ROOT / 'rebuild/tests/00/9a/CBankInfoMap_Iterators_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/bank-info-iterators.json'


def main():
    work = Path(tempfile.mkdtemp(prefix='bank-info-iterators-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    checks = []
    with pefile.PE(str(EXE)) as pe:
        for address, name, symbol, retail_size, built_size, threshold in CASES:
            source = ROOT / 'rebuild/src/compiled/00/9a' / (name + '.cpp')
            obj = work / (name + '.obj')
            result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
            assert result.returncode == 0, result.stdout + result.stderr
            objects.append(obj)
            provider = Coff(obj)
            section, offset = provider.symbols[symbol]
            body, relocs = provider.sections[section]
            assert offset == 0 and len(body) == built_size and not relocs
            retail = pe.get_data(int(address, 16) - pe.OPTIONAL_HEADER.ImageBase, retail_size)
            similarity = compare(body, retail, (), min_similarity=threshold)
            assert similarity.near_match, similarity
            checks.append(dict(address=address, candidate_bytes=len(body),
                               retail_bytes=len(retail), similarity=similarity.to_dict(),
                               source_sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    fixture_obj = work / (FIXTURE.stem + '.obj')
    result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(fixture_obj), str(FIXTURE)], env, work)
    assert result.returncode == 0, result.stdout + result.stderr
    objects.append(fixture_obj)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'iterators.exe')]
    for omitted in (0, 1):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'omitted provider linked: ' + CASES[omitted][0]
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'iterators.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'BANK_INFO_ITERATORS_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='NEAR_MATCH', providers=checks,
                                      missing_provider_checks=2,
                                      fixture_sha256=hashlib.sha256(FIXTURE.read_bytes()).hexdigest()), indent=2) + '\n')
    print('BANK_INFO_ITERATORS_PASS advance=61/62 retreat=79/79 omitted=2')


if __name__ == '__main__':
    main()
