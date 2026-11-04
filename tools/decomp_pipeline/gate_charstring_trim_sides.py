"""Check leading and trailing CCharString trim methods against retail."""
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
BASE = ROOT / 'rebuild/src/compiled/00/99'
TESTS = ROOT / 'rebuild/tests/00/99'
REPORT = ROOT / 'rebuild/compile-gate/charstring-trim-sides.json'
CASES = [
    ('0099f450', 'Begin', 117, 4, 'CHARSTRING_TRIM_BEGIN_PASS'),
    ('0099f4d0', 'End', 135, 5, 'CHARSTRING_TRIM_END_PASS'),
]


def main():
    work = Path(tempfile.mkdtemp(prefix='charstring-trim-sides-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    reports = []
    with pefile.PE(str(EXE)) as pe:
        for address, name, retail_size, reloc_count, marker in CASES:
            stem = f'CCharString_RemoveWhiteSpace{name}_{address}'
            source = BASE / (stem + '.cpp')
            fixture = TESTS / (stem + '_test.cpp')
            objects = []
            for path in (source, fixture):
                obj = work / (path.stem + '.obj')
                result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(path)], env, work)
                assert result.returncode == 0, result.stdout + result.stderr
                objects.append(obj)
            provider, test = Coff(objects[0]), Coff(objects[1])
            symbol = f'?RemoveWhiteSpace{name}@CCharString@@QAEXXZ'
            section, offset = provider.symbols[symbol]
            body, relocs = provider.sections[section]
            assert offset == 0 and len(relocs) == reloc_count
            assert symbol not in test.symbols
            assert {target for target, _ in relocs.values()} == ({
                '?GetSubString@CCharString@@QBE?AV1@KK@Z',
                '?UnassignString@CCharString@@AAEXXZ',
                '?g_CCharStringInstanceCount_013BD800@@3HA',
            } | ({'?g_FableEmptyNarrowString_0129AAF4@@3PADA'} if name == 'End' else set()))
            retail = pe.get_data(int(address, 16) - pe.OPTIONAL_HEADER.ImageBase, retail_size)
            similarity = compare(body, retail, relocs.keys(), retail_relocations(address, retail_size))
            assert similarity.near_match and similarity.opcode_similarity >= 0.90, similarity
            if name == 'Begin':
                assert len(body) == retail_size and similarity.opcode_similarity == 1.0
            else:
                assert len(body) == 134
            link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / (name + '.exe'))]
            result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
            assert result.returncode != 0, 'omitted provider linked'
            result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
            assert result.returncode == 0, result.stdout + result.stderr
            result = subprocess.run([str(work / (name + '.exe'))], env=env, capture_output=True, text=True, timeout=30)
            assert result.returncode == 0 and marker in result.stdout, (result.returncode, result.stdout, result.stderr)
            reports.append(dict(address=address, grade='RELOCATION_MATCH' if name == 'Begin' else 'NEAR_MATCH',
                                candidate_bytes=len(body), retail_bytes=retail_size,
                                similarity=similarity.to_dict(), compiled_cases=5,
                                missing_provider_checks=1,
                                source_sha256=hashlib.sha256(source.read_bytes()).hexdigest(),
                                fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest()))
    composed = TESTS / 'CCharString_RemoveWhiteSpace_Composed_test.cpp'
    composed_obj = work / 'composed.obj'
    result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(composed_obj), str(composed)], env, work)
    assert result.returncode == 0, result.stdout + result.stderr
    wrapper = BASE / 'CCharString_RemoveWhiteSpaceBeginEnd_0099f560.cpp'
    wrapper_obj = work / 'wrapper.obj'
    result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(wrapper_obj), str(wrapper)], env, work)
    assert result.returncode == 0, result.stdout + result.stderr
    providers = [work / f'CCharString_RemoveWhiteSpace{name}_{address}.obj'
                 for address, name, *_ in CASES]
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'composed.exe')]
    result = subprocess.run(link + [str(composed_obj), str(wrapper_obj)], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted trim methods linked'
    result = subprocess.run(link + [str(composed_obj), str(wrapper_obj)] + [str(obj) for obj in providers],
                            env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'composed.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CHARSTRING_TRIM_COMPOSED_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(reports, indent=2) + '\n')
    print('CHARSTRING_TRIM_SIDES_PASS begin=117/117 end=134/135 compiled=14 omitted=3')


if __name__ == '__main__':
    main()
