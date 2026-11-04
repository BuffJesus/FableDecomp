"""Validate two particle-bank pointer setters and their zeroed globals."""
import argparse, hashlib, json, struct, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / 'tools/decomp_pipeline'))
import check_one, pefile
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE

ROOT = check_one.ROOT
SPECS = (('Graphic', 0xaeaa80, 0x13d2e08), ('Mesh', 0xaeaa90, 0x13d2e0c))

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/ae/ParticleBankSetters_00aeaa80.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/ParticleBankSetters_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/particle-bank-setters.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='particle-bank-setters-'))
    env = check_one.env()
    obj = work / 'source.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    entries = []
    with pefile.PE(str(EXE)) as pe:
        for name, address, target in SPECS:
            function = [s for s in coff.symbols if s.startswith('?Set' + name + 'Bank@NParticleEngine')]
            assert len(function) == 1, function
            symbol = function[0]
            section, offset = coff.symbols[symbol]
            built, relocations = coff.sections[section]
            assert offset == 0 and len(built) == 7 and len(relocations) == 1
            site, (ref, kind) = next(iter(relocations.items()))
            assert site == 2 and kind == 6 and ref.startswith('?P' + name + 'DataBank@NParticleEngine')
            patched = bytearray(built)
            struct.pack_into('<I', patched, site, target)
            raw = pe.get_data(address - pe.OPTIONAL_HEADER.ImageBase, 7)
            assert patched == raw
            assert coff_data(coff, ref, 4) == bytes(4)
            rva = target - pe.OPTIONAL_HEADER.ImageBase
            sections = [s for s in pe.sections if s.VirtualAddress <= rva < s.VirtualAddress + s.Misc_VirtualSize]
            assert len(sections) == 1
            pe_section = sections[0]
            tail = rva - pe_section.VirtualAddress
            assert tail >= pe_section.SizeOfRawData and tail + 4 <= pe_section.Misc_VirtualSize
            entries.append(dict(address=f'{address:08x}', symbol=symbol, storage=hex(target),
                                data_symbol=ref, grade='RELOCATION_MATCH'))
    test = work / 'test.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(test), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    assert not ({e['symbol'] for e in entries} & Coff(test).symbols.keys())
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(cmd + [str(test)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(obj), str(test)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'PARTICLE_BANK_SETTERS_BEHAVIOR_PASS cases=9' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(entries=entries, data_bytes=8, behavior=r.stdout.strip(),
                  missing_provider_checks=1,
                  provider_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('PARTICLE_BANK_SETTERS_PASS functions=2 bytes=14 data=8 compiled=9')

if __name__ == '__main__':
    main()
