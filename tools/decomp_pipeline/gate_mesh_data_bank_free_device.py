"""Validate the typed CMeshDataBank draw-procedural interface accessor."""
import argparse
import hashlib
import json
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / 'tools/decomp_pipeline'))
import check_one
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT = check_one.ROOT
SYMBOL = '?FreeDeviceSpecificResources@CMeshDataBank@@QAEXXZ'
ADDRESS = 0x00A24000

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/a2/CMeshDataBank_FreeDeviceSpecificResources_00a24000.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/00/a2/CMeshDataBank_FreeDeviceSpecificResources_00a24000_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/mesh-data-bank-free-device.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='mesh-data-bank-free-device-'))
    env = check_one.env()
    obj = work / 'provider.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    code, reloc = coff.sections[section]
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 73)
    assert offset == 0 and code == retail and not reloc, (code.hex(), retail.hex(), reloc)
    fixture = work / 'fixture.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(fixture), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    command = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(command + [str(fixture)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(command + [str(obj), str(fixture)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'MESH_DATA_BANK_FREE_DEVICE_PASS cases=24' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address='00a24000', symbol=SYMBOL, bytes=73, grade='MATCH',
                  compiled_cases=24, missing_provider_checks=1, behavior=r.stdout.strip(),
                  source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('MESH_DATA_BANK_FREE_DEVICE_PASS bytes=73 compiled=24 omitted=1')

if __name__ == '__main__':
    main()
