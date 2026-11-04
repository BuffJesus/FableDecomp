"""Check the six definition-array shrink bodies and their full linked path."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import capstone
import check_one
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE_PATHS = [
    '00/9a/CDefinitionManager_RemoveUnusedMemory_009ae5f0.cpp',
    '00/9e/CSymbolMap_DeallocateUnusedMemory_009e5350.cpp',
    '00/9b/CArrayVector_DeallocateUnusedMemory_009b6e00.cpp',
    '00/9b/CVectorMap_ChildDef_DeallocateUnusedMemory_009b79b0.cpp',
    '00/9b/CArrayVector_Definition_DeallocateMemory_009b4a90.cpp',
    '00/9e/CVectorMap_SymbolMap_DeallocateUnusedMemory_009e7c70.cpp',
    '00/9e/CSymbolMap_VectorMemory_009e7680.cpp',
    '00/99/CCharString_CopyAssignment_0099efb0.cpp',
    '00/99/CCharString_Destructor_0099eae0.cpp',
    '00/99/CCharString_CCharString_0099ec30.cpp',
    '00/99/CCharString_UnassignString_0099e9b0.cpp',
    '01/3b/CharStringInstanceCount_013bd800.cpp',
]
SOURCES = [ROOT / 'rebuild/src/compiled' / name for name in SOURCE_PATHS]
FIXTURE = ROOT / 'rebuild/tests/00/9a/CDefinitionManager_RemoveUnusedMemory_Full_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/definition-shrink-helpers.json'
CASES = [
    (2, '009b6e00', '?DeallocateUnusedMemory@?$CArrayVector@K@@', 135,
     [0x009B4030, 0x009B4240, 0x009B4A90, 0x00BFEA14]),
    (2, '009b6eb0', '?DeallocateUnusedMemory@?$CArrayVector@PAVCInstantiatedDefInfo@', 135,
     [0x009B4F30, 0x009B4430, 0x009B4B80, 0x00BFEA14]),
    (3, '009b79b0', '?DeallocateUnusedMemory@?$CVectorMap@KVCSubDefInfo@', 153,
     [0x009B4D80, 0x009B5310, 0x009B5F30, 0x00BFEA14]),
    (4, '009b4a90', '?DeallocateMemory@?$CArrayVector@K@@', 73, None),
    (4, '009b4b80', '?DeallocateMemory@?$CArrayVector@PAVCInstantiatedDefInfo@', 73, None),
    (4, '009b5f30', '?DeallocateMemory@?$CArrayVector@U?$pair@KVCSubDefInfo@', 90,
     [0x009B1620, 0x00BFEA14]),
]


def main():
    work = Path(tempfile.mkdtemp(prefix='definition-shrink-helpers-'))
    env = check_one.env()
    objects = []
    for source in SOURCES + [FIXTURE]:
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)

    checks = []
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    with pefile.PE(str(EXE)) as pe:
        for index, address, symbol_prefix, length, expected_calls in CASES:
            provider = Coff(objects[index])
            symbol = next(symbol for symbol in provider.symbols if symbol.startswith(symbol_prefix))
            section, offset = provider.symbols[symbol]
            body, relocs = provider.sections[section]
            assert offset == 0 and len(body) == length and len(relocs) == (4 if index in (2, 3) else 2)
            retail = pe.get_data(int(address, 16) - pe.OPTIONAL_HEADER.ImageBase, length)
            masked = bytearray(body)
            retail_masked = bytearray(retail)
            for site in relocs:
                masked[site:site+4] = b'\0'*4
                retail_masked[site:site+4] = b'\0'*4
            assert masked == retail_masked, address
            if expected_calls is not None:
                calls = [int(ins.op_str, 16) for ins in decoder.disasm(retail, int(address, 16))
                         if ins.mnemonic == 'call' and ins.op_str.startswith('0x')]
                assert calls == expected_calls, (address, calls)
            checks.append(dict(address=address, bytes=length, source=SOURCES[index].name,
                               grade='RELOCATION_MATCH',
                               source_sha256=hashlib.sha256(SOURCES[index].read_bytes()).hexdigest()))

    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'definition-shrink-full.exe')]
    for omitted in (0, 1, 2, 3, 4):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'fixture linked without provider ' + SOURCES[omitted].name
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'definition-shrink-full.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'DEFINITION_SHRINK_FULL_PASS' in result.stdout, (
        result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(providers=checks, missing_provider_checks=5,
                                      fixture_sha256=hashlib.sha256(FIXTURE.read_bytes()).hexdigest()), indent=2) + '\n')
    print('DEFINITION_SHRINK_HELPERS_PASS 659/659 full=PASS omitted=5')


if __name__ == '__main__':
    main()
