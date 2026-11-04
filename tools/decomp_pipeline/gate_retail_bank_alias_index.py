"""Check alias-map operator[] with retail code shape and linked searches."""
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
    ROOT / 'rebuild/src/compiled/00/9a/CBasicRetailBankAliasMap_Index_009ac700.cpp',
    ROOT / 'rebuild/src/compiled/00/9a/CBasicRetailBankAliasMap_LowerBound_009aae00.cpp',
    ROOT / 'rebuild/tests/00/9a/CBasicRetailBankAliasMap_Index_009ac700_test.cpp',
]
REPORT = ROOT / 'rebuild/compile-gate/retail-bank-alias-index.json'
SYMBOL = '??ACBasicRetailBankAliasMap@@QAEAAVCCharString@@ABV1@@Z'


def main():
    work = Path(tempfile.mkdtemp(prefix='retail-bank-alias-index-'))
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
    assert offset == 0 and len(body) == 150
    assert len(relocs) == 9, relocs
    assert any(target == '?LowerBound@CBasicRetailBankAliasMap@@QBEPAUCNode@1@ABVCCharString@@@Z'
               for target, _ in relocs.values())
    assert any(target.startswith('?InsertUnique@CBasicRetailBankAliasMap@@') for target, _ in relocs.values())
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x009AC700 - pe.OPTIONAL_HEADER.ImageBase, 149)
    similarity = compare(body, retail, relocs.keys(), max_instruction_delta=0.10, min_similarity=0.94)
    assert similarity.near_match, similarity
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'alias-index.exe')]
    for omitted in (0, 1):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'omitted provider linked: ' + SOURCES[omitted].name
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'alias-index.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'RETAIL_BANK_ALIAS_INDEX_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='NEAR_MATCH', address='009ac700',
                                      candidate_bytes=len(body), retail_bytes=len(retail),
                                      similarity=similarity.to_dict(), missing_provider_checks=2,
                                      source_sha256=hashlib.sha256(SOURCES[0].read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(SOURCES[2].read_bytes()).hexdigest()), indent=2) + '\n')
    print('RETAIL_BANK_ALIAS_INDEX_PASS 150/149 omitted=2')


if __name__ == '__main__':
    main()
