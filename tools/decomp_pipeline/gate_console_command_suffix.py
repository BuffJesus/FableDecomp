"""Check the typed console command suffix and its retail CRT initializer."""
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
SOURCE = ROOT / 'rebuild/src/compiled/01/3c/ConsoleCommandNameSuffix_013caa48.cpp'
TEST = ROOT / 'rebuild/tests/integration/ConsoleCommandNameSuffix_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/console-command-suffix.json'
GLOBAL = '?g_013caa48@@3VCCharString@@B'
INIT = 0x012241D0
FINI = 0x0122A8D0
TARGETS = {
    '??_C@_01ODHLEDKK@?$CI?$AA@': 0x0122E028,
    GLOBAL: 0x013CAA48,
    '??0CCharString@@QAE@PBDJ@Z': 0x0099EBF0,
    '_$E2': FINI,
    '_atexit': 0x004012BC,
    '??1CCharString@@QAE@XZ': 0x0099EAE0,
}


def retail_body(coff, symbol, address, expected_length, pe):
    section, offset = coff.symbols[symbol]
    body, relocs = coff.sections[section]
    assert offset == 0 and len(body) == expected_length
    patched = bytearray(body)
    for pos, (target_symbol, kind) in relocs.items():
        assert target_symbol in TARGETS and kind in (6, 20)
        addend = struct.unpack_from('<i', body, pos)[0]
        target = TARGETS[target_symbol] + addend
        value = target - address - pos - 4 if kind == 20 else target
        struct.pack_into('<I', patched, pos, value & 0xffffffff)
    assert patched == pe.get_data(address - pe.OPTIONAL_HEADER.ImageBase, expected_length)
    return len(relocs)


def main():
    work = Path(tempfile.mkdtemp(prefix='console-command-suffix-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    provider, fixture = Coff(objects[0]), Coff(objects[1])
    assert GLOBAL in provider.symbols and GLOBAL not in fixture.symbols
    section, offset = provider.symbols[GLOBAL]
    body, relocs = provider.sections[section]
    raw_size, raw_pointer = struct.unpack_from('<II', provider.data, 20 + 40 * (section - 1) + 16)
    characteristics = struct.unpack_from('<I', provider.data, 20 + 40 * (section - 1) + 36)[0]
    assert offset == 0 and raw_size == 4 and raw_pointer == 0 and not relocs
    assert characteristics & 0x80, 'expected uninitialized data section'
    with pefile.PE(str(EXE)) as pe:
        assert pe.get_data(0x0122E028 - pe.OPTIONAL_HEADER.ImageBase, 2) == b'(\0'
        relocation_count = retail_body(provider, '_$E1', INIT, 29, pe)
        relocation_count += retail_body(provider, '_$E2', FINI, 10, pe)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CONSOLE_COMMAND_SUFFIX_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='013caa48', data_bytes=4,
                                      initializer_bytes=29, finalizer_bytes=10,
                                      relocations=relocation_count, compiled_cases=1,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CONSOLE_COMMAND_SUFFIX_PASS data=4 init=29 fini=10 compiled=1 omitted=1')


if __name__ == '__main__':
    main()
