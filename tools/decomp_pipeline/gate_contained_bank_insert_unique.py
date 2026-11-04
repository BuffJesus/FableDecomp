"""Check contained-bank unhinted unique insertion against retail and linked trees."""
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
    ROOT / 'rebuild/src/compiled/00/9a/CContainedBankMap_InsertUnique_009ab020.cpp',
    ROOT / 'rebuild/src/compiled/00/9a/CContainedBankMap_InsertNode_009aaf70.cpp',
    ROOT / 'rebuild/src/compiled/00/9a/CContainedBankMap_CreateNode_009aa9e0.cpp',
    ROOT / 'rebuild/src/compiled/00/9a/CContainedBankMap_IteratorRetreat_009aa2d0.cpp',
    ROOT / 'rebuild/src/compiled/00/42/STL_Rb_global_Rebalance_0042971d.cpp',
    ROOT / 'rebuild/tests/00/9a/CContainedBankMap_InsertUnique_009ab020_test.cpp',
]
REPORT = ROOT / 'rebuild/compile-gate/contained-bank-insert-unique.json'
SYMBOL = '?InsertUnique@CContainedBankMap@NBankFileManager@@QAE?AU?$pair@Viterator@CContainedBankMap@NBankFileManager@@_N@std@@ABU?$pair@$$CBVCCharString@@UCContainedBank@NBankFileManager@@@4@@Z'


def main():
    work = Path(tempfile.mkdtemp(prefix='contained-bank-insert-unique-'))
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
    assert offset == 0 and len(body) == 254 and len(relocs) == 5
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x009AB020 - pe.OPTIONAL_HEADER.ImageBase, 254)
    masked = bytearray(body)
    retail_masked = bytearray(retail)
    for site in relocs:
        masked[site:site+4] = b'\0'*4
        retail_masked[site:site+4] = b'\0'*4
    assert masked == retail_masked
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'insert-unique.exe')]
    for omitted in range(4):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'omitted provider linked: ' + SOURCES[omitted].name
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'insert-unique.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CONTAINED_BANK_INSERT_UNIQUE_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='RELOCATION_MATCH', address='009ab020', bytes=254,
                                      missing_provider_checks=4,
                                      source_sha256=hashlib.sha256(SOURCES[0].read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(SOURCES[-1].read_bytes()).hexdigest()), indent=2) + '\n')
    print('CONTAINED_BANK_INSERT_UNIQUE_PASS 254/254 omitted=4')


if __name__ == '__main__':
    main()
