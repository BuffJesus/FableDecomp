"""Verify the typed CCharString::Sprintf body and linked formatting cases."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/99/CCharString_Sprintf_0099f1f0.cpp'
TEST = ROOT / 'rebuild/tests/00/99/CCharString_Sprintf_0099f1f0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/charstring-sprintf.json'
SYMBOL = '?Sprintf@CCharString@@QAAXPBDZZ'
ADDRESS = 0x0099F1F0
TARGETS = {
    '_malloc': 0x00BFEA0E,
    '__vsnprintf': 0x00BFEDAC,
    '?UnassignString@CCharString@@AAEXXZ': 0x0099E9B0,
    '??2@YAPAXI@Z': 0x00BFEA1A,
    '??0?$CBasicString@D@@QAE@PBD@Z': 0x009A0590,
    '_free': 0x00BFEA14,
}


def main():
    work = Path(tempfile.mkdtemp(prefix='charstring-sprintf-'))
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
    assert offset == 0 and len(body) == 172 and len(relocs) == 6
    assert SYMBOL not in fixture.symbols
    patched = bytearray(body)
    for pos, (symbol, kind) in relocs.items():
        assert symbol in TARGETS and kind in (6, 20)
        target = TARGETS[symbol] + struct.unpack_from('<i', body, pos)[0]
        value = target - ADDRESS - pos - 4 if kind == 20 else target
        struct.pack_into('<I', patched, pos, value & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        assert patched == pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 172)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CHARSTRING_SPRINTF_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='0099f1f0', grade='RELOCATION_MATCH',
                                      bytes=172, relocations=6, compiled_cases=4,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CHARSTRING_SPRINTF_PASS bytes=172 compiled=4 omitted=1')


if __name__ == '__main__':
    main()
