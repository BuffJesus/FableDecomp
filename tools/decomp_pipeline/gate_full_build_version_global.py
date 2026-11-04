"""Check the native full-build-version string and its retail CRT lifetime."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/01/3b/FullBuildVersion_013b85e0.cpp'
TEST = ROOT / 'rebuild/tests/integration/FullBuildVersion_Global_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/full-build-version-global.json'
SYMBOL = '?FableFullBuildVersion@@3VCCharString@@A'
ADDRESS = 0x013B85E0


def main():
    work = Path(tempfile.mkdtemp(prefix='full-build-version-global-'))
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
    assert offset == 0 and len(body) == 4 and not relocs
    assert SYMBOL not in fixture.symbols
    with pefile.PE(str(EXE)) as pe:
        rva = ADDRESS - pe.OPTIONAL_HEADER.ImageBase
        section = next(s for s in pe.sections
                       if s.VirtualAddress <= rva < s.VirtualAddress + s.Misc_VirtualSize)
        assert section.Name.startswith(b'.data')
        assert rva - section.VirtualAddress >= section.SizeOfRawData
        assert pe.get_data(0x01219F27 - pe.OPTIONAL_HEADER.ImageBase, 5) == b'\xb9\xe0\x85\x3b\x01'
        assert pe.get_data(0x01228920 - pe.OPTIONAL_HEADER.ImageBase, 5) == b'\xb9\xe0\x85\x3b\x01'
        assert pe.get_data(0x0122E5A4 - pe.OPTIONAL_HEADER.ImageBase, 8) == b'Unknown\0'
        call = pe.get_data(0x01219F2C - pe.OPTIONAL_HEADER.ImageBase, 5)
        assert call[0] == 0xE8
        assert 0x01219F31 + int.from_bytes(call[1:], 'little', signed=True) == 0x0099EBF0
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'FULL_BUILD_VERSION_GLOBAL_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='013b85e0', grade='DATA_LAYOUT_MATCH',
                                      object_bytes=4, initial_text='Unknown',
                                      crt_constructor='01219f20', crt_destructor='01228920',
                                      compiled_cases=3, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('FULL_BUILD_VERSION_GLOBAL_PASS bytes=4 crt=2 text=Unknown omitted=1')


if __name__ == '__main__':
    main()
