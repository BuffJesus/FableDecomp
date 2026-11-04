"""Verify the native classless console command callback forwarding."""
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
SOURCE = ROOT / 'rebuild/src/compiled/00/41/CConsoleClasslessCommand_CallCommandFunction_00415040.cpp'
TEST = ROOT / 'rebuild/tests/00/41/CConsoleClasslessCommand_CallCommandFunction_00415040_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/classless-command-call.json'
SYMBOL = '?CallCommandFunction@CConsoleClasslessCommand@@EBEXABV?$vector@VCGenericVar@@V?$allocator@VCGenericVar@@@std@@@std@@@Z'
ADDRESS = 0x00415040


def main():
    work = Path(tempfile.mkdtemp(prefix='classless-command-call-'))
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
    assert offset == 0 and len(body) == 12 and not relocs
    assert SYMBOL not in fixture.symbols
    with pefile.PE(str(EXE)) as pe:
        assert body == pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 12)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CLASSLESS_COMMAND_CALL_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='00415040', grade='MATCH', bytes=12,
                                      compiled_cases=2, missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CLASSLESS_COMMAND_CALL_PASS bytes=12 compiled=2 omitted=1')


if __name__ == '__main__':
    main()
