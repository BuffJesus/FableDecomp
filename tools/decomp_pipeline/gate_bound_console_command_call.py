"""Verify CConsole-bound command callback forwarding and exact retail code."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/9e/CConsoleCommandCConsole_CallCommandFunction_009efeb0.cpp'
TEST = ROOT / 'rebuild/tests/00/9e/CConsoleCommandCConsole_CallCommandFunction_009efeb0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/bound-console-command-call.json'
SYMBOL = '?CallCommandFunction@?$CConsoleCommand@VCConsole@@@@UBEXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z'
ADDRESS = 0x009EFEB0


def main():
    work = Path(tempfile.mkdtemp(prefix='bound-console-command-call-'))
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
    assert offset == 0 and len(body) == 15 and not relocs
    assert SYMBOL not in fixture.symbols
    with pefile.PE(str(EXE)) as pe:
        assert body == pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 15)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'BOUND_CONSOLE_COMMAND_CALL_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='009efeb0', grade='MATCH', bytes=15,
                                      compiled_cases=2, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('BOUND_CONSOLE_COMMAND_CALL_PASS bytes=15 compiled=2 omitted=1')


if __name__ == '__main__':
    main()
