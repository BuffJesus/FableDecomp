"""Verify the three retail bank-map lower-bound walks and linked behavior."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
CASES = [
    ('009aad60', 'CContainedBankMap_LowerBound_009aad60',
     '?LowerBound@CContainedBankMap@NBankFileManager@@QBEPAUCNode@12@ABVCCharString@@@Z'),
    ('009aadb0', 'CBankInfoMap_LowerBound_009aadb0',
     '?LowerBound@CBankInfoMap@CBankFileManager@@QBEPAUCNode@12@ABVCCharString@@@Z'),
    ('009aae00', 'CBasicRetailBankAliasMap_LowerBound_009aae00',
     '?LowerBound@CBasicRetailBankAliasMap@@QBEPAUCNode@1@ABVCCharString@@@Z'),
]
REPORT = ROOT / 'rebuild/compile-gate/bank-maps-lower-bound.json'


def main():
    work = Path(tempfile.mkdtemp(prefix='bank-maps-lower-bound-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    checks = []
    with pefile.PE(str(EXE)) as pe:
        for address, name, symbol in CASES:
            source = ROOT / 'rebuild/src/compiled/00/9a' / (name + '.cpp')
            obj = work / (name + '.obj')
            result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
            assert result.returncode == 0, result.stdout + result.stderr
            objects.append(obj)
            provider = Coff(obj)
            section, offset = provider.symbols[symbol]
            body, relocs = provider.sections[section]
            assert offset == 0 and len(body) == 65
            assert len(relocs) == 1, relocs
            site, (target, kind) = next(iter(relocs.items()))
            assert target == '??M?$CBasicString@D@@QBE_NABV0@@Z' and kind == 0x14, relocs
            retail = pe.get_data(int(address, 16) - pe.OPTIONAL_HEADER.ImageBase, 65)
            masked = bytearray(body)
            retail_masked = bytearray(retail)
            masked[site:site+4] = b'\0'*4
            retail_masked[site:site+4] = b'\0'*4
            assert masked == retail_masked, address
            checks.append(dict(address=address, source=source.name, bytes=65,
                               comparator_call_offset=site,
                               source_sha256=hashlib.sha256(source.read_bytes()).hexdigest()))
    fixture = ROOT / 'rebuild/tests/00/9a/BankMaps_LowerBound_test.cpp'
    comparator = ROOT / 'rebuild/src/compiled/00/42/CBasicStringchar_operator_00429950.cpp'
    for source in (fixture, comparator):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'lower-bound.exe')]
    for omitted in range(3):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'omitted provider linked: ' + CASES[omitted][0]
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'lower-bound.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'BANK_MAPS_LOWER_BOUND_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='RELOCATION_MATCH', providers=checks,
                                      missing_provider_checks=3,
                                      fixture_sha256=hashlib.sha256(fixture.read_bytes()).hexdigest()), indent=2) + '\n')
    print('BANK_MAPS_LOWER_BOUND_PASS 3x65/65 omitted=3')


if __name__ == '__main__':
    main()
