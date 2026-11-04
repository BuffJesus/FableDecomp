"""Validate the PSC-manager string-entry destructor against retail."""
import argparse, hashlib, json, struct, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / 'tools/decomp_pipeline'))
import check_one, pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from psc_manager_destructor_oracle import compare, ADDRESS, STRING_DTOR, ARRAY_FREE

ROOT = check_one.ROOT
SYMBOL = '??1CPSCManager@NParticleEngine@@QAE@XZ'

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/ae/CPSCManager_Destructor_00aeca80.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/CPSCManagerDestructor_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/psc-manager-destructor.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='psc-manager-destructor-'))
    env = check_one.env()
    obj = work / 'source.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    built, relocations = coff.sections[section]
    assert offset == 0 and len(built) == 57
    patched = bytearray(built)
    for site, (name, kind) in relocations.items():
        assert kind == 20 and patched[site:site + 4] == bytes(4)
        target = STRING_DTOR if name == '??1CCharString@@QAE@XZ' else ARRAY_FREE
        assert name in ('??1CCharString@@QAE@XZ', '??_V@YAXPAX@Z')
        struct.pack_into('<I', patched, site, (target - ADDRESS - site - 4) & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        raw = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 57)
    assert patched == raw
    cases = compare(patched, raw)
    test = work / 'test.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(test), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    assert SYMBOL not in Coff(test).symbols
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(cmd + [str(test)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(obj), str(test)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'PSC_MANAGER_DESTRUCTOR_BEHAVIOR_PASS cases=18' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address=f'{ADDRESS:08x}', symbol=SYMBOL, grade='RELOCATION_MATCH',
                  comparative_scenarios=cases, behavior=r.stdout.strip(),
                  missing_provider_checks=1, boundaries=['CCharString destructor', 'operator delete[]'],
                  provider_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('PSC_MANAGER_DESTRUCTOR_PASS native=' + str(cases) + ' compiled=18')

if __name__ == '__main__':
    main()
