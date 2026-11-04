"""Validate the two typed CWorld thing-manager accessors."""
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
ENTRIES = [
    ('0049e1b0', 'CWorld_GetThingManager_0049e1b0.cpp',
     '?GetThingManager@CWorld@@UAEAAVCThingManager@@XZ'),
    ('0049e1c0', 'CWorld_PeekThingManager_0049e1c0.cpp',
     '?PeekThingManager@CWorld@@UBEABVCThingManager@@XZ'),
]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source-dir', type=Path, default=ROOT / 'rebuild/src/compiled/00/49')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/CWorldThingManagerAccessors_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/world-thing-manager-accessors.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='world-thing-manager-accessors-'))
    env = check_one.env()
    objects = []
    report_entries = []
    with pefile.PE(str(EXE)) as pe:
        for address, filename, symbol in ENTRIES:
            src = args.source_dir / filename
            obj = work / (filename + '.obj')
            r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(src.resolve())], env, work)
            assert not r.returncode, r.stdout + r.stderr
            coff = Coff(obj)
            section, offset = coff.symbols[symbol]
            code, reloc = coff.sections[section]
            retail = pe.get_data(int(address, 16) - pe.OPTIONAL_HEADER.ImageBase, 4)
            assert offset == 0 and not reloc and code == retail
            objects.append(obj)
            report_entries.append(dict(address=address, symbol=symbol, bytes=4,
                                       grade='MATCH', sha256=hashlib.sha256(src.read_bytes()).hexdigest()))
    test = work / 'fixture.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(test), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    for omitted in objects:
        r = subprocess.run(cmd + [str(o) for o in objects if o != omitted] + [str(test)],
                           env=env, capture_output=True, text=True)
        assert r.returncode, 'provider omission linked: ' + str(omitted)
    r = subprocess.run(cmd + [str(o) for o in objects] + [str(test)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'WORLD_THING_MANAGER_ACCESSORS_PASS cases=12' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(entries=report_entries, compiled_cases=12, missing_provider_checks=2,
                  behavior=r.stdout.strip(), fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('WORLD_THING_MANAGER_ACCESSORS_PASS bytes=8 compiled=12 omitted=2')


if __name__ == '__main__':
    main()
