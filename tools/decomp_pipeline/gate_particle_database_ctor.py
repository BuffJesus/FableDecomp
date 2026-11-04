"""Validate typed particle-database construction against retail."""
import argparse, hashlib, json, struct, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / 'tools/decomp_pipeline'))
import check_one, pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from particle_database_ctor_oracle import compare, ADDRESS, MAP_CTOR, STRING_CTOR

ROOT = check_one.ROOT
SYMBOL = '??0CParticleEmitterDatabase@NParticleEngine@@QAE@XZ'

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/ae/CParticleEmitterDatabase_Ctor_00ae8bf0.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/CParticleEmitterDatabaseCtor_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/particle-database-ctor.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='particle-database-ctor-'))
    env = check_one.env()
    obj = work / 'source.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    built, relocations = coff.sections[section]
    assert offset == 0 and len(built) == 55
    patched = bytearray(built)
    for site, (name, kind) in relocations.items():
        assert kind == 20 and patched[site:site + 4] == bytes(4)
        target = MAP_CTOR if name == '??0CSymbolMap@@QAE@XZ' else STRING_CTOR
        assert name in ('??0CSymbolMap@@QAE@XZ', '??0CCharString@@QAE@XZ')
        struct.pack_into('<I', patched, site, (target - ADDRESS - site - 4) & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        raw = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 55)
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
    assert not r.returncode and 'PARTICLE_DATABASE_CTOR_BEHAVIOR_PASS cases=5' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address=f'{ADDRESS:08x}', symbol=SYMBOL, grade='RELOCATION_MATCH',
                  comparative_scenarios=cases, behavior=r.stdout.strip(),
                  missing_provider_checks=1, boundaries=['CSymbolMap constructor', 'CCharString constructor'],
                  provider_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),
                  header_sha256=hashlib.sha256((args.source.parent / 'fable_particle_engine.h').read_bytes()).hexdigest()
                  if (args.source.parent / 'fable_particle_engine.h').exists() else None)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('PARTICLE_DATABASE_CTOR_PASS native=' + str(cases) + ' compiled=5')

if __name__ == '__main__':
    main()
