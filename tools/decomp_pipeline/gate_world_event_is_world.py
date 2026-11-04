"""Validate CWorld's retail dispatch-table membership query."""
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
from pe_oracle import EXE

ROOT = check_one.ROOT
ADDRESS = 0x0049D8C0
TABLE = 0x013B9288
SYMBOL = '?EventIsWorldEvent@CWorld@@QBE_NPBVCGameEvent@@@Z'
TABLE_SYMBOL = '?EventDispatch@CMainGameComponent@@1QBVCGameEventDispatch@@B'

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/49/CWorld_EventIsWorldEvent_0049d8c0.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/00/49/CWorld_EventIsWorldEvent_0049d8c0_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/world-event-is-world.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='world-event-is-world-'))
    env = check_one.env()
    obj = work / 'provider.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    code, reloc = coff.sections[section]
    assert offset == 0 and len(code) == 23 and reloc == {10: (TABLE_SYMBOL, 6)}, (offset, code.hex(), reloc)
    patched = bytearray(code)
    patched[10:14] = struct.pack('<I', TABLE)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 23)
    assert patched == retail, (patched.hex(), retail.hex())
    fixture = work / 'fixture.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(fixture), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    assert TABLE_SYMBOL in Coff(fixture).symbols
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(cmd + [str(fixture)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(obj), str(fixture)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'WORLD_EVENT_IS_WORLD_PASS cases=16' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address='0049d8c0', symbol=SYMBOL, bytes=23, grade='RELOCATION_MATCH',
                  compiled_cases=16, missing_provider_checks=1, behavior=r.stdout.strip(),
                  source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('WORLD_EVENT_IS_WORLD_PASS bytes=23 compiled=16 omitted=1')

if __name__ == '__main__':
    main()
