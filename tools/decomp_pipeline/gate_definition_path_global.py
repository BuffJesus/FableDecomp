"""Verify GFMain's process-lifetime compiled-definition path string."""
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
SOURCE = ROOT / 'rebuild/src/compiled/01/3c/Global_DefinitionPath_013ca7dc.cpp'
TEST = ROOT / 'rebuild/tests/01/3c/Global_DefinitionPath_013ca7dc_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/definition-path-global.json'
SYMBOL = '?g_013ca7dc@@3VCWideString@@A'
ADDRESS = 0x013CA7DC


def main():
    work = Path(tempfile.mkdtemp(prefix='definition-path-global-'))
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
        initializer = pe.get_data(0x01224050 - pe.OPTIONAL_HEADER.ImageBase, 5)
        destructor = pe.get_data(0x0122A6B0 - pe.OPTIONAL_HEADER.ImageBase, 5)
        assert initializer == destructor == b'\xb9\xdc\xa7\x3c\x01'
        call = pe.get_data(0x01224055 - pe.OPTIONAL_HEADER.ImageBase, 5)
        assert call[0] == 0xE8
        assert 0x0122405A + int.from_bytes(call[1:], 'little', signed=True) == 0x0099AED0
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'DEFINITION_PATH_GLOBAL_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='013ca7dc', grade='DATA_LAYOUT_MATCH',
                                      object_bytes=4, retail_storage='uninitialized .data tail',
                                      crt_constructor='01224050', crt_destructor='0122a6b0',
                                      compiled_cases=2, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('DEFINITION_PATH_GLOBAL_PASS bytes=4 crt=2 omitted=1')


if __name__ == '__main__':
    main()
