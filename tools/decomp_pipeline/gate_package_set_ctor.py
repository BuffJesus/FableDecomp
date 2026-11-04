"""Validate the package-set constructor and its two emitted member constructors."""
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
    ('009f1a80', '??0CGameEventPackageSet@@QAE@XZ', 60),
    ('00419220', '??0CGameEventPackage@@QAE@XZ', 35),
    ('004159a0', '??0CGameEvent@@QAE@XZ', 11),
]


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/9f/CGameEventPackageSet_Ctor_009f1a80.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/CGameEventPackageSetCtor_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/package-set-ctor.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='package-set-ctor-'))
    env = check_one.env()
    obj = work / 'source.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    report_entries = []
    with pefile.PE(str(EXE)) as pe:
        for address, symbol, size in ENTRIES:
            section, offset = coff.symbols[symbol]
            code, reloc = coff.sections[section]
            retail = pe.get_data(int(address, 16) - pe.OPTIONAL_HEADER.ImageBase, size)
            if address == '009f1a80':
                import struct
                patched = bytearray(code)
                for site, (target, kind) in reloc.items():
                    actual = 0x4159a0 if kind == 6 else 0x4038c0
                    assert (kind == 6 and target == '??0CGameEvent@@QAE@XZ') or kind == 20
                    struct.pack_into('<I', patched, site,
                                     actual if kind == 6 else (actual - int(address, 16) - site - 4) & 0xffffffff)
                assert patched == retail
                grade = 'RELOCATION_MATCH'
            else:
                assert not reloc and code == retail
                grade = 'MATCH'
            assert offset == 0 and len(code) == size
            report_entries.append(dict(address=address, symbol=symbol, bytes=size, grade=grade))
    test = work / 'fixture.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(test), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(cmd + [str(test)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(obj), str(test)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'PACKAGE_SET_CTOR_PASS cases=2051' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(entries=report_entries, compiled_cases=2051, missing_provider_checks=1,
                  behavior=r.stdout.strip(), provider_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('PACKAGE_SET_CTOR_PASS bytes=106 compiled=2051 omitted=1')


if __name__ == '__main__':
    main()
