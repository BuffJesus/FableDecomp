"""Check bank-info map insertion code and its linked operator[] path."""
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

ROOT = Path(__file__).resolve().parents[2]
NAMES = [
    'CBankInfoMap_Index_009ac5d0',
    'CBankInfoMap_LowerBound_009aadb0',
    'CBankInfoMap_InsertUniqueHint_009abb10',
    'CBankInfoMap_InsertUnique_009ab1f0',
    'CBankInfoMap_InsertNode_009ab120',
    'CBankInfoMap_IteratorAdvance_009a9e20',
    'CBankInfoMap_IteratorRetreat_009aa320',
]
SOURCES = [ROOT / 'rebuild/src/compiled/00/9a' / (name + '.cpp') for name in NAMES] + [
    ROOT / 'rebuild/src/compiled/00/42/STL_Mfind_0042b467.cpp',
    ROOT / 'rebuild/src/compiled/00/42/STL_Rb_global_Rebalance_0042971d.cpp',
    ROOT / 'rebuild/tests/00/9a/CBankInfoMap_InsertionChain_test.cpp',
]
CASES = [
    (2, '009abb10', '?InsertUnique@CBankInfoMap@CBankFileManager@@QAE?AViterator@12@V312@ABU?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@@Z', 668, 672, 20, 'NEAR_MATCH'),
    (3, '009ab1f0', '?InsertUnique@CBankInfoMap@CBankFileManager@@QAE?AU?$pair@Viterator@CBankInfoMap@CBankFileManager@@_N@std@@ABU?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@4@@Z', 254, 254, 5, 'RELOCATION_MATCH'),
    (4, '009ab120', '?InsertNode@CBankInfoMap@CBankFileManager@@QAE?AViterator@12@PAUCNode@12@0ABU?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@I@Z', 206, 206, 6, 'RELOCATION_MATCH'),
]
REPORT = ROOT / 'rebuild/compile-gate/bank-info-insertion.json'
CALL_TARGETS = {
    '??Eiterator@CBankInfoMap': 0x009A9E20,
    '??Fiterator@CBankInfoMap': 0x009AA320,
    '?InsertNode@CBankInfoMap': 0x009AB120,
    '?InsertUnique@CBankInfoMap': None,
    '??M?$CBasicString@D@@': 0x00429950,
    '??R?$less@VCCharString@@@_STL@@': 0x0042AAB3,
    '??0CCharString@@': 0x0099EC30,
    '??0CWideString@@': 0x0099B720,
    '_malloc': 0x00BFEA0E,
    '??0?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@': 0x009A9AD0,
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
        if symbol.startswith('?InsertUnique@CBankInfoMap'):
            expected = 0x009AB1F0
        assert expected == target, (symbol, hex(target), expected)


def main():
    work = Path(tempfile.mkdtemp(prefix='bank-info-insertion-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in SOURCES:
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    # Prove the allocator name against the PE import thunk, rather than treating
    # an arbitrary allocation symbol as equivalent to the observed call address.
    raw=EXE.read_bytes();thunk=pefile.PE(data=raw).get_data(0xBFEA0E-0x400000,6)
    assert thunk[:2]==b'\xff\x25'
    import struct
    assert import_names(raw)[struct.unpack_from('<I',thunk,2)[0]]=='malloc'
    node_source=SOURCES[4].read_text();assert node_source.count('malloc(')==1
    fault=work/'wrong-allocator.cpp';fault.write_text(node_source.replace('malloc(', '::operator new('));fault_obj=work/'wrong-allocator.obj'
    result=check_one.cl(['/nologo','/c','/O2','/MT','/Fo'+str(fault_obj),str(fault)],env,work);assert result.returncode==0,result.stdout+result.stderr
    wrong=Coff(fault_obj);symbol=CASES[2][2];sec,off=wrong.symbols[symbol];body,rels=wrong.sections[sec]
    rejected=False
    try:check_call_targets(body,rels,pefile.PE(data=raw).get_data(0x9ab120-0x400000,206),'009ab120')
    except AssertionError:rejected=True
    assert rejected,'wrong operator-new target accepted as malloc'
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
        pair_symbol = ('??0?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@'
                       'QAE@ABU01@@Z')
        provider = Coff(objects[4])
        section, offset = provider.symbols[pair_symbol]
        body, relocs = provider.sections[section]
        retail = pe.get_data(0x009A9AD0 - pe.OPTIONAL_HEADER.ImageBase, 79)
        assert offset == 0 and len(body) == 79 and len(relocs) == 5
        masked = bytearray(body)
        retail_masked = bytearray(retail)
        for site in relocs:
            masked[site:site+4] = b'\0'*4
            retail_masked[site:site+4] = b'\0'*4
        assert masked == retail_masked
        check_call_targets(body, relocs, retail, '009a9ad0')
        checks.append(dict(address='009a9ad0', source=SOURCES[4].name,
                           grade='RELOCATION_MATCH', candidate_bytes=79, retail_bytes=79,
                           source_sha256=hashlib.sha256(SOURCES[4].read_bytes()).hexdigest()))
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'bank-info-insertion.exe')]
    for omitted in range(7):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'omitted provider linked: ' + SOURCES[omitted].name
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'bank-info-insertion.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'BANK_INFO_INSERTION_CHAIN_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(providers=checks, missing_provider_checks=7,allocator_target_control=True,allocator_import="malloc",
                                      fixture_sha256=hashlib.sha256(SOURCES[-1].read_bytes()).hexdigest()), indent=2) + '\n')
    print('BANK_INFO_INSERTION_CHAIN_PASS hint=672/668 unique=254/254 node=206/206 pair=79/79 omitted=7 allocator=malloc controls=1')


if __name__ == '__main__':
    main()
