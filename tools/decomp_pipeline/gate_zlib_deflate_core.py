"""Verify the upstream zlib 1.2.1 C++ deflate core and real compression behavior."""
import hashlib
import json
import struct
import subprocess
import sys
import tempfile
import zlib
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/00/c0/Zlib_DeflateCore_00c006a0.cpp'
TEST = ROOT / 'rebuild/tests/integration/ZlibDeflateCore_test.cpp'
TREES = ROOT / 'rebuild/src/compiled/00/c0/Zlib_TreesCore_00c051c0.cpp'
REPORT = ROOT / 'rebuild/compile-gate/zlib-deflate-core.json'
SUPPORT = [
    '00/c0/z_deflateInit__00c021c0.cpp',
    '00/c0/z_deflateInit2__00c01f90.cpp',
    '00/c0/z_deflateEnd_00c00ae0.cpp',
    '00/c0/zcalloc_00c06030.cpp',
    '00/c0/zcfree_00c06050.cpp',
    '00/c0/Zlib_zadler32_00c03cb0.cpp',
    '00/c0/Zlib_zcrc32_00c05fd0.cpp',
    '01/2b/Zlib_z_errmsg_012b1780.cpp',
    '00/c0/z_compressBound_00c03eb0.cpp',
    '00/c0/z_compress2_00c03de0.cpp',
]
TARGETS = {
    '_z_crc32': 0x00C05FD0, '_z_adler32': 0x00C03CB0,
    '_tr_init': 0x00C051C0, '__tr_init': 0x00C051C0,
    '_tr_align': 0x00C055D0, '__tr_align': 0x00C055D0,
    '_tr_stored_block': 0x00C05530, '__tr_stored_block': 0x00C05530,
    '?lm_init@@YAXPAUinternal_state@@@Z': 0x00C00DD0,
    '?putShortMSB@@YAXPAUinternal_state@@I@Z': 0x00C00600,
    '?flush_pending@@YAXPAUz_stream_s@@@Z': 0x00C00630,
    '_z_errmsg': 0x012B1780, '_configuration_table': 0x012AE260,
}
SPECS = [
    ('_z_deflate', 0x00C006A0, 1080),
    ('_z_deflateReset', 0x00C01B60, 145),
    ('?lm_init@@YAXPAUinternal_state@@@Z', 0x00C00DD0, 135),
    ('?putShortMSB@@YAXPAUinternal_state@@I@Z', 0x00C00600, 39),
    ('?flush_pending@@YAXPAUz_stream_s@@@Z', 0x00C00630, 100),
]

def main():
    work = Path(tempfile.mkdtemp(prefix='zlib-deflate-core-'))
    env = check_one.env()
    env['INCLUDE'] = (str(ROOT / 'rebuild/include') + ';' +
                      str(ROOT / 'rebuild/include/third_party/zlib121') + ';' + env['INCLUDE'])
    objects = []
    all_sources = [SOURCE] + [ROOT / 'rebuild/src/compiled' / rel for rel in SUPPORT] + [TREES, TEST]
    for index, source in enumerate(all_sources):
        obj = work / ('source%d.obj' % index)
        args = ['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj)]
        if source.suffix == '.c':
            args.append('/DZ_PREFIX')
        result = check_one.cl(args + [str(source)], env, work)
        assert result.returncode == 0, (source, result.stdout, result.stderr)
        objects.append(obj)
    coff = Coff(objects[0])
    assert all(name not in coff.symbols for name in ('_z_deflateInit_', '_z_deflateInit2_', '_z_deflateEnd'))
    checks = []
    with pefile.PE(str(EXE)) as pe:
        for name, address, size in SPECS:
            section, offset = coff.symbols[name]
            body, relocs = coff.sections[section]
            assert offset == 0 and len(body) == size, (name, len(body), size)
            patched = bytearray(body)
            for pos, (symbol, kind) in relocs.items():
                assert symbol in TARGETS and kind in (6, 20), (name, pos, symbol, kind)
                addend = struct.unpack_from('<i', body, pos)[0]
                target = TARGETS[symbol] + addend
                struct.pack_into('<I', patched, pos,
                                 (target - address - pos - 4 if kind == 20 else target) & 0xffffffff)
            retail = pe.get_data(address - pe.OPTIONAL_HEADER.ImageBase, size)
            assert patched == retail, name
            checks.append(dict(name=name, address=f'{address:08x}', bytes=size,
                               relocations=len(relocs)))
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(obj) for obj in objects[1:]], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted core provider linked'
    result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != len(objects) - 2],
                            env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted trees provider linked'
    result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != 1 + SUPPORT.index('00/c0/z_compressBound_00c03eb0.cpp')],
                            env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted bound provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0, (result.returncode, result.stdout, result.stderr)
    lines = result.stdout.splitlines()
    assert len(lines) == 7, lines
    for case_index, line in enumerate(lines):
        size_text, encoded_text, blob_hex = line.split()
        size, encoded, blob = int(size_text), int(encoded_text), bytes.fromhex(blob_hex)
        assert len(blob) == encoded
        expected = bytes((i * 19 + case_index * 37) & 255 for i in range(size))
        assert zlib.decompress(blob) == expected, (case_index, size)
    REPORT.write_text(json.dumps(dict(entries=checks, roundtrip_cases=7,
                                      missing_provider_checks=3,
                                      boundary='both deflate and trees providers are reconstructed C++',
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('ZLIB_DEFLATE_CORE_PASS bodies=5 roundtrips=7 omitted=3')

if __name__ == '__main__':
    main()
