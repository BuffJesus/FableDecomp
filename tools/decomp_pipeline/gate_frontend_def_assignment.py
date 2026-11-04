"""Validate CFrontEndDef's resource, map, and movie assignment sequence."""
import argparse
import hashlib
import json
import struct
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
ADDRESS = 0x00432E60
SYMBOL = '??4CFrontEndDef@@QAEAAV0@ABV0@@Z'
CALLEES = {12: ('??4CResource@@QAEAAV0@ABV0@@Z', 0x00431F60),
           88: ('??4?$vector@U?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@V?$allocator@U?$pair@KVCSubDefInfo@CDefClassBase@@@_STL@@@2@@_STL@@QAEAAV01@ABV01@@Z', 0x00431F90),
           106: ('??4?$vector@VCCharString@@V?$allocator@VCCharString@@@_STL@@@_STL@@QAEAAV01@ABV01@@Z', 0x00432EE9)}

def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--source', type=Path, default=ROOT / 'rebuild/src/compiled/00/43/CFrontEndDef_Assign_00432e60.cpp')
    ap.add_argument('--resource', type=Path, default=ROOT / 'rebuild/src/compiled/00/43/CResource_Assign_00431f60.cpp')
    ap.add_argument('--copy', type=Path, default=ROOT / 'rebuild/src/compiled/00/43/CFrontEndDef_Copy_00432e50.cpp')
    ap.add_argument('--test', type=Path, default=ROOT / 'rebuild/tests/00/43/CFrontEndDef_Assign_00432e60_test.cpp')
    ap.add_argument('--output', type=Path, default=ROOT / 'rebuild/compile-gate/frontend-def-assignment.json')
    args = ap.parse_args()
    work = Path(tempfile.mkdtemp(prefix='frontend-def-assignment-'))
    env = check_one.env()
    objs = []
    for source in (args.source, args.resource, args.copy, args.test):
        obj = work / (source.stem + '.obj')
        r = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source.resolve())], env, work)
        assert not r.returncode, r.stdout + r.stderr
        objs.append(obj)
    coff = Coff(objs[0])
    section, offset = coff.symbols[SYMBOL]
    code, reloc = coff.sections[section]
    assert offset == 0 and len(code) == 137
    assert reloc == {site: (symbol, 20) for site, (symbol, _) in CALLEES.items()}, (code.hex(), reloc)
    patched = bytearray(code)
    for site, (_, target) in CALLEES.items():
        patched[site:site+4] = struct.pack('<I', (target - ADDRESS - site - 4) & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 137)
    assert patched == retail, (patched.hex(), retail.hex())
    fixture = Coff(objs[3])
    assert CALLEES[88][0] in fixture.symbols and CALLEES[106][0] in fixture.symbols
    assert CALLEES[12][0] in Coff(objs[1]).symbols
    cmd = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    for selected in ((objs[3],), (objs[1], objs[2], objs[3]),
                     (objs[0], objs[2], objs[3]), (objs[0], objs[1], objs[3])):
        r = subprocess.run(cmd + [str(o) for o in selected], env=env, capture_output=True, text=True)
        assert r.returncode, 'provider omission linked'
    r = subprocess.run(cmd + [str(o) for o in objs], env=env, capture_output=True, text=True)
    assert not r.returncode, r.stdout + r.stderr
    r = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert not r.returncode and 'FRONTEND_DEF_ASSIGN_PASS cases=16' in r.stdout, (r.returncode, r.stdout, r.stderr)
    report = dict(address='00432e60', symbol=SYMBOL, bytes=137, grade='RELOCATION_MATCH',
                  compiled_cases=16, missing_provider_checks=4, behavior=r.stdout.strip(),
                  source_sha256=hashlib.sha256(args.source.read_bytes()).hexdigest(),
                  fixture_sha256=hashlib.sha256(args.test.read_bytes()).hexdigest(),
                  resource_sha256=hashlib.sha256(args.resource.read_bytes()).hexdigest(),
                  copy_sha256=hashlib.sha256(args.copy.read_bytes()).hexdigest())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print('FRONTEND_DEF_ASSIGN_PASS bytes=137 compiled=16 omitted=4')

if __name__ == '__main__':
    main()
