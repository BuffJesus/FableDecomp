"""Verify the typed CConsoleCommandBase formatted-name method."""
import hashlib
import json
import struct
import subprocess
import tempfile
from pathlib import Path

import check_one
import pefile
from gate_game_component_abi import Coff
from pe_oracle import EXE

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'rebuild/src/compiled/00/9e/CConsoleCommandBase_GetFormattedNameCConsoleCommandBaseMBEAVCCharStr_009ea3a0.cpp'
TEST = ROOT / 'rebuild/tests/00/9e/CConsoleCommandBase_GetFormattedNameCConsoleCommandBaseMBEAVCCharStr_009ea3a0_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/console-command-formatted-name.json'
SYMBOL = '?GetFormattedName@CConsoleCommandBase@@MBE?AVCCharString@@XZ'
ADDRESS = 0x009EA3A0
TARGETS = {
    '?g_013caa48@@3VCCharString@@B': 0x013CAA48,
    '??H@YI?AVCCharString@@ABV0@0@Z': 0x0099F570,
}

def main():
    work = Path(tempfile.mkdtemp(prefix='console-formatted-name-'))
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
    assert offset == 0 and len(body) == 26 and len(relocs) == 2
    assert SYMBOL not in fixture.symbols
    patched = bytearray(body)
    for pos, (symbol, kind) in relocs.items():
        assert symbol in TARGETS and kind in (6, 20)
        addend = struct.unpack_from('<i', body, pos)[0]
        target = TARGETS[symbol] + addend
        value = target - ADDRESS - pos - 4 if kind == 20 else target
        struct.pack_into('<I', patched, pos, value & 0xffffffff)
    with pefile.PE(str(EXE)) as pe:
        assert patched == pe.get_data(ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 26)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'GETFMTNAME_OK' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='009ea3a0', grade='RELOCATION_MATCH',
                                      bytes=26, relocations=2, compiled_cases=1,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CONSOLE_COMMAND_FORMATTED_NAME_PASS bytes=26 compiled=1 omitted=1')

if __name__ == '__main__':
    main()
