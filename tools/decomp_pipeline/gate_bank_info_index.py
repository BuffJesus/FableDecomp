"""Check bank-info map operator[] against retail and linked defaults."""
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
SOURCES = [
    ROOT / 'rebuild/src/compiled/00/9a/CBankInfoMap_Index_009ac5d0.cpp',
    ROOT / 'rebuild/src/compiled/00/9a/CBankInfoMap_LowerBound_009aadb0.cpp',
    ROOT / 'rebuild/tests/00/9a/CBankInfoMap_Index_009ac5d0_test.cpp',
]
REPORT = ROOT / 'rebuild/compile-gate/bank-info-index.json'
SYMBOL = '??ACBankInfoMap@CBankFileManager@@QAEAAVCBankInfo@1@ABVCCharString@@@Z'


def main():
    work = Path(tempfile.mkdtemp(prefix='bank-info-index-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in SOURCES:
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    provider = Coff(objects[0])
    section, offset = provider.symbols[SYMBOL]
    body, relocs = provider.sections[section]
    assert offset == 0 and len(body) == 302 and len(relocs) == 21
    assert any('LowerBound@CBankInfoMap' in target for target, _ in relocs.values())
    assert any('InsertUnique@CBankInfoMap' in target for target, _ in relocs.values())
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x009AC5D0 - pe.OPTIONAL_HEADER.ImageBase, 302)
    masked = bytearray(body)
    retail_masked = bytearray(retail)
    for site in relocs:
        masked[site:site+4] = b'\0'*4
        retail_masked[site:site+4] = b'\0'*4
    assert masked == retail_masked
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'bank-info-index.exe')]
    for omitted in (0, 1):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'omitted provider linked: ' + SOURCES[omitted].name
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'bank-info-index.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'BANK_INFO_INDEX_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='RELOCATION_MATCH', address='009ac5d0', bytes=302,
                                      missing_provider_checks=2,
                                      source_sha256=hashlib.sha256(SOURCES[0].read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(SOURCES[2].read_bytes()).hexdigest()), indent=2) + '\n')
    print('BANK_INFO_INDEX_PASS 302/302 omitted=2')


if __name__ == '__main__':
    main()
