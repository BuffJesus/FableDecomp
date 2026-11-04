"""Validate the particle environment raw counted-pointer constructor."""
import argparse, hashlib, json, struct, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / 'tools/decomp_pipeline'))
import check_one, near_match, pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from particle_pointer_raw_oracle import compare, ADDRESS, DELETE_DATA, ALLOC

ROOT = check_one.ROOT
SYMBOL = '??0?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@QAE@PAVCAEnvironmentInteraction@NParticleEngine@@@Z'

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/41/ParticleEnvironmentPointer_Ctor_0041956c.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/ParticleEnvironmentPointerRaw_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/particle-pointer-raw.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='particle-pointer-raw-'))
    env = check_one.env()
    obj = work / 'source.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    built, relocations = coff.sections[section]
    assert offset == 0 and len(built) == 58
    patched = bytearray(built)
    for site, (name, kind) in relocations.items():
        assert patched[site:site + 4] == bytes(4)
        if name == '??2@YAPAXI@Z':
            assert kind == 20
            value = (ALLOC - ADDRESS - site - 4) & 0xffffffff
        else:
            assert name.startswith('?DeleteData@?$CCountedPointer@VCAEnvironmentInteraction') and kind == 6
            value = DELETE_DATA
        struct.pack_into('<I', patched, site, value)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 62)
    similarity = near_match.compare(built, retail, relocations.keys(), near_match.retail_relocations('0041956c', 62))
    assert similarity.near_match and similarity.opcode_similarity >= 0.90, similarity
    cases = compare(patched, retail)
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
    assert not r.returncode and 'PARTICLE_POINTER_RAW_BEHAVIOR_PASS cases=192' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address='0041956c', symbol=SYMBOL, grade='NEAR_MATCH',
                  similarity=similarity.to_dict(), comparative_scenarios=cases,
                  behavior=r.stdout.strip(), missing_provider_checks=1,
                  provider_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),
                  boundaries=['operator new'])
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('PARTICLE_POINTER_RAW_PASS native=' + str(cases) + ' compiled=192 similarity=' + str(round(similarity.opcode_similarity * 100, 2)))

if __name__ == '__main__':
    main()
