"""Verify the reconstructed zlib Huffman-tree core against retail and linked state resets."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/c0/Zlib_TreesCore_00c051c0.cpp'
TEST = ROOT / 'rebuild/tests/00/c0/Zlib_TreesCore_00c051c0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/zlib-trees-core.json'
SPECS = [
    ('?init_block@@YAXPAUinternal_state@@@Z', 0x00C03EE0, 100),
    ('?pqdownheap@@YAXPAUinternal_state@@PAUct_data_s@@H@Z', 0x00C03F50, 196),
    ('?gen_bitlen@@YAXPAUinternal_state@@PAUtree_desc_s@@@Z', 0x00C04020, 563),
    ('?scan_tree@@YAXPAUinternal_state@@PAUct_data_s@@H@Z', 0x00C04260, 216),
    ('?send_tree@@YAXPAUinternal_state@@PAUct_data_s@@H@Z', 0x00C04340, 1290),
    ('?send_all_trees@@YAXPAUinternal_state@@HHH@Z', 0x00C04850, 621),
    ('?set_data_type@@YAXPAUinternal_state@@@Z', 0x00C04F90, 129),
    ('?bi_reverse@@YAIIH@Z', 0x00C05020, 23),
    ('?bi_flush@@YAXPAUinternal_state@@@Z', 0x00C05040, 127),
    ('?bi_windup@@YAXPAUinternal_state@@@Z', 0x00C050C0, 109),
    ('?copy_block@@YAXPAUinternal_state@@PADIH@Z', 0x00C05130, 130),
    ('__tr_init', 0x00C051C0, 100),
    ('?build_bl_tree@@YAHPAUinternal_state@@@Z', 0x00C054C0, 108),
    ('__tr_stored_block', 0x00C05530, 160),
    ('__tr_align', 0x00C055D0, 497),
    ('__tr_flush_block', 0x00C057D0, 485),
]
TARGETS = {
    '_bl_order': 0x012AEE84,
    '?scan_tree@@YAXPAUinternal_state@@PAUct_data_s@@H@Z': 0x00C04260,
    '?send_tree@@YAXPAUinternal_state@@PAUct_data_s@@H@Z': 0x00C04340,
    '?pqdownheap@@YAXPAUinternal_state@@PAUct_data_s@@H@Z': 0x00C03F50,
    '_static_l_desc': 0x0139C8B0, '_static_d_desc': 0x0139C8C4,
    '_static_bl_desc': 0x0139C8D8,
    '?init_block@@YAXPAUinternal_state@@@Z': 0x00C03EE0,
    '?copy_block@@YAXPAUinternal_state@@PADIH@Z': 0x00C05130,
    '?bi_flush@@YAXPAUinternal_state@@@Z': 0x00C05040,
    '?set_data_type@@YAXPAUinternal_state@@@Z': 0x00C04F90,
    '?build_tree@@YAXPAUinternal_state@@PAUtree_desc_s@@@Z': 0x00C052C0,
    '?build_bl_tree@@YAHPAUinternal_state@@@Z': 0x00C054C0,
    '__tr_stored_block': 0x00C05530,
    '_static_dtree': 0x012AF318, '_static_ltree': 0x012AEE98,
    '?compress_block@@YAXPAUinternal_state@@PAUct_data_s@@1@Z': 0x00C04B70,
    '?send_all_trees@@YAXPAUinternal_state@@HHH@Z': 0x00C04850,
    '?bi_windup@@YAXPAUinternal_state@@@Z': 0x00C050C0,
}

def main():
    work = Path(tempfile.mkdtemp(prefix='zlib-trees-core-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    coff, fixture = Coff(objects[0]), Coff(objects[1])
    checks = []
    with pefile.PE(str(EXE)) as pe:
        for name, address, size in SPECS:
            assert name not in fixture.symbols
            section, offset = coff.symbols[name]
            body, relocs = coff.sections[section]
            assert offset == 0 and len(body) == size, (name, len(body))
            patched = bytearray(body)
            for pos, (symbol, kind) in relocs.items():
                assert symbol in TARGETS and kind in (6, 20), (name, pos, symbol)
                addend = struct.unpack_from('<i', body, pos)[0]
                target = TARGETS[symbol] + addend
                struct.pack_into('<I', patched, pos,
                                 (target - address - pos - 4 if kind == 20 else target) & 0xffffffff)
            retail = pe.get_data(address - pe.OPTIONAL_HEADER.ImageBase, size)
            assert patched == retail, name
            checks.append(dict(name=name, address=f'{address:08x}', bytes=size,
                               relocations=len(relocs)))
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'ZLIB_TREES_CORE_PASS cases=16' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(entries=checks, compiled_cases=16,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('ZLIB_TREES_CORE_PASS bodies=16 compiled=16 omitted=1')

if __name__ == '__main__':
    main()
