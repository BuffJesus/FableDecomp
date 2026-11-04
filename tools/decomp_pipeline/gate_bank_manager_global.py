"""Verify the single retail bank-manager object and its two source names."""
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
SOURCE = ROOT / 'rebuild/src/compiled/01/3c/Global_BankFileManager_013ca79c.cpp'
TEST = ROOT / 'rebuild/tests/01/3c/Global_BankFileManager_013ca79c_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/bank-manager-global.json'
SYMBOL = '?g_FableBankFileManager_013CA79C@@3VCBankFileManager@@A'
ALIAS = '?g_013ca79c@@3VCBankFileManager@@A'
ADDRESS = 0x013CA79C


def main():
    work = Path(tempfile.mkdtemp(prefix='bank-manager-global-'))
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
    assert offset == 0 and len(body) == 0x38 and not relocs
    assert ALIAS not in provider.symbols and SYMBOL not in fixture.symbols
    dump = subprocess.run([str(check_one.VC / 'bin/dumpbin.exe'), '/directives', str(objects[0])],
                          env=env, capture_output=True, text=True, check=True).stdout
    assert f'/alternatename:{ALIAS}={SYMBOL}' in dump
    with pefile.PE(str(EXE)) as pe:
        rva = ADDRESS - pe.OPTIONAL_HEADER.ImageBase
        section = next(s for s in pe.sections
                       if s.VirtualAddress <= rva < s.VirtualAddress + s.Misc_VirtualSize)
        assert section.Name.startswith(b'.data')
        assert rva - section.VirtualAddress >= section.SizeOfRawData
        constructor = pe.get_data(0x01224010 - pe.OPTIONAL_HEADER.ImageBase, 5)
        destructor = pe.get_data(0x0122A6A0 - pe.OPTIONAL_HEADER.ImageBase, 5)
        assert constructor == destructor == b'\xb9\x9c\xa7\x3c\x01'
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    result = subprocess.run(link + [str(objects[1])], env=env, capture_output=True, text=True)
    assert result.returncode != 0, 'omitted provider linked'
    result = subprocess.run(link + [str(obj) for obj in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'BANK_MANAGER_GLOBAL_PASS' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='013ca79c', grade='DATA_LAYOUT_MATCH',
                                      object_bytes=56, retail_storage='uninitialized .data tail',
                                      aliases=[SYMBOL, ALIAS], crt_constructor='01224010',
                                      crt_destructor='0122a6a0', compiled_cases=2,
                                      missing_provider_checks=1,
                                      source_sha256=hashlib.sha256(SOURCE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('BANK_MANAGER_GLOBAL_PASS bytes=56 aliases=2 crt=2 omitted=1')


if __name__ == '__main__':
    main()
