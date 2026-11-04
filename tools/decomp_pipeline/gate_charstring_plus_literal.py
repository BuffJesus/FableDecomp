"""Validate native string + literal ownership and the retail near match."""
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from capstone import Cs, CS_ARCH_X86, CS_MODE_32
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/00/99/CCharString_AddCharStringAndLiteral_0099f600.cpp'
TEST = ROOT / 'rebuild/tests/00/99/CCharString_AddCharStringAndLiteral_0099f600_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/charstring-plus-literal.json'
SYMBOL = '??H@YI?AVCCharString@@ABV0@PBD@Z'
ADDRESS = 0x0099F600
TARGETS = {
    '?g_CCharStringInstanceCount_013BD800@@3HA': 0x013BD800,
    '??YCCharString@@QAEABV0@PBD@Z': 0x0099F100,
    '?UnassignString@CCharString@@AAEXXZ': 0x0099E9B0,
}


def main():
    work = Path(tempfile.mkdtemp(prefix='charstring-plus-literal-'))
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
    assert offset == 0 and len(body) == 126 and len(relocs) == 7
    assert SYMBOL not in fixture.symbols
    patched = bytearray(body)
    for pos, (symbol, kind) in relocs.items():
        assert symbol in TARGETS and kind in (6, 20)
        retail_pos = pos + 4 if pos >= 0x26 else pos
        target = TARGETS[symbol] + struct.unpack_from('<i', body, pos)[0]
        value = target - ADDRESS - retail_pos - 4 if kind == 20 else target
        struct.pack_into('<I', patched, pos, value & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 130)
    assert len(retail) == 130
    assert patched[:0x17] == retail[:0x17]
    assert patched[0x26:] == retail[0x2A:]
    decoder = Cs(CS_ARCH_X86, CS_MODE_32)
    candidate_copy = [(i.mnemonic, i.op_str) for i in decoder.disasm(bytes(body[0x17:0x26]), 0)]
    retail_copy = [(i.mnemonic, i.op_str) for i in decoder.disasm(retail[0x17:0x2A], 0)]
    assert [m for m, _ in candidate_copy] == ['je', 'mov', 'test', 'je', 'mov', 'inc']
    assert [m for m, _ in retail_copy] == ['je', 'mov', 'test', 'je', 'mov', 'inc', 'mov', 'mov']
    assert candidate_copy[1:3] == retail_copy[1:3]
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'FABLETLC_CCHARSTRING_STRING_PLUS_LITERAL_0099F600_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='0099f600', grade='NEAR_MATCH',
                                      candidate_bytes=126, retail_bytes=130,
                                      exact_prefix_bytes=23, exact_tail_bytes=88,
                                      relocations=7, compiled_cases=3,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CHARSTRING_PLUS_LITERAL_PASS near=126/130 compiled=3 omitted=1')


if __name__ == '__main__':
    main()
