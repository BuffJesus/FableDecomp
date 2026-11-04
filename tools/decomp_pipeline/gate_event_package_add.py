"""Validate the native event-package append/replacement operation."""
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
ADDRESS = 0x9f1650
SYMBOL = '?AddEvent@CGameEventPackage@@QAEXPBVCGameEvent@@@Z'


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/9f/CGameEventPackage_AddEvent_009f1650.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/00/9f/CGameEventPackage_AddEvent_009f1650_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/event-package-add.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='event-package-add-'))
    env = check_one.env()
    obj = work / 'source.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    section, offset = Coff(obj).symbols[SYMBOL]
    code, reloc = Coff(obj).sections[section]
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 105)
    assert offset == 0 and not reloc and code == retail
    test = work / 'fixture.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(test), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(cmd + [str(test)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(obj), str(test)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'EVENT_PACKAGE_ADD_PASS cases=96' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address=f'{ADDRESS:08x}', symbol=SYMBOL, bytes=105, grade='MATCH',
                  compiled_cases=96, missing_provider_checks=1, behavior=r.stdout.strip(),
                  provider_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('EVENT_PACKAGE_ADD_PASS bytes=105 compiled=96 omitted=1')


if __name__ == '__main__':
    main()
