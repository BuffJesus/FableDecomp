"""Validate CWorld's typed thing-manager checksum and frame read."""
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
ADDRESS = 0x49e200
SYMBOL = '?GetChecksum@CWorld@@QBEKXZ'
TARGETS = {'?GetNonLocalThingChecksum@CThingManager@@QBEKXZ': 0x51e530,
           '?Frame@CWorld@@1JA': 0x13b89bc}


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/49/CWorld_GetChecksum_0049e200.cpp')
    ap.add_argument('--frame-source', type=Path, default=ROOT / 'rebuild/src/compiled/01/3b/CWorld_Frame_013b89bc.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/00/49/CWorld_GetChecksum_0049e200_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/world-checksum.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='world-checksum-'))
    env = check_one.env()
    objects = []
    for src, defines in ((args.source, []), (args.frame_source, []),
                         (args.test, ['/DWORLD_FRAME_EXTERNAL_PROVIDER'])):
        obj = work / (src.stem + '.obj')
        r = check_one.cl(['/nologo', '/c', '/O2', '/MT', *defines,
                          '/Fo' + str(obj), str(src.resolve())], env, work)
        assert not r.returncode, r.stdout + r.stderr
        objects.append(obj)
    candidate, frame, test = objects
    coff = Coff(candidate)
    section, offset = coff.symbols[SYMBOL]
    code, reloc = coff.sections[section]
    assert offset == 0 and len(code) == 17
    assert {name for name, kind in reloc.values()} == set(TARGETS)
    patched = bytearray(code)
    for site, (name, kind) in reloc.items():
        target = TARGETS[name]
        assert kind == (20 if name.startswith('?GetNonLocal') else 6)
        struct.pack_into('<I', patched, site,
                         (target - ADDRESS - site - 4) & 0xffffffff if kind == 20 else target)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 17)
    assert patched == retail
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    for subset in ((frame, test), (candidate, test)):
        r = subprocess.run(cmd + [str(o) for o in subset], env=env, capture_output=True, text=True)
        assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'WORLD_CHECKSUM_PASS cases=32' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address=f'{ADDRESS:08x}', symbol=SYMBOL, bytes=17, grade='RELOCATION_MATCH',
                  compiled_cases=32, missing_provider_checks=2, behavior=r.stdout.strip(),
                  provider_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('WORLD_CHECKSUM_PASS bytes=17 compiled=32 omitted=2')


if __name__ == '__main__':
    main()
