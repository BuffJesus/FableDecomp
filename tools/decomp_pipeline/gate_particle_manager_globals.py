"""Validate three zero-initialized particle-manager counted-pointer globals."""
import argparse, hashlib, json, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / 'tools/decomp_pipeline'))
import check_one, pefile
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE

ROOT = check_one.ROOT
SPECS = [
    ('013d2e14', 'PEmitterDatabase_Global_013d2e14.cpp',
     '?PEmitterDatabase@NParticleEngine@@3V?$CCountedPointer@VCParticleEmitterDatabase@NParticleEngine@@@@A'),
    ('013d2e1c', 'PEmitterManager_Global_013d2e1c.cpp',
     '?PEmitterManager@NParticleEngine@@3V?$CCountedPointer@VCParticleEmitterManager@NParticleEngine@@@@A'),
    ('013d2e24', 'PPSCManager_Global_013d2e24.cpp',
     '?PPSCManager@NParticleEngine@@3V?$CCountedPointer@VCPSCManager@NParticleEngine@@@@A'),
]

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source-dir', type=Path, default=ROOT / 'rebuild/src/compiled/01/3d')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/ParticleManagerGlobals_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/particle-manager-globals.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='particle-manager-globals-'))
    env = check_one.env()
    objects = []
    sources = []
    with pefile.PE(str(EXE)) as pe:
        for address, filename, symbol in SPECS:
            source = args.source_dir / filename
            obj = work / (address + '.obj')
            r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source.resolve())], env, work)
            assert not r.returncode, r.stdout + r.stderr
            assert coff_data(Coff(obj), symbol, 8) == bytes(8)
            offset = int(address, 16) - pe.OPTIONAL_HEADER.ImageBase
            sections = [s for s in pe.sections if s.VirtualAddress <= offset < s.VirtualAddress + s.Misc_VirtualSize]
            assert len(sections) == 1
            section = sections[0]
            tail = offset - section.VirtualAddress
            assert tail >= section.SizeOfRawData and tail + 8 <= section.Misc_VirtualSize
            objects.append(obj)
            sources.append(dict(address=address, symbol=symbol, source=str(source.resolve()),
                                sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    test = work / 'test.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(test), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    assert not ({s[2] for s in SPECS} & Coff(test).symbols.keys())
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe'), str(test)]
    for omit in range(len(objects)):
        r = subprocess.run(cmd + [str(obj) for i, obj in enumerate(objects) if i != omit],
                           env=env, capture_output=True, text=True)
        assert r.returncode, ('provider omission linked', omit)
    r = subprocess.run(cmd + list(map(str, objects)), env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'PARTICLE_MANAGER_GLOBALS_BEHAVIOR_PASS cases=36' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(sources=sources, data_bytes=24, behavior=r.stdout.strip(), missing_provider_checks=3,
                  coverage_claim='data only; no new reconstructed functions',
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('PARTICLE_MANAGER_GLOBALS_PASS symbols=3 bytes=24 compiled=36')

if __name__ == '__main__':
    main()
