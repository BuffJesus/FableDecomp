"""Check the exact wide-string introsort/insertion helpers and linked paths."""
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import pefile
import check_one
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
BOUNDED = ROOT / 'rebuild/src/compiled/00/9b/STL_InsertionSort_WideString_009b68d0.cpp'
INSERTION = ROOT / 'rebuild/src/compiled/00/9b/STL_UnguardedInsertionSort_WideString_009b2ac0.cpp'
INTROSORT = ROOT / 'rebuild/src/compiled/00/9b/STL_Introsort_WideString_009b7cf0.cpp'
TEST = ROOT / 'rebuild/tests/00/9b/STL_WideStringSortHelpers_009b2ac0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/widestring-sort-helpers.json'
INTROSORT_SYMBOL = '??$__introsort_loop@PAVCWideString@@V1@HU?$less@VCWideString@@@_STL@@@_STL@@YIXPAVCWideString@@00HU?$less@VCWideString@@@0@@Z'
TARGETS = {
    '??0CWideString@@QAE@ABV0@@Z': 0x0099B720,
    '??MCWideString@@QBE_NABV0@@Z': 0x0099AF70,
    '??4CWideString@@QAEAAV0@ABV0@@Z': 0x0099B7D0,
    '??1CWideString@@QAE@XZ': 0x0099B510,
    '??$__unguarded_linear_insert@PAVCWideString@@V1@U?$less@VCWideString@@@_STL@@@_STL@@YIXPAVCWideString@@V1@U?$less@VCWideString@@@0@@Z': 0x009B1870,
    '??$__unguarded_partition@PAVCWideString@@V1@U?$less@VCWideString@@@_STL@@@_STL@@YIPAVCWideString@@PAV1@0V1@U?$less@VCWideString@@@0@@Z': 0x009B3580,
    INTROSORT_SYMBOL: 0x009B7CF0,
    '??$__partial_sort@PAVCWideString@@V1@U?$less@VCWideString@@@_STL@@@_STL@@YIXPAVCWideString@@000U?$less@VCWideString@@@0@@Z': 0x009B6A40,
}
PROVIDERS = (
    (BOUNDED, '??$__insertion_sort@PAVCWideString@@U?$less@VCWideString@@@_STL@@@_STL@@YIXPAVCWideString@@0U?$less@VCWideString@@@0@@Z', 0x009B68D0, 200, 7),
    (INSERTION, '??$__unguarded_insertion_sort_aux@PAVCWideString@@V1@U?$less@VCWideString@@@_STL@@@_STL@@YIXPAVCWideString@@00U?$less@VCWideString@@@0@@Z', 0x009B2AC0, 111, 6),
    (INTROSORT, INTROSORT_SYMBOL, 0x009B7CF0, 217, 9),
)


def main():
    work = Path(tempfile.mkdtemp(prefix='widestring-sort-helpers-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (BOUNDED, INSERTION, INTROSORT, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    reports = []
    with pefile.PE(str(EXE)) as pe:
        for (source, symbol, address, size, relocation_count), obj in zip(PROVIDERS, objects):
            provider = Coff(obj)
            section, offset = provider.symbols[symbol]
            body, relocations = provider.sections[section]
            body = body[offset:]
            assert len(body) == size and len(relocations) == relocation_count
            patched = bytearray(body)
            for position, (target_symbol, kind) in relocations.items():
                assert target_symbol in TARGETS and kind == 20, (target_symbol, kind)
                target = TARGETS[target_symbol] + struct.unpack_from('<i', body, position)[0]
                displacement = target - address - position - 4
                struct.pack_into('<I', patched, position, displacement & 0xffffffff)
            retail = pe.get_data(address - pe.OPTIONAL_HEADER.ImageBase, size)
            assert bytes(patched) == retail, (source.name, address)
            reports.append(dict(address=f'{address:08x}', bytes=size,
                                relocations=relocation_count,
                                source_sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    fixture = Coff(objects[3])
    assert all(symbol not in fixture.symbols for _, symbol, _, _, _ in PROVIDERS)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    for omitted in (0, 1, 2):
        remaining = [str(obj) for index, obj in enumerate(objects) if index != omitted]
        result = subprocess.run(link + remaining, env=env, capture_output=True, text=True)
        assert result.returncode != 0, ('omitted provider linked', omitted)
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'WIDESTRING_SORT_HELPERS_OK' in result.stdout, (
        result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='RELOCATION_MATCH', providers=reports,
                                      linked_cases=9, omitted_provider_checks=3,
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('WIDESTRING_SORT_HELPERS_PASS bytes=528 cases=9 omitted=3')


if __name__ == '__main__':
    main()
