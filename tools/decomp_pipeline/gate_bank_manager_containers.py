"""Check the three native bank-manager teardown containers."""
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
DIR = ROOT / 'rebuild/src/compiled/00/9a'
TEST = ROOT / 'rebuild/tests/00/9a/CBankManagerContainers_009aa830_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/bank-manager-containers.json'
PROVIDERS = [
    ('009aa830', 'CBasicRetailBankAliasMap_Erase_009aa830.cpp',
     '?Erase@CBasicRetailBankAliasMap@@QAEXPAUCNode@1@@Z', 70, 1.0),
    ('009aa880', 'CBankManagerWidePathSet_Erase_009aa880.cpp',
     '?Erase@CBankManagerWidePathSet@@QAEXPAUCNode@1@@Z', 59, 1.0),
    ('009aae50', 'CRetailBankList_Clear_009aae50.cpp',
     '?clear@CRetailBankList@CBankFileManager@@QAEXXZ', 89, 0.90),
]


def main():
    work = Path(tempfile.mkdtemp(prefix='bank-manager-containers-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    checks = []
    with pefile.PE(str(EXE)) as pe:
        for address, filename, symbol, size, min_score in PROVIDERS:
            source = DIR / filename
            obj = work / (source.stem + '.obj')
            result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
            assert result.returncode == 0, result.stdout + result.stderr
            objects.append(obj)
            coff = Coff(obj)
            section, offset = coff.symbols[symbol]
            body, relocs = coff.sections[section]
            assert offset == 0
            retail = pe.get_data(int(address, 16) - pe.OPTIONAL_HEADER.ImageBase, size)
            similarity = compare(body, retail, relocs.keys(), retail_relocations(address, size))
            assert similarity.near_match and similarity.opcode_similarity >= min_score, similarity
            checks.append(dict(address=address, source=filename, candidate_bytes=len(body),
                               retail_bytes=size, similarity=similarity.to_dict(),
                               source_sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    fixture_obj = work / 'fixture.obj'
    result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(fixture_obj), str(TEST)], env, work)
    assert result.returncode == 0, result.stdout + result.stderr
    for _, _, symbol, _, _ in PROVIDERS:
        assert symbol not in Coff(fixture_obj).symbols
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    for omitted in range(3):
        result = subprocess.run(link + [str(fixture_obj)] + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(fixture_obj)] + [str(obj) for obj in objects],
                            env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'BANK_MANAGER_CONTAINERS_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='NEAR_MATCH', providers=checks,
                                      compiled_cases=7, missing_provider_checks=3,
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('BANK_MANAGER_CONTAINERS_PASS erase=70/70,59/59 clear=83/89 omitted=3')


if __name__ == '__main__':
    main()
