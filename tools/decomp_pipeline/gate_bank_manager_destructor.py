"""Verify the bank-manager destructor shape and teardown behavior."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/9a/CBankFileManager_Destructor_009a7b70.cpp'
TEST = ROOT / 'rebuild/tests/00/9a/CBankFileManager_Destructor_009a7b70_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/bank-manager-destructor.json'
SYMBOL = '??1CBankFileManager@@UAE@XZ'
ADDRESS = 0x009A7B70


def main():
    work = Path(tempfile.mkdtemp(prefix='bank-manager-destructor-'))
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
    assert offset == 0 and len(body) == 229 and len(relocs) == 12
    assert SYMBOL not in fixture.symbols
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 229)
    similarity = compare(body, retail, relocs.keys(), retail_relocations('009a7b70', 229))
    assert similarity.near_match and similarity.opcode_similarity >= 0.98, similarity
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'BANK_MANAGER_DESTRUCTOR_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='009a7b70', grade='NEAR_MATCH',
                                      candidate_bytes=229, retail_bytes=229,
                                      similarity=similarity.to_dict(), compiled_cases=7,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('BANK_MANAGER_DESTRUCTOR_PASS near=229/229 teardown=7 omitted=1')


if __name__ == '__main__':
    main()
