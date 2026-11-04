"""Verify the exact native console parameter-type switch and its results."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/9e/CConsoleCommandParameters_GetParameterTypeAsString_009ea740.cpp'
TEST = ROOT / 'rebuild/tests/00/9e/CConsoleCommandParameters_GetParameterTypeAsString_009ea740_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/console-parameter-type-string.json'
SYMBOL = '?GetParameterTypeAsString@CConsoleCommandParameters@@QBE?AVCCharString@@J@Z'
ADDRESS = 0x009EA740
TARGETS = {
    '??0CCharString@@QAE@XZ': 0x0099E4B0,
    '??4CCharString@@QAEABV0@PBD@Z': 0x0099EFE0,
    '??0CCharString@@QAE@ABV0@@Z': 0x0099EC30,
    '??1CCharString@@QAE@XZ': 0x0099EAE0,
    '??_C@_04DDKJGDCM@bool?$AA@': 0x0129C434,
    '??_C@_05JEPJEHGA@slong?$AA@': 0x0129C424,
    '??_C@_05ECKAKEHN@ulong?$AA@': 0x0129C458,
    '??_C@_05KNFBNCOH@float?$AA@': 0x0129C42C,
    '??_C@_06ICGJLFIM@string?$AA@': 0x0129C41C,
    '??_C@_04MLLLIHIP@null?$AA@': 0x0129C450,
}
TABLE = (0x009EA76A, 0x009EA771, 0x009EA778, 0x009EA763, 0x009EA77F)


def main():
    work = Path(tempfile.mkdtemp(prefix='console-parameter-type-'))
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
    assert offset == 0 and len(body) == 136 and len(relocs) == 16
    assert SYMBOL not in fixture.symbols
    patched = bytearray(body)
    for pos, (symbol, kind) in relocs.items():
        assert kind in (6, 20)
        if symbol.startswith('$L'):
            target = 0x009EA7B4 if pos == 31 else TABLE[(pos - 116) // 4]
        else:
            target = TARGETS[symbol]
        addend = struct.unpack_from('<i', body, pos)[0]
        target += addend
        value = target - ADDRESS - pos - 4 if kind == 20 else target
        struct.pack_into('<I', patched, pos, value & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 136)
    assert patched == retail, [(i, a, b) for i, (a, b) in enumerate(zip(patched, retail)) if a != b][:20]
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'PARAMETER_TYPE_STRING_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='009ea740', grade='RELOCATION_MATCH',
                                      bytes=136, relocations=16, compiled_cases=9,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CONSOLE_PARAMETER_TYPE_STRING_PASS bytes=136 compiled=9 omitted=1')


if __name__ == '__main__':
    main()
