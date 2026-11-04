"""Verify retail compress2 bytes and linked return paths."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/c0/z_compress2_00c03de0.cpp'
TEST = ROOT / 'rebuild/tests/00/c0/z_compress2_00c03de0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/z-compress2.json'
ADDRESS = 0x00C03DE0
TARGETS = {
    '_z_deflateInit_': 0x00C021C0,
    '_z_deflate': 0x00C006A0,
    '_z_deflateEnd': 0x00C00AE0,
}

def main():
    work = Path(tempfile.mkdtemp(prefix='z-compress2-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    coff = Coff(objects[0])
    section, offset = coff.symbols['_z_compress2']
    body, relocs = coff.sections[section]
    assert offset == 0 and len(body) == 169 and len(relocs) == 5
    patched = bytearray(body)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 169)
        for pos, (symbol, kind) in relocs.items():
            if kind == 6:
                assert symbol.startswith('??_C@_05') and pos == 31
                target = struct.unpack_from('<I', retail, pos)[0]
                assert pe.get_data(target - pe.OPTIONAL_HEADER.ImageBase, 6) == b'1.2.1\0'
                struct.pack_into('<I', patched, pos, target)
            else:
                assert kind == 20 and symbol in TARGETS
                addend = struct.unpack_from('<i', body, pos)[0]
                struct.pack_into('<I', patched, pos,
                                 (TARGETS[symbol] + addend - ADDRESS - pos - 4) & 0xffffffff)
    assert patched == retail
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'Z_COMPRESS2_PASS cases=7' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='00c03de0', grade='RELOCATION_MATCH', bytes=169,
                                      relocations=5, compiled_cases=7,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('Z_COMPRESS2_PASS bytes=169 compiled=7 omitted=1')

if __name__ == '__main__':
    main()
