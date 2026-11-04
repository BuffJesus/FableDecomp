"""Validate six typed display/window globals against retail storage."""
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/01/38/NativeDisplayRuntimeData_0138e0b8.cpp'
TEST = ROOT / 'rebuild/tests/integration/NativeDisplayRuntimeData_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/native-display-runtime-data.json'
SPECS = (
    ('?FableWindowGamma_0138e0b8@@3MA', 0x0138E0B8, struct.pack('<f', 1.0)),
    ('?FableSavedSoundVolume_013ca60c@@3MA', 0x013CA60C, bytes(4)),
    ('?FableSoundDeactivated_013ca610@@3_NA', 0x013CA610, bytes(1)),
    ('?FableWindowSystemManager_013ca798@@3PAVCSystemManager@@A', 0x013CA798, bytes(4)),
    ('?FableVirtualPixelScaleX_013ca800@@3NA', 0x013CA800, bytes(8)),
    ('?FableVirtualPixelScaleY_013ca808@@3NA', 0x013CA808, bytes(8)),
)

def main():
    work = Path(tempfile.mkdtemp(prefix='native-display-runtime-data-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    data, fixture = Coff(objects[0]), Coff(objects[1])
    with pefile.PE(str(EXE)) as pe:
        for symbol, address, expected in SPECS:
            assert symbol in data.symbols and symbol not in fixture.symbols, symbol
            assert coff_data(data, symbol, len(expected)) == expected, symbol
            rva = address - pe.OPTIONAL_HEADER.ImageBase
            section = next(s for s in pe.sections if s.VirtualAddress <= rva and
                           rva + len(expected) <= s.VirtualAddress + s.Misc_VirtualSize)
            assert section.Name.rstrip(b'\0') == b'.data', symbol
            offset = rva - section.VirtualAddress
            if offset < section.SizeOfRawData:
                assert pe.get_data(rva, len(expected)) == expected, symbol
            else:
                assert expected == bytes(len(expected)), symbol
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'NATIVE_DISPLAY_RUNTIME_DATA_PASS cases=16' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(symbols=len(SPECS), data_bytes=sum(len(v) for _, _, v in SPECS),
                                      compiled_cases=16, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest(),
                                      coverage_claim='data only'), indent=2) + '\n')
    print('NATIVE_DISPLAY_RUNTIME_DATA_PASS symbols=6 compiled=16 omitted=1')

if __name__ == '__main__':
    main()
