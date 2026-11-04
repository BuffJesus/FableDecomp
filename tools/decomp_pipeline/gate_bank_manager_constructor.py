"""Verify the bank-manager constructor layout and retail control flow."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/9a/CBankFileManager_Constructor_009a7ac0.cpp'
TEST = ROOT / 'rebuild/tests/00/9a/CBankFileManager_Constructor_009a7ac0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/bank-manager-constructor.json'
SYMBOL = '??0CBankFileManager@@QAE@XZ'
ADDRESS = 0x009A7AC0


def main():
    work = Path(tempfile.mkdtemp(prefix='bank-manager-constructor-'))
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
    assert offset == 0 and len(body) == 172 and len(relocs) == 7
    assert SYMBOL not in fixture.symbols
    assert {symbol for symbol, _ in relocs.values()} == {
        '??0CBaseClass@@QAE@XZ', '??_7CBankFileManager@@6B@',
        '_malloc', '??0CWideString@@QAE@XZ',
    }
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 172)
    similarity = compare(body, retail, relocs.keys(), retail_relocations('009a7ac0', 172))
    assert similarity.near_match and similarity.opcode_similarity >= 0.96, similarity
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'BANK_MANAGER_CONSTRUCTOR_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='009a7ac0', grade='NEAR_MATCH',
                                      candidate_bytes=172, retail_bytes=172,
                                      similarity=similarity.to_dict(), compiled_cases=6,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('BANK_MANAGER_CONSTRUCTOR_PASS near=172/172 layout=6 omitted=1')


if __name__ == '__main__':
    main()
