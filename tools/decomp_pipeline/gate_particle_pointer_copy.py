"""Validate the native particle environment counted-pointer copy constructor."""
import argparse, hashlib, json, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / 'tools/decomp_pipeline'))
import check_one, pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from bank_pointer_copy_oracle import compare

ROOT = check_one.ROOT
ADDRESS = '00418fbc'
SYMBOL = '??0?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@QAE@ABV0@@Z'

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/41/ParticleEnvironmentPointer_Copy_00418fbc.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/ParticleEnvironmentPointerCopy_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/particle-pointer-copy.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='particle-pointer-copy-'))
    env = check_one.env()
    obj = work / 'source.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    built, relocations = coff.sections[section]
    assert offset == 0 and not relocations and len(built) == 25
    with pefile.PE(str(EXE)) as pe:
        raw = pe.get_data(int(ADDRESS, 16) - pe.OPTIONAL_HEADER.ImageBase, 25)
    assert built == raw
    cases = compare({int(ADDRESS, 16): built}, {int(ADDRESS, 16): raw})
    test = work / 'test.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(test), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    assert SYMBOL not in Coff(test).symbols
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(cmd + [str(test)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(obj), str(test)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=60)
    assert not r.returncode and 'PARTICLE_POINTER_COPY_BEHAVIOR_PASS cases=288' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address=ADDRESS, symbol=SYMBOL, grade='MATCH',
                  comparative_scenarios=cases, behavior=r.stdout.strip(),
                  provider_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),
                  missing_provider_checks=1)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('PARTICLE_POINTER_COPY_PASS native=' + str(cases) + ' compiled=288')

if __name__ == '__main__':
    main()
