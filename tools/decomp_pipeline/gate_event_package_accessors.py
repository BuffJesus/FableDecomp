"""Validate retail event-package count, indexing, and clear operations."""
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
    ('009f16c0', 'CGameEventPackage_Clear_009f16c0.cpp', '?Clear@CGameEventPackage@@QAEXXZ', 8),
    ('009f16d0', 'CGameEventPackage_GetEvent_009f16d0.cpp',
     '?GetEvent@CGameEventPackage@@QBEPBVCGameEvent@@K@Z', 14),
    ('009f16e0', 'CGameEventPackage_GetNoEvents_009f16e0.cpp',
     '?GetNoEvents@CGameEventPackage@@QBEKXZ', 4),
]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source-dir', type=Path, default=ROOT / 'rebuild/src/compiled/00/9f')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/EventPackageAccessors_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/event-package-accessors.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='event-package-accessors-'))
    env = check_one.env()
    objects = []
    report_entries = []
    with pefile.PE(str(EXE)) as pe:
        for address, filename, symbol, size in ENTRIES:
            src = args.source_dir / filename
            obj = work / (filename + '.obj')
            r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(src.resolve())], env, work)
            assert not r.returncode, r.stdout + r.stderr
            coff = Coff(obj)
            section, offset = coff.symbols[symbol]
            code, reloc = coff.sections[section]
            retail = pe.get_data(int(address, 16) - pe.OPTIONAL_HEADER.ImageBase, size)
            assert offset == 0 and not reloc and code == retail
            objects.append(obj)
            report_entries.append(dict(address=address, symbol=symbol, bytes=size,
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
    assert not r.returncode and 'EVENT_PACKAGE_ACCESSORS_PASS cases=48' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(entries=report_entries, compiled_cases=48, missing_provider_checks=3,
                  behavior=r.stdout.strip(), fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('EVENT_PACKAGE_ACCESSORS_PASS bytes=26 compiled=48 omitted=3')


if __name__ == '__main__':
    main()
