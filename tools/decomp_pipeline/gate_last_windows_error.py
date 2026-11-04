"""Verify GFGetLastWindowsError retail bytes and linked Win32 paths."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/a5/Global_GFGetLastWindowsError_00a5ed60.cpp'
TEST = ROOT / 'rebuild/tests/00/a5/Global_GFGetLastWindowsError_00a5ed60_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/last-windows-error.json'
SYMBOL = '?GFGetLastWindowsError@@YI?AVCCharString@@XZ'
ADDRESS = 0x00A5ED60
TARGETS = {
    '__imp__GetLastError@0': 0x0143FD58,
    '__imp__FormatMessageW@28': 0x0143FE7C,
    '__imp__LocalFree@4': 0x0143FE78,
    '??0CWideString@@QAE@PBG@Z': 0x0099B6B0,
    '?ToCharString@CWideString@@QBE?AVCCharString@@XZ': 0x0099B2C0,
    '??1CWideString@@QAE@XZ': 0x0099B510,
    '??0CCharString@@QAE@ABV0@@Z': 0x0099EC30,
    '??1CCharString@@QAE@XZ': 0x0099EAE0,
    '??_C@_00CNPNBAHC@?$AA@': 0x0122D70E,
    '??0CCharString@@QAE@PBDJ@Z': 0x0099EBF0,
}


def main():
    work = Path(tempfile.mkdtemp(prefix='last-windows-error-'))
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
    assert offset == 0 and len(body) == 136 and len(relocs) == 10
    assert SYMBOL not in fixture.symbols
    patched = bytearray(body)
    for pos, (symbol, kind) in relocs.items():
        assert symbol in TARGETS and kind in (6, 20)
        target = TARGETS[symbol] + struct.unpack_from('<i', body, pos)[0]
        value = target - ADDRESS - pos - 4 if kind == 20 else target
        struct.pack_into('<I', patched, pos, value & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        assert patched == pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 136)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1]), 'kernel32.lib'],
                            env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects] + ['kernel32.lib'],
                            env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'LAST_WINDOWS_ERROR_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='00a5ed60', grade='RELOCATION_MATCH',
                                      bytes=136, relocations=10, compiled_cases=2,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('LAST_WINDOWS_ERROR_PASS bytes=136 compiled=2 omitted=1')


if __name__ == '__main__':
    main()
