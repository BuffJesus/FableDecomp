"""Verify six typed startup graphics and recording globals."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/01/3b/StartupGraphicsAndRecordingData_013b7c90.cpp'
TEST = ROOT / 'rebuild/tests/integration/StartupGraphicsAndRecordingData_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/startup-graphics-recording-data.json'
SPECS = (
    ('?g_DeleteCheckPointSaveOnInitWorld_013B7C90@@3_NA', 0x013B7C90, 1),
    ('?g_FableMaxTextureHeight_013B7D60@@3KA', 0x013B7D60, 4),
    ('?g_FableMaxTextureWidth_013B7D64@@3KA', 0x013B7D64, 4),
    ('?g_FableTextureBitDepth_013B7D68@@3KA', 0x013B7D68, 4),
    ('?g_RenderOnlyWhenRecordingMovie_013B860C@@3_NA', 0x013B860C, 1),
    ('?g_RecordMovie_013B8629@@3_NA', 0x013B8629, 1),
)

def main():
    work = Path(tempfile.mkdtemp(prefix='startup-graphics-recording-data-'))
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
        for symbol, address, size in SPECS:
            assert symbol in data.symbols and symbol not in fixture.symbols, symbol
            expected = bytes(size)
            assert coff_data(data, symbol, size) == expected, symbol
            rva = address - pe.OPTIONAL_HEADER.ImageBase
            section = next(s for s in pe.sections if s.VirtualAddress <= rva and
                           rva + size <= s.VirtualAddress + s.Misc_VirtualSize)
            assert section.Name.rstrip(b'\0') == b'.data', symbol
            offset = rva - section.VirtualAddress
            if offset < section.SizeOfRawData:
                assert pe.get_data(rva, size) == expected, symbol
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'STARTUP_GRAPHICS_RECORDING_DATA_PASS cases=18' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(symbols=6, data_bytes=15,
                                      compiled_cases=18, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest(),
                                      coverage_claim='data only'), indent=2) + '\n')
    print('STARTUP_GRAPHICS_RECORDING_DATA_PASS symbols=6 bytes=15 compiled=18 omitted=1')

if __name__ == '__main__':
    main()
