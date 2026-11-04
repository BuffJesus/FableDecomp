"""Verify the bank-info map erase body and recursive destructor order."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/9a/CBankInfoMap_Erase_009aa7c0.cpp'
TEST = ROOT / 'rebuild/tests/00/9a/CBankInfoMap_Erase_009aa7c0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/bank-info-erase.json'
SYMBOL = '?Erase@CBankInfoMap@CBankFileManager@@QAEXPAUCNode@12@@Z'
ADDRESS = 0x009AA7C0


def main():
    work = Path(tempfile.mkdtemp(prefix='bank-info-erase-'))
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
    assert offset == 0 and len(body) == 100 and len(relocs) == 7
    assert SYMBOL not in fixture.symbols
    assert {symbol for symbol, _ in relocs.values()} == {
        SYMBOL, '??1CWideString@@QAE@XZ', '??1CCharString@@QAE@XZ', '_free',
    }
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 100)
    similarity = compare(body, retail, relocs.keys(), retail_relocations('009aa7c0', 100))
    assert similarity.near_match and similarity.opcode_similarity == 1.0, similarity
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'BANK_INFO_ERASE_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    composed = ROOT / 'rebuild/tests/00/9a/CBankFileManager_CloseAndErase_composed_test.cpp'
    close_source = ROOT / 'rebuild/src/compiled/00/9a/CBankFileManager_Close_009a7990.cpp'
    more_objects = []
    for source in (close_source, composed):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        more_objects.append(obj)
    composed_link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'composed.exe')]
    result = subprocess.run(composed_link + [str(obj) for obj in more_objects], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'composed fixture linked without map erase'
    result = subprocess.run(composed_link + [str(objects[0])] + [str(obj) for obj in more_objects],
                            env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'composed.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'BANK_CLOSE_ERASE_COMPOSED_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='009aa7c0', grade='RELOCATION_MATCH',
                                      candidate_bytes=100, retail_bytes=100,
                                      similarity=similarity.to_dict(), compiled_cases=2,
                                      missing_provider_checks=2, composed_cases=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('BANK_INFO_ERASE_PASS exact=100/100 tree=2 composed=1 omitted=2')


if __name__ == '__main__':
    main()
