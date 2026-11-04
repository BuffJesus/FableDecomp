"""Validate particle manager counted-pointer Reset providers against retail."""
import argparse, hashlib, json, struct, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / 'tools/decomp_pipeline'))
import check_one, near_match, pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE
from particle_manager_reset_oracle import compare, SPECS, NEW, FREE

ROOT = check_one.ROOT
TYPES = ('CParticleEmitterDatabase', 'CParticleEmitterManager', 'CPSCManager')

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/ae/ParticleManagerPointers_Reset_00aecac0.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/ParticleManagerPointersReset_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/particle-manager-reset.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='particle-manager-reset-'))
    env = check_one.env()
    obj = work / 'source.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    candidate, native, entries = {}, {}, []
    with pefile.PE(str(EXE)) as pe:
        for (address, delete_data), typename in zip(SPECS, TYPES):
            matches = [s for s in coff.symbols if s.startswith('?Reset@') and typename in s]
            assert len(matches) == 1, matches
            symbol = matches[0]
            section, offset = coff.symbols[symbol]
            built, relocations = coff.sections[section]
            assert offset == 0 and len(built) == 103
            patched = bytearray(built)
            for site, (name, kind) in relocations.items():
                assert patched[site:site + 4] == bytes(4)
                if name == '??3@YAXPAX@Z':
                    assert kind == 20
                    value = (FREE - address - site - 4) & 0xffffffff
                elif name == '??2@YAPAXI@Z':
                    assert kind == 20
                    value = (NEW - address - site - 4) & 0xffffffff
                else:
                    assert name.startswith('?DeleteData@?$CCountedPointer@V' + typename) and kind == 6
                    value = delete_data
                struct.pack_into('<I', patched, site, value)
            raw = pe.get_data(address - pe.OPTIONAL_HEADER.ImageBase, 103)
            assert patched == raw, typename
            sim = near_match.compare(built, raw, relocations.keys(),
                                     near_match.retail_relocations(f'{address:08x}', 103))
            assert sim.near_match and sim.opcode_similarity >= 0.90, (typename, sim)
            candidate[address] = patched
            native[address] = raw
            entries.append(dict(address=f'{address:08x}', symbol=symbol,
                                grade='RELOCATION_MATCH', similarity=sim.to_dict()))
    cases = compare(candidate, native)
    test = work / 'test.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(test), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    assert not ({e['symbol'] for e in entries} & Coff(test).symbols.keys())
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(cmd + [str(test)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(obj), str(test)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=60)
    assert not r.returncode and 'PARTICLE_MANAGER_RESET_BEHAVIOR_PASS cases=72' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(entries=entries, comparative_scenarios=cases, behavior=r.stdout.strip(),
                  missing_provider_checks=1,
                  boundaries=['operator delete', 'operator new', 'deletion callback'],
                  provider_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('PARTICLE_MANAGER_RESET_PASS native=' + str(cases) + ' compiled=72')

if __name__ == '__main__':
    main()
