"""Validate zlib 1.2.1's error table and all retail string targets."""
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
SOURCE = ROOT / 'rebuild/src/compiled/01/2b/Zlib_z_errmsg_012b1780.cpp'
TEST = ROOT / 'rebuild/tests/integration/ZlibErrorMessages_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/zlib-error-messages.json'
ADDRESS = 0x012B1780

def main():
    work = Path(tempfile.mkdtemp(prefix='zlib-error-messages-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    coff, fixture = Coff(objects[0]), Coff(objects[1])
    section, offset = coff.symbols['_z_errmsg']
    body, relocs = coff.sections[section]
    assert '_z_errmsg' not in fixture.symbols
    assert offset == 0 and len(body) == 40 and len(relocs) == 10
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 40)
        for i in range(10):
            pos = i * 4
            symbol, kind = relocs[pos]
            assert kind == 6
            string_section, string_offset = coff.symbols[symbol]
            blob = coff.sections[string_section][0]
            value = blob[string_offset:].split(b'\0', 1)[0] + b'\0'
            target = struct.unpack_from('<I', retail, pos)[0]
            assert pe.get_data(target - pe.OPTIONAL_HEADER.ImageBase, len(value)) == value, (i, value)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'ZLIB_ERROR_MESSAGES_PASS cases=10' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='012b1780', data_bytes=40,
                                      retail_string_targets=10, compiled_cases=10,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('ZLIB_ERROR_MESSAGES_PASS targets=10 compiled=10 omitted=1')

if __name__ == '__main__':
    main()
