"""Validate CWorld's zero-filled frame storage through its retail reader."""
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
ADDRESS = 0x13b89bc
SYMBOL = '?Frame@CWorld@@1JA'


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/01/3b/CWorld_Frame_013b89bc.cpp')
    ap.add_argument('--reader', type=Path, default=ROOT / 'rebuild/src/compiled/00/49/CWorld_GetFrame_0049d870.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/integration/CWorldFrameGlobal_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/world-frame-global.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='world-frame-global-'))
    env = check_one.env()
    objects = []
    for src in (args.source, args.reader, args.test):
        obj = work / (src.stem + '.obj')
        r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(src.resolve())], env, work)
        assert not r.returncode, r.stdout + r.stderr
        objects.append(obj)
    frame, reader, test = objects
    assert coff_data(Coff(frame), SYMBOL, 4) == bytes(4)
    with pefile.PE(str(EXE)) as pe:
        offset = ADDRESS - pe.OPTIONAL_HEADER.ImageBase
        sections = [s for s in pe.sections if s.VirtualAddress <= offset < s.VirtualAddress + s.Misc_VirtualSize]
        assert len(sections) == 1
        section = sections[0]
        assert section.Name.rstrip(b'\0') == b'.data'
        assert offset - section.VirtualAddress >= section.SizeOfRawData
        assert offset - section.VirtualAddress + 4 <= section.Misc_VirtualSize
    assert SYMBOL not in Coff(test).symbols
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    for subset in ((reader, test), (frame, test)):
        r = subprocess.run(cmd + [str(o) for o in subset], env=env, capture_output=True, text=True)
        assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'WORLD_FRAME_GLOBAL_PASS cases=8' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address=f'{ADDRESS:08x}', symbol=SYMBOL, data_bytes=4, retail_bytes='00000000',
                  compiled_cases=8, missing_provider_checks=2, behavior=r.stdout.strip(),
                  coverage_claim='data only; no new reconstructed function',
                  provider_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('WORLD_FRAME_GLOBAL_PASS bytes=4 compiled=8 omitted=2')


if __name__ == '__main__':
    main()
