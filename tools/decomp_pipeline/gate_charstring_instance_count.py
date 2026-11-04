"""Check the native CCharString instance-counter storage."""
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/01/3b/CharStringInstanceCount_013bd800.cpp'
TEST = ROOT / 'rebuild/tests/integration/CharStringInstanceCount_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/charstring-instance-count.json'
SYMBOL = '?g_CCharStringInstanceCount_013BD800@@3HA'
ADDRESS = 0x013BD800


def main():
    work = Path(tempfile.mkdtemp(prefix='charstring-instance-count-'))
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
    _, relocations = provider.sections[section]
    header = 20 + 40 * (section - 1)
    size, raw = struct.unpack_from('<II', provider.data, header + 16)
    characteristics = struct.unpack_from('<I', provider.data, header + 36)[0]
    assert offset == 0 and size == 4 and raw == 0 and not relocations
    assert characteristics & 0x80 and SYMBOL not in fixture.symbols
    with pefile.PE(str(EXE)) as pe:
        rva = ADDRESS - pe.OPTIONAL_HEADER.ImageBase
        retail_section = next(s for s in pe.sections if s.VirtualAddress <= rva < s.VirtualAddress + s.Misc_VirtualSize)
        assert retail_section.Name.rstrip(b'\0') == b'.data'
        assert rva - retail_section.VirtualAddress >= retail_section.SizeOfRawData
        assert rva + 4 <= retail_section.VirtualAddress + retail_section.Misc_VirtualSize
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CHARSTRING_INSTANCE_COUNT_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='013bd800', data_bytes=4,
                                      compiled_cases=4, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CHARSTRING_INSTANCE_COUNT_PASS bytes=4 compiled=4 omitted=1')


if __name__ == '__main__':
    main()
