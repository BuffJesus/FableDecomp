"""Validate CDefinitionManager::ClearSymbols and its symbol-map delegate."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/9a/CDefinitionManager_ClearSymbols_009acb10.cpp'
FIXTURE = ROOT / 'rebuild/tests/00/9a/CDefinitionManager_ClearSymbols_009acb10_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/definition-clear-symbols.json'
SYMBOL = '?ClearSymbols@CDefinitionManager@@QAEXXZ'


def main():
    work = Path(tempfile.mkdtemp(prefix='definition-clear-symbols-'))
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
    assert relocs[4][0] == '?Clear@CSymbolMap@@QAEXXZ'
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(0x009ACB10 - pe.OPTIONAL_HEADER.ImageBase, 8)
    masked = bytearray(body)
    retail_masked = bytearray(retail)
    masked[4:8] = b'\0'*4
    retail_masked[4:8] = b'\0'*4
    assert masked == retail_masked
    instructions = list(capstone.Cs(capstone.CS_ARCH_X86, capstone.CS_MODE_32).disasm(retail, 0x009ACB10))
    assert len(instructions) == 2 and instructions[1].mnemonic == 'jmp'
    assert int(instructions[1].op_str, 16) == 0x009E5250

    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'clear-symbols.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'fixture linked without ClearSymbols provider'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'clear-symbols.exe')], env=env,
                            capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'DEFINITION_CLEAR_SYMBOLS_PASS' in result.stdout, (
        result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(grade='RELOCATION_MATCH', address='009acb10', bytes=8,
                                      delegate='009e5250', missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(FIXTURE.read_bytes()).hexdigest()), indent=2) + '\n')
    print('DEFINITION_CLEAR_SYMBOLS_PASS 8/8 omitted=1')


if __name__ == '__main__':
    main()
