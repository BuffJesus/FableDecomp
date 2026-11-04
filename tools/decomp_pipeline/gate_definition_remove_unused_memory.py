"""Check definition-manager shrink orchestration and linked delegate order."""
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
SOURCES = [
    ROOT / 'rebuild/src/compiled/00/9a/CDefinitionManager_RemoveUnusedMemory_009ae5f0.cpp',
    ROOT / 'rebuild/src/compiled/00/9e/CSymbolMap_DeallocateUnusedMemory_009e5350.cpp',
    ROOT / 'rebuild/tests/00/9a/CDefinitionManager_RemoveUnusedMemory_009ae5f0_test.cpp',
]
REPORT = ROOT / 'rebuild/compile-gate/definition-remove-unused-memory.json'
SYMBOL = '?RemoveUnusedMemory@CDefinitionManager@@IAEXXZ'
TARGETS = [0x009B6EB0, 0x009E5350, 0x009B6E00, 0x009B79B0]


def main():
    work = Path(tempfile.mkdtemp(prefix='definition-remove-unused-memory-'))
    env = check_one.env()
    objects = []
    for source in SOURCES:
        obj = work / (source.stem + '.obj')
        args = ['/nologo', '/c', '/O2', '/MT']
        if source == SOURCES[2]:
            args.append('/DFABLE_GATE_COMPOSED=1')
        result = check_one.cl(args + ['/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    provider = Coff(objects[0])
    section, offset = provider.symbols[SYMBOL]
    body, relocs = provider.sections[section]
    assert offset == 0 and len(body) == 182 and len(relocs) == 4
    assert [symbol.split('@', 1)[0] for symbol, _ in relocs.values()] == [
        '?DeallocateUnusedMemory', '?DeallocateUnusedMemory',
        '?DeallocateUnusedMemory', '?DeallocateUnusedMemory']
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x009AE5F0 - pe.OPTIONAL_HEADER.ImageBase, 182)
    masked = bytearray(body)
    retail_masked = bytearray(retail)
    for site in relocs:
        masked[site:site+4] = b'\0'*4
        retail_masked[site:site+4] = b'\0'*4
    assert masked == retail_masked
    decoder = capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32)
    retail_calls = [int(ins.op_str, 16) for ins in decoder.disasm(retail, 0x009AE5F0)
                    if ins.mnemonic == 'call' and ins.op_str.startswith('0x')]
    assert retail_calls == TARGETS, retail_calls

    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'definition-shrink.exe')]
    for omitted in (0, 1):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, 'fixture linked without provider ' + SOURCES[omitted].name
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'definition-shrink.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'DEFINITION_REMOVE_UNUSED_MEMORY_PASS' in result.stdout, (
        result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='RELOCATION_MATCH', address='009ae5f0', bytes=182,
                                      call_targets=[hex(x) for x in TARGETS],
                                      missing_provider_checks=2,
                                      source_sha256=hashlib.sha256(SOURCES[0].read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(SOURCES[2].read_bytes()).hexdigest()), indent=2) + '\n')
    print('DEFINITION_REMOVE_UNUSED_MEMORY_PASS 182/182 omitted=2')


if __name__ == '__main__':
    main()
