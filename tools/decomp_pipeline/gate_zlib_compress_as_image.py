"""Validate the C++ CCompressorZlib image-compression forwarder."""
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
from pe_oracle import EXE

ROOT = check_one.ROOT
ADDRESS = 0x0052AE10
SYMBOL = '?CompressAsImage@CCompressorZlib@@UAEXPBXKPAXKJJJAAVCCompressionInfo@@@Z'

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/52/CCompressorZlib_CompressAsImage_0052ae10.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/00/52/CCompressorZlib_CompressAsImage_0052ae10_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/zlib-compress-as-image.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='zlib-compress-as-image-'))
    env = check_one.env()
    obj = work / 'provider.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(args.source.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    coff = Coff(obj)
    section, offset = coff.symbols[SYMBOL]
    code, reloc = coff.sections[section]
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 33)
    assert offset == 0 and len(code) == 33 and code == retail and not reloc, (code.hex(), retail.hex(), reloc)
    fixture = work / 'fixture.obj'
    r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(fixture), str(args.test.resolve())], env, work)
    assert not r.returncode, r.stdout + r.stderr
    command = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    r = subprocess.run(command + [str(fixture)], env=env, capture_output=True, text=True)
    assert r.returncode, 'provider omission linked'
    r = subprocess.run(command + [str(obj), str(fixture)], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'ZLIB_COMPRESS_AS_IMAGE_PASS cases=32' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address='0052ae10', symbol=SYMBOL, bytes=33, grade='MATCH',
                  compiled_cases=32, missing_provider_checks=1, behavior=r.stdout.strip(),
                  source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('ZLIB_COMPRESS_AS_IMAGE_PASS bytes=33 compiled=32 omitted=1')

if __name__ == '__main__':
    main()
