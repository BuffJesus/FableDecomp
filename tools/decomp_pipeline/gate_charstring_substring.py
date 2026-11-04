"""Check the typed CCharString substring method against retail and linked behavior."""
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from near_match import compare, retail_relocations
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/00/99/CCharString_GetSubString_0099ec70.cpp'
TEST = ROOT / 'rebuild/tests/00/99/CCharString_GetSubString_0099ec70_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/charstring-substring.json'
SYMBOL = '?GetSubString@CCharString@@QBE?AV1@KK@Z'
ADDRESS = 0x0099EC70
EXPECTED_RELOCS = {
    '?g_CCharStringInstanceCount_013BD800@@3HA': 5,
    '?g_FableEmptyNarrowString_0129AAF4@@3PADA': 1,
    '?AllocStringData@CCharString@@AAEPAVCCharStringData@@PBDJ@Z': 1,
    '?UnassignString@CCharString@@AAEXXZ': 2,
}


def main():
    work = Path(tempfile.mkdtemp(prefix='charstring-substring-'))
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
    assert offset == 0 and len(body) == 150 and SYMBOL not in fixture.symbols
    assert '?GetSubString@CCharString@@QAEPAU1@PAU1@KK@Z' not in provider.symbols
    counts = {}
    for symbol, kind in relocs.values():
        assert kind in (6, 20)
        counts[symbol] = counts.get(symbol, 0) + 1
    assert counts == EXPECTED_RELOCS, counts
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 148)
        constructor_retail = pe.get_data(0x0099E4B0 - pe.OPTIONAL_HEADER.ImageBase, 15)
    similarity = compare(body, retail, relocs.keys(), retail_relocations('0099ec70', 148))
    assert similarity.near_match and similarity.complete_decode, similarity
    constructor_section, constructor_offset = provider.symbols['??0CCharString@@QAE@XZ']
    constructor_body, constructor_relocs = provider.sections[constructor_section]
    assert constructor_offset == 0 and len(constructor_body) == 15
    assert constructor_relocs == {10: ('?g_CCharStringInstanceCount_013BD800@@3HA', 6)}
    constructor = bytearray(constructor_body)
    struct.pack_into('<I', constructor, 10, 0x013BD800)
    assert constructor == constructor_retail
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'GETSUBSTRING_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='0099ec70', grade='NEAR_MATCH',
                                      candidate_bytes=150, retail_bytes=148,
                                      constructor_bytes=15,
                                      similarity=similarity.to_dict(), compiled_cases=3,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CHARSTRING_SUBSTRING_PASS near=150/148 compiled=3 omitted=1')


if __name__ == '__main__':
    main()
