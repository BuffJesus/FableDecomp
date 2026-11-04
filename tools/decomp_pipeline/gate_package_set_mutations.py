"""Validate package-set append and clear against retail and compiled behavior."""
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
    ('009f16f0', 'CGameEventPackageSet_AddPackage_009f16f0.cpp',
     '?AddPackage@CGameEventPackageSet@@QAEXPBVCGameEventPackage@@@Z', 37),
    ('009f1720', 'CGameEventPackageSet_Clear_009f1720.cpp',
     '?Clear@CGameEventPackageSet@@QAEXXZ', 7),
]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source-dir', type=Path, default=ROOT / 'rebuild/src/compiled/00/9f')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/PackageSetMutations_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/package-set-mutations.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='package-set-mutations-'))
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
    assert not r.returncode and 'PACKAGE_SET_MUTATIONS_PASS cases=32' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(entries=report_entries, compiled_cases=32, missing_provider_checks=2,
                  behavior=r.stdout.strip(), fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('PACKAGE_SET_MUTATIONS_PASS bytes=44 compiled=32 omitted=2')


if __name__ == '__main__':
    main()
