"""Check the native wide-string instance counter and legacy symbol alias."""
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
SOURCE = ROOT / 'rebuild/src/compiled/01/3b/WideStringInstanceCount_013bca20.cpp'
TEST = ROOT / 'rebuild/tests/integration/WideStringInstanceCount_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/widestring-instance-count.json'
SYMBOL = '?g_CWideStringInstanceCount_013BCA20@@3HA'
ALIAS = '_g_charStringRefTrace'
ADDRESS = 0x013BCA20


def main():
    work = Path(tempfile.mkdtemp(prefix='widestring-instance-count-'))
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
    assert offset == 0 and size == 4 and raw == 0 and not relocations
    assert SYMBOL not in fixture.symbols and ALIAS not in provider.symbols
    dump = subprocess.run([str(check_one.VC / 'bin/dumpbin.exe'), '/directives', str(objects[0])],
                          env=env, capture_output=True, text=True, check=True).stdout
    assert f'/alternatename:{ALIAS}={SYMBOL}' in dump
    with pefile.PE(str(EXE)) as pe:
        rva = ADDRESS - pe.OPTIONAL_HEADER.ImageBase
        section = next(s for s in pe.sections
                       if s.VirtualAddress <= rva < s.VirtualAddress + s.Misc_VirtualSize)
        assert section.Name.startswith(b'.data')
        assert rva - section.VirtualAddress >= section.SizeOfRawData
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'WIDESTRING_INSTANCE_COUNT_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='013bca20', grade='DATA_LAYOUT_MATCH',
                                      data_bytes=4, aliases=[SYMBOL, ALIAS],
                                      compiled_cases=4, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('WIDESTRING_INSTANCE_COUNT_PASS bytes=4 aliases=2 omitted=1')


if __name__ == '__main__':
    main()
