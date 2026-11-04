"""Validate the zero-initialized particle environment counted-pointer global."""
import argparse, hashlib, json, subprocess, sys, tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[3] / 'tools/decomp_pipeline'))
import check_one, pefile
from gate_game_component_abi import Coff
from gate_render_state_static_data import coff_data
from pe_oracle import EXE

ROOT = check_one.ROOT
ADDRESS = 0x13d2e2c
SYMBOL = '?PEnvironmentInteraction@NParticleEngine@@3V?$CCountedPointer@VCAEnvironmentInteraction@NParticleEngine@@@@A'

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/01/3d/ParticleEnvironment_Global_013d2e2c.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/ParticleEnvironmentGlobal_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/particle-environment-global.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='particle-environment-global-'))
    env = check_one.env()
    obj = work / 'source.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    assert coff_data(coff, SYMBOL, 8) == bytes(8)
    with pefile.PE(str(EXE)) as pe:
        offset = ADDRESS - pe.OPTIONAL_HEADER.ImageBase
        sections = [s for s in pe.sections if s.VirtualAddress <= offset < s.VirtualAddress + s.Misc_VirtualSize]
        assert len(sections) == 1
        section = sections[0]
        tail = offset - section.VirtualAddress
        assert tail >= section.SizeOfRawData and tail + 8 <= section.Misc_VirtualSize
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
    assert not r.returncode and 'PARTICLE_ENVIRONMENT_GLOBAL_BEHAVIOR_PASS cases=12' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address=hex(ADDRESS), symbol=SYMBOL, data_bytes=8,
                  retail_bytes='0000000000000000', behavior=r.stdout.strip(),
                  missing_provider_checks=1, coverage_claim='data only; no new reconstructed function',
                  provider_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('PARTICLE_ENVIRONMENT_GLOBAL_PASS bytes=8 compiled=12')

if __name__ == '__main__':
    main()
