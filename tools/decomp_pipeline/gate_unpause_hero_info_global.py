"""Validate the initialized hero-information unpause switch."""
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
from gate_render_state_static_data import coff_data
from pe_oracle import EXE

ROOT = check_one.ROOT
ADDRESS = 0x137d76c
SYMBOL = '?GUnpauseAfterClosingHeroInformationScreen@@3_NA'


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/01/37/Global_UnpauseAfterHeroInfo_0137d76c.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/Global_UnpauseAfterHeroInfo_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/unpause-hero-info-global.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='unpause-hero-info-global-'))
    env = check_one.env()
    obj = work / 'source.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    assert coff_data(Coff(obj), SYMBOL, 1) == b'\x01'
    with pefile.PE(str(EXE)) as pe:
        offset = ADDRESS - pe.OPTIONAL_HEADER.ImageBase
        sections = [s for s in pe.sections if s.VirtualAddress <= offset < s.VirtualAddress + s.Misc_VirtualSize]
        assert len(sections) == 1 and sections[0].Name.rstrip(b'\0') == b'.data'
        assert offset - sections[0].VirtualAddress < sections[0].SizeOfRawData
        assert pe.get_data(offset, 1) == b'\x01'
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
    assert not r.returncode and 'UNPAUSE_HERO_INFO_GLOBAL_PASS cases=8' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address=hex(ADDRESS), symbol=SYMBOL, data_bytes=1,
                  retail_bytes='01', behavior=r.stdout.strip(), missing_provider_checks=1,
                  coverage_claim='data only; no new reconstructed function',
                  provider_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('UNPAUSE_HERO_INFO_GLOBAL_PASS bytes=1 compiled=8')


if __name__ == '__main__':
    main()
