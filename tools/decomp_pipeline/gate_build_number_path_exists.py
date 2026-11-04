"""Check that build-number setup calls the real static CAFile path probe."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/40/Global_GFGetBuildNumber2_00401f30.cpp'
TEST = ROOT / 'rebuild/tests/00/40/Global_GFGetBuildNumber2_00401f30_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/build-number-path-exists.json'
SYMBOL = '?GFGetBuildNumber2@@YAXXZ'
CALLEE = '?PathExists@CAFile@@SI_NABVCWideString@@@Z'
ADDRESS = 0x00401F30


def main():
    work = Path(tempfile.mkdtemp(prefix='build-number-path-exists-'))
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
    assert offset == 0 and len(body) == 160
    targets = [symbol for symbol, _ in relocs.values()]
    assert CALLEE in targets and not any('PathExists@CWideString' in s for s in targets)
    assert '??0CWideString@@QAE@PBG@Z' in targets
    assert '??0CDiskFileWin32@@QAE@ABVCWideString@@W4FileAccessType@@K@Z' in targets
    assert '??0CCharString@@QAE@PAVCAFile@@J@Z' in targets
    assert '??4CCharString@@QAEAAV0@ABV0@@Z' in targets
    assert SYMBOL not in fixture.symbols
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 162)
    # Literal, path-global and vtable immediates are linker-selected addresses
    # in retail's image; the PE has no base relocation entries for this code.
    similarity = compare(body, retail, relocs.keys(), (5, 44, 105, 131))
    assert similarity.near_match and similarity.opcode_similarity >= 0.95, similarity
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'GF_GET_BUILD_NUMBER_TYPED_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='00401f30', grade='NEAR_MATCH',
                                      candidate_bytes=160, retail_bytes=162,
                                      similarity=similarity.to_dict(), corrected_callee=CALLEE,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('BUILD_NUMBER_PATH_EXISTS_PASS near=160/162 typed=4 omitted=1')


if __name__ == '__main__':
    main()
