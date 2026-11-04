"""Check the typed console command help routine and linked parameter cases."""
import hashlib
import json
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from near_match import compare, retail_relocations
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/00/9e/CConsoleCommandBase_GetHelp_009ea7d0.cpp'
TEST = ROOT / 'rebuild/tests/00/9e/CConsoleCommandBase_GetHelp_009ea7d0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/console-command-help.json'
SYMBOL = '?GetHelp@CConsoleCommandBase@@MAE?AVCCharString@@XZ'
ADDRESS = 0x009EA7D0
CALLEES = {
    '??H@YI?AVCCharString@@ABV0@PBD@Z',
    '??YCCharString@@QAEABV0@PBD@Z',
    '?GFIntToCharString@@YI?AVCCharString@@J@Z',
    '??YCCharString@@QAEABV0@ABV0@@Z',
    '?GetParameterTypeAsString@CConsoleCommandParameters@@QBE?AVCCharString@@J@Z',
    '??0CCharString@@QAE@ABV0@@Z',
    '??1CCharString@@QAE@XZ',
}


def main():
    work = Path(tempfile.mkdtemp(prefix='console-command-help-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (SOURCE, TEST):
        obj = work / (source.stem + '.obj')
        result = check_one.cl(['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj), str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    provider, fixture = Coff(objects[0]), Coff(objects[1])
    section, offset = provider.symbols[SYMBOL]
    body, relocs = provider.sections[section]
    assert offset == 0 and len(body) == 313 and len(relocs) == 23
    assert {name for name, kind in relocs.values() if kind == 20} == CALLEES
    assert SYMBOL not in fixture.symbols
    with pefile.PE(str(EXE)) as pe:
        retail = pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 313)
    similarity = compare(body, retail, relocs.keys(), retail_relocations('009ea7d0', 313))
    assert similarity.near_match and similarity.opcode_similarity >= 0.9, similarity
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CONSOLE_COMMAND_HELP_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='009ea7d0', grade='NEAR_MATCH',
                                      candidate_bytes=313, retail_bytes=313,
                                      similarity=similarity.to_dict(), compiled_cases=3,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CONSOLE_COMMAND_HELP_PASS near=313/313 compiled=3 omitted=1')


if __name__ == '__main__':
    main()
