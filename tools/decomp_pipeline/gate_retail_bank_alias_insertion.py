"""Check alias-map insertion code and its linked operator[] path."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import capstone
import check_one
import pefile
from gate_game_component_abi import Coff
from near_match import compare
from pe_oracle import EXE
from native_exact_roots import import_names
import struct

ROOT = Path(__file__).resolve().parents[2]
NAMES = [
    'CBasicRetailBankAliasMap_Index_009ac700',
    'CBasicRetailBankAliasMap_LowerBound_009aae00',
    'CBasicRetailBankAliasMap_InsertUniqueHint_009abdb0',
    'CBasicRetailBankAliasMap_InsertUnique_009ab3f0',
    'CBasicRetailBankAliasMap_InsertNode_009ab2f0',
    'CBasicRetailBankAliasMap_IteratorAdvance_009a9860',
    'CBasicRetailBankAliasMap_IteratorRetreat_009aa370',
]
SOURCES = [ROOT / 'rebuild/src/compiled/00/9a' / (name + '.cpp') for name in NAMES] + [
    ROOT / 'rebuild/src/compiled/00/42/STL_Mfind_0042b467.cpp',
    ROOT / 'rebuild/src/compiled/00/42/STL_Rb_global_Rebalance_0042971d.cpp',
    ROOT / 'rebuild/tests/00/9a/CBasicRetailBankAliasMap_InsertionChain_test.cpp',
]
CASES = [
    (2, '009abdb0', '?InsertUnique@CBasicRetailBankAliasMap@@QAE?AViterator@1@V21@ABU?$pair@$$CBVCCharString@@V1@@std@@@Z', 668, 672, 20, 'NEAR_MATCH'),
    (3, '009ab3f0', '?InsertUnique@CBasicRetailBankAliasMap@@QAE?AU?$pair@Viterator@CBasicRetailBankAliasMap@@_N@std@@ABU?$pair@$$CBVCCharString@@V1@@3@@Z', 254, 254, 5, 'RELOCATION_MATCH'),
    (4, '009ab2f0', '?InsertNode@CBasicRetailBankAliasMap@@QAE?AViterator@1@PAUCNode@1@0ABU?$pair@$$CBVCCharString@@V1@@std@@I@Z', 250, 250, 8, 'RELOCATION_MATCH'),
]
REPORT = ROOT / 'rebuild/compile-gate/retail-bank-alias-insertion.json'
CALL_TARGETS = {
    '??Eiterator@CBasicRetailBankAliasMap': 0x009A9860,
    '??Fiterator@CBasicRetailBankAliasMap': 0x009AA370,
    '?InsertNode@CBasicRetailBankAliasMap': 0x009AB2F0,
    '?InsertUnique@CBasicRetailBankAliasMap': None,
    '??M?$CBasicString@D@@': 0x00429950,
    '??R?$less@VCCharString@@@_STL@@': 0x0042AAB3,
    '??0CCharString@@': 0x0099EC30,
    '_malloc': 0x00BFEA0E,
    '?_Rebalance@': 0x0042971D,
}


def check_call_targets(body, relocs, retail, address):
    disassembler = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    built_calls = [relocs[ins.address + 1][0]
                   for ins in disassembler.disasm(body, 0) if ins.mnemonic == 'call']
    retail_calls = [int(ins.op_str, 16)
                    for ins in disassembler.disasm(retail, int(address, 16)) if ins.mnemonic == 'call']
    assert len(built_calls) == len(retail_calls)
    for symbol, target in zip(built_calls, retail_calls):
        expected = next((value for prefix, value in CALL_TARGETS.items()
                         if symbol.startswith(prefix)), None)
        if symbol.startswith('?InsertUnique@CBasicRetailBankAliasMap'):
            expected = 0x009AB3F0
        assert expected == target, (symbol, hex(target), expected)


def main():
    work = Path(tempfile.mkdtemp(prefix='retail-bank-alias-insertion-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in SOURCES:
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    raw=EXE.read_bytes();pe=pefile.PE(data=raw);thunk=pe.get_data(0xBFEA0E-0x400000,6)
    assert thunk[:2]==b'\xff\x25' and import_names(raw)[struct.unpack_from('<I',thunk,2)[0]]=='malloc'
    text=SOURCES[4].read_text();assert text.count('malloc(')==1
    fault=work/'wrong-allocator.cpp';fault.write_text(text.replace('malloc(', '::operator new('));wrong_obj=work/'wrong-allocator.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(wrong_obj),str(fault)],env,work);assert not result.returncode,result.stdout+result.stderr
    wrong=Coff(wrong_obj);sec,off=wrong.symbols[CASES[2][2]];wrong_body,wrong_relocs=wrong.sections[sec];rejected=False
    try:check_call_targets(wrong_body,wrong_relocs,pe.get_data(0x9ab2f0-0x400000,250),'009ab2f0')
    except AssertionError:rejected=True
    assert rejected,'operator new accepted as malloc'
    checks = []
    with pefile.PE(str(EXE)) as pe:
        for index, address, symbol, retail_size, built_size, reloc_count, grade in CASES:
            provider = Coff(objects[index])
            section, offset = provider.symbols[symbol]
            body, relocs = provider.sections[section]
            assert offset == 0 and len(body) == built_size and len(relocs) == reloc_count
            retail = pe.get_data(int(address, 16) - pe.OPTIONAL_HEADER.ImageBase, retail_size)
            similarity = compare(body, retail, relocs.keys(), min_similarity=0.95)
            assert similarity.near_match, similarity
            check_call_targets(body, relocs, retail, address)
            if grade == 'RELOCATION_MATCH':
                masked = bytearray(body)
                retail_masked = bytearray(retail)
                for site in relocs:
                    masked[site:site+4] = b'\0'*4
                    retail_masked[site:site+4] = b'\0'*4
                assert masked == retail_masked
            checks.append(dict(address=address, source=SOURCES[index].name, grade=grade,
                               candidate_bytes=len(body), retail_bytes=len(retail),
                               similarity=similarity.to_dict(),
                               source_sha256=hashlib.sha256(SOURCES[index].read_bytes()).hexdigest()))
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'alias-insertion.exe')]
    for omitted in range(7):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'omitted provider linked: ' + SOURCES[omitted].name
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'alias-insertion.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'RETAIL_BANK_ALIAS_INSERTION_CHAIN_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(providers=checks, missing_provider_checks=7,allocator_import="malloc",allocator_target_control=True,
                                      fixture_sha256=hashlib.sha256(SOURCES[-1].read_bytes()).hexdigest()), indent=2) + '\n')
    print('RETAIL_BANK_ALIAS_INSERTION_CHAIN_PASS hint=672/668 unique=254/254 node=250/250 omitted=7 allocator=malloc controls=1')


if __name__ == '__main__':
    main()
