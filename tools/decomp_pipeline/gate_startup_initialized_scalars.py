"""Validate eleven initialized startup globals against retail image bytes."""
import argparse
import hashlib
import json
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / 'tools/decomp_pipeline'))
import check_one
import pefile
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE

ROOT = check_one.ROOT
SPECS = [
    (0x01375444, 'RunUserScript', '_N', 1, 1),
    (0x01375448, 'FrontendFirstRun', '_N', 1, 1),
    (0x0137544A, 'PlayIntroMovies', '_N', 1, 1),
    (0x0137544C, 'HalfRefreshRate', 'J', 4, 35),
    (0x01375452, 'DitheringOn', '_N', 1, 1),
    (0x01375453, 'UseCompressedTextures', '_N', 1, 1),
    (0x01375459, 'AllowDataGeneration', '_N', 1, 1),
    (0x0137546C, 'MaximumTextureSize', 'K', 4, 2048),
    (0x01375470, 'RequestedTextureBitDepth', 'K', 4, 16),
    (0x01375494, 'InstallerBufferSize', 'J', 4, 0x00180000),
    (0x01375498, 'NoInstallBuffers', 'J', 4, 12),
]

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/01/37/StartupInitializedScalars_01375444.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/StartupInitializedScalars_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/startup-initialized-scalars.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='startup-initialized-scalars-'))
    env = check_one.env()
    objs = []
    for source in (args.source, args.test):
        obj = work / (source.stem + '.obj')
        r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source.resolve())], env, work)
        assert not r.returncode, r.stdout + r.stderr
        objs.append(obj)
    data = Coff(objs[0])
    fixture = Coff(objs[1])
    with pefile.PE(str(EXE)) as pe:
        for address, name, kind, size, value in SPECS:
            symbol = f'?g_Fable{name}_{address:08X}@@3{kind}A'
            assert symbol in data.symbols and symbol not in fixture.symbols, (address, symbol)
            expected = bytes([value]) if size == 1 else struct.pack('<I', value)
            assert coff_data(data, symbol, size) == expected, (symbol, coff_data(data, symbol, size).hex())
            rva = address - pe.OPTIONAL_HEADER.ImageBase
            section = next(s for s in pe.sections if s.VirtualAddress <= rva and rva + size <= s.VirtualAddress + s.Misc_VirtualSize)
            assert section.Name.rstrip(b'\0') == b'.data'
            assert rva - section.VirtualAddress + size <= section.SizeOfRawData
            assert pe.get_data(rva, size) == expected
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(cmd + [str(objs[1])], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(o) for o in objs], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'STARTUP_INITIALIZED_SCALARS_PASS cases=16' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(addresses=[f'{a:08x}' for a, *_ in SPECS], data_bytes=26,
                  compiled_cases=16, missing_provider_checks=1, behavior=r.stdout.strip(),
                  coverage_claim='data only; no new reconstructed functions',
                  source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('STARTUP_INITIALIZED_SCALARS_PASS symbols=11 compiled=16 omitted=1')

if __name__ == '__main__':
    main()
