"""Verify CGenericVarRef::PrintToString bytes, switch table, and behavior."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/a6/CGenericVarRef_PrintToString_00a63c00.cpp'
TEST = ROOT / 'rebuild/tests/00/a6/CGenericVarRef_PrintToString_00a63c00_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/generic-var-ref-print.json'
SYMBOL = '?PrintToString@CGenericVarRef@@QAEXAAVCCharString@@@Z'
ADDRESS = 0x00A63C00
TARGETS = {
    '??_C@_02NJPGOMH@?$CFf?$AA@': 0x0129DAD4,
    '??_C@_02DPKJAMEF@?$CFd?$AA@': 0x01265DE8,
    '??_C@_04HCDDPBNL@TRUE?$AA@': 0x0122D6D4,
    '??_C@_05MAJJAKPI@FALSE?$AA@': 0x0122D6CC,
    '?Sprintf@CCharString@@QAAXPBDZZ': 0x0099F1F0,
    '??0CCharString@@QAE@ABV0@@Z': 0x0099EC30,
    '??4CCharString@@QAEAAV0@ABV0@@Z': 0x0099EFB0,
    '??1CCharString@@QAE@XZ': 0x0099EAE0,
    '??4CCharString@@QAEABV0@PBD@Z': 0x0099EFE0,
}
TABLE = (0x00A63C83, 0x00A63C9B, 0x00A63C14, 0x00A63C5B, 0x00A63C35)


def main():
    work = Path(tempfile.mkdtemp(prefix='generic-var-ref-print-'))
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
    assert offset == 0 and len(body) == 180 and len(relocs) == 17
    assert SYMBOL not in fixture.symbols
    patched = bytearray(body)
    for pos, (symbol, kind) in relocs.items():
        assert kind in (6, 20)
        if symbol.startswith('$L'):
            target = 0x00A63CA0 if pos == 16 else TABLE[(pos - 160) // 4]
        else:
            target = TARGETS[symbol]
        addend = struct.unpack_from('<i', body, pos)[0]
        target += addend
        value = target - ADDRESS - pos - 4 if kind == 20 else target
        struct.pack_into('<I', patched, pos, value & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 180)
    assert patched == retail, [(i, a, b) for i, (a, b) in enumerate(zip(patched, retail)) if a != b][:20]
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'GENERIC_VAR_REF_PRINT_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='00a63c00', grade='RELOCATION_MATCH',
                                      bytes=180, relocations=17, compiled_cases=9,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('GENERIC_VAR_REF_PRINT_PASS bytes=180 compiled=9 omitted=1')


if __name__ == '__main__':
    main()
