"""Check the symbol-map storage-shrink tail-call against retail."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/9e/CSymbolMap_DeallocateUnusedMemory_009e5350.cpp'
FIXTURE = ROOT / 'rebuild/tests/00/9e/CSymbolMap_DeallocateUnusedMemory_009e5350_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/symbol-map-shrink.json'
SYMBOL = '?DeallocateUnusedMemory@CSymbolMap@@QAEXXZ'


def main():
    work = Path(tempfile.mkdtemp(prefix='symbol-map-shrink-'))
    env = check_one.env()
    objects = []
    for source in (SOURCE, FIXTURE):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    provider = Coff(objects[0])
    section, offset = provider.symbols[SYMBOL]
    body, relocs = provider.sections[section]
    assert offset == 0 and len(body) == 8 and len(relocs) == 1
    assert relocs[4][0].startswith('?DeallocateUnusedMemory@?$CVectorMap@VCCharString@@')
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x009E5350 - pe.OPTIONAL_HEADER.ImageBase, 8)
    masked = bytearray(body)
    retail_masked = bytearray(retail)
    masked[4:8] = b'\0'*4
    retail_masked[4:8] = b'\0'*4
    assert masked == retail_masked
    instructions = list(capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32).disasm(retail, 0x009E5350))
    assert len(instructions) == 2 and instructions[1].mnemonic == 'jmp'
    assert int(instructions[1].op_str, 16) == 0x009E7C70
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'symbol-map-shrink.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'fixture linked without wrapper provider'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'symbol-map-shrink.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'SYMBOL_MAP_SHRINK_PASS' in result.stdout, (
        result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='RELOCATION_MATCH', address='009e5350', bytes=8,
                                      delegate='009e7c70', missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(FIXTURE.read_bytes()).hexdigest()), indent=2) + '\n')
    print('SYMBOL_MAP_SHRINK_PASS 8/8 omitted=1')


if __name__ == '__main__':
    main()
