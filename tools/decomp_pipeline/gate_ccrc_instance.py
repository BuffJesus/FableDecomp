"""Verify the CRC lookup table and the retail instance CRC method."""
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
METHOD = ROOT / 'rebuild/src/compiled/00/a0/CCRC_Calc_00a00950.cpp'
TABLE = ROOT / 'rebuild/src/compiled/01/29/FableCRCTable_0129a168.cpp'
TEST = ROOT / 'rebuild/tests/00/a0/CCRC_Calc_00a00950_test.cpp'
REPORT = ROOT / 'rebuild/compile-gate/ccrc-instance.json'
METHOD_ADDRESS = 0x00A00950
TABLE_ADDRESS = 0x0129A168

def main():
    work = Path(tempfile.mkdtemp(prefix='ccrc-instance-'))
    env = check_one.env()
    env['INCLUDE'] = str(ROOT / 'rebuild/include') + ';' + env['INCLUDE']
    objects = []
    for source in (METHOD, TABLE, TEST):
        obj = work / (source.stem + '.obj')
        args = ['/nologo', '/c', '/O2', '/MT', '/Fo' + str(obj)]
        if source == TEST:
            args.append('/DCRC_EXTERNAL_TABLE')
        result = check_one.cl(args + [str(source)], env, work)
        assert result.returncode == 0, result.stdout + result.stderr
        objects.append(obj)
    method, table = Coff(objects[0]), Coff(objects[1])
    section, offset = method.symbols['?Calc@CCRC@@QBEIPBXI@Z']
    body, relocs = method.sections[section]
    assert offset == 0 and len(body) == 69 and len(relocs) == 1
    patched = bytearray(body)
    for pos, (symbol, kind) in relocs.items():
        assert pos == 49 and symbol == '?g_FableCRCTable_0129A168@@3PAIA' and kind == 6
        struct.pack_into('<I', patched, pos, TABLE_ADDRESS)
    section, offset = table.symbols['?g_FableCRCTable_0129A168@@3PAIA']
    table_bytes, table_relocs = table.sections[section]
    assert offset == 0 and len(table_bytes) == 1024 and not table_relocs
    with pefile.PE(str(EXE)) as pe:
        assert patched == pe.get_data(METHOD_ADDRESS - pe.OPTIONAL_HEADER.ImageBase, len(body))
        assert table_bytes == pe.get_data(TABLE_ADDRESS - pe.OPTIONAL_HEADER.ImageBase, 1024)
    link = [str(check_one.VC / 'bin/link.exe'), '/nologo', '/out:' + str(work / 'fixture.exe')]
    for omitted in (0, 1):
        result = subprocess.run(link + [str(obj) for i, obj in enumerate(objects) if i != omitted],
                                env=env, capture_output=True, text=True)
        assert result.returncode != 0, ('omitted provider linked', omitted)
    result = subprocess.run(link + [str(o) for o in objects], env=env, capture_output=True, text=True)
    assert result.returncode == 0, result.stdout + result.stderr
    result = subprocess.run([str(work / 'fixture.exe')], env=env, capture_output=True, text=True, timeout=30)
    assert result.returncode == 0 and 'CCRC_INSTANCE_PASS cases=17' in result.stdout, (result.returncode, result.stdout, result.stderr)
    REPORT.write_text(json.dumps(dict(address='00a00950', grade='RELOCATION_MATCH',
                                      method_bytes=69, table_address='0129a168',
                                      table_bytes=1024, compiled_cases=17,
                                      missing_provider_checks=2,
                                      source_sha256=hashlib.sha256(METHOD.read_bytes()).hexdigest(),
                                      table_sha256=hashlib.sha256(TABLE.read_bytes()).hexdigest(),
                                      fixture_sha256=hashlib.sha256(TEST.read_bytes()).hexdigest()), indent=2) + '\n')
    print('CCRC_INSTANCE_PASS method=69 table=1024 compiled=17 omitted=2')

if __name__ == '__main__':
    main()
