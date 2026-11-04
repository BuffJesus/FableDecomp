"""Verify the typed integer-to-CCharString helper and retail code bytes."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/99/Global_GFIntToCharString_0099f830.cpp'
TEST = ROOT / 'rebuild/tests/00/99/Global_GFIntToCharString_0099f830_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/gfint-to-charstring.json'
SYMBOL = '?GFIntToCharString@@YI?AVCCharString@@J@Z'
ADDRESS = 0x0099F830
TARGETS = {
    '?g_CCharStringInstanceCount_013BD800@@3HA': 0x013BD800,
    '??_C@_02DPKJAMEF@?$CFd?$AA@': 0x01265DE8,
    '?Sprintf@CCharString@@QAAXPBDZZ': 0x0099F1F0,
    '?UnassignString@CCharString@@AAEXXZ': 0x0099E9B0,
}


def main():
    work = Path(tempfile.mkdtemp(prefix='gfint-to-charstring-'))
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
    assert offset == 0 and len(body) == 116 and len(relocs) == 9
    assert SYMBOL not in fixture.symbols
    patched = bytearray(body)
    for pos, (symbol, kind) in relocs.items():
        assert symbol in TARGETS and kind in (6, 20)
        target = TARGETS[symbol] + struct.unpack_from('<i', body, pos)[0]
        value = target - ADDRESS - pos - 4 if kind == 20 else target
        struct.pack_into('<I', patched, pos, value & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        assert patched == pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 116)
        assert pe.get_data(0x01265DE8 - pe.OPTIONAL_HEADER.ImageBase, 3) == b'%d\0'
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'GFINT_TO_CHARSTRING_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='0099f830', grade='RELOCATION_MATCH',
                                      bytes=116, relocations=9, compiled_cases=4,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('GFINT_TO_CHARSTRING_PASS bytes=116 compiled=4 omitted=1')


if __name__ == '__main__':
    main()
