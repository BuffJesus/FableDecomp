"""Check the contained-bank hinted insertion path and its retail code shape."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from near_match import compare
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCES = [
    ROOT / 'rebuild/src/compiled/00/9a/CContainedBankMap_InsertUniqueHint_009ab870.cpp',
    ROOT / 'rebuild/src/compiled/00/9a/CContainedBankMap_InsertUnique_009ab020.cpp',
    ROOT / 'rebuild/src/compiled/00/9a/CContainedBankMap_InsertNode_009aaf70.cpp',
    ROOT / 'rebuild/src/compiled/00/9a/CContainedBankMap_CreateNode_009aa9e0.cpp',
    ROOT / 'rebuild/src/compiled/00/9a/CContainedBankMap_IteratorAdvance_009a9820.cpp',
    ROOT / 'rebuild/src/compiled/00/9a/CContainedBankMap_IteratorRetreat_009aa2d0.cpp',
    ROOT / 'rebuild/src/compiled/00/42/STL_Mfind_0042b467.cpp',
    ROOT / 'rebuild/src/compiled/00/42/STL_Rb_global_Rebalance_0042971d.cpp',
    ROOT / 'rebuild/tests/00/9a/CContainedBankMap_InsertUniqueHint_009ab870_test.cpp',
]
REPORT = ROOT / 'rebuild/compile-gate/contained-bank-insert-hint.json'
SYMBOL = '?InsertUnique@CContainedBankMap@NBankFileManager@@QAE?AViterator@12@V312@ABU?$pair@$$CBVCCharString@@UCContainedBank@NBankFileManager@@@std@@@Z'


def main():
    work = Path(tempfile.mkdtemp(prefix='contained-bank-insert-hint-'))
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
    assert offset == 0 and len(body) == 672 and len(relocs) == 20
    targets = {target for target, _ in relocs.values()}
    assert any('InsertNode@CContainedBankMap' in target for target in targets)
    assert any('??Eiterator@CContainedBankMap' in target for target in targets)
    assert any('??Fiterator@CContainedBankMap' in target for target in targets)
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x009AB870 - pe.OPTIONAL_HEADER.ImageBase, 668)
    similarity = compare(body, retail, relocs.keys(), min_similarity=0.95)
    assert similarity.near_match, similarity
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'insert-hint.exe')]
    for omitted in range(6):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'omitted provider linked: ' + SOURCES[omitted].name
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'insert-hint.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CONTAINED_BANK_INSERT_HINT_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='NEAR_MATCH', address='009ab870',
                                      candidate_bytes=len(body), retail_bytes=len(retail),
                                      similarity=similarity.to_dict(), missing_provider_checks=6,
                                      source_sha256=hashlib.sha256(SOURCES[0].read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(SOURCES[-1].read_bytes()).hexdigest()), indent=2) + '\n')
    print('CONTAINED_BANK_INSERT_HINT_PASS 672/668 omitted=6')


if __name__ == '__main__':
    main()
